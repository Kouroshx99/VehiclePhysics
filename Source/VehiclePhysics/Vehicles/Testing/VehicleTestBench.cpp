// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleTestBench.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

#include "VehiclePhysics/Vehicles/Physics/Car.h"
#include "VehiclePhysics/Vehicles/Physics/Drivetrain.h"
#include "VehiclePhysics/Vehicles/Physics/WishboneSuspension.h"
#include "VehiclePhysics/Vehicles/Physics/WishbonePose.h"
#include "VehiclePhysics/Vehicles/Physics/PacejkaTyreComponent.h"
// Included rather than relied on: PacejkaTyreComponent.h only forward-declares
// UTireParamsDataAsset, and this file reads members off it.
#include "VehiclePhysics/Vehicles/Physics/TireParamsDataAsset.h"
#include "PhysicsEngine/BodySetup.h"

DEFINE_LOG_CATEGORY_STATIC(LogVehicleBench, Log, All);

namespace
{
	constexpr float BenchCmToM = 0.01f;
	constexpr float GravityMPerSec2 = 9.81f;

	/** Corner order matches ACar::Corners - FR, FL, RR, RL. */
	const TCHAR* CornerName(int32 i)
	{
		switch (i)
		{
		case 0: return TEXT("FR");
		case 1: return TEXT("FL");
		case 2: return TEXT("RR");
		default: return TEXT("RL");
		}
	}

	/**
	 * Where two lines meet in the front view (the Y-Z plane).
	 *
	 * Returns false when they are within a whisker of parallel, which is not an
	 * edge case to be defended against but a real and common geometry: equal-length
	 * parallel arms put the instant centre at infinity, and the honest answer is
	 * "there isn't one" rather than a huge number.
	 */
	bool IntersectYZ(const FVector2D& A0, const FVector2D& A1,
	                 const FVector2D& B0, const FVector2D& B1, FVector2D& Out)
	{
		const FVector2D DA = A1 - A0;
		const FVector2D DB = B1 - B0;
		const float Denominator = (DA.X * DB.Y) - (DA.Y * DB.X);
		if (FMath::Abs(Denominator) < 1.e-6f)
		{
			return false;
		}
		const FVector2D Delta = B0 - A0;
		const float T = ((Delta.X * DB.Y) - (Delta.Y * DB.X)) / Denominator;
		Out = A0 + (DA * T);
		return true;
	}

	/** The point on a line at a given X - the front-view pivot of a fore-aft hinge. */
	FVector PointOnLineAtX(const FVector& P0, const FVector& P1, float X)
	{
		const FVector D = P1 - P0;
		if (FMath::Abs(D.X) < KINDA_SMALL_NUMBER)
		{
			return P0;
		}
		return P0 + (D * ((X - P0.X) / D.X));
	}

	/** Camber from a spindle axis, degrees, positive = top outboard. Doc (W.43). */
	float CamberFromSpindle(const FVector& SpindleBody)
	{
		return FMath::RadiansToDegrees(-FMath::Asin(FMath::Clamp(SpindleBody.GetSafeNormal().Z, -1.f, 1.f)));
	}

	/** Toe from a spindle axis, degrees, positive = toe-IN. */
	float ToeFromSpindle(const FVector& SpindleBody, bool bMirrored)
	{
		const FVector S = SpindleBody.GetSafeNormal();
		const float Toe = FMath::RadiansToDegrees(FMath::Atan2(S.X, FMath::Abs(S.Y)));
		return bMirrored ? -Toe : Toe;
	}

	/**
	 * How much of y the fitted line actually explains, 0..1.
	 *
	 * Reported next to every gradient because a slope on its own cannot be told
	 * apart from noise. The first steady-state runs returned gradients between -11
	 * and +7 deg/g that looked like real drivetrain differences and were fitted at
	 * R-squared 0.004 - no relationship at all. Without this number there was
	 * nothing on the report to say so.
	 */
	float RSquared(const TArray<float>& X, const TArray<float>& Y)
	{
		const int32 N = FMath::Min(X.Num(), Y.Num());
		if (N < 3) { return 0.f; }
		float MeanX = 0.f, MeanY = 0.f;
		for (int32 i = 0; i < N; ++i) { MeanX += X[i]; MeanY += Y[i]; }
		MeanX /= N; MeanY /= N;
		float Sxy = 0.f, Sxx = 0.f, Syy = 0.f;
		for (int32 i = 0; i < N; ++i)
		{
			const float dx = X[i] - MeanX, dy = Y[i] - MeanY;
			Sxy += dx * dy; Sxx += dx * dx; Syy += dy * dy;
		}
		return (Sxx > KINDA_SMALL_NUMBER && Syy > KINDA_SMALL_NUMBER)
			? FMath::Clamp((Sxy * Sxy) / (Sxx * Syy), 0.f, 1.f) : 0.f;
	}

	/**
	 * Standard error of the fitted slope, in the slope's own units.
	 *
	 * The statistic that actually says whether a gradient is trustworthy, and the
	 * reason R-squared alone is not enough: R-squared asks how much of the scatter
	 * the line explains, so a genuinely NEUTRAL car - slope near zero, nothing to
	 * explain - scores near zero however clean the data. One run here fitted 153
	 * samples at 0.11 km/h of speed hold and returned R-squared 0.063, which read as
	 * a failure and was nothing of the sort: the standard error was 0.19, putting the
	 * gradient between 0.24 and 0.99 deg/g. A real measurement of a neutral car.
	 */
	float SlopeStdError(const TArray<float>& X, const TArray<float>& Y)
	{
		const int32 N = FMath::Min(X.Num(), Y.Num());
		if (N < 3) { return 0.f; }
		float MeanX = 0.f, MeanY = 0.f;
		for (int32 i = 0; i < N; ++i) { MeanX += X[i]; MeanY += Y[i]; }
		MeanX /= N; MeanY /= N;
		float Sxy = 0.f, Sxx = 0.f;
		for (int32 i = 0; i < N; ++i)
		{
			Sxy += (X[i] - MeanX) * (Y[i] - MeanY);
			Sxx += FMath::Square(X[i] - MeanX);
		}
		if (Sxx < KINDA_SMALL_NUMBER) { return 0.f; }
		const float B = Sxy / Sxx;
		const float A = MeanY - (B * MeanX);
		float Resid = 0.f;
		for (int32 i = 0; i < N; ++i) { Resid += FMath::Square(Y[i] - (A + B * X[i])); }
		return FMath::Sqrt((Resid / (N - 2)) / Sxx);
	}

	/** Least-squares slope of y against x. Zero when x never varies. */
	float Slope(const TArray<float>& X, const TArray<float>& Y)
	{
		const int32 N = FMath::Min(X.Num(), Y.Num());
		if (N < 2) { return 0.f; }
		float SumX = 0.f, SumY = 0.f, SumXX = 0.f, SumXY = 0.f;
		for (int32 i = 0; i < N; ++i)
		{
			SumX += X[i]; SumY += Y[i]; SumXX += X[i] * X[i]; SumXY += X[i] * Y[i];
		}
		const float D = (N * SumXX) - (SumX * SumX);
		return FMath::Abs(D) < KINDA_SMALL_NUMBER ? 0.f : ((N * SumXY) - (SumX * SumY)) / D;
	}

	/**
	 * Where the fitted line crosses y at x = 0 - and on an understeer plot that has
	 * to be ZERO, which makes it a free test of whether the fit means anything.
	 *
	 * y is steer minus the Ackermann angle: the steering used beyond what the corner's
	 * geometry alone asks for. At zero lateral acceleration the tyres need no slip
	 * angle to make no force, so the geometric angle is exactly right and the extra is
	 * nothing. The line through the data must pass through the origin. That is the
	 * definition of the quantity, not a convention.
	 *
	 * So a large intercept says the fitted stretch is NOT on the straight part of the
	 * curve. Fit a line across a bend and you get the slope of whichever piece you
	 * happened to cover, lifted off the origin to make it reach - and the slope you
	 * report is then a property of the window rather than of the car.
	 *
	 * R-squared does not catch this and cannot: it asks how tightly the points hug the
	 * line, and a smooth curve hugs a straight line quite well. The offroad car fitted
	 * its power-on run at R^2 0.785, which reads as acceptable, while its real
	 * understeer fell from 5.46 to 2.58 deg/g across the very band being fitted - the
	 * single slope of 1.639 described neither end. Its intercept was 0.671 deg where
	 * every sound run in the fleet sits inside 0.13 of zero, so the one number that
	 * was being thrown away was the one that knew.
	 */
	/**
	 * Standard error of the intercept - what counts as "far from zero" for THIS fit.
	 *
	 * A fixed tolerance cannot do that job. The intercept's precision depends on the
	 * scatter, the sample count and how far the fitted band sits from the origin, and
	 * those differ per car: measured across the fleet the standard error runs 0.009 to
	 * 0.034 deg, nearly four to one. A 0.2 deg rule is therefore lenient on a tight fit
	 * and harsh on a loose one, which is the same mistake the fixed lateral-g band made
	 * before 40580.
	 *
	 * In standard errors the fleet separates cleanly: 1.77, 0.02 and 1.96 for the three
	 * cars whose curves are straight, against 8.10 for the one that is not. Three sigma
	 * puts the line where the data already puts it.
	 */
	float InterceptStdError(const TArray<float>& X, const TArray<float>& Y)
	{
		const int32 N = FMath::Min(X.Num(), Y.Num());
		if (N < 3) { return 0.f; }
		float MeanX = 0.f, MeanY = 0.f;
		for (int32 i = 0; i < N; ++i) { MeanX += X[i]; MeanY += Y[i]; }
		MeanX /= N; MeanY /= N;

		float Sxx = 0.f;
		for (int32 i = 0; i < N; ++i) { Sxx += FMath::Square(X[i] - MeanX); }
		if (Sxx < KINDA_SMALL_NUMBER) { return 0.f; }

		const float M = Slope(X, Y);
		const float C = MeanY - M * MeanX;
		float SSR = 0.f;
		for (int32 i = 0; i < N; ++i) { SSR += FMath::Square(Y[i] - (M * X[i] + C)); }

		const float SigmaSq = SSR / float(N - 2);
		return FMath::Sqrt(FMath::Max(SigmaSq, 0.f) * (1.f / float(N) + (MeanX * MeanX) / Sxx));
	}

	/**
	 * The gradient over the lower and upper halves of the fitted band.
	 *
	 * For a straight line these agree and the single gradient is the whole story. For a
	 * curve they are the story: reporting one slope for a car whose gradient runs from
	 * 2.7 near centre to 1.0 at mid-corner describes neither condition, and averaging
	 * them into one number is how "1.155 deg/g" came to stand for a car that is nothing
	 * like a 1.155 deg/g car anywhere in its range.
	 */
	void SlopeHalves(const TArray<float>& X, const TArray<float>& Y,
		float& OutLower, float& OutUpper, float& OutMid)
	{
		OutLower = OutUpper = OutMid = 0.f;
		const int32 N = FMath::Min(X.Num(), Y.Num());
		if (N < 8) { return; }

		float Lo = FLT_MAX, Hi = -FLT_MAX;
		for (int32 i = 0; i < N; ++i) { Lo = FMath::Min(Lo, X[i]); Hi = FMath::Max(Hi, X[i]); }
		OutMid = 0.5f * (Lo + Hi);

		TArray<float> LX, LY, UX, UY;
		for (int32 i = 0; i < N; ++i)
		{
			if (X[i] <= OutMid) { LX.Add(X[i]); LY.Add(Y[i]); }
			else                { UX.Add(X[i]); UY.Add(Y[i]); }
		}
		if (LX.Num() >= 4) { OutLower = Slope(LX, LY); }
		if (UX.Num() >= 4) { OutUpper = Slope(UX, UY); }
	}

	float Intercept(const TArray<float>& X, const TArray<float>& Y)
	{
		const int32 N = FMath::Min(X.Num(), Y.Num());
		if (N < 2) { return 0.f; }
		float SumX = 0.f, SumY = 0.f;
		for (int32 i = 0; i < N; ++i)
		{
			SumX += X[i]; SumY += Y[i];
		}
		// Same least-squares line as Slope, rearranged: it passes through the means.
		return (SumY - Slope(X, Y) * SumX) / float(N);
	}
}

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------

AVehicleTestBench::AVehicleTestBench()
{
	PrimaryActorTick.bCanEverTick = true;
	// After the physics sync, so a sample reads the state the substeps produced
	// rather than the one they started from.
	PrimaryActorTick.TickGroup = TG_PostPhysics;
}

void AVehicleTestBench::BeginPlay()
{
	Super::BeginPlay();

	// -runbench on the command line forces every bench in the level to run, whatever
	// the placed actor has stored. Added because a headless run is the only way to get
	// numbers without a person driving the editor, and the placed benches had
	// bRunOnBeginPlay saved as false - a property nobody would think to check, on an
	// actor nobody opens, silently producing an empty log that looks like a crash.
	//
	// -benchtest=<name> picks the test the same way, so a full sweep is a shell loop
	// rather than a sequence of clicks.
	const bool bForced = FParse::Param(FCommandLine::Get(), TEXT("runbench"));

	// A HEADLESS RUN WITHOUT A FIXED TIMESTEP IS NOT A MEASUREMENT.
	//
	// -nullrhi has nothing to wait for, so the world ticks as fast as the machine allows
	// - around twenty times PIE's 60 Hz here. The vehicle substeps internally and copes,
	// but everything OUTSIDE the substep does not: the speed and steering controllers run
	// per frame, so they act twenty times more finely, and lateral acceleration is
	// differentiated from velocity, so a twentieth of the timestep is twenty times the
	// numerical noise.
	//
	// It does not look like an error. The sports car reported a peak lateral grip of
	// 2.331 g on a tyre whose peak is 1.15, from 21773 telemetry samples where the same
	// test in PIE produces about 1100. With -usefixedtimestep -fps=60 the same run reads
	// 1.155 g. A number twice its true value, with no warning attached, is worse than a
	// crash.
	if (bForced && !FApp::UseFixedTimeStep())
	{
		UE_LOG(LogVehicleBench, Warning,
			TEXT("-runbench without -usefixedtimestep: the driving tests will be sampled "
				 "at whatever rate this machine ticks at, and their numbers are not "
				 "comparable to an editor run. Add '-usefixedtimestep -fps=60'."));
	}
	if (bForced)
	{
		FString Wanted;
		if (FParse::Value(FCommandLine::Get(), TEXT("benchtest="), Wanted))
		{
			const UEnum* Enum = StaticEnum<EVehicleBenchTest>();
			const int64 Found = Enum ? Enum->GetValueByNameString(Wanted) : INDEX_NONE;
			if (Found != INDEX_NONE)
			{
				Test = static_cast<EVehicleBenchTest>(Found);
			}
			else
			{
				UE_LOG(LogVehicleBench, Warning,
					TEXT("-benchtest=%s is not a test name; leaving it as configured."), *Wanted);
			}
		}
		UE_LOG(LogVehicleBench, Log, TEXT("-runbench: forcing %s on '%s'."),
			*StaticEnum<EVehicleBenchTest>()->GetNameStringByValue(int64(Test)), *GetName());
	}

	if (bRunOnBeginPlay || bForced)
	{
		// Deferred to the first tick, NOT started here. ACar computes TrackWidth and
		// WheelBase in its own BeginPlay, and actor BeginPlay order is arbitrary - a
		// bench that started here could measure a car whose track was still zero,
		// which silently produces a roll stiffness of zero rather than an error.
		bStartPending = true;
	}
}

bool AVehicleTestBench::ResolveTarget()
{
	// -benchspawn=<class path> REPLACES the car in the level with one of the given class,
	// standing exactly where the old one stood.
	//
	// A level is a binary asset, and benching a vehicle that is not in it should not mean
	// editing one - the transform of a car that already sits in a clear piece of the
	// level is the only thing needed, and that is cheap to borrow. It also means all four
	// vehicles can be benched from one map without four copies of it drifting apart.
	//
	// Takes the full object path with the Blueprint's _C suffix, or without it - the
	// suffix is added if loading the bare path fails. Two assets can share a short name,
	// so a substring would be a coin toss.
	FString SpawnPath;
	if (FParse::Value(FCommandLine::Get(), TEXT("benchspawn="), SpawnPath) && !SpawnPath.IsEmpty())
	{
		UClass* SpawnClass = LoadClass<ACar>(nullptr, *SpawnPath);
		if (SpawnClass == nullptr)
		{
			SpawnClass = LoadClass<ACar>(nullptr, *(SpawnPath + TEXT("_C")));
		}

		if (SpawnClass == nullptr)
		{
			UE_LOG(LogVehicleBench, Error,
				TEXT("-benchspawn=%s did not load as a car class. Give the full object "
					 "path, e.g. /Game/Path/BP_Car.BP_Car_C"), *SpawnPath);
		}
		else
		{
			// Collect first, destroy after - destroying through a live TActorIterator is
			// how a perfectly good replacement ends up spawning into an empty world.
			TArray<ACar*> Existing;
			for (TActorIterator<ACar> It(GetWorld()); It; ++It)
			{
				Existing.Add(*It);
			}

			if (Existing.Num() == 0)
			{
				UE_LOG(LogVehicleBench, Error,
					TEXT("-benchspawn needs a car already in the level to borrow a "
						 "transform from, and this level has none."));
			}
			else
			{
				const FTransform Where = Existing[0]->GetActorTransform();
				const FString Replaced = Existing[0]->GetName();
				for (ACar* Old : Existing)
				{
					Old->Destroy();
				}

				FActorSpawnParameters Params;
				// The old car occupied this spot a moment ago, so the spawn point tests
				// as blocked. It is not - the thing blocking it has just been destroyed.
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				Target = GetWorld()->SpawnActor<ACar>(SpawnClass, Where, Params);

				if (Target != nullptr)
				{
					UE_LOG(LogVehicleBench, Log,
						TEXT("-benchspawn: replaced %d car(s) - first was '%s' - with '%s' at %s"),
						Existing.Num(), *Replaced, *Target->GetName(),
						*Where.GetLocation().ToCompactString());
					return true;
				}
				UE_LOG(LogVehicleBench, Error, TEXT("-benchspawn=%s failed to spawn."), *SpawnPath);
			}
		}
	}

	// -benchtarget=<substring> picks which car, matched against the actor's name. Needed
	// because a level with two vehicles has the bench take whichever the iterator
	// reaches first, which is not a choice anyone made - and benching one car twice
	// while believing it is two is a worse outcome than benching neither.
	FString Wanted;
	if (FParse::Value(FCommandLine::Get(), TEXT("benchtarget="), Wanted) && !Wanted.IsEmpty())
	{
		for (TActorIterator<ACar> It(GetWorld()); It; ++It)
		{
			if (It->GetName().Contains(Wanted))
			{
				Target = *It;
				UE_LOG(LogVehicleBench, Log, TEXT("-benchtarget=%s matched '%s'."),
					*Wanted, *Target->GetName());
				return true;
			}
		}
		UE_LOG(LogVehicleBench, Warning,
			TEXT("-benchtarget=%s matched no car in this level; falling back to the first found."),
			*Wanted);
	}

	if (Target != nullptr)
	{
		return true;
	}
	for (TActorIterator<ACar> It(GetWorld()); It; ++It)
	{
		Target = *It;
		return true;
	}
	UE_LOG(LogVehicleBench, Error, TEXT("No ACar in the level, and Target is unset. Nothing to test."));
	return false;
}

void AVehicleTestBench::RunBench()
{
	if (!ResolveTarget())
	{
		return;
	}

	Runs.Reset();
	VariantIndex = 0;
	VariantCount = (Sweep == EVehicleBenchSweep::None) ? 1 : 3;
	if (Sweep == EVehicleBenchSweep::AntiRollBar)
	{
		VariantCount = 2;
	}

	LogWheelSpinStability();

	SaveVehicleConfig();
	StartTransform = Target->GetActorTransform();

	// The player has to be taken off the controls for the duration.
	//
	// Axis bindings fire EVERY frame with their current value, zero included, and
	// they run ahead of the physics step - so a possessed car would have the bench's
	// throttle and steer overwritten with the zeroes of a driver who is not touching
	// anything, and the run would silently measure a car coasting in a straight line.
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		Target->DisableInput(PC);
	}

	// MOST of the static test needs no ground and no driving - a motion ratio, a roll
	// centre, a camber curve are properties of the linkage and read the same whether the
	// car is parked or falling. TWO of its figures are not: static camber and rest offset
	// are whatever the corner reads at the pose it is in.
	//
	// Those were being sampled here, at bench start, which is the car mid-drop. It is why
	// static camber never matched StaticCamberDeg, and why comparing the ride height of
	// two runs measured the settling transient rather than the setup.
	//
	// So Kinematics now goes through the same Settle phase the driving tests use, and the
	// measurement happens on the far side of it.
	if (Test == EVehicleBenchTest::Kinematics)
	{
		bKinematicsAwaitingSettle = true;
		bKinematicsSettleConverged = false;
		KinematicsSettleTookSeconds = 0.f;
		const TArray<FCornerAssembly>& StartCorners = Target->GetCorners();
		for (int32 i = 0; i < 4; ++i)
		{
			const UWishboneSuspension* C = StartCorners.IsValidIndex(i) ? StartCorners[i].Suspension : nullptr;
			KinematicsPrevTravelCm[i] = C ? C->GetWheelTravelCm() : 0.f;
		}
		Phase = EPhase::Settle;
		PhaseSeconds = 0.f;
		return;
	}

	BeginVariant();
}

void AVehicleTestBench::LogWheelSpinStability() const
{
	// A wheel and the tyre carcass it stands on are a spring-mass oscillator: the
	// carcass twists, pushes Fx, Fx spins the wheel, the wheel unwinds the carcass.
	// The suspension knows nothing about it and neither does the driver - it lives
	// entirely between FShaft and the relaxation equation.
	//
	//   Vsx = Vcx - Omega*R0                                 slip velocity
	//   kappa_n+1 = (sigma*kappa_n - Vsx*dt) / (sigma + |Vx|*dt)      backward Euler
	//   Omega_n+1 = Omega_n - Fx(kappa_n+1)*R0*dt / I                 explicit
	//
	// Linearised about rolling, with C = dFx/dkappa, that iteration is
	//
	//   [kappa]     [   beta        delta   ] [kappa]
	//   [  e  ] <-  [ -g*beta    1 - g*delta] [  e  ]
	//
	//   beta = sigma/(sigma + |Vx|dt),  delta = dt/(sigma + |Vx|dt),
	//   g    = C*R0^2*dt / I
	//
	// Determinant is beta < 1, so the whole stability question reduces to the trace:
	// STABLE IFF  g*delta < 2*(1 + beta).  Substituting sigma = C/C_Fx (7.39) removes
	// the slip stiffness entirely and leaves
	//
	//   g*delta = sigma * C_Fx * R0^2 * dt^2 / (I * (sigma + |Vx|*dt))
	//
	// so only the carcass stiffness, the wheel inertia, the radius and the timestep
	// decide it. Note the dt^2: halving the substep quarters the left side. That is
	// the signature to look for in a run - if the amplitude collapses when the rate
	// doubles, it is this and not physics.
	const TArray<FCornerAssembly>& Corners = Target->GetCorners();
	const float Dt = SubstepSecondsForDiagnostic;
	const float V = FMath::Max(SkidpadSpeedKph / 3.6f, 1.f);

	UE_LOG(LogVehicleBench, Display, TEXT("  wheel-spin / carcass mode, at %.0f km/h and %.0f Hz substep"),
		SkidpadSpeedKph, 1.f / Dt);

	static const TCHAR* Names[4] = { TEXT("FR"), TEXT("FL"), TEXT("RR"), TEXT("RL") };
	for (int32 i = 0; i < Corners.Num() && i < 4; ++i)
	{
		UPacejkaTyreComponent* T = Corners[i].Tyre;
		const UTireParamsDataAsset* P = T ? T->GetTireParams() : nullptr;
		if (P == nullptr)
		{
			continue;
		}

		const float I = FMath::Max(T->GetShaft()->GetInertia(), KINDA_SMALL_NUMBER);
		const float R0 = T->GetUnloadedRadiusCm() * BenchCmToM;

		// Undamped natural frequency. sigma cancels here for the same reason it does
		// above, so this is a property of the tyre and the wheel and nothing else.
		const float Fn = (R0 * FMath::Sqrt(P->C_Fx / I)) / (2.f * PI);

		UE_LOG(LogVehicleBench, Display,
			TEXT("    %s  I %.3f kg m^2  R0 %.3f m  C_Fx %.0f N/m  f_n %.1f Hz"),
			Names[i], I, R0, P->C_Fx, Fn);

		// Evaluated at both ends of the relaxation length's range. sigma is largest
		// while the force curve is steep and collapses to sigma_min once the tyre
		// slides, and the margin is not monotonic in it - so check both.
		for (int32 k = 0; k < 2; ++k)
		{
			const float Sigma = (k == 0) ? FMath::Max(P->sigma_kappa0, P->sigma_min) : P->sigma_min;
			const float Denom = Sigma + (V * Dt);
			const float Beta = Sigma / Denom;
			const float GDelta = (Sigma * P->C_Fx * R0 * R0 * Dt * Dt) / (I * Denom);
			const float Limit = 2.f * (1.f + Beta);

			UE_LOG(LogVehicleBench, Display,
				TEXT("        transients ON,  sigma %.3f m: g*delta %.2f vs limit %.2f  %s"),
				Sigma, GDelta, Limit, GDelta < Limit ? TEXT("stable") : TEXT("<-- UNSTABLE"));

			// WITH TRANSIENTS OFF THERE IS NO RELAXATION EQUATION, so the two-state
			// analysis above does not apply - kappa becomes algebraic in Omega and the
			// loop collapses to ONE state:
			//
			//   kappa_n   = (R0*Omega_n - Vcx)/|Vcx|
			//   Omega_n+1 = Omega_n - C*kappa_n*R0*dt / I
			//
			// which is plain explicit Euler on a stiff spring, stable only for
			//
			//   G = C*R0^2*dt / (I*|Vcx|)  <  2
			//
			// G is the SAME loop gain as above, just without the relaxation length
			// filtering it: g*delta = G * |Vx|dt/(sigma + |Vx|dt), so the transient
			// model buys a factor of (sigma + |Vx|dt)/(|Vx|dt) - here about 5x. That
			// factor is the entire stability margin. Turning transients off to
			// "simplify" removes the only damping in the wheel-spin loop.
			//
			// Note G grows as speed FALLS, so if this is over the limit it is over it
			// worst at walking pace, and dt only helps linearly.
		}

		// The slip stiffness the loop actually runs at, from 7.39's sigma = C/C_Fx.
		//
		// Taken at sigma_kappa0 and NOT at sigma_min. sigma_min describes a tyre that
		// is already sliding, where the force curve has gone flat and C with it; the
		// wheel-spin loop lives near ZERO slip, where the curve is steep. Deriving C
		// from sigma_min understated G six-fold here - it printed 0.87 "stable" for a
		// loop actually running at 5.2, well past the limit, while the car could not
		// pull away with transients off.
		const float C_Nominal = FMath::Max(P->sigma_kappa0, P->sigma_min) * P->C_Fx;
		const float G = (C_Nominal * R0 * R0 * Dt) / (I * V);

		UE_LOG(LogVehicleBench, Display,
			TEXT("        transients OFF, algebraic:  dFx/dkappa %.0f N  G %.2f vs limit 2.00  %s   (needs %.0f Hz)"),
			C_Nominal, G, G < 2.f ? TEXT("stable") : TEXT("<-- UNSTABLE"),
			(G / 2.f) / Dt);

		// G scales as 1/|Vx|, so the quoted rate is the EASY case. Pulling away from
		// rest is where it bites, and the tyre model's own epsilon_v floor of 0.1 m/s
		// bounds how bad it gets.
		if (G >= 2.f)
		{
			UE_LOG(LogVehicleBench, Display,
				TEXT("            and G goes as 1/|Vx| - at walking pace it is %.0f, needing %.0f Hz"),
				G * (V / 1.f), ((G * V) / 2.f) / Dt);
		}
	}
}

void AVehicleTestBench::AbortBench()
{
	if (Phase == EPhase::Idle || Phase == EPhase::Done)
	{
		return;
	}
	UE_LOG(LogVehicleBench, Warning, TEXT("Bench aborted - reporting %d completed run(s)."), Runs.Num());
	FinishVariant();
	FinishBench();
}

// ---------------------------------------------------------------------------
// Vehicle configuration for a sweep
// ---------------------------------------------------------------------------

void AVehicleTestBench::SaveVehicleConfig()
{
	if (bConfigSaved)
	{
		return;
	}
	SavedAntiRollRates.Reset();
	for (const FCornerAssembly& C : Target->GetCorners())
	{
		SavedAntiRollRates.Add(C.Suspension ? C.Suspension->GetAntiRollRateNPerM() : 0.f);
	}
	if (UDrivetrain* DT = Target->GetDrivetrain())
	{
		SavedDrivetrainType = static_cast<uint8>(DT->DrivetrainType);
	}
	bConfigSaved = true;
}

void AVehicleTestBench::RestoreVehicleConfig()
{
	if (!bConfigSaved)
	{
		return;
	}
	const TArray<FCornerAssembly>& Corners = Target->GetCorners();
	for (int32 i = 0; i < Corners.Num() && i < SavedAntiRollRates.Num(); ++i)
	{
		if (Corners[i].Suspension)
		{
			Corners[i].Suspension->SetAntiRollRateNPerM(SavedAntiRollRates[i]);
		}
	}
	if (UDrivetrain* DT = Target->GetDrivetrain())
	{
		DT->DrivetrainType = static_cast<EDrivetrainType>(SavedDrivetrainType);
		DT->RebuildDrivetrain();
	}
}

void AVehicleTestBench::ApplyVariant(int32 Index, FString& OutLabel)
{
	RestoreVehicleConfig();

	switch (Sweep)
	{
	case EVehicleBenchSweep::AntiRollBar:
		if (Index == 0)
		{
			OutLabel = TEXT("bars fitted");
		}
		else
		{
			OutLabel = TEXT("bars DISCONNECTED");
			for (const FCornerAssembly& C : Target->GetCorners())
			{
				if (C.Suspension) { C.Suspension->SetAntiRollRateNPerM(0.f); }
			}
		}
		break;

	case EVehicleBenchSweep::Drivetrain:
		if (UDrivetrain* DT = Target->GetDrivetrain())
		{
			const EDrivetrainType Types[3] = {
				EDrivetrainType::EDT_FrontWheelDrive,
				EDrivetrainType::EDT_RearWheelDrive,
				EDrivetrainType::EDT_AllWheelDrive };
			const TCHAR* Names[3] = { TEXT("FWD"), TEXT("RWD"), TEXT("AWD") };
			DT->DrivetrainType = Types[FMath::Clamp(Index, 0, 2)];
			OutLabel = Names[FMath::Clamp(Index, 0, 2)];

			// Setting the enum alone changes nothing - the constraint graph was built
			// at BeginPlay. Without this the sweep runs the same drivetrain three
			// times and reports three near-identical results as if they meant
			// something.
			DT->RebuildDrivetrain();
		}
		break;

	default:
		OutLabel = TEXT("as configured");
		break;
	}
}

// ---------------------------------------------------------------------------
// Run flow
// ---------------------------------------------------------------------------

void AVehicleTestBench::BeginVariant()
{
	FBenchRun Run;
	ApplyVariant(VariantIndex, Run.Variant);
	Runs.Add(MoveTemp(Run));

	Samples.Reset();
	ShiftCooldown = 0.f;
	SteerRampStartTime = -1.f;
	PowerOnSteerHeldTime = -1.f;
	PowerOnRampStartTime = -1.f;
	PowerOnFrozenSteer = 0.f;
	PowerOnHeldGear = -1;
	PowerOnThrottleIntegral = 0.f;
	PowerOnLastThrottle = 0.f;
	PowerOnSaturatedSamples = 0;
	PowerOnDoseSamples = 0;
	WaitReportTimer = 0.f;
	SpeedIntegral = 0.f;
	LastShiftTime = -1000.f;
	BestLateralG = 0.f;
	TimeOfBestG = 0.f;
	DistanceTravelledM = 0.f;
	PreviousVelocity = FVector::ZeroVector;
	PhaseSeconds = 0.f;
	Phase = EPhase::Settle;

	ResetVehicleToStart();

	if (Test == EVehicleBenchTest::Articulation && SpawnedObstacle == nullptr)
	{
		// A block under the NEAR-SIDE wheels only. One side up, one side down is
		// what loads a bar in the way an off-road driver means when they talk about
		// disconnecting it - a kerb strike, not a corner.
		const float HalfTrackCm = Target->GetTrackWidthCm() * 0.5f;
		const FTransform& T = StartTransform;
		const FVector Where = T.GetLocation()
			+ (T.GetUnitAxis(EAxis::X) * 900.f)
			+ (T.GetUnitAxis(EAxis::Y) * HalfTrackCm)
			+ (T.GetUnitAxis(EAxis::Z) * (ArticulationObstacleHeightCm * 0.5f - 2.f));

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(Where, T.GetRotation().Rotator(), Params);
		if (Block)
		{
			Block->SetMobility(EComponentMobility::Movable);
			if (UStaticMeshComponent* Mesh = Block->GetStaticMeshComponent())
			{
				UStaticMesh* Cube = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
				if (Cube)
				{
					Mesh->SetStaticMesh(Cube);
					// The engine cube is 100 cm on a side, so the scale IS the size in metres.
					Mesh->SetWorldScale3D(FVector(6.f, 1.6f, ArticulationObstacleHeightCm * 0.01f));
				}
				Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				Mesh->SetCollisionProfileName(TEXT("BlockAll"));
			}
			SpawnedObstacle = Block;
		}
		else
		{
			UE_LOG(LogVehicleBench, Warning,
				TEXT("Could not spawn the articulation block - the run will measure a flat-ground pass instead."));
		}
	}

	UE_LOG(LogVehicleBench, Log, TEXT("--- %s | run %d/%d: %s ---"),
		*TestName(), VariantIndex + 1, VariantCount, *Runs.Last().Variant);
}

void AVehicleTestBench::FinishVariant()
{
	if (Runs.Num() == 0)
	{
		return;
	}
	FBenchRun& Run = Runs.Last();
	switch (Test)
	{
	case EVehicleBenchTest::Skidpad:      MeasureSkidpad(Run); break;
	case EVehicleBenchTest::PowerOnCornering: MeasurePowerOn(Run); break;
	case EVehicleBenchTest::StepSteer:    MeasureStepSteer(Run); break;
	case EVehicleBenchTest::Articulation: MeasureArticulation(Run); break;
	case EVehicleBenchTest::StraightLine: MeasureStraightLine(Run); break;
	default: break;
	}
	if (bWriteCsv)
	{
		WriteCsv();
	}
}

void AVehicleTestBench::FinishBench()
{
	Target->ApplyDriverInput(0.f, 1.f, 0.f);
	RestoreVehicleConfig();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		Target->EnableInput(PC);
	}

	if (SpawnedObstacle)
	{
		SpawnedObstacle->Destroy();
		SpawnedObstacle = nullptr;
	}

	// THE INTEGRATOR AGAINST THE CLOCK, over the whole run rather than one frame.
	// Simulated physics time has to equal elapsed time; a shortfall means the body was
	// advanced across time the vehicle never applied forces over, and every rate
	// measured is scaled by it.
	if (Target != nullptr && Samples.Num() > 1)
	{
		const double Elapsed = double(Samples.Last().Time) - double(Samples[0].Time);
		const double Simulated = Target->GetSubstepTimeTotalSeconds() - SubstepTimeAtRunStart;
		if (Elapsed > 0.0)
		{
			UE_LOG(LogVehicleBench, Log,
				TEXT("   integrator: %.3f s simulated against %.3f s elapsed (%.1f%%), "
					 "mean step %.5f s"),
				Simulated, Elapsed, 100.0 * Simulated / Elapsed,
				Target->GetSubstepCountThisFrame() > 0 ? Target->GetSubstepTimeSumThisFrame()
					/ float(Target->GetSubstepCountThisFrame()) : 0.f);

			if (FMath::Abs(Simulated - Elapsed) > 0.05 * Elapsed)
			{
				UE_LOG(LogVehicleBench, Warning,
					TEXT("   the solve covered %.1f%% of the run's elapsed time, not 100%%. "
						 "Every RATE measured here is off by roughly that factor; grip "
						 "numbers do not depend on the timeline and still stand."),
					100.0 * Simulated / Elapsed);
			}
		}
	}

	ReportRuns();
	if (Target != nullptr) { Target->UnlockSteerLimit(); }
	Phase = EPhase::Done;

	// A HEADLESS RUN HAS TO END BY ITSELF, or it is not a batch.
	//
	// -runbench prints its table and then the world just carries on ticking, forever,
	// with nobody driving and nothing left to measure. Interactively that is harmless -
	// the editor is open and a person closes it. Headless it means the process never
	// returns, so a shell loop over four cars runs the first one and then waits for a
	// car that has already finished. Every sweep so far has therefore been run one at a
	// time and killed by hand, which is also how two cars came to be compared from runs
	// made hours and several code changes apart.
	//
	// Only under -runbench: an editor session must not exit out from under someone.
	if (FParse::Param(FCommandLine::Get(), TEXT("runbench")))
	{
		UE_LOG(LogVehicleBench, Log, TEXT("-runbench complete - exiting."));
		FPlatformMisc::RequestExit(false);
	}
}

void AVehicleTestBench::ResetVehicleToStart()
{
	USkeletalMeshComponent* Mesh = Target->GetCarMesh();
	if (Mesh == nullptr)
	{
		return;
	}
	Target->ApplyDriverInput(0.f, 0.f, 0.f);
	Target->SetActorTransform(StartTransform, false, nullptr, ETeleportType::TeleportPhysics);
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
}

// ---------------------------------------------------------------------------
// Tick
// ---------------------------------------------------------------------------

void AVehicleTestBench::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Telemetry stands alone - it is the readout you want while driving by hand, so
	// it must not depend on a test having been run to find the car.
	if (bShowLiveTelemetry && DeltaSeconds > 0.f && (Target != nullptr || ResolveTarget()))
	{
		DrawLiveTelemetry(DeltaSeconds);
	}

	if (bStartPending)
	{
		bStartPending = false;
		RunBench();
		return;
	}

	if (Phase == EPhase::Idle || Phase == EPhase::Done || Target == nullptr || DeltaSeconds <= 0.f)
	{
		return;
	}

	PhaseSeconds += DeltaSeconds;

	if (Phase == EPhase::Settle)
	{
		// Brakes on while the springs take up the car's weight. Sampling through
		// this would fold the settling transient into the measurement.
		Target->ApplyDriverInput(0.f, 1.f, 0.f);

		// Kinematics waits for the ride height to stop moving rather than for a fixed
		// time, because how long a car takes to settle is one of the things that changes
		// when a spring is retuned - a fixed wait would be measuring a different point of
		// the transient on every setup. The cap still produces a report, and says so.
		if (bKinematicsAwaitingSettle)
		{
			float MaxRate = 0.f;
			const TArray<FCornerAssembly>& C4 = Target->GetCorners();
			for (int32 i = 0; i < 4; ++i)
			{
				const UWishboneSuspension* C = C4.IsValidIndex(i) ? C4[i].Suspension : nullptr;
				const float Now = C ? C->GetWheelTravelCm() : 0.f;
				MaxRate = FMath::Max(MaxRate, FMath::Abs(Now - KinematicsPrevTravelCm[i]) / DeltaSeconds);
				KinematicsPrevTravelCm[i] = Now;
			}

			const bool bStill = (PhaseSeconds > 0.5f) && (MaxRate < KinematicsSettleRateCmPerSec);
			if (!bStill && PhaseSeconds < KinematicsSettleCapSeconds)
			{
				return;
			}

			bKinematicsSettleConverged = bStill;
			KinematicsSettleTookSeconds = PhaseSeconds;

			// The settled loads, which this path used to skip because it returns before
			// the block below. They are what a corner ACTUALLY carries, and the ride
			// frequency below is meaningless without them on any car that is not 50/50.
			bSettledLoadValid = C4.Num() >= 4;
			for (int32 i = 0; i < C4.Num() && i < 4; ++i)
			{
				const UWishboneSuspension* C = C4[i].Suspension;
				SettledLoadN[i] = C ? C->GetContactForceN() : 0.f;
				bSettledLoadValid &= (C != nullptr);
			}
			if (bStill)
			{
				UE_LOG(LogVehicleBench, Log,
					TEXT("  ride height settled after %.2f s (%.3f cm/s)"), PhaseSeconds, MaxRate);
			}
			else
			{
				UE_LOG(LogVehicleBench, Warning,
					TEXT("  ride height STILL MOVING at the %.2f s cap (%.3f cm/s) - ")
					TEXT("the live figures below are not at rest"), PhaseSeconds, MaxRate);
			}

			for (int32 i = 0; i < VariantCount; ++i)
			{
				FBenchRun Run;
				ApplyVariant(i, Run.Variant);
				MeasureKinematics(Run);
				Runs.Add(MoveTemp(Run));
			}
			bKinematicsAwaitingSettle = false;
			Phase = EPhase::Done;
			FinishBench();
			return;
		}

		if (PhaseSeconds >= SettleSeconds)
		{
			// The car is stationary, braked and off the settling transient - which is
			// the only moment in the run when the corner loads are the STATIC ones.
			const TArray<FCornerAssembly>& SettleCorners = Target->GetCorners();
			bSettledLoadValid = SettleCorners.Num() >= 4;
			for (int32 i = 0; i < SettleCorners.Num() && i < 4; ++i)
			{
				const UWishboneSuspension* C = SettleCorners[i].Suspension;
				SettledLoadN[i] = C ? C->GetContactForceN() : 0.f;
				bSettledLoadValid &= (C != nullptr);
			}

			Phase = EPhase::Running;
			PhaseSeconds = 0.f;
			PreviousVelocity = Target->GetCarMesh()->GetPhysicsLinearVelocity();
		}
		return;
	}

	if (Phase == EPhase::Running)
	{
		RecordSample(DeltaSeconds);
		AutoShift(DeltaSeconds);

		bool bDone = false;
		switch (Test)
		{
		case EVehicleBenchTest::Skidpad:      bDone = DriveSkidpad(PhaseSeconds); break;
		case EVehicleBenchTest::PowerOnCornering: bDone = DrivePowerOn(PhaseSeconds); break;
		case EVehicleBenchTest::StepSteer:    bDone = DriveStepSteer(PhaseSeconds); break;
		case EVehicleBenchTest::Articulation: bDone = DriveArticulation(PhaseSeconds); break;
		case EVehicleBenchTest::StraightLine: bDone = DriveStraightLine(PhaseSeconds); break;
		default: bDone = true; break;
		}

		if (bDone)
		{
			FinishVariant();
			++VariantIndex;
			if (VariantIndex >= VariantCount)
			{
				FinishBench();
			}
			else
			{
				Phase = EPhase::Between;
				PhaseSeconds = 0.f;
			}
		}
		return;
	}

	if (Phase == EPhase::Between)
	{
		Target->ApplyDriverInput(0.f, 1.f, 0.f);
		if (PhaseSeconds >= 1.f)
		{
			BeginVariant();
		}
	}
}

void AVehicleTestBench::RecordSample(float DeltaSeconds)
{
	USkeletalMeshComponent* Mesh = Target->GetCarMesh();
	if (Mesh == nullptr)
	{
		return;
	}

	const FTransform Body = Mesh->GetComponentTransform();
	const FVector VelocityWS = Mesh->GetPhysicsLinearVelocity();
	const FVector AngularWS = Mesh->GetPhysicsAngularVelocityInDegrees();

	// Accelerations in the BODY frame, which is what an accelerometer bolted to
	// the car reads and what every published figure is quoted in.
	const FVector AccelWS = (VelocityWS - PreviousVelocity) / DeltaSeconds;
	PreviousVelocity = VelocityWS;
	const FVector AccelBody = Body.InverseTransformVector(AccelWS) * BenchCmToM;

	const FVector VelBody = Body.InverseTransformVector(VelocityWS) * BenchCmToM;
	const FVector AngBody = Body.InverseTransformVector(AngularWS);

	FSample S;
	S.Time = PhaseSeconds;
	S.SpeedMPerSec = VelocityWS.Size() * BenchCmToM;
	S.YawRateDegPerSec = AngBody.Z;

	// LATERAL ACCELERATION FROM STATES, not by differentiating velocity.
	//
	// a_y = omega_z * v_x is the centripetal term, computed from two quantities the
	// solver already holds exactly. The old form differenced world velocity across a
	// frame, and a frame boundary is not where the physics steps: the solver runs at
	// 200-240 Hz while this samples at 60, so a frame occasionally catches an extra
	// step's worth of velocity change and the derivative spikes. The mean was always
	// right - measured at 0.413 g against 0.413 g for the centripetal form - but the
	// PEAK was not, and peak grip is a headline number.
	//
	// Measured across the fleet, differentiated against centripetal, on the same runs:
	//
	//                  reported    differentiated   centripetal
	//     saloon          1.072             1.322         1.048
	//     sports          1.158             1.589         1.168
	//     rally           0.625             0.774         0.635
	//
	// Three cars inside 2.3% on the centripetal form; 20-37% high on the derivative.
	//
	// And the error compounds rather than just adding noise, because the fitted band is
	// a FRACTION of the measured peak (see MeasureSkidpad). An inflated peak widens the
	// band into tyre saturation: the rally car's gradient came back 16.713 deg/g at
	// R^2 0.390 where the same car on the same setup reads 3.151 at 0.911.
	//
	// This is also the standard definition rather than a convenience - ISO 4138 quotes
	// lateral acceleration as V^2/R, which is exactly this. The transient term v_y-dot
	// is dropped; every test here fits over steady or quasi-steady cornering, where it
	// is zero by construction.
	S.LateralAccelG = (VelBody.X * FMath::DegreesToRadians(AngBody.Z)) / GravityMPerSec2;

	// LONGITUDINAL stays a derivative, because there is no state to read it from -
	// tangential acceleration IS the rate of change of speed. Both places that care
	// already smooth it: the straight-line peak over PeakAccelSmoothingSamples, and the
	// power-on dose controller over the same window.
	S.LongAccelG = AccelBody.X / GravityMPerSec2;
	S.RollDeg = Body.GetRotation().Rotator().Roll;
	S.PitchDeg = Body.GetRotation().Rotator().Pitch;

	const TArray<FCornerAssembly>& Corners = Target->GetCorners();
	for (int32 i = 0; i < Corners.Num() && i < 4; ++i)
	{
		if (const UWishboneSuspension* C = Corners[i].Suspension)
		{
			S.LoadN[i] = C->GetContactForceN();
			S.TravelCm[i] = C->GetWheelTravelCm();
			S.CamberDeg[i] = C->GetCamberDeg();
		}
		if (UPacejkaTyreComponent* T = Corners[i].Tyre)
		{
			S.KappaSpan[i] = T->ReadAndResetKappaSpan();
			S.OmegaRadPerSec[i] = T->GetShaft()->GetAngularVelocity();
			S.OmegaUsed[i] = T->GetOmegaUsed();
			S.VcxUsed[i] = T->GetVcxUsed();
			// Which axle is actually doing the driving, and how hard. Fx leaves the
			// Magic Formula pre-multiplied into UU, so back to N here to read against
			// the loads beside it.
			S.Kappa[i] = T->GetResult().Kappa;
			S.FxN[i] = T->GetResult().Fx * BenchCmToM;
			// Alpha is in radians in the result; Fy carries the same 100x the tyre model
			// applies to Fx, so it takes the same conversion to newtons.
			S.AlphaDeg[i] = FMath::RadiansToDegrees(T->GetResult().Alpha);
			S.FyN[i] = T->GetResult().Fy * BenchCmToM;
		}
	}

	// The road-wheel angle, not the driver's input. Logged because the understeer
	// gradient is computed from it, and a CSV that omits it cannot be used to check
	// the gradient the bench reports.
	{
		float SteerSum = 0.f; int32 SteerCount = 0;
		for (int32 i = 0; i < Corners.Num() && i < 2; ++i)
		{
			if (Corners[i].Suspension)
			{
				SteerSum += FMath::Abs(Corners[i].Suspension->GetSteerAngleDeg());
				++SteerCount;
			}
		}
		S.SteerAngleDeg = SteerCount ? SteerSum / SteerCount : 0.f;
	}

	S.bShiftSettling = (PhaseSeconds - LastShiftTime) < ShiftSettleSeconds;

	DistanceTravelledM += S.SpeedMPerSec * DeltaSeconds;
	S.DistanceM = DistanceTravelledM;
	S.SteerInput = 0.f;

	// THE BASELINE FOR THE RUN-LENGTH CHECK, taken at the FIRST SAMPLE rather than at
	// StartRun. The solve is already running through the settle phase, so a baseline
	// taken before sampling begins counts that settle on the simulated side and not on
	// the elapsed side - which read as the integrator running 18% fast when it was not.
	if (Samples.Num() == 0 && Target != nullptr)
	{
		SubstepTimeAtRunStart = Target->GetSubstepTimeTotalSeconds();
	}

	Samples.Add(S);
}

void AVehicleTestBench::DrawLiveTelemetry(float DeltaSeconds)
{
	USkeletalMeshComponent* Mesh = Target->GetCarMesh();
	if (Mesh == nullptr || GEngine == nullptr)
	{
		return;
	}

	const FTransform Body = Mesh->GetComponentTransform();
	const FVector VelWS = Mesh->GetPhysicsLinearVelocity();
	const FVector AngWS = Mesh->GetPhysicsAngularVelocityInDegrees();

	const FVector AccelBody = Body.InverseTransformVector((VelWS - LiveLastVelocity) / DeltaSeconds) * BenchCmToM;
	LiveLastVelocity = VelWS;

	// Heavily smoothed. Raw frame-to-frame acceleration off a substepped body is
	// far too noisy to read, and a number nobody can read is not telemetry.
	const float Blend = FMath::Clamp(DeltaSeconds * 5.f, 0.f, 1.f);
	LiveLatG = FMath::Lerp(LiveLatG, AccelBody.Y / GravityMPerSec2, Blend);
	LiveLongG = FMath::Lerp(LiveLongG, AccelBody.X / GravityMPerSec2, Blend);

	const float SpeedMs = VelWS.Size() * BenchCmToM;
	const float YawRate = Body.InverseTransformVector(AngWS).Z;

	// Radius from speed and yaw rate rather than from the steering angle, so it is
	// the path the car is ACTUALLY on - the gap between this and the geometric
	// radius the wheels are asking for is understeer, made into a number.
	const float RadiusM = FMath::Abs(YawRate) > 1.f
		? SpeedMs / FMath::DegreesToRadians(FMath::Abs(YawRate)) : 0.f;

	int32 Key = 20000;
	auto Line = [&Key](const FColor& C, const FString& S)
	{
		GEngine->AddOnScreenDebugMessage(Key++, 0.f, C, S);
	};

	Line(FColor::White, FString::Printf(TEXT("speed %5.1f km/h   yaw %+6.1f deg/s   radius %s"),
		SpeedMs * 3.6f, YawRate,
		RadiusM > 0.f ? *FString::Printf(TEXT("%.1f m"), RadiusM) : TEXT("straight")));

	Line(FMath::Abs(LiveLatG) > 0.9f ? FColor::Orange : FColor::Green,
		FString::Printf(TEXT("lateral %+5.2f g   long %+5.2f g   roll %+5.2f deg   pitch %+5.2f deg"),
			LiveLatG, LiveLongG, Body.GetRotation().Rotator().Roll, Body.GetRotation().Rotator().Pitch));

	const TArray<FCornerAssembly>& Corners = Target->GetCorners();
	FString Loads = TEXT("load N   ");
	FString Travels = TEXT("travel cm");
	bool bAnyAirborne = false;
	for (int32 i = 0; i < Corners.Num() && i < 4; ++i)
	{
		if (const UWishboneSuspension* S = Corners[i].Suspension)
		{
			Loads += FString::Printf(TEXT("  %s %6.0f"), CornerName(i), S->GetContactForceN());
			Travels += FString::Printf(TEXT("  %s %+5.2f"), CornerName(i), S->GetWheelTravelCm());
			if (!S->IsInContact()) { bAnyAirborne = true; }
		}
	}
	Line(bAnyAirborne ? FColor::Red : FColor::Silver, Loads + (bAnyAirborne ? TEXT("   WHEEL LIFT") : TEXT("")));
	Line(FColor::Silver, Travels);
}

// ---------------------------------------------------------------------------
// Driving
// ---------------------------------------------------------------------------

void AVehicleTestBench::AutoShift(float DeltaSeconds)
{
	UDrivetrain* DT = Target->GetDrivetrain();
	if (DT == nullptr)
	{
		return;
	}

	ShiftCooldown = FMath::Max(ShiftCooldown - DeltaSeconds, 0.f);
	if (ShiftCooldown > 0.f)
	{
		return;
	}

	// Nothing shifts once a steady-state sweep is under way. The point of holding a
	// constant speed is that the driveline is left alone; a shift mid-sweep puts a
	// torque step through the driven axle and costs a second of samples either side.
	//
	// It was tempting to dodge this by testing slower, low enough never to need
	// second gear. That trades one confound for a worse one: halving the speed halves
	// the radius at a given lateral g, which nearly doubles the inner-to-outer wheel
	// speed difference the differential has to absorb - and a limited-slip diff
	// resists exactly that, adding the anti-yaw understeer the test is trying to
	// measure. Hold the gear and keep the radius large instead.
	if (Test == EVehicleBenchTest::Skidpad && SteerRampStartTime >= 0.f)
	{
		return;
	}

	// Nor once power is going in. This test wants full throttle from a low speed,
	// which in the lower gears means four or five shifts - and at 1.2 s masked
	// either side, one RWD run lost 344 samples, over half its power phase, to
	// gearchanges. Holding the gear costs the top of the rev range and keeps the
	// sweep continuous, which is the better trade when the sweep IS the measurement.
	// From the SETTLE window on, not just the power phase - otherwise the automatic
	// box simply undoes the gear the test just selected, before power ever goes in.
	if (Test == EVehicleBenchTest::PowerOnCornering && PowerOnSteerHeldTime >= 0.f)
	{
		return;
	}

	// Not while one is already running. With the clutch open mid-shift the engine is
	// driving nothing and flares straight to the limiter, which looks exactly like a
	// car that needs the next gear - so it takes it, flares again, and walks up the
	// whole gearbox. Stuck in top it never reaches the target speed, and the steer
	// ramp that waits on that speed never starts.
	if (DT->IsShifting())
	{
		return;
	}

	// Plain rev-band shifting. Wide enough either side that a gear change does not
	// immediately satisfy the opposite condition and send it hunting, and the
	// cooldown covers the rest while the driveline settles.
	// SHIFT POINTS RELATIVE TO THE ENGINE, not absolute rpm.
	//
	// These were 6200 and 1600, written when every car here revved to 7000 or 8000.
	// AOffroadCar redlines at 5200 * 0.95 = 4940 and could NEVER reach 6200, so it sat in
	// first gear on its limiter at 46 km/h while the skidpad waited for 50 - forever. The
	// test then reported an understeer gradient, a roll gradient and a peak grip of
	// exactly 0.000, which is what a regression over no samples returns.
	//
	// A fraction of each engine's own redline shifts every car just below its limiter,
	// whatever that limiter is.
	const float RedlineRPM = FMath::Max(Target->GetRedlineRPM(), 1000.f);
	const float RPM = Target->GetEngineRPM();
	if (RPM > RedlineRPM * 0.90f)
	{
		DT->GearUp();
		ShiftCooldown = 0.6f;
		LastShiftTime = PhaseSeconds;
	}
	else if (RPM < RedlineRPM * 0.25f && DT->CurrentGear > 1)
	{
		DT->GearDown();
		ShiftCooldown = 0.6f;
		LastShiftTime = PhaseSeconds;
	}
}

void AVehicleTestBench::SpeedController(float TargetKph, float& OutThrottle, float& OutBrake)
{
	const float Error = TargetKph - Target->GetForwardSpeedKph();
	const float Proportional = Error * 0.08f;

	// Integrate only while the throttle still has somewhere to go. Left running
	// through the launch - where the demand is saturated at full throttle for
	// several seconds - the term winds up enormous, and then has to be unwound by
	// overshooting the target just as far on the other side.
	const float Demand = Proportional + (SpeedIntegral * 0.05f);
	if (Demand > 0.f && Demand < 1.f)
	{
		SpeedIntegral += Error * GetWorld()->GetDeltaSeconds();
	}

	OutThrottle = FMath::Clamp(Proportional + (SpeedIntegral * 0.05f), 0.f, 1.f);
	OutBrake = FMath::Clamp(-Error * 0.05f, 0.f, 1.f);
}

bool AVehicleTestBench::DriveSkidpad(float Elapsed)
{
	// Hold the speed, wind the steer on. Sweeping with steer rather than with speed
	// is what makes this steady state: no acceleration, so no longitudinal load
	// transfer shifting the balance being measured, and the gearbox stays put.
	float Throttle, Brake;
	SpeedController(SkidpadSpeedKph, Throttle, Brake);

	const bool bAtSpeed = Target->GetForwardSpeedKph() > (SkidpadSpeedKph - 3.f);
	if (bAtSpeed && SteerRampStartTime < 0.f)
	{
		SteerRampStartTime = Elapsed;
	}

	// Straight until up to speed. Winding steer on during the launch would put the
	// low-g end of the sweep - exactly the linear range the gradient is fitted over
	// - inside the acceleration transient.
	const float Steer = (SteerRampStartTime < 0.f)
		? 0.f
		: FMath::Min((Elapsed - SteerRampStartTime) * SkidpadSteerRatePerSec, SkidpadMaxSteerInput);

	if (Steer != 0.f) { EnsureSteerLimitLocked(); }
	Target->ApplyDriverInput(Throttle, Brake, Steer);

	if (Samples.Num() > 0)
	{
		Samples.Last().SteerInput = Steer;
	}

	if (SteerRampStartTime < 0.f)
	{
		// Say WHY nothing is happening. A run that never gets up to speed steers not
		// at all and looks from the outside like broken steering, which is a long way
		// from where the fault actually is - so report the thing being waited on, and
		// enough of the powertrain to see what is holding it back.
		WaitReportTimer -= GetWorld()->GetDeltaSeconds();
		if (WaitReportTimer <= 0.f)
		{
			WaitReportTimer = 1.f;
			const UDrivetrain* DT = Target->GetDrivetrain();
			UE_LOG(LogVehicleBench, Warning,
				TEXT("   waiting for speed: %.1f / %.1f km/h  (gear %d, %.0f rpm, clutch %.2f%s)  t=%.1fs"),
				Target->GetForwardSpeedKph(), SkidpadSpeedKph,
				DT ? DT->CurrentGear : -1, Target->GetEngineRPM(),
				DT ? DT->GetClutchEngagement() : -1.f,
				(DT && DT->IsShifting()) ? TEXT(", SHIFTING") : TEXT(""),
				Elapsed);
		}

		// Give up rather than sit here forever if it cannot reach the target speed.
		if (Elapsed > 40.f)
		{
			UE_LOG(LogVehicleBench, Error,
				TEXT("   never reached %.1f km/h - topped out at %.1f. The steer sweep never "
					 "started, so this run measured a car driving in a straight line."),
				SkidpadSpeedKph, Target->GetForwardSpeedKph());
			return true;
		}
		return false;
	}

	// Peak tracking starts with the sweep. Launch throws a lateral spike - the RWD
	// run peaked at 0.29 g in its first moments and never bettered it - which would
	// set the benchmark before the test began and trip the plateau immediately.
	if (Samples.Num() > 0)
	{
		const float Ay = FMath::Abs(Samples.Last().LateralAccelG);
		if (Ay > BestLateralG + 0.005f)
		{
			BestLateralG = Ay;
			TimeOfBestG = Elapsed;
		}
	}

	// Grip has stopped improving: that is the limit, and the only thing the sweep
	// is climbing towards.
	const float SweepSeconds = Elapsed - SteerRampStartTime;
	const bool bGripPlateaued = SweepSeconds > 6.f && (Elapsed - TimeOfBestG) > 3.f;
	const bool bSweepFinished = Steer >= SkidpadMaxSteerInput && (Elapsed - TimeOfBestG) > 2.f;

	return bGripPlateaued || bSweepFinished;
}

bool AVehicleTestBench::DrivePowerOn(float Elapsed)
{
	// Four stages: reach entry speed, wind steer on, let the circle settle, then
	// freeze steer and put the power in. Only the fourth is measured.
	float Throttle, Brake;
	SpeedController(PowerOnEntrySpeedKph, Throttle, Brake);

	const bool bAtSpeed = Target->GetForwardSpeedKph() > (PowerOnEntrySpeedKph - 3.f);
	if (bAtSpeed && SteerRampStartTime < 0.f)
	{
		SteerRampStartTime = Elapsed;
	}

	if (SteerRampStartTime < 0.f)
	{
		Target->ApplyDriverInput(Throttle, Brake, 0.f);
		if (Samples.Num() > 0)
		{
			Samples.Last().SteerInput = 0.f;
		}

		WaitReportTimer -= GetWorld()->GetDeltaSeconds();
		if (WaitReportTimer <= 0.f)
		{
			WaitReportTimer = 1.f;
			const UDrivetrain* DT = Target->GetDrivetrain();
			UE_LOG(LogVehicleBench, Warning,
				TEXT("   waiting for entry speed: %.1f / %.1f km/h  (gear %d, %.0f rpm)  t=%.1fs"),
				Target->GetForwardSpeedKph(), PowerOnEntrySpeedKph,
				DT ? DT->CurrentGear : -1, Target->GetEngineRPM(), Elapsed);
		}
		if (Elapsed > 40.f)
		{
			UE_LOG(LogVehicleBench, Error,
				TEXT("   never reached %.1f km/h - topped out at %.1f. No circle, so no run."),
				PowerOnEntrySpeedKph, Target->GetForwardSpeedKph());
			return true;
		}
		return false;
	}

	// Steer winds on at the skidpad's rate until the car MEASURES the entry lateral
	// g, then freezes at whatever angle produced it. Everything past that point is
	// the car responding to power at a fixed hand position.
	//
	// Ramping to a fixed steer INPUT instead was the original design and it did not
	// survive contact: the input-to-angle mapping is a curve asset, so 0.35 put this
	// car at 0.47 g - above the fitted band's ceiling, from where power could only
	// climb further away - and the test returned no samples at all. Targeting a
	// measured g makes the entry mean the same thing on any car.
	float Steer = PowerOnFrozenSteer;
	if (PowerOnSteerHeldTime < 0.f)
	{
		Steer = FMath::Min(
			(Elapsed - SteerRampStartTime) * SkidpadSteerRatePerSec, PowerOnSteerInput);

		// Averaged: LateralAccelG is a frame difference and a single noisy sample
		// would freeze the steer early, at a circle tighter than asked for.
		float RecentAy = 0.f;
		int32 AyCount = 0;
		for (int32 i = Samples.Num() - 1; i >= 0 && AyCount < 5; --i, ++AyCount)
		{
			RecentAy += FMath::Abs(Samples[i].LateralAccelG);
		}
		RecentAy = AyCount > 0 ? RecentAy / AyCount : 0.f;

		const bool bReachedTarget = RecentAy >= PowerOnEntryLateralG;
		const bool bOutOfSteer = Steer >= PowerOnSteerInput;
		if (bReachedTarget || bOutOfSteer)
		{
			PowerOnSteerHeldTime = Elapsed;
			PowerOnFrozenSteer = Steer;

			// Chosen HERE rather than at power-on, so the settle window absorbs the
			// gearchange and the throttle ramp lands on a driveline that is already
			// closed up. Requested through TargetGear so it goes through the clutch
			// like any other shift instead of swapping ratios under load.
			if (UDrivetrain* DT = Target->GetDrivetrain())
			{
				DT->TargetGear = FMath::Clamp(PowerOnGear, 0, DT->GearRatios.Num() - 1);
			}

			UE_LOG(LogVehicleBench, Log,
				TEXT("   circle at %.3f g on %.3f steer input%s - selecting gear %d"),
				RecentAy, Steer,
				bOutOfSteer && !bReachedTarget
					? TEXT(" - STEER CAP HIT before the target lateral g") : TEXT(""),
				PowerOnGear);
		}
	}

	// Power waits for the circle to be steady. Landing the throttle ramp on top of
	// the steer transient would leave the two indistinguishable in the fit.
	if (PowerOnSteerHeldTime >= 0.f && PowerOnRampStartTime < 0.f
		&& (Elapsed - PowerOnSteerHeldTime) >= PowerOnSettleSeconds)
	{
		PowerOnRampStartTime = Elapsed;
		// Seed the dose controller with the throttle that was already holding the
		// circle steady. Starting it from zero would drop the throttle on the first
		// frame of the phase and put a deceleration transient into the very samples
		// the fit is made of.
		PowerOnThrottleIntegral = Throttle;
		PowerOnLastThrottle = Throttle;
		const UDrivetrain* DT = Target->GetDrivetrain();
		PowerOnHeldGear = DT ? DT->CurrentGear : -1;
		UE_LOG(LogVehicleBench, Log,
			TEXT("   circle steady at %.1f km/h, %.2f g lateral, gear %d at %.0f rpm - power on"),
			Target->GetForwardSpeedKph(),
			Samples.Num() > 0 ? FMath::Abs(Samples.Last().LateralAccelG) : 0.f,
			PowerOnHeldGear, Target->GetEngineRPM());
	}

	if (PowerOnRampStartTime < 0.f)
	{
		if (Steer != 0.f) { EnsureSteerLimitLocked(); }
		Target->ApplyDriverInput(Throttle, Brake, Steer);
		if (Samples.Num() > 0)
		{
			Samples.Last().SteerInput = Steer;
		}
		return false;
	}

	// The speed controller is OFF from here. A SECOND controller takes over and holds a
	// longitudinal ACCELERATION, so the car accelerates around the circle and lateral g
	// climbs with v^2 - which is what sweeps the fitted band without ever touching the
	// steering.
	//
	// Holding acceleration rather than throttle is the point of this phase. The gradient
	// it produces is only meaningful next to the skidpad's, and that subtraction assumes
	// every car got the same power; a throttle fraction does not deliver that and a
	// target acceleration does. See PowerOnTargetLongG.
	//
	// Shifts are NOT blocked, unlike the skidpad. Pulling from entry speed will need a
	// gear, and holding one would hit the limiter and cap the sweep before it reached
	// the top of the band. Samples either side of a shift are already flagged and
	// dropped from the fit, which is the right tool for it.
	const float SincePower = Elapsed - PowerOnRampStartTime;

	// The target ramps up from zero rather than starting at the dose: at zero the
	// controller settles on the throttle that holds speed, so the ramp runs continuously
	// from "steady circle" into "full dose" with no step at either end.
	const float TargetLongG = PowerOnTargetLongG * FMath::Min(
		SincePower / FMath::Max(PowerOnThrottleRampSeconds, KINDA_SMALL_NUMBER), 1.f);

	// MEASURED VALUE, SMOOTHED. LongAccelG is a frame difference of velocity against a
	// 200 Hz substep, so single samples carry aliasing spikes of a few tenths of a g -
	// the straight-line test smooths its peak for the same reason. Feeding that raw to
	// an integrator would make the throttle chatter and put the noise into the car.
	float MeasuredLongG = 0.f;
	int32 AccelCount = 0;
	for (int32 i = Samples.Num() - 1; i >= 0 && AccelCount < PeakAccelSmoothingSamples; --i)
	{
		MeasuredLongG += Samples[i].LongAccelG;
		++AccelCount;
	}
	MeasuredLongG = AccelCount > 0 ? MeasuredLongG / AccelCount : 0.f;

	// A gearshift cuts torque, so the measured value collapses for reasons that have
	// nothing to do with the pedal. Integrating through that would wind the throttle up
	// during the shift and slam it in when the clutch re-engages - the controller would
	// be manufacturing exactly the transient the shift mask exists to remove. Hold the
	// last output instead and let the mask drop those samples.
	const bool bShifting = Samples.Num() > 0 && Samples.Last().bShiftSettling;

	float PowerThrottle = PowerOnLastThrottle;
	if (!bShifting)
	{
		const float Dt = GetWorld()->GetDeltaSeconds();
		const float Error = TargetLongG - MeasuredLongG;

		// Conditional integration: accumulate only when the output is not already
		// pinned against a limit in the direction the error pushes. Without it a car
		// that cannot make the target winds the integral up for the whole hold and then
		// cannot come off the throttle at the point where it finally could.
		const bool bPinnedHigh =
			PowerOnLastThrottle >= PowerOnMaxThrottle - KINDA_SMALL_NUMBER && Error > 0.f;
		const bool bPinnedLow = PowerOnLastThrottle <= KINDA_SMALL_NUMBER && Error < 0.f;
		if (!bPinnedHigh && !bPinnedLow)
		{
			PowerOnThrottleIntegral += Error * Dt * PowerOnDoseGainI;
			PowerOnThrottleIntegral = FMath::Clamp(PowerOnThrottleIntegral, 0.f, PowerOnMaxThrottle);
		}

		PowerThrottle = FMath::Clamp(PowerOnThrottleIntegral + Error * PowerOnDoseGainP,
			0.f, PowerOnMaxThrottle);
		PowerOnLastThrottle = PowerThrottle;

		// Counted only once the ramp is over, where the target is the full dose and
		// saturation therefore means the car ran out of engine rather than that the
		// controller is still on its way up.
		if (SincePower >= PowerOnThrottleRampSeconds)
		{
			++PowerOnDoseSamples;
			if (PowerThrottle >= PowerOnMaxThrottle - KINDA_SMALL_NUMBER)
			{
				++PowerOnSaturatedSamples;
			}
		}
	}

	EnsureSteerLimitLocked();
	Target->ApplyDriverInput(PowerThrottle, 0.f, Steer);
	if (Samples.Num() > 0)
	{
		Samples.Last().SteerInput = Steer;
	}

	return SincePower >= (PowerOnThrottleRampSeconds + PowerOnHoldSeconds);
}

bool AVehicleTestBench::DriveStepSteer(float Elapsed)
{
	float Throttle, Brake;
	SpeedController(StepSteerSpeedKph, Throttle, Brake);

	const bool bAtSpeed = FMath::Abs(Target->GetForwardSpeedKph() - StepSteerSpeedKph) < 3.f;
	const float Steer = (bAtSpeed || Elapsed > 12.f) ? StepSteerInput : 0.f;

	// Mark where the step happened so the measurement can find it.
	if (Samples.Num() > 0)
	{
		Samples.Last().SteerInput = Steer;
	}

	if (true) { EnsureSteerLimitLocked(); }
	Target->ApplyDriverInput(Throttle, Brake, Steer);

	// End a fixed hold after the first steered sample.
	for (int32 i = 0; i < Samples.Num(); ++i)
	{
		if (Samples[i].SteerInput > KINDA_SMALL_NUMBER)
		{
			return (Elapsed - Samples[i].Time) >= StepSteerHoldSeconds;
		}
	}
	return Elapsed > 25.f;
}

void AVehicleTestBench::EnsureSteerLimitLocked()
{
	// Called the first time a test commands steer, by which point every steering test
	// here has already brought the car to its entry speed. Freezing then means the angle
	// the input asks for is the angle the wheels hold, whatever the car does to its speed
	// afterwards - which is the assumption behind 'steer hold' ever reading near zero.
	if (Target != nullptr && !Target->IsSteerLimitLocked())
	{
		Target->LockSteerLimitAtCurrentSpeed();
		UE_LOG(LogVehicleBench, Log,
			TEXT("   steer limit frozen at entry speed - a constant input is now a constant angle"));
	}
}

bool AVehicleTestBench::DriveArticulation(float Elapsed)
{
	float Throttle, Brake;
	SpeedController(ArticulationApproachKph, Throttle, Brake);
	Target->ApplyDriverInput(Throttle, Brake, 0.f);
	return DistanceTravelledM > 22.f || Elapsed > 30.f;
}

bool AVehicleTestBench::DriveStraightLine(float Elapsed)
{
	// Full throttle to the target, then straight onto the brakes.
	bool bReached = false;
	for (const FSample& S : Samples)
	{
		if (S.SpeedMPerSec * 3.6f >= StraightLineTargetKph) { bReached = true; break; }
	}
	if (!bReached)
	{
		Target->ApplyDriverInput(1.f, 0.f, 0.f);
		return Elapsed > 60.f;
	}
	Target->ApplyDriverInput(0.f, 1.f, 0.f);
	return Target->GetForwardSpeedKph() < 1.f || Elapsed > 90.f;
}

// ---------------------------------------------------------------------------
// Static kinematics
// ---------------------------------------------------------------------------

void AVehicleTestBench::MeasureKinematics(FBenchRun& Run) const
{
	USkeletalMeshComponent* Mesh = Target->GetCarMesh();
	const TArray<FCornerAssembly>& Corners = Target->GetCorners();
	if (Mesh == nullptr || Corners.Num() < 4)
	{
		return;
	}

	const FTransform Body = Mesh->GetComponentTransform();
	const float SprungKg = SprungMassKg();

	// Track and wheelbase are MEASURED off the corners rather than read from ACar.
	// Both are load-bearing here - roll stiffness goes as track squared - so taking
	// them from a cached field that can legitimately still be zero would turn a
	// setup problem into a silently wrong report rather than a visible one.
	float MeasuredTrackCm = 0.f;
	float MeasuredWheelbaseCm = 0.f;
	if (Corners[0].Suspension && Corners[1].Suspension && Corners[2].Suspension)
	{
		MeasuredTrackCm = FMath::Abs(Corners[0].Suspension->GetWheelCentreRest().Y
		                           - Corners[1].Suspension->GetWheelCentreRest().Y);
		MeasuredWheelbaseCm = FMath::Abs(Corners[0].Suspension->GetWheelCentreRest().X
		                               - Corners[2].Suspension->GetWheelCentreRest().X);
	}
	const float TrackM = MeasuredTrackCm * BenchCmToM;

	// Per-axle roll stiffness, accumulated as the corners are walked.
	float AxleRollStiffness[2] = { 0.f, 0.f };   // 0 = front, 1 = rear, N*m/rad

	// THE SAME THING WITH THE TYRE IN SERIES, kept separately on purpose.
	//
	// The shares below - front bias, and how much of it the bars provide - describe the
	// SETUP, so they stay suspension-only and keep meaning what a setup sheet means by
	// them. The roll ANGLE is a different question: the tyre is a spring too, and it
	// carries every newton the suspension does, so it belongs in that one.
	float AxleRollStiffnessWithTyre[2] = { 0.f, 0.f };
	float AxleRollCentreCm[2] = { 0.f, 0.f };
	int32 AxleCount[2] = { 0, 0 };

	for (int32 i = 0; i < 4; ++i)
	{
		const UWishboneSuspension* S = Corners[i].Suspension;
		if (S == nullptr) { continue; }

		const int32 Axle = (i < 2) ? 0 : 1;
		const FString Tag = FString(CornerName(i));

		// --- sweep the corner through its travel and differentiate ---------------
		TArray<float> TravelCm, CamberDeg, ToeDeg, StrutCm;
		const int32 Steps = 21;
		for (int32 k = 0; k < Steps; ++k)
		{
			const float Fraction = -1.f + (2.f * k / (Steps - 1));      // -1 droop .. +1 bump
			const float Angle = S->TravelFractionToAngleDeg(Fraction);
			const FWishbonePose P = S->GetPose(Angle, 0.f);

			const FVector Spindle = Body.InverseTransformVector(P.SpindleAxis);
			const FVector Damper = Body.InverseTransformPosition(P.DamperMount);
			const FVector Top = Body.InverseTransformPosition(P.CoiloverTop);

			TravelCm.Add(S->GetWheelTravelAtAngleCm(Angle));
			CamberDeg.Add(CamberFromSpindle(Spindle));
			ToeDeg.Add(ToeFromSpindle(Spindle, S->IsMirrored()));
			StrutCm.Add(FVector::Dist(Damper, Top));
		}

		// Motion ratio measured rather than recomputed: strut length per unit wheel
		// height IS the definition, and taking it off the same pose the sim uses
		// means the number cannot drift from the model.
		const float MotionRatio = FMath::Abs(Slope(TravelCm, StrutCm));

		// Endpoint deltas, NOT a least-squares slope. On this linkage camber against
		// travel is very nearly a parabola centred on rest - both arms are level and
		// their hinges share a lateral position, which leaves only a second-order
		// lateral term - and a straight line through a symmetric parabola has a slope
		// of about zero however much camber the corner actually gains. Reporting the
		// fit would hide the curve behind a plausible-looking 0.00.
		const int32 Mid = Steps / 2;
		const float CamberAtBump = CamberDeg.Last() - CamberDeg[Mid];
		const float CamberAtDroop = CamberDeg[0] - CamberDeg[Mid];
		const float ToeAtBump = ToeDeg.Last() - ToeDeg[Mid];
		const float TravelSpanCm = TravelCm.Last() - TravelCm[0];

		// --- rates ---------------------------------------------------------------
		const float WheelRateNPerM = S->GetSpringRateNPerM() * MotionRatio * MotionRatio;
		// THE CORNER'S OWN MASS, weighed, not a quarter of the car.
		//
		// This was SprungKg * 0.25, which is only right on a 50/50 car - and the saloon
		// is 50/50, so nothing showed it. On ARallyCar at 56/44 it reported the REAR
		// ride frequency below the front when the car is designed the other way round,
		// because with equal assumed masses the axle with the softer spring simply reads
		// softer. The whole point of a flat-ride split is that the rear is stiffer AND
		// lighter; assuming the masses equal throws half of it away.
		//
		// The settle phase already weighs each corner. Subtracting the unsprung mass
		// leaves what the spring is actually holding up, which is the mass in
		// f = sqrt(k/m)/2pi and in the damping ratio underneath it.
		float CornerMassKg = FMath::Max(SprungKg * 0.25f, 1.f);
		if (bSettledLoadValid && SettledLoadN[i] > KINDA_SMALL_NUMBER)
		{
			CornerMassKg = FMath::Max(
				(SettledLoadN[i] / GravityMPerSec2) - S->GetUnsprungMassKg(), 1.f);
		}
		const float RideHz = FMath::Sqrt(WheelRateNPerM / CornerMassKg) / (2.f * PI);
		const float CriticalDamping = 2.f * FMath::Sqrt(WheelRateNPerM * CornerMassKg);
		const float DampingRatio = CriticalDamping > KINDA_SMALL_NUMBER
			? (S->GetDamperRateNsPerM() * MotionRatio * MotionRatio) / CriticalDamping : 0.f;

		const float CornerRateNPerM = WheelRateNPerM + S->GetAntiRollRateNPerM();
		AxleRollStiffness[Axle] += 0.5f * CornerRateNPerM * TrackM * TrackM * 0.5f;

		// THE TYRE IS A SPRING, AND IT IS IN SERIES WITH THE SUSPENSION.
		//
		// Everything the spring and bar push against the road goes through the sidewall
		// first, so the two stack the way series springs do - and the combination is
		// softer than either. Leaving it out made this test report 2.740 deg/g on a car
		// the skidpad measured rolling at 4.245: a 55 per cent error, in the direction
		// that flatters the setup, which is the worst direction for a number somebody
		// tunes against.
		//
		// The rate comes from the tyre's own vertical coefficients rather than a constant.
		// Book (4.E68) linearised at small deflection is dFz/drho_z = qFz1 * Fz0 / R0,
		// which for the shipped road tyre is about 136 kN/m against a 59 kN/m front wheel
		// rate - the same order as the suspension, not a rounding error.
		float CornerWithTyre = CornerRateNPerM;
		if (const UPacejkaTyreComponent* Tyre = Corners[i].Tyre)
		{
			if (const UTireParamsDataAsset* Params = Tyre->GetTireParams())
			{
				const float TyreRateNPerM = (Params->R0 > KINDA_SMALL_NUMBER)
					? (Params->qFz1 * Params->FZ0 / Params->R0) : 0.f;
				if (TyreRateNPerM > KINDA_SMALL_NUMBER)
				{
					CornerWithTyre = (CornerRateNPerM * TyreRateNPerM)
						/ (CornerRateNPerM + TyreRateNPerM);
				}
			}
		}
		AxleRollStiffnessWithTyre[Axle] += 0.5f * CornerWithTyre * TrackM * TrackM * 0.5f;
		++AxleCount[Axle];

		// --- roll centre, from the front-view instant centre ---------------------
		const FWishbonePose Rest = S->GetPose(0.f, 0.f);
		const FVector L0 = Body.InverseTransformPosition(Rest.LcaFront);
		const FVector L1 = Body.InverseTransformPosition(Rest.LcaRear);
		const FVector U0 = Body.InverseTransformPosition(Rest.UcaFront);
		const FVector U1 = Body.InverseTransformPosition(Rest.UcaRear);
		const FVector LBJ = Body.InverseTransformPosition(Rest.LowerBallJoint);
		const FVector UBJ = Body.InverseTransformPosition(Rest.UpperBallJoint);
		const FVector WC = Body.InverseTransformPosition(Rest.WheelCentre);

		const FVector PivotL = PointOnLineAtX(L0, L1, WC.X);
		const FVector PivotU = PointOnLineAtX(U0, U1, WC.X);

		FVector2D IC;
		float RollCentreCm = 0.f;
		bool bHasRollCentre = IntersectYZ(
			FVector2D(PivotL.Y, PivotL.Z), FVector2D(LBJ.Y, LBJ.Z),
			FVector2D(PivotU.Y, PivotU.Z), FVector2D(UBJ.Y, UBJ.Z), IC);
		if (bHasRollCentre)
		{
			// Contact patch, then the line from it through the instant centre; where
			// that crosses the centreline is the roll centre.
			const FVector2D Patch(WC.Y, WC.Z - S->GetWheelRadiusCm());
			const FVector2D D = IC - Patch;
			if (FMath::Abs(D.X) > KINDA_SMALL_NUMBER)
			{
				// ABOVE THE CONTACT PATCH, not in body space. It was reported in body
				// coordinates and checked against a range quoted above ground, so a
				// perfectly good roll centre read as 6 cm underground and was flagged on
				// every corner of every car.
				//
				// Subtracting Patch.Y turns the height at the centreline into a height
				// above the ground, and the expression collapses to the rise along the
				// patch-to-instant-centre line. ASaloonCar documents +5.9 cm front;
				// body space reported -5.975, and the patch sits at -11.863, which is
				// the same number seen from the wrong origin.
				//
				// Note the lateral scale cancels here - (-k*Patch.X)/(k*D.X) - so two
				// cars whose geometry differs only by track report the SAME roll centre.
				// That is correct and worth knowing before it reads as a stuck value.
				RollCentreCm = D.Y * ((0.f - Patch.X) / D.X);
				AxleRollCentreCm[Axle] += RollCentreCm;
			}
			else
			{
				bHasRollCentre = false;
			}
		}

		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" motion ratio"), MotionRatio, TEXT(""),
			0.4f, 1.0f, TEXT("double wishbone 0.5-0.8")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" wheel rate"), WheelRateNPerM * 0.001f, TEXT("N/mm"),
			15.f, 60.f, TEXT("road car 15-40, sports 30-60")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" ride frequency"), RideHz, TEXT("Hz"),
			1.0f, 2.2f, TEXT("road 1.0-1.5, sports 1.5-2.2, race 2.5+")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" damping ratio"), DampingRatio, TEXT(""),
			0.2f, 0.7f, TEXT("road 0.2-0.4, race 0.6-0.7")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" camber @ full bump"), CamberAtBump, TEXT("deg"),
			-3.5f, -0.5f, TEXT("SLA gains 1-3 deg negative over full bump; long travel goes NON-LINEAR")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" camber @ full droop"), CamberAtDroop, TEXT("deg"),
			1.f, 0.f, FString(), TEXT("same sign as the bump figure means the curve is symmetric - second-order gain only")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" toe @ full bump"), ToeAtBump, TEXT("deg"),
			-0.3f, 0.3f, TEXT("bump steer; want near zero")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" travel swept"), TravelSpanCm, TEXT("cm"),
			10.f, 45.f, TEXT("road car 15-25, rally 26-30, long-travel 4x4 38-45")));
		// WHERE THE CAR IS ACTUALLY SITTING, which every reading above quietly depends on.
		//
		// Static camber, and the live camber generally, is whatever the corner reads at
		// the pose it happens to be in - so it only matches StaticCamberDeg if the car is
		// resting at the DESIGN position. Every geometry figure in this report is
		// measured there: the roll centre, the camber curve, the motion ratio. A car
		// resting somewhere else is running geometry nobody specified, and until this
		// line existed there was no way to tell from the report which case you were in.
		//
		// It is set by the spring, not the geometry. The coil is squeezed by exactly
		// PreloadForceN at design, so preload zero means the spring holds nothing there
		// and the car has to sink until it makes its own corner load.
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" rest offset from design"),
			S->GetWheelTravelCm(), TEXT("cm"), -2.f, 2.f,
			TEXT("+ is compressed; measured once the ride height has stopped moving")));
		// THE ANSWER, not just the symptom. The offset above says the corner is not at its
		// design height; this says what to do about it, because every term is already
		// here. Sag is the corner load over the WHEEL rate and preload acts through the
		// MOTION ratio, so the shortfall in spring force is:
		//
		//     offset * wheel rate / motion ratio
		//
		// Add it to PreloadForceN and the corner parks at design. It reads zero when the
		// car is already there, which is the point - a tuning report should say what to
		// change, not leave arithmetic for the reader.
		const float PreloadCorrectionN = MotionRatio > KINDA_SMALL_NUMBER
			? (S->GetWheelTravelCm() * BenchCmToM * WheelRateNPerM) / MotionRatio
			: 0.f;
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" preload correction"), PreloadCorrectionN,
			TEXT("N"), -120.f, 120.f,
			TEXT("ADD this to PreloadForceN to park the corner at design height")));
		Run.Metrics.Add(FBenchMetric(Tag + TEXT(" static camber"), S->GetCamberDeg(), TEXT("deg"),
			-3.5f, 0.5f, TEXT("road -0.5 to -1.5, track -2 to -3.5")));

		if (bHasRollCentre)
		{
			Run.Metrics.Add(FBenchMetric(Tag + TEXT(" roll centre height"), RollCentreCm, TEXT("cm"),
				0.f, 15.f, TEXT("road car 2.5-12.5 cm above ground")));
		}
		else
		{
			Run.Metrics.Add(FBenchMetric(Tag + TEXT(" roll centre height"), 0.f, TEXT("cm"),
				1.f, 0.f, FString(), TEXT("arms are parallel - instant centre at infinity, no roll centre")));
		}
	}

	// --- whole-vehicle roll behaviour -------------------------------------------
	const float FrontRC = AxleCount[0] ? AxleRollCentreCm[0] / AxleCount[0] : 0.f;
	const float RearRC = AxleCount[1] ? AxleRollCentreCm[1] / AxleCount[1] : 0.f;

	// The ground plane, in body space: where the front tyre touches at design height.
	// Everything below is quoted against it, which is the only frame in which "how high
	// is it" means anything to a reader.
	const UWishboneSuspension* FrontForGround = Corners[0].Suspension;
	const float GroundZ = FrontForGround
		? Body.InverseTransformPosition(FrontForGround->GetPose(0.f, 0.f).WheelCentre).Z
			- FrontForGround->GetWheelRadiusCm()
		: 0.f;

	float ComHeightCm = 50.f;
	if (FBodyInstance* BI = Mesh->GetBodyInstance())
	{
		ComHeightCm = Body.InverseTransformPosition(BI->GetCOMPosition()).Z - GroundZ;
	}

	const float RollArmM = FMath::Max((ComHeightCm - (0.5f * (FrontRC + RearRC))) * BenchCmToM, 0.01f);
	const float TotalRollStiffness = AxleRollStiffness[0] + AxleRollStiffness[1];
	const float TotalRollStiffnessWithTyre = AxleRollStiffnessWithTyre[0] + AxleRollStiffnessWithTyre[1];

	// K MINUS THE ROLL MOMENT, not K alone.
	//
	// Rolling moves the sprung mass outboard, which lengthens its own lever and adds to
	// the moment that is rolling it - the body works partly against itself, and the
	// equilibrium is M / (K - M) rather than M / K. Dividing by stiffness alone treats
	// the car as if the mass stayed put and under-reports the angle; here it was worth
	// about 0.27 deg/g on its own.
	const float RollMomentPerG = SprungKg * GravityMPerSec2 * RollArmM;
	const float RollDenominator = TotalRollStiffnessWithTyre - RollMomentPerG;
	const float RollGradient = RollDenominator > KINDA_SMALL_NUMBER
		? FMath::RadiansToDegrees(RollMomentPerG / RollDenominator) : 0.f;
	const float FrontShare = TotalRollStiffness > KINDA_SMALL_NUMBER
		? 100.f * AxleRollStiffness[0] / TotalRollStiffness : 50.f;

	Run.Metrics.Add(FBenchMetric(TEXT("CoG height"), ComHeightCm, TEXT("cm"),
		35.f, 85.f, TEXT("sports 40-50, saloon 50-60, rally 52-58, 4x4 65-80")));
	// NAMED "predicted", because Skidpad reports a "roll gradient (measured)" from a car
	// actually cornering. Two tests reporting the same name by different methods is how a
	// reader ends up trusting whichever they saw first; the pair is a useful cross-check
	// only if it is obvious which is which.
	Run.Metrics.Add(FBenchMetric(TEXT("predicted roll gradient"), RollGradient, TEXT("deg/g"),
		2.f, 10.f, TEXT("sports 3-5, road 5-7, soft SUV 8-10; cf. Skidpad's measured value")));
	Run.Metrics.Add(FBenchMetric(TEXT("front roll-stiffness share"), FrontShare, TEXT("%"),
		50.f, 70.f, TEXT("front-biased understeers; 50% is neutral")));

	// ---- the chassis collision itself ---------------------------------------------
	//
	// NONE of this comes from the plugin. The shapes live in the skeletal mesh's
	// physics asset, and the CoG above is wherever those shapes put it - so a car can
	// have perfectly derived spring rates and still roll like something else entirely
	// because its collision is the wrong size or in the wrong place.
	//
	// It is measured here because it is invisible otherwise. Two cars sharing one
	// skeletal mesh share one physics asset and therefore report an IDENTICAL CoG
	// height, to every decimal, no matter how differently their masses are set up.
	// That is worth seeing rather than discovering.
	//
	// A physics asset can be overridden per component - USkinnedMeshComponent has
	// PhysicsAssetOverride - so two cars can differ here without duplicating the mesh.
	//
	// GROUND CLEARANCE is the one to read first: the gap between the bottom of the
	// collision and the ground the tyres stand on. Negative means the chassis is inside
	// the road at design height, and the car rests on its box rather than its tyres.
	{
		// THE SCALE HAS TO BE PUT BACK BY HAND, AND IT IS NOT THE COMPONENT'S.
		//
		// Physics never carries scale on a transform - PhysX bakes it into the geometry
		// when the body is built - so GetUnrealWorldTransform() reports 1 and the shapes
		// keep their authored dimensions. Read them straight and a 250 cm chassis
		// measures 2.5.
		//
		// The scale that was baked in comes from the BONE transform, not the component:
		// USkeletalMeshComponent builds its bodies from the bone, which carries the
		// reference skeleton's scale as well as the component's. This mesh is authored
		// in metres with the hundredfold on its root bone, so the component honestly
		// reports 1 while the shapes are a hundred times bigger than they look.
		//
		// Believing the component here reads a real 250 x 122 x 50 cm chassis as a 2.5 cm
		// sugar cube. Car.cpp::ShapeScaleForBone exists for exactly this and does the
		// same lookup.
		FBox Chassis(ForceInit);
		FVector ShapeScale = Mesh->GetComponentTransform().GetScale3D();
		for (const FBodyInstance* BI : Mesh->Bodies)
		{
			const UBodySetup* BS = BI ? BI->GetBodySetup() : nullptr;
			if (BS == nullptr) { continue; }

			const int32 BoneIndex = BS->BoneName.IsNone() ? INDEX_NONE : Mesh->GetBoneIndex(BS->BoneName);
			if (BoneIndex != INDEX_NONE)
			{
				ShapeScale = Mesh->GetBoneTransform(BoneIndex).GetScale3D();
			}

			// CalcAABB rather than reading BoxElems, so a hull or a capsule counts too.
			// It scales the shape's own offset as well as its extent, which is what the
			// baked geometry did.
			FTransform ToBody = BI->GetUnrealWorldTransform() * Body.Inverse();
			ToBody.SetScale3D(ShapeScale);
			Chassis += BS->AggGeom.CalcAABB(ToBody);
		}

		UE_LOG(LogVehicleBench, Log,
			TEXT("  chassis collision: %d bodies, shapes built at bone scale %s ")
			TEXT("(component scale is %s - not the same thing)"),
			Mesh->Bodies.Num(), *ShapeScale.ToCompactString(),
			*Mesh->GetComponentTransform().GetScale3D().ToCompactString());

		if (Chassis.IsValid)
		{
			const FVector Size = Chassis.GetSize();
			Run.Metrics.Add(FBenchMetric(TEXT("chassis ground clearance"),
				Chassis.Min.Z - GroundZ, TEXT("cm"),
				6.f, 40.f, TEXT("sports 8-12, road 12-20, rally 18-22, 4x4 28-35; NEGATIVE is in the road")));
			Run.Metrics.Add(FBenchMetric(TEXT("chassis collision height"), Size.Z, TEXT("cm"),
				90.f, 200.f, TEXT("a car is 130-150 cm tall, a 4x4 170-200; a slab has too little roll inertia")));
			Run.Metrics.Add(FBenchMetric(TEXT("chassis collision width"), Size.Y, TEXT("cm"),
				150.f, 235.f, TEXT("should exceed the track, since the body is wider than the tyres")));
			Run.Metrics.Add(FBenchMetric(TEXT("chassis collision length"), Size.X, TEXT("cm"),
				380.f, 520.f, TEXT("wheelbase plus overhangs, so well over the wheelbase")));
		}
	}

	// ---- rotational inertia -------------------------------------------------------
	//
	// The number the collision shapes exist FOR, and the one nothing here could see
	// until now. Springs and dampers set how fast the body returns; inertia sets how
	// hard it is to start moving in the first place, and no amount of damper tuning
	// substitutes for it. A single box the size of a chassis carries roughly 40% of a
	// real car's inertia on all three axes - it rotates too eagerly in yaw and snaps in
	// roll and pitch - and that is invisible in every other metric in this report.
	//
	// THE FRAME IS THE WHOLE DIFFICULTY. GetBodyInertiaTensor returns the PRINCIPAL
	// moments - the diagonal in mass space - not the body-space tensor. Those frames are
	// only the same when the products of inertia vanish, and on a car they do not: mass
	// is low at the nose and high at the tail, so Ixz is non-zero and the principal axes
	// sit rotated about Y. Read the diagonal straight and roll and yaw are quietly mixed.
	//
	// Rebuilding it as the sum of lambda_k * e_k * e_k^T puts it back in body space with
	// no matrix-convention guesswork: the principal axes expressed in the body frame are
	// exactly the columns of the mass-space rotation.
	if (FBodyInstance* BI = Mesh->GetBodyInstance())
	{
		const FVector Principal = BI->GetBodyInertiaTensor();      // kg*cm^2, mass space
		const FQuat MassRot = BI->GetMassSpaceLocal().GetRotation();
		const FVector Axis[3] = { MassRot.GetAxisX(), MassRot.GetAxisY(), MassRot.GetAxisZ() };

		double Diag[3] = { 0.0, 0.0, 0.0 };
		for (int32 k = 0; k < 3; ++k)
		{
			const FVector& E = Axis[k];
			const double Lambda = static_cast<double>(Principal[k]);
			for (int32 i = 0; i < 3; ++i)
			{
				Diag[i] += Lambda * E[i] * E[i];
			}
		}

		// kg*cm^2 to kg*m^2. Unreal carries mass in kg and length in centimetres, so the
		// tensor comes back a factor of 100^2 larger than the SI figure every published
		// vehicle number is quoted in.
		const double ToSI = 1.0 / 10000.0;
		const float Ixx = static_cast<float>(Diag[0] * ToSI);
		const float Iyy = static_cast<float>(Diag[1] * ToSI);
		const float Izz = static_cast<float>(Diag[2] * ToSI);
		const float MassKg = FMath::Max(BI->GetBodyMass(), 1.f);

		Run.Metrics.Add(FBenchMetric(TEXT("roll inertia Ixx"), Ixx, TEXT("kg m^2"),
			300.f, 1200.f, TEXT("sports 380-420, saloon 450-600, 4x4 850-1000; a slab reads far under")));
		Run.Metrics.Add(FBenchMetric(TEXT("pitch inertia Iyy"), Iyy, TEXT("kg m^2"),
			1400.f, 5000.f, TEXT("sports ~1830, saloon 2200-2600, 4x4 4000-4600")));
		Run.Metrics.Add(FBenchMetric(TEXT("yaw inertia Izz"), Izz, TEXT("kg m^2"),
			1400.f, 5500.f, TEXT("sports ~2050, saloon 2200-2800, 4x4 4400-5000")));

		// The absolute figures above only mean anything against a car of the same mass
		// and size, so these two carry the check across vehicles.
		//
		// DYNAMIC INDEX, Iyy / (m*a*b), where a and b run from the CoG to each axle. At
		// exactly 1.0 the front and rear suspensions are dynamically DECOUPLED - a bump
		// at one axle produces no motion at the other - and road cars sit near it. Below
		// 1 the ends fight each other; well above, the car pitches about a point outside
		// the wheelbase.
		const FVector ComBody = Body.InverseTransformPosition(BI->GetCOMPosition());
		float FrontX = 0.f, RearX = 0.f;
		if (Corners.Num() >= 4 && Corners[0].Suspension && Corners[2].Suspension)
		{
			FrontX = Corners[0].Suspension->GetWheelCentreRest().X;
			RearX = Corners[2].Suspension->GetWheelCentreRest().X;
		}
		const float ACm = FMath::Abs(FrontX - ComBody.X);
		const float BCm = FMath::Abs(ComBody.X - RearX);
		const float WheelbaseM = FMath::Max((ACm + BCm) * BenchCmToM, 0.01f);
		const float ABM2 = FMath::Max(ACm * BenchCmToM * BCm * BenchCmToM, 0.0001f);

		Run.Metrics.Add(FBenchMetric(TEXT("dynamic index Iyy/(m.a.b)"), Iyy / (MassKg * ABM2),
			TEXT(""), 0.75f, 1.15f,
			TEXT("1.0 decouples the axles; road cars sit near it")));

		// Yaw radius of gyration over the wheelbase. Dimensionless, so it compares a
		// hatchback to a limousine, and it is the honest test of whether the mass is
		// spread along the car or bunched in the middle.
		Run.Metrics.Add(FBenchMetric(TEXT("yaw gyration / wheelbase"),
			FMath::Sqrt(Izz / MassKg) / WheelbaseM, TEXT(""), 0.40f, 0.55f,
			TEXT("passenger cars 0.45-0.50")));
	}

	// The bar's own contribution, which is the number the sweep is really about.
	float BarOnly = 0.f;
	for (int32 i = 0; i < 4; ++i)
	{
		if (const UWishboneSuspension* S = Corners[i].Suspension)
		{
			BarOnly += 0.5f * S->GetAntiRollRateNPerM() * TrackM * TrackM * 0.5f;
		}
	}
	Run.Metrics.Add(FBenchMetric(TEXT("roll stiffness from bars"),
		TotalRollStiffness > KINDA_SMALL_NUMBER ? 100.f * BarOnly / TotalRollStiffness : 0.f, TEXT("%"),
		1.f, 0.f, FString(), TEXT("share of total roll stiffness the anti-roll bars provide")));
}

// ---------------------------------------------------------------------------
// Dynamic measurements
// ---------------------------------------------------------------------------

void AVehicleTestBench::MeasureSkidpad(FBenchRun& Run) const
{
	const float WheelBaseM = Target->GetWheelBaseCm() * BenchCmToM;
	TArray<float> Ay, SteerMinusAckermann, Roll, BandSpeedKph;
	float PeakAy = 0.f;
	int32 ShiftMasked = 0;

	// THE LINEAR RANGE IS A FRACTION OF THE CAR'S OWN GRIP, not a fixed lateral g.
	//
	// A fixed ceiling is wrong in both directions, and this bench had it wrong both ways
	// in turn. At 0.40 g it CLIPPED the grippy cars: ASportsCar peaks at 1.158 g, its
	// gradient is still falling at 0.4, and fitting there fitted the curved part and
	// nothing else - R^2 0.481. Raised to 0.60 g it then over-ran the low-grip ones:
	// ARallyCar peaks at 0.625 g, so 0.60 is 96% of everything it has, and the fit ran
	// straight through the saturation at the limit. Its gradient came back 12.956 deg/g.
	//
	// 0.60 g is mid-corner for one car and falling-off-the-end for another. What they have
	// in common is a linear range occupying roughly the same FRACTION of their grip, so
	// the band is taken from the peak this run actually measured.
	//
	// The absolute floor stays a floor rather than scaling. Below about a tenth of a g the
	// signal is smaller than the car's static toe whatever its grip is, and scaling that
	// down with grip would push a low-grip car straight back into the noise.
	float PeakForBand = 0.f;
	for (const FSample& PS : Samples)
	{
		if (FMath::Abs((PS.SpeedMPerSec * 3.6f) - SkidpadSpeedKph) <= 3.f)
		{
			PeakForBand = FMath::Max(PeakForBand, FMath::Abs(PS.LateralAccelG));
		}
	}
	const float BandLoG = FMath::Max(UndersteerLinearRangeMinG, 0.15f * PeakForBand);
	const float BandHiG = PeakForBand > KINDA_SMALL_NUMBER
		? FMath::Min(UndersteerLinearRangeG, 0.55f * PeakForBand)
		: UndersteerLinearRangeG;

	for (const FSample& S : Samples)
	{
		// SPEED FIRST, BEFORE THE SHIFT BRANCH, and the order is the whole point.
		//
		// Below walking pace the yaw rate is noise and the radius meaningless.
		if (S.SpeedMPerSec < 4.f || FMath::Abs(S.YawRateDegPerSec) < 1.f) { continue; }

		// Only samples taken AT the hold speed.
		//
		// This is a constant-speed test, so a sample that is not at the speed is not
		// part of it. That matters at the end of a run that breaks away: the car slows
		// as it spins down and on the way past it crosses the fitted lateral-g band
		// again at 15-38 km/h, so the DESCENDING tail of a spin lands in the same bin
		// as the ascending sweep. One RWD run held 49.9 km/h for eighteen seconds and
		// still reported 7.7 km/h of spread inside the band, with an R-squared of
		// 0.002, entirely from the few seconds after it let go.
		//
		// This guard used to sit BELOW the shift branch, which let a shift-settling
		// sample reach the peak grip without ever being speed-checked. No run in the
		// fleet currently exercises that - moving it changed no measured number - so
		// this is a latent hole rather than a live bug, closed while it was in view.
		if (FMath::Abs((S.SpeedMPerSec * 3.6f) - SkidpadSpeedKph) > 3.f) { continue; }

		// Peak grip still counts these - a shift does not make the tyre grip less -
		// but the gradient must not, since the yaw it produces is the gearbox talking.
		if (S.bShiftSettling)
		{
			PeakAy = FMath::Max(PeakAy, FMath::Abs(S.LateralAccelG));
			++ShiftMasked;
			continue;
		}

		const float RadiusM = S.SpeedMPerSec / FMath::DegreesToRadians(FMath::Abs(S.YawRateDegPerSec));
		const float AckermannDeg = FMath::RadiansToDegrees(WheelBaseM / FMath::Max(RadiusM, 1.f));

		// From the SAMPLE. This used to read GetSteerAngleDeg() off the live corners,
		// inside a loop that runs after the manoeuvre is over - so every sample got
		// the one steer angle the wheels happened to be sitting at when the report was
		// generated, and (delta - ack) collapsed to (constant - ack).
		//
		// That fits beautifully. Ackermann against lateral acceleration is a smooth
		// curve, so the regression returned R-squared around 0.985 while measuring
		// nothing but the geometry of a circle - reporting -8.5 deg/g of oversteer for
		// a car whose logged data says it understeers at about +2.5. A high R-squared
		// is only reassuring once the right quantity is going into the fit.
		const float SteerDeg = FMath::Abs(S.SteerAngleDeg);

		const float AbsAy = FMath::Abs(S.LateralAccelG);

		// Peak grip is measured over EVERYTHING; the gradient only over the linear
		// range. Two different questions - one asks how much the tyres have, the other
		// how the car is balanced before they run out.
		PeakAy = FMath::Max(PeakAy, AbsAy);

		// Also skip the bottom of the range. Below about a tenth of a g both the steer
		// angle and the Ackermann term are small enough that their difference is mostly
		// noise, and those points sit at the end of the lever arm where they drag the
		// slope hardest.
		if (AbsAy > BandHiG || AbsAy < BandLoG) { continue; }

		Ay.Add(AbsAy);
		SteerMinusAckermann.Add(SteerDeg - AckermannDeg);
		Roll.Add(FMath::Abs(S.RollDeg));
		BandSpeedKph.Add(S.SpeedMPerSec * 3.6f);
	}

	Run.Metrics.Add(FBenchMetric(TEXT("understeer gradient"), Slope(Ay, SteerMinusAckermann), TEXT("deg/g"),
		0.5f, 6.f, TEXT("road car 2-4, sports 1-2; negative = oversteer")));
	// Beside the gradient, not buried: a slope whose fit is poor is not a weaker
	// result, it is not a result.
	const float FitQuality = RSquared(Ay, SteerMinusAckermann);
	const float SlopeSE = SlopeStdError(Ay, SteerMinusAckermann);

	// Read this one FIRST. It bounds the gradient directly, where R-squared only
	// says how much of the scatter the line accounts for - and a neutral car has no
	// trend to account for, so it scores badly while being perfectly well measured.
	Run.Metrics.Add(FBenchMetric(TEXT("  gradient +/- (95%)"), 1.96f * SlopeSE, TEXT("deg/g"),
		0.f, 0.5f, TEXT("half-width of the confidence interval; under ~0.5 the gradient is pinned")));

	// Must be zero - see Intercept() - and judged against its OWN standard error rather
	// than a fixed tolerance, see InterceptStdError().
	ReportInterceptAndHalves(Run, Ay, SteerMinusAckermann);

	Run.Metrics.Add(FBenchMetric(TEXT("  understeer fit R^2"), FitQuality, TEXT(""),
		0.8f, 1.0f, TEXT("low R^2 with a small +/- above just means a NEUTRAL car, not a bad run")));

	// Whether the test's own premise held. R-squared says the line matches the
	// points; this says the points were taken under the conditions the measurement
	// assumes. They fail independently - one run held 49 km/h to a fifth of a km/h
	// and fitted at 0.47, another wandered from 49 down to 22 and fitted at 0.99.
	// A gradient from a run whose speed collapsed is a gradient of the deceleration.
	if (BandSpeedKph.Num() > 1)
	{
		float MeanKph = 0.f;
		for (float V : BandSpeedKph) { MeanKph += V; }
		MeanKph /= BandSpeedKph.Num();
		float Variance = 0.f;
		for (float V : BandSpeedKph) { Variance += FMath::Square(V - MeanKph); }
		Run.Metrics.Add(FBenchMetric(TEXT("  speed hold (std dev)"),
			FMath::Sqrt(Variance / BandSpeedKph.Num()), TEXT("km/h"),
			0.f, 1.5f, TEXT("over the fitted band; above ~1.5 it was not a constant-speed run")));
	}

	Run.Metrics.Add(FBenchMetric(TEXT("roll gradient (measured)"), Slope(Ay, Roll), TEXT("deg/g"),
		2.f, 8.f, TEXT("sports 3-5, road 5-7")));
	Run.Metrics.Add(FBenchMetric(TEXT("  roll fit R^2"), RSquared(Ay, Roll), TEXT(""),
		0.8f, 1.0f, TEXT("below ~0.8 the gradient above is noise, not a measurement")));
	Run.Metrics.Add(FBenchMetric(TEXT("peak lateral grip"), PeakAy, TEXT("g"),
		0.75f, 1.3f, TEXT("road tyre 0.8-0.9, performance 1.0-1.2")));

	// The two numbers that explain a gradient. Without them the report says the car
	// oversteers and stops, which tells you to go looking but not where.
	//
	// Static distribution first, because it usually dominates: cornering stiffness
	// does not rise in proportion to load, so the more heavily laden axle runs at the
	// larger slip angle and gives way first. A rear-biased car therefore oversteers
	// unless the load transfer is deliberately skewed forward to offset it.
	//
	// TLLTD is that skew - the front's share of the load moved across the car in a
	// corner. It is NOT the roll-stiffness split: geometric transfer through the roll
	// centres rides along with it, so a rear roll centre higher than the front pulls
	// TLLTD rearward and quietly eats into whatever the front bar was doing.
	// Taken from the settled car rather than from driving samples - see
	// SettledLoadN for why the old window measured the launch instead.
	float StaticFront = 0.f, StaticRear = 0.f;
	int32 StaticCount = 0;
	if (bSettledLoadValid)
	{
		StaticFront = SettledLoadN[0] + SettledLoadN[1];
		StaticRear = SettledLoadN[2] + SettledLoadN[3];
		StaticCount = 1;
	}
	float TransferFront = 0.f, TransferRear = 0.f;
	int32 TransferCount = 0;

	for (const FSample& S : Samples)
	{
		const float AbsAy = FMath::Abs(S.LateralAccelG);

		// Fallback only, for a run with no usable settle capture. Longitudinal
		// acceleration is excluded here too, without which this measures the launch.
		if (!bSettledLoadValid && AbsAy < 0.03f && S.SpeedMPerSec > 8.f
			&& FMath::Abs(S.LongAccelG) < 0.03f)
		{
			StaticFront += S.LoadN[0] + S.LoadN[1];
			StaticRear += S.LoadN[2] + S.LoadN[3];
			++StaticCount;
		}
		// Mid-range: high enough to be a real transfer, below the limit where a
		// sliding axle stops reporting what the springs are doing.
		if (AbsAy > 0.25f && AbsAy < 0.5f)
		{
			TransferFront += FMath::Abs(S.LoadN[0] - S.LoadN[1]);
			TransferRear += FMath::Abs(S.LoadN[2] - S.LoadN[3]);
			++TransferCount;
		}
	}

	if (StaticCount > 0 && (StaticFront + StaticRear) > KINDA_SMALL_NUMBER)
	{
		Run.Metrics.Add(FBenchMetric(TEXT("static weight on front"),
			100.f * StaticFront / (StaticFront + StaticRear), TEXT("%"),
			50.f, 65.f, TEXT("front-engined road car 55-62; under 50 is rear-biased")));
	}

	if (TransferCount > 0 && (TransferFront + TransferRear) > KINDA_SMALL_NUMBER)
	{
		Run.Metrics.Add(FBenchMetric(TEXT("load transfer on front (TLLTD)"),
			100.f * TransferFront / (TransferFront + TransferRear), TEXT("%"),
			1.f, 0.f, FString(),
			TEXT("vs the static share above: transfer skewed forward of it pushes toward "
				 "understeer, rearward toward oversteer - a direction, not the balance itself")));
	}
	Run.Metrics.Add(FBenchMetric(TEXT("samples in regression"), Ay.Num(), TEXT(""),
		1.f, 0.f, FString(),
		FString::Printf(TEXT("linear range %.2f to %.2f g; %d excluded as post-shift%s"),
			BandLoG, BandHiG, ShiftMasked,
			Ay.Num() < 30 ? TEXT(" - THIN, treat the gradients as indicative only") : TEXT(""))));
}

void AVehicleTestBench::MeasurePowerOn(FBenchRun& Run) const
{
	if (PowerOnRampStartTime < 0.f)
	{
		UE_LOG(LogVehicleBench, Error,
			TEXT("   the circle was never established, so power never went in - nothing to measure."));
		return;
	}

	const float WheelBaseM = Target->GetWheelBaseCm() * BenchCmToM;
	TArray<float> Ay, SteerMinusAckermann, Roll;
	TArray<float> BandLongG, HeldSteerDeg;
	float PeakAy = 0.f, PeakLongG = 0.f;
	float EntryAy = 0.f;
	int32 EntryCount = 0, ShiftMasked = 0;

	for (const FSample& S : Samples)
	{
		const float AbsAy = FMath::Abs(S.LateralAccelG);

		// The last second of the steady circle, for the "before" row. Without it the
		// gradient has nothing to be read against.
		if (S.Time < PowerOnRampStartTime)
		{
			if (S.Time > (PowerOnRampStartTime - 1.f))
			{
				EntryAy += AbsAy;
				++EntryCount;
			}
			continue;
		}

		PeakAy = FMath::Max(PeakAy, AbsAy);
		PeakLongG = FMath::Max(PeakLongG, S.LongAccelG);

		if (S.bShiftSettling)
		{
			++ShiftMasked;
			continue;
		}
		if (S.SpeedMPerSec < 4.f || FMath::Abs(S.YawRateDegPerSec) < 1.f)
		{
			continue;
		}

		const float RadiusM = S.SpeedMPerSec / FMath::DegreesToRadians(FMath::Abs(S.YawRateDegPerSec));
		const float AckermannDeg = FMath::RadiansToDegrees(WheelBaseM / FMath::Max(RadiusM, 1.f));
		const float SteerDeg = FMath::Abs(S.SteerAngleDeg);
		HeldSteerDeg.Add(SteerDeg);

		// A LOWER ceiling than the skidpad's, and deliberately - see
		// PowerOnLinearCeilingG. Under power the tyre leaves its linear range earlier,
		// so sharing the skidpad's band would guarantee this fit was across a bend.
		if (AbsAy > PowerOnLinearCeilingG || AbsAy < UndersteerLinearRangeMinG)
		{
			continue;
		}

		Ay.Add(AbsAy);
		SteerMinusAckermann.Add(SteerDeg - AckermannDeg);
		Roll.Add(FMath::Abs(S.RollDeg));
		BandLongG.Add(S.LongAccelG);
	}

	// Read against the skidpad's gradient - but the two are fitted over DIFFERENT
	// bands now (this one lower, see PowerOnLinearCeilingG), so the difference is the
	// drivetrain plus whatever the curve does between the two ceilings. On a car whose
	// intercept row is near zero in both tests that second part is small, because a
	// straight line through the origin has the same slope wherever you cut it. On a car
	// that fails the intercept row it is not small and the subtraction is meaningless.
	Run.Metrics.Add(FBenchMetric(TEXT("understeer gradient under power"),
		Slope(Ay, SteerMinusAckermann), TEXT("deg/g"), 0.5f, 6.f,
		TEXT("vs the SKIDPAD gradient; only subtractable if BOTH intercept rows are ok")));

	Run.Metrics.Add(FBenchMetric(TEXT("  gradient +/- (95%)"),
		1.96f * SlopeStdError(Ay, SteerMinusAckermann), TEXT("deg/g"), 0.f, 0.5f,
		TEXT("half-width of the confidence interval; under ~0.5 the gradient is pinned")));

	// Must be zero - see Intercept(). This is the row that caught the offroad car.
	ReportInterceptAndHalves(Run, Ay, SteerMinusAckermann);

	Run.Metrics.Add(FBenchMetric(TEXT("  fit R^2"),
		RSquared(Ay, SteerMinusAckermann), TEXT(""), 0.8f, 1.0f,
		TEXT("low R^2 with a small +/- above just means a NEUTRAL car, not a bad run")));

	// The dose. A gradient measured under power means nothing without saying how
	// much power - and unlike the skidpad, this number is supposed to be large.
	if (BandLongG.Num() > 0)
	{
		float MeanLongG = 0.f;
		for (float G : BandLongG) { MeanLongG += G; }
		// Banded against the TARGET now rather than against a bare floor. This row is
		// the check that the dose controller did its job: the whole comparability
		// argument rests on every car landing on the same number here, so a car that
		// missed it has to be visible rather than quietly averaged into the table.
		Run.Metrics.Add(FBenchMetric(TEXT("  long accel over the band (mean)"),
			MeanLongG / BandLongG.Num(), TEXT("g"),
			0.8f * PowerOnTargetLongG, 1.2f * PowerOnTargetLongG,
			FString::Printf(
				TEXT("the dose; every car should land on %.2f g or the gradients are not comparable"),
				PowerOnTargetLongG)));
	}
	Run.Metrics.Add(FBenchMetric(TEXT("  peak long accel"), PeakLongG, TEXT("g"),
		1.f, 0.f, FString(), TEXT("highest longitudinal g reached during the power phase")));

	// Whether the test's own premise held: steer was supposed to be FROZEN, so any
	// spread here is compliance or a bug, and either way the result is no longer a
	// constant-steer gradient.
	if (HeldSteerDeg.Num() > 1)
	{
		float MeanSteer = 0.f;
		for (float D : HeldSteerDeg) { MeanSteer += D; }
		MeanSteer /= HeldSteerDeg.Num();
		float Variance = 0.f;
		for (float D : HeldSteerDeg) { Variance += FMath::Square(D - MeanSteer); }
		Run.Metrics.Add(FBenchMetric(TEXT("  steer hold (std dev)"),
			FMath::Sqrt(Variance / HeldSteerDeg.Num()), TEXT("deg"), 0.f, 0.25f,
			TEXT("steer is frozen through the power phase; above ~0.25 it was not")));
	}

	// Reported so the sweep comparison can be checked rather than assumed: if this
	// differs between variants, the gradients above were measured at different
	// wheel torques and cannot be read against each other.
	Run.Metrics.Add(FBenchMetric(TEXT("  gear held"), PowerOnHeldGear, TEXT(""),
		1.f, 0.f, FString(), TEXT("must match across variants or the dose is not comparable")));

	Run.Metrics.Add(FBenchMetric(TEXT("lateral g before power"),
		EntryCount > 0 ? EntryAy / EntryCount : 0.f, TEXT("g"),
		1.f, 0.f, FString(), TEXT("the steady circle the power was applied to")));

	// No band on this one, unlike the skidpad's identically-named row. That test
	// sweeps steer until the tyres let go, so its peak IS the grip. This one sweeps
	// a fixed steer angle from 0.12 g upward and stops long before the limit, so its
	// peak is wherever the run happened to end - reported because it says how far up
	// the range the sweep got, judged against nothing because there is nothing to
	// judge it against. Banded, it flagged 0.65 g as a grip failure on a car the
	// skidpad measures at 0.85.
	Run.Metrics.Add(FBenchMetric(TEXT("highest lateral g reached"), PeakAy, TEXT("g"),
		1.f, 0.f, FString(), TEXT("how far up the range the sweep got, NOT the grip limit")));

	Run.Metrics.Add(FBenchMetric(TEXT("roll gradient (measured)"),
		Slope(Ay, Roll), TEXT("deg/g"), 2.f, 8.f, TEXT("sports 3-5, road 5-7")));

	Run.Metrics.Add(FBenchMetric(TEXT("samples in regression"), Ay.Num(), TEXT(""),
		1.f, 0.f, FString(),
		FString::Printf(TEXT("power phase only, %.2f-%.2f g; %d excluded as post-shift"),
			UndersteerLinearRangeMinG, PowerOnLinearCeilingG, ShiftMasked)));

	// The dose controller's own report. A car pinned against the throttle ceiling did
	// not receive the dose that was asked for, and its gradient therefore cannot be
	// subtracted from the skidpad's on the same footing as the others.
	if (PowerOnDoseSamples > 0)
	{
		const float SaturatedFraction = float(PowerOnSaturatedSamples) / float(PowerOnDoseSamples);
		Run.Metrics.Add(FBenchMetric(TEXT("  throttle saturated"), 100.f * SaturatedFraction,
			TEXT("%"), 0.f, 25.f,
			TEXT("share of the hold pinned at PowerOnMaxThrottle - high means the car could not make the dose")));

		if (SaturatedFraction > 0.25f)
		{
			UE_LOG(LogVehicleBench, Warning,
				TEXT("   throttle was pinned at the %.2f ceiling for %.0f%% of the hold - this car "
					 "could not reach %.2f g and was dosed at whatever it managed. Its gradient is "
					 "not comparable with cars that held the target."),
				PowerOnMaxThrottle, 100.f * SaturatedFraction, PowerOnTargetLongG);
		}
	}

	// Say WHY when the fit is thin, and separate the two reasons - they need
	// opposite corrections and the sample count alone cannot tell them apart.
	const float MeanEntryAy = EntryCount > 0 ? EntryAy / EntryCount : 0.f;
	if (MeanEntryAy >= PowerOnLinearCeilingG)
	{
		UE_LOG(LogVehicleBench, Error,
			TEXT("   the circle was established at %.2f g, at or above the band's %.2f g ceiling. "
				 "Power sweeps lateral g UPWARD, so this run climbed away from the band and "
				 "fitted %d samples. Lower PowerOnEntryLateralG (currently %.2f)."),
			MeanEntryAy, PowerOnLinearCeilingG, Ay.Num(), PowerOnEntryLateralG);
	}
	else if (Ay.Num() < 30)
	{
		UE_LOG(LogVehicleBench, Warning,
			TEXT("   only %d samples in the %.2f-%.2f g band - the car crossed it too fast to fit. "
				 "Lengthen PowerOnThrottleRampSeconds, or lower PowerOnEntryLateralG so the sweep "
				 "starts further below the band."),
			Ay.Num(), UndersteerLinearRangeMinG, PowerOnLinearCeilingG);
	}
}

void AVehicleTestBench::ReportInterceptAndHalves(FBenchRun& Run,
	const TArray<float>& Ay, const TArray<float>& SteerMinusAckermann) const
{
	const float C = Intercept(Ay, SteerMinusAckermann);
	const float SE = InterceptStdError(Ay, SteerMinusAckermann);

	// The intercept has to clear TWO bars, and it needs both because either alone is
	// wrong in a way this bench has already been caught by once.
	//
	// Three sigma asks "is this measurable". On its own it is useless here: these fits
	// carry three to six hundred samples, so a standard error runs 0.008 to 0.03 deg and
	// ANY real curvature is significant. Applied alone it flagged three of the four
	// power-on runs, including cars whose gradient varies by a few percent across the
	// band - a test that fires on almost everything reports nothing.
	//
	// Twelve percent of the fitted span asks "does it matter": the intercept against the
	// range of y the fit actually covers, which is gradient x band width. That is
	// scale-free - it does not tighten as samples accumulate, and it does not punish a
	// shallow-gradient car for being shallow.
	float MinY = FLT_MAX, MaxY = -FLT_MAX;
	for (float V : SteerMinusAckermann) { MinY = FMath::Min(MinY, V); MaxY = FMath::Max(MaxY, V); }
	const float Span = (MaxY > MinY) ? (MaxY - MinY) : 0.f;

	const float Tol = FMath::Max3(3.f * SE, 0.12f * Span, 0.05f);
	Run.Metrics.Add(FBenchMetric(TEXT("  fit intercept at 0 g"), C, TEXT("deg"), -Tol, Tol,
		FString::Printf(
			TEXT("has to be ~0; %.1f sigma and %.0f%% of the fitted span. No lateral g needs no slip, so no steer beyond Ackermann"),
			SE > KINDA_SMALL_NUMBER ? FMath::Abs(C) / SE : 0.f,
			Span > KINDA_SMALL_NUMBER ? 100.f * FMath::Abs(C) / Span : 0.f)));

	if (FMath::Abs(C) <= Tol)
	{
		return;
	}

	// The fit is not a line, so stop pretending one number describes it and report the
	// two that do. See SlopeHalves().
	float Lower = 0.f, Upper = 0.f, Mid = 0.f;
	SlopeHalves(Ay, SteerMinusAckermann, Lower, Upper, Mid);
	if (Mid <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	Run.Metrics.Add(FBenchMetric(TEXT("  gradient below the midpoint"), Lower, TEXT("deg/g"),
		1.f, 0.f, FString(),
		FString::Printf(TEXT("fitted under %.2f g - the single gradient above describes neither half"), Mid)));
	Run.Metrics.Add(FBenchMetric(TEXT("  gradient above the midpoint"), Upper, TEXT("deg/g"),
		1.f, 0.f, FString(),
		FString::Printf(TEXT("fitted over %.2f g"), Mid)));

	UE_LOG(LogVehicleBench, Warning,
		TEXT("   the fit crosses zero at %.3f deg - %.0f%% of the span it covers - so it is "
			 "not a straight line: the gradient runs %.3f below %.2f g and %.3f above it. "
			 "Read those two, not the single number."),
		C, Span > KINDA_SMALL_NUMBER ? 100.f * FMath::Abs(C) / Span : 0.f, Lower, Mid, Upper);
}

void AVehicleTestBench::MeasureStepSteer(FBenchRun& Run) const
{
	int32 StepIndex = INDEX_NONE;
	for (int32 i = 0; i < Samples.Num(); ++i)
	{
		if (Samples[i].SteerInput > KINDA_SMALL_NUMBER) { StepIndex = i; break; }
	}
	if (StepIndex == INDEX_NONE || StepIndex >= Samples.Num() - 5)
	{
		Run.Metrics.Add(FBenchMetric(TEXT("step steer"), 0.f, TEXT(""), 1.f, 0.f, FString(),
			TEXT("never reached the target speed - no step was applied")));
		return;
	}

	const float StepTime = Samples[StepIndex].Time;
	float Peak = 0.f, PeakTime = 0.f, Steady = 0.f;
	int32 SteadyCount = 0;
	for (int32 i = StepIndex; i < Samples.Num(); ++i)
	{
		const float Y = FMath::Abs(Samples[i].YawRateDegPerSec);
		if (Y > Peak) { Peak = Y; PeakTime = Samples[i].Time - StepTime; }
		// Average the last half second as the settled value.
		if (Samples[i].Time > Samples.Last().Time - 0.5f) { Steady += Y; ++SteadyCount; }
	}
	Steady = SteadyCount ? Steady / SteadyCount : 0.f;

	float RiseTime = 0.f;
	for (int32 i = StepIndex; i < Samples.Num(); ++i)
	{
		if (FMath::Abs(Samples[i].YawRateDegPerSec) >= 0.9f * Steady)
		{
			RiseTime = Samples[i].Time - StepTime;
			break;
		}
	}

	Run.Metrics.Add(FBenchMetric(TEXT("yaw response time (90%)"), RiseTime, TEXT("s"),
		0.1f, 0.5f, TEXT("road car 0.2-0.4 s")));
	Run.Metrics.Add(FBenchMetric(TEXT("yaw overshoot"),
		Steady > KINDA_SMALL_NUMBER ? 100.f * (Peak - Steady) / Steady : 0.f, TEXT("%"),
		0.f, 30.f, TEXT("well damped under 20%; over 40% feels nervous")));
	Run.Metrics.Add(FBenchMetric(TEXT("peak yaw rate"), Peak, TEXT("deg/s")));
	Run.Metrics.Add(FBenchMetric(TEXT("time to peak"), PeakTime, TEXT("s")));

	// WHERE the step landed, which decides whether anything above is meaningful.
	//
	// A step steer measures how the car gets from one steady state to another, and
	// that only describes the car while both are inside the linear range. Land the
	// step at the grip limit and the settled yaw rate is set by how much grip there
	// is rather than by the steer angle, so "overshoot" stops being a damping
	// measurement and becomes a measure of how far past the tyres the step went.
	//
	// That is not hypothetical either. A run at 0.3 input and 80 km/h settled at
	// 20.7 deg/s, which is a 61.5 m radius and 0.82 g on a car the skidpad measures
	// at 0.848 - the step was at the limit, all three variants reported 62-64%
	// overshoot, and RWD simply spun. ISO 7401 runs this around 0.4 g for exactly
	// this reason.
	//
	// Derived from the settled yaw rate and speed rather than read off Ay, because
	// the accelerometer trace is a frame difference and noisy, while yaw rate and
	// speed are both states.
	float SteadySpeed = 0.f;
	int32 SpeedCount = 0;
	for (int32 i = StepIndex; i < Samples.Num(); ++i)
	{
		if (Samples[i].Time > Samples.Last().Time - 0.5f)
		{
			SteadySpeed += Samples[i].SpeedMPerSec;
			++SpeedCount;
		}
	}
	SteadySpeed = SpeedCount ? SteadySpeed / SpeedCount : 0.f;

	const float SteadyAy =
		(SteadySpeed * FMath::DegreesToRadians(Steady)) / GravityMPerSec2;

	Run.Metrics.Add(FBenchMetric(TEXT("settled lateral g"), SteadyAy, TEXT("g"),
		0.15f, 0.55f, TEXT("ISO 7401 runs this near 0.4 g; at the limit the numbers above are not damping")));

	if (SteadyAy > 0.55f)
	{
		UE_LOG(LogVehicleBench, Warning,
			TEXT("   the step settled at %.2f g, near this car's grip. Overshoot and rise time "
				 "above describe a saturated tyre, not the car's damping - lower StepSteerInput "
				 "(currently %.2f) or StepSteerSpeedKph (currently %.0f) until this reads about 0.4."),
			SteadyAy, StepSteerInput, StepSteerSpeedKph);
	}
}

void AVehicleTestBench::MeasureArticulation(FBenchRun& Run) const
{
	// The near-side wheels ride the block; the far-side diagonal is the one a bar
	// unloads. Corner order is FR, FL, RR, RL and the block sits on the +Y side,
	// so FR is the wheel that climbs and RL is its diagonal.
	float MinDiagonalLoad = FLT_MAX;
	float MaxCrossTravelCm = 0.f;
	int32 AirborneSamples = 0;
	float MaxRoll = 0.f;

	for (const FSample& S : Samples)
	{
		MinDiagonalLoad = FMath::Min(MinDiagonalLoad, S.LoadN[3]);
		MaxCrossTravelCm = FMath::Max(MaxCrossTravelCm, FMath::Abs(S.TravelCm[0] - S.TravelCm[3]));
		MaxRoll = FMath::Max(MaxRoll, FMath::Abs(S.RollDeg));
		if (S.LoadN[3] < 50.f) { ++AirborneSamples; }
	}
	if (MinDiagonalLoad == FLT_MAX) { MinDiagonalLoad = 0.f; }

	const float StaticCornerN = (TotalMassKg() * GravityMPerSec2) * 0.25f;

	Run.Metrics.Add(FBenchMetric(TEXT("min diagonal wheel load"), MinDiagonalLoad, TEXT("N")));
	Run.Metrics.Add(FBenchMetric(TEXT("diagonal load retained"),
		StaticCornerN > KINDA_SMALL_NUMBER ? 100.f * MinDiagonalLoad / StaticCornerN : 0.f, TEXT("%"),
		1.f, 0.f, FString(), TEXT("share of its static weight the diagonal wheel keeps; lower = more cross-loading")));
	Run.Metrics.Add(FBenchMetric(TEXT("cross-axle articulation"), MaxCrossTravelCm, TEXT("cm"),
		1.f, 0.f, FString(), TEXT("travel difference across the diagonal - this is what a bar limits")));
	Run.Metrics.Add(FBenchMetric(TEXT("wheel-lift samples"), AirborneSamples, TEXT("")));
	Run.Metrics.Add(FBenchMetric(TEXT("peak roll over obstacle"), MaxRoll, TEXT("deg")));
}

void AVehicleTestBench::MeasureStraightLine(FBenchRun& Run) const
{
	float TimeToTarget = 0.f, PeakLongG = 0.f, PeakBrakeG = 0.f, BrakeStartM = 0.f, StopM = 0.f;
	bool bReached = false;
	TArray<float> LongG, Pitch;

	// Peak acceleration comes off a SMOOTHED trace, not off single samples.
	//
	// LongAccelG is a frame difference of velocity, and physics substeps at 200 Hz
	// under frames at about 60 - so 3.33 substeps land per frame and occasionally a
	// frame catches a fourth while the next reads short. That aliasing puts isolated
	// spikes in the derivative with no counterpart in the motion: one AWD run read
	// 1.002 g at 59 km/h, in a frame where velocity rose 0.590 km/h against a steady
	// 0.230, and the NEXT frame read -0.412 g. Speed through the whole episode went
	// 57.5 to 60.2 km/h without a kink, so the car was never doing anything unusual.
	//
	// A single-sample max of a noisy derivative is not a measurement of anything -
	// it finds the worst sampling artifact in the run and reports it as the car's
	// capability, which is how a 1660 kg saloon on 0.86 g tyres came to claim more
	// longitudinal grip than lateral. The window is short enough that a genuine peak
	// survives it and long enough that one frame cannot define the result.
	const int32 Half = FMath::Max(PeakAccelSmoothingSamples / 2, 1);
	for (int32 i = 0; i < Samples.Num(); ++i)
	{
		float Sum = 0.f;
		int32 Count = 0;
		for (int32 k = FMath::Max(i - Half, 0); k <= FMath::Min(i + Half, Samples.Num() - 1); ++k)
		{
			Sum += Samples[k].LongAccelG;
			++Count;
		}
		const float Smoothed = Count > 0 ? Sum / Count : 0.f;
		PeakLongG = FMath::Max(PeakLongG, Smoothed);
		PeakBrakeG = FMath::Min(PeakBrakeG, Smoothed);
	}

	for (const FSample& S : Samples)
	{
		LongG.Add(S.LongAccelG);
		Pitch.Add(S.PitchDeg);

		if (!bReached && S.SpeedMPerSec * 3.6f >= StraightLineTargetKph)
		{
			bReached = true;
			TimeToTarget = S.Time;
			BrakeStartM = S.DistanceM;
		}
		if (bReached && S.SpeedMPerSec > 0.5f) { StopM = S.DistanceM; }
	}

	Run.Metrics.Add(FBenchMetric(FString::Printf(TEXT("0-%.0f km/h"), StraightLineTargetKph),
		bReached ? TimeToTarget : 0.f, TEXT("s"), 2.5f, 15.f, TEXT("hot hatch ~7 s, sports ~4 s")));
	Run.Metrics.Add(FBenchMetric(TEXT("peak acceleration"), PeakLongG, TEXT("g"),
		0.25f, 1.0f, TEXT("road car 0.3-0.5, supercar up to 1.0"),
		FString::Printf(TEXT("over a %d-sample window - see the note in MeasureStraightLine"),
			PeakAccelSmoothingSamples)));
	Run.Metrics.Add(FBenchMetric(TEXT("peak braking"), -PeakBrakeG, TEXT("g"),
		0.8f, 1.4f, TEXT("road car 0.9-1.1 on dry tarmac")));
	Run.Metrics.Add(FBenchMetric(TEXT("braking distance"), FMath::Max(StopM - BrakeStartM, 0.f), TEXT("m"),
		1.f, 0.f, FString(), TEXT("from the target speed to a stop")));
	Run.Metrics.Add(FBenchMetric(TEXT("pitch gradient"), Slope(LongG, Pitch), TEXT("deg/g"),
		-6.f, 6.f, TEXT("dive/squat; road car within a couple of degrees per g")));
}

// ---------------------------------------------------------------------------
// Output
// ---------------------------------------------------------------------------

float AVehicleTestBench::SprungMassKg() const
{
	USkeletalMeshComponent* Mesh = Target ? Target->GetCarMesh() : nullptr;
	return Mesh ? FMath::Max(Mesh->GetMass(), 1.f) : 1500.f;
}

float AVehicleTestBench::TotalMassKg() const
{
	float Unsprung = 0.f;
	if (Target)
	{
		for (const FCornerAssembly& C : Target->GetCorners())
		{
			if (C.Suspension) { Unsprung += C.Suspension->GetUnsprungMassKg(); }
		}
	}
	return SprungMassKg() + Unsprung;
}

FString AVehicleTestBench::TestName() const
{
	switch (Test)
	{
	case EVehicleBenchTest::Kinematics:   return TEXT("Kinematics & Rates");
	case EVehicleBenchTest::Skidpad:      return TEXT("Steady-State Cornering");
	case EVehicleBenchTest::PowerOnCornering: return TEXT("Power-On Cornering");
	case EVehicleBenchTest::StepSteer:    return TEXT("Step Steer");
	case EVehicleBenchTest::Articulation: return TEXT("Articulation / ARB");
	default:                              return TEXT("Acceleration & Braking");
	}
}

void AVehicleTestBench::ReportRuns() const
{
	UE_LOG(LogVehicleBench, Log, TEXT("=============================================================="));
	UE_LOG(LogVehicleBench, Log, TEXT("  %s   -   %s"), *TestName(), *Target->GetName());
	float MeasTrack = 0.f, MeasBase = 0.f;
	const TArray<FCornerAssembly>& C = Target->GetCorners();
	if (C.Num() >= 3 && C[0].Suspension && C[1].Suspension && C[2].Suspension)
	{
		MeasTrack = FMath::Abs(C[0].Suspension->GetWheelCentreRest().Y - C[1].Suspension->GetWheelCentreRest().Y);
		MeasBase = FMath::Abs(C[0].Suspension->GetWheelCentreRest().X - C[2].Suspension->GetWheelCentreRest().X);
	}
	UE_LOG(LogVehicleBench, Log, TEXT("  sprung %.0f kg, total %.0f kg"), SprungMassKg(), TotalMassKg());
	UE_LOG(LogVehicleBench, Log, TEXT("  measured from corners: wheelbase %.1f cm, track %.1f cm"), MeasBase, MeasTrack);
	if (!FMath::IsNearlyEqual(MeasTrack, Target->GetTrackWidthCm(), 1.f)
		|| !FMath::IsNearlyEqual(MeasBase, Target->GetWheelBaseCm(), 1.f))
	{
		// ACar caches these in its BeginPlay and uses them for the Ackermann split.
		// If they disagree with the corners the steering geometry is being computed
		// from the wrong numbers, which no amount of suspension tuning will fix.
		UE_LOG(LogVehicleBench, Warning,
			TEXT("  ACar has wheelbase %.1f cm, track %.1f cm - DISAGREES with the corners. "
				 "ACar::SetSteering builds its Ackermann split from those, so steering is wrong until this is."),
			Target->GetWheelBaseCm(), Target->GetTrackWidthCm());
	}
	UE_LOG(LogVehicleBench, Log, TEXT("=============================================================="));

	for (const FBenchRun& Run : Runs)
	{
		UE_LOG(LogVehicleBench, Log, TEXT(""));
		UE_LOG(LogVehicleBench, Log, TEXT("[ %s ]"), *Run.Variant);
		for (const FBenchMetric& M : Run.Metrics)
		{
			const TCHAR* Verdict = !M.HasBand() ? TEXT("     ") : (M.InBand() ? TEXT("  ok ") : TEXT(" <-- "));
			UE_LOG(LogVehicleBench, Log, TEXT("   %-28s %10.3f %-14s %s %s"),
				*M.Label, M.Value, *M.Unit, Verdict, *M.Reference);
			if (!M.Note.IsEmpty())
			{
				UE_LOG(LogVehicleBench, Log, TEXT("       %s"), *M.Note);
			}
		}
	}

	// The comparison is the payload of a sweep, so print it explicitly rather than
	// leaving two tables to be eyeballed.
	if (Runs.Num() > 1)
	{
		UE_LOG(LogVehicleBench, Log, TEXT(""));
		UE_LOG(LogVehicleBench, Log, TEXT("[ what changed, vs '%s' ]"), *Runs[0].Variant);
		for (const FBenchMetric& Base : Runs[0].Metrics)
		{
			FString Line = FString::Printf(TEXT("   %-28s %10.3f"), *Base.Label, Base.Value);
			for (int32 r = 1; r < Runs.Num(); ++r)
			{
				const FBenchMetric* Other = Runs[r].Find(Base.Label);
				if (Other == nullptr) { continue; }
				const float Delta = Other->Value - Base.Value;
				const float Percent = FMath::Abs(Base.Value) > KINDA_SMALL_NUMBER
					? 100.f * Delta / FMath::Abs(Base.Value) : 0.f;
				Line += FString::Printf(TEXT("   |  %s: %.3f (%+.1f%%)"), *Runs[r].Variant, Other->Value, Percent);
			}
			UE_LOG(LogVehicleBench, Log, TEXT("%s"), *Line);
		}
	}

	UE_LOG(LogVehicleBench, Log, TEXT(""));
	UE_LOG(LogVehicleBench, Log,
		TEXT("  Bands are textbook ranges, not measurements of a specific car."));
	UE_LOG(LogVehicleBench, Log,
		TEXT("  In-band means plausible, not correct. Out-of-band means go and look."));
	UE_LOG(LogVehicleBench, Log, TEXT("=============================================================="));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Green,
			FString::Printf(TEXT("%s complete - %d run(s). See the Output Log (LogVehicleBench)."),
				*TestName(), Runs.Num()));
	}
}

void AVehicleTestBench::WriteCsv() const
{
	if (Samples.Num() == 0)
	{
		return;
	}
	const FString Dir = FPaths::ProjectSavedDir() / TEXT("VehicleTests");
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();
	if (!File.DirectoryExists(*Dir))
	{
		File.CreateDirectoryTree(*Dir);
	}

	FString Csv = TEXT("time_s,speed_mps,lat_g,long_g,yaw_deg_s,roll_deg,pitch_deg,steer_input,steer_deg,dist_m,")
	              TEXT("load_FR,load_FL,load_RR,load_RL,travel_FR,travel_FL,travel_RR,travel_RL,")
	              TEXT("camber_FR,camber_FL,camber_RR,camber_RL,")
	              TEXT("kappa_FR,kappa_FL,kappa_RR,kappa_RL,fx_FR,fx_FL,fx_RR,fx_RL,")
	              TEXT("alpha_FR,alpha_FL,alpha_RR,alpha_RL,fy_FR,fy_FL,fy_RR,fy_RL,")
	              TEXT("kspan_FR,kspan_FL,kspan_RR,kspan_RL,")
	              TEXT("omega_FR,omega_FL,omega_RR,omega_RL,")
	              TEXT("wused_FR,wused_FL,wused_RR,wused_RL,vcx_FR,vcx_FL,vcx_RR,vcx_RL\n");
	for (const FSample& S : Samples)
	{
		Csv += FString::Printf(
			TEXT("%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.3f,%.4f,%.3f,")
			TEXT("%.1f,%.1f,%.1f,%.1f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,")
			TEXT("%.5f,%.5f,%.5f,%.5f,%.1f,%.1f,%.1f,%.1f,")
			TEXT("%.4f,%.4f,%.4f,%.4f,%.1f,%.1f,%.1f,%.1f,")
			TEXT("%.5f,%.5f,%.5f,%.5f,")
			TEXT("%.3f,%.3f,%.3f,%.3f,")
			TEXT("%.3f,%.3f,%.3f,%.3f,%.4f,%.4f,%.4f,%.4f\n"),
			S.Time, S.SpeedMPerSec, S.LateralAccelG, S.LongAccelG, S.YawRateDegPerSec,
			S.RollDeg, S.PitchDeg, S.SteerInput, S.SteerAngleDeg, S.DistanceM,
			S.LoadN[0], S.LoadN[1], S.LoadN[2], S.LoadN[3],
			S.TravelCm[0], S.TravelCm[1], S.TravelCm[2], S.TravelCm[3],
			S.CamberDeg[0], S.CamberDeg[1], S.CamberDeg[2], S.CamberDeg[3],
			S.Kappa[0], S.Kappa[1], S.Kappa[2], S.Kappa[3],
			S.FxN[0], S.FxN[1], S.FxN[2], S.FxN[3],
			S.AlphaDeg[0], S.AlphaDeg[1], S.AlphaDeg[2], S.AlphaDeg[3],
			S.FyN[0], S.FyN[1], S.FyN[2], S.FyN[3],
			S.KappaSpan[0], S.KappaSpan[1], S.KappaSpan[2], S.KappaSpan[3],
			S.OmegaRadPerSec[0], S.OmegaRadPerSec[1], S.OmegaRadPerSec[2], S.OmegaRadPerSec[3],
			S.OmegaUsed[0], S.OmegaUsed[1], S.OmegaUsed[2], S.OmegaUsed[3],
			S.VcxUsed[0], S.VcxUsed[1], S.VcxUsed[2], S.VcxUsed[3]);
	}

	const FString Variant = Runs.Num() ? Runs.Last().Variant.Replace(TEXT(" "), TEXT("_")) : TEXT("run");

	// THE CAR'S NAME IS IN THE FILENAME, and leaving it out cost real time. The path was
	// test plus variant only, so benching two cars in a row wrote both to the same file
	// and the second silently replaced the first. Reading that file back and believing it
	// belonged to the earlier car produced a completely wrong analysis - a 47/53 sports
	// car appearing to carry 59.6% on its front axle, which was the saloon's figure
	// sitting in the sports car's file.
	const FString CarName = Target != nullptr ? Target->GetName() : TEXT("unknown_car");
	const FString Path = Dir / FString::Printf(TEXT("%s_%s_%s.csv"),
		*TestName().Replace(TEXT(" "), TEXT("_")), *CarName, *Variant);
	if (FFileHelper::SaveStringToFile(Csv, *Path))
	{
		UE_LOG(LogVehicleBench, Log, TEXT("   telemetry -> %s (%d samples)"), *Path, Samples.Num());
	}
}
