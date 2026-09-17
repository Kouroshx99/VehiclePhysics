// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VehicleTestBench.generated.h"

class ACar;
class UWishboneSuspension;

/**
 * Which manoeuvre to run.
 *
 * The first is static - it interrogates the linkage without moving the car and
 * finishes in one frame. The rest drive the vehicle and take seconds to minutes.
 */
UENUM(BlueprintType)
enum class EVehicleBenchTest : uint8
{
	/** Kinematics and rate report. No driving, no ground needed. */
	Kinematics       UMETA(DisplayName = "Kinematics & Rates (static)"),

	/** Fixed steer, slow speed ramp. Understeer gradient, roll gradient, grip. */
	Skidpad          UMETA(DisplayName = "Steady-State Cornering"),

	/** Straight run, then a steer step. Yaw response time and overshoot. */
	StepSteer        UMETA(DisplayName = "Step Steer (transient)"),

	/** One wheel over a block. What the anti-roll bar does to the diagonal. */
	Articulation     UMETA(DisplayName = "Articulation / ARB Disconnect"),

	/** Full throttle to target, then full brake. Pitch, traction, stopping. */
	StraightLine     UMETA(DisplayName = "Acceleration & Braking"),

	// APPENDED, not slotted in beside Skidpad where it belongs by subject.
	// Inserting an entry renumbers every one after it, and while UE4 remaps tagged
	// enum properties by NAME - so a saved asset should survive it - "should" is a
	// poor reason to gamble a bench actor's saved settings on a detail of the
	// serialiser. Append is free; the dropdown order is cosmetic.

	/** Fixed steer on a circle, then throttle. What POWER does to the balance. */
	PowerOnCornering UMETA(DisplayName = "Power-On Cornering")
};

/**
 * Run the manoeuvre several times, changing ONE thing between runs.
 *
 * This is the whole point of the bench. A single number off a single run says
 * almost nothing - "3.1 deg/g" is only meaningful against the same car measured
 * the same way with the bar disconnected. Sweeping re-runs an identical script
 * and changes exactly one variable, so the difference is attributable.
 */
UENUM(BlueprintType)
enum class EVehicleBenchSweep : uint8
{
	/** One run, as the vehicle is configured. */
	None             UMETA(DisplayName = "None (single run)"),

	/** As-configured, then with both bars set to zero. */
	AntiRollBar      UMETA(DisplayName = "Anti-roll bar: fitted vs disconnected"),

	/** Front-, rear- and all-wheel drive. */
	Drivetrain       UMETA(DisplayName = "Drivetrain: FWD / RWD / AWD")
};

/**
 * One measured quantity, with the real-world range it is being judged against.
 *
 * The range is the reason this struct exists. A bench that prints "roll gradient
 * 6.2 deg/g" tells you nothing unless you already know what a car does; printing
 * it beside "road car 5-7" turns it into an answer.
 */
USTRUCT()
struct FBenchMetric
{
	GENERATED_BODY()

	FString Label;
	float Value = 0.f;
	FString Unit;

	/** Plausible real-world band. Lo > Hi disables the verdict (report-only). */
	float RealLo = 1.f;
	float RealHi = 0.f;

	/** What the band represents, e.g. "road car". Printed next to it. */
	FString Reference;

	/** Free-text caveat, printed under the row when non-empty. */
	FString Note;

	FBenchMetric() = default;
	FBenchMetric(const FString& InLabel, float InValue, const FString& InUnit,
		float InLo = 1.f, float InHi = 0.f, const FString& InRef = FString(), const FString& InNote = FString())
		: Label(InLabel), Value(InValue), Unit(InUnit), RealLo(InLo), RealHi(InHi), Reference(InRef), Note(InNote) {}

	bool HasBand() const { return RealHi >= RealLo; }
	bool InBand() const { return HasBand() && Value >= RealLo && Value <= RealHi; }
};

/** Everything one run produced, kept so runs can be diffed against each other. */
USTRUCT()
struct FBenchRun
{
	GENERATED_BODY()

	/** What was varied for this run, e.g. "bars disconnected". */
	FString Variant;

	TArray<FBenchMetric> Metrics;

	const FBenchMetric* Find(const FString& Label) const
	{
		return Metrics.FindByPredicate([&](const FBenchMetric& M) { return M.Label == Label; });
	}
};

/**
 * Automated vehicle-dynamics bench.
 *
 * Drop one in a level, point it at a car (or leave Target empty and it takes the
 * first one it finds), pick a test and a sweep, and press play. It drives the car
 * through a scripted manoeuvre, measures the standard industry quantities, and
 * prints them against the range a real vehicle of that type sits in.
 *
 * WHY THIS EXISTS, AND WHAT IT CANNOT DO
 *
 * It answers "is this car behaving like a car" and "did that change do what I
 * think" - questions that are almost impossible to answer by driving, because a
 * simulated car can feel entirely convincing while its understeer gradient is
 * three times a real one's, and because the seat of your pants cannot separate a
 * bar change from a tyre change.
 *
 * It is NOT a validation against a specific real vehicle. Nothing here has been
 * correlated to measured data from a physical car, and the reference bands are
 * textbook ranges (Milliken, Gillespie, Dixon) not measurements of your car. A
 * value inside its band means "plausible", never "correct" - and a value outside
 * one is a prompt to go and look, not proof of a bug. Read them as a smell test.
 *
 * The manoeuvres follow the shape of the ISO/SAE tests they are named after but
 * are not certified implementations of them.
 */
UCLASS()
class VEHICLEPHYSICS_API AVehicleTestBench : public AActor
{
	GENERATED_BODY()

public:
	AVehicleTestBench();

	/** The vehicle under test. Empty takes the first ACar in the level. */
	UPROPERTY(EditAnywhere, Category = "Bench")
	ACar* Target = nullptr;

	UPROPERTY(EditAnywhere, Category = "Bench")
	EVehicleBenchTest Test = EVehicleBenchTest::Kinematics;

	UPROPERTY(EditAnywhere, Category = "Bench")
	EVehicleBenchSweep Sweep = EVehicleBenchSweep::None;

	/** Start automatically. Off means call RunBench yourself. */
	UPROPERTY(EditAnywhere, Category = "Bench")
	bool bRunOnBeginPlay = true;

	/** Seconds to let the car settle on its springs before a run starts. */
	UPROPERTY(EditAnywhere, Category = "Bench", meta = (ClampMin = "0.0", Units = "s"))
	float SettleSeconds = 2.f;

	/**
	 * How long the Kinematics test lets the car settle before reading the two LIVE
	 * figures in its report - static camber and rest offset from design.
	 *
	 * The rest of that test asks the linkage questions and needs no settling at all: a
	 * motion ratio or a roll centre is a property of the geometry and is the same whether
	 * the car is parked or falling. But static camber is whatever the corner reads at the
	 * pose it happens to be in, and read at bench start that pose is the car mid-drop.
	 * It is why static camber never matched StaticCamberDeg and why comparing two runs'
	 * ride heights measured nothing.
	 *
	 * This waits for the ride height to STOP MOVING rather than for a fixed time, since
	 * how long a car takes to settle is exactly what changes when a spring is retuned.
	 * The cap is there so a car that never settles still produces a report - and says so.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench", meta = (Units = "s"))
	float KinematicsSettleCapSeconds = 8.f;

	/** Ride height is called settled below this rate of change. */
	UPROPERTY(EditAnywhere, Category = "Bench")
	float KinematicsSettleRateCmPerSec = 0.05f;

	/**
	 * Draw live telemetry on screen every frame, whether or not a test is running.
	 *
	 * For driving the proving ground by hand. The scripted tests answer "what is
	 * this car's understeer gradient"; this answers "what is it doing right now",
	 * which is the question you actually have with a wheel in your hands. Turn
	 * radius and lateral g in particular are impossible to judge by feel and
	 * trivial to read off a number.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench")
	bool bShowLiveTelemetry = false;

	/** Write a per-sample CSV to Saved/VehicleTests/ alongside the summary. */
	UPROPERTY(EditAnywhere, Category = "Bench")
	bool bWriteCsv = true;

	// ---------------- Steady-state cornering ----------------

	/** Steer input held through the run, -1..1. Modest is better - see the .cpp. */
	/**
	 * Speed held throughout, km/h. Lateral acceleration is swept with STEER, not
	 * with speed.
	 *
	 * The other way round - fixed steer, rising speed - is easier to write and is
	 * what this did first, but it is not a steady-state test. The car accelerates
	 * continuously so it never settles; it changes gear part way, and with no clutch
	 * each change dumps a torque step through the driven axle; and the longitudinal
	 * load transfer that comes with accelerating moves the very balance the test is
	 * trying to read. Measured that way the understeer regression came back with an
	 * R-squared of 0.004 to 0.4 - no relationship worth reporting.
	 *
	 * At a held speed the throttle is small and near-constant, the gearbox stays put,
	 * and each steer increment settles before the next arrives. That is what steady
	 * state means, and it is what makes the gradient a property of the car rather
	 * than of the run.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Cornering", meta = (ClampMin = "10.0"))
	float SkidpadSpeedKph = 50.f;

	/**
	 * The substep the stability report assumes, seconds. Purely a diagnostic input -
	 * it changes nothing about the sim. Keep it matching Project Settings > Physics >
	 * Max Substep Delta Time, which is what actually sets the rate.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Diagnostics", meta = (ClampMin = "0.0001"))
	float SubstepSecondsForDiagnostic = 0.005f;

	/** How fast steer is wound on, input units per second. Slow keeps it settled. */
	UPROPERTY(EditAnywhere, Category = "Bench|Cornering", meta = (ClampMin = "0.005", ClampMax = "0.5"))
	float SkidpadSteerRatePerSec = 0.035f;

	/** Steer input the sweep stops at, -1..1. */
	UPROPERTY(EditAnywhere, Category = "Bench|Cornering", meta = (ClampMin = "0.05", ClampMax = "1.0"))
	float SkidpadMaxSteerInput = 1.f;

	/**
	 * Upper lateral acceleration for the understeer regression, g.
	 *
	 * The understeer gradient is defined on the LINEAR part of the handling curve -
	 * ISO 4138 measures it around 0.3-0.4 g and no higher. Past that the tyres go
	 * progressive, delta = L/R + K*Ay stops holding, and a straight line fitted
	 * through the lot returns a number that describes neither the linear range nor
	 * the limit. Raise it only if you want to see how the balance MOVES with load;
	 * do not raise it and then quote the result as the understeer gradient.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Cornering", meta = (ClampMin = "0.05", ClampMax = "1.5"))
	// 0.6, RAISED FROM 0.4. The ceiling has to leave room for the linear range to exist.
	//
	// Measured per-sample on ASportsCar, its instantaneous gradient is still falling at
	// 0.4 g - 2.44 at 0.15-0.20, 2.06 at 0.20-0.30, 1.71 at 0.30-0.40 - and only settles
	// near 1.5 between 0.3 and 0.9. Fitting a straight line to 0.15-0.40 was fitting the
	// curved part and nothing else, which is why that car returned R^2 0.481 while the
	// saloon managed 0.960 over the same band.
	//
	// A stiffer car reaches a given lateral g at less steer, so it enters its linear range
	// LATER in lateral g than a soft one. A fixed ceiling written for a saloon clips the
	// stiff car exactly where it starts being measurable.
	float UndersteerLinearRangeG = 0.6f;

	/** Lower end of the regression band, g. Below this the difference is noise. */
	UPROPERTY(EditAnywhere, Category = "Bench|Cornering", meta = (ClampMin = "0.0", ClampMax = "0.3"))
	// 0.15, RAISED FROM 0.10, because below that the measurement is smaller than the
	// car's own alignment. On ASportsCar at 0.05 g the whole signal is steer 0.78 deg
	// minus ackermann 0.54 - a quarter of a degree - while its static toe is 0.05 front
	// and 0.15 rear. The slip-angle ratio between the axles reads 4.0 down there and
	// 1.45 where the car is working; the 4.0 is toe, not cornering.
	//
	// Measured per-sample on that car, the instantaneous gradient runs 3.07 at 0.05-0.10 g,
	// 2.89 at 0.10-0.15, then settles near 1.5 from 0.3 to 0.9 before climbing to 3.10 at
	// the limit as the front saturates. Only the middle of that is the linear range this
	// metric claims to report, and a floor of 0.10 still had a third of its samples below
	// it - which is why the sports car fitted at R^2 0.613 where the saloon managed 0.957.
	// A stiffer car reaches a given lateral g at less steer, so it spends longer under the
	// noise floor, and the metric punished it for being stiff.
	float UndersteerLinearRangeMinG = 0.15f;

	// ---------------- Power-on cornering ----------------
	//
	// The skidpad answers what the car does at neutral throttle, and at 0.02 g of
	// longitudinal demand the drivetrain is invisible in it - correctly so, since
	// the understeer gradient K = Wf/Cf - Wr/Cr has no term for which wheels are
	// driven. Torque reaches the balance only through the friction ellipse, whose
	// cost goes as (Fx/Fxmax)^2: worth 0.7% at 0.02 g, dominant at 0.5.
	//
	// So this test puts the torque in. Same measurement, same band, same units -
	// only the condition changes, and the DIFFERENCE between the two gradients is
	// the drivetrain's contribution.

	/**
	 * Speed the circle is established at before any power goes in, km/h.
	 *
	 * Deliberately below the skidpad's. The car needs somewhere to accelerate TO:
	 * the measurement is the second half of the run, where speed and therefore
	 * lateral g climb under throttle.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "5.0"))
	float PowerOnEntrySpeedKph = 35.f;

	/**
	 * Lateral acceleration the circle is established at, g, BEFORE power goes in.
	 *
	 * Must sit BELOW UndersteerLinearRangeMinG's band, because this manoeuvre sweeps
	 * lateral g UPWARD: power raises speed, speed raises Ay at a fixed steer angle.
	 * Enter above the band and the run climbs further away from it and fits nothing.
	 *
	 * That is not hypothetical - it is what the first run of this test did. A steer
	 * input of 0.35 at 35 km/h put the circle at 0.47 g against a 0.40 g ceiling, and
	 * FWD and AWD returned ZERO samples. RWD returned 24, and only because it slid,
	 * which dropped Ay back into the band from above; its -3.59 deg/g was the
	 * breakaway being measured, not a gradient.
	 *
	 * Expressed as a measured g rather than a steer input on purpose: the input to
	 * angle mapping runs through a curve asset, so the same number means a different
	 * circle on a different car. The bench ramps steer until it MEASURES this, then
	 * freezes whatever angle produced it.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float PowerOnEntryLateralG = 0.12f;

	/**
	 * Ceiling on the steer input while establishing the circle, -1..1.
	 *
	 * A safety stop, not the target - PowerOnEntryLateralG decides where the ramp
	 * ends. This only matters if the car cannot reach that lateral g at all, in which
	 * case the run proceeds at whatever the cap allowed and says so.
	 *
	 * Holding steer rather than holding radius keeps the car the only variable. The
	 * alternative - a path controller correcting to hold radius - would fold that
	 * controller's gains into every number the test produces, and two drivetrains
	 * would then differ partly because the controller chased them differently.
	 *
	 * Fixed steer with rising speed is ISO 4138's constant-steer method, so the
	 * gradient this yields is comparable with the swept-steer one from the skidpad.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "0.05", ClampMax = "1.0"))
	float PowerOnSteerInput = 0.6f;

	/**
	 * Gear the whole power phase runs in, as an index into UDrivetrain::GearRatios.
	 *
	 * Selected during the settle window and held, identically for every variant.
	 * That is the point: leaving the box to its own devices had FWD reach the circle
	 * in 2nd while RWD was still in 1st, and 5.25 against 3.36 is a 1.56x difference
	 * in wheel torque - so the two were given different amounts of power and their
	 * gradients were never comparable. Letting them shift freely does not fix that,
	 * it just moves the mismatch around inside the run.
	 *
	 * One gear is enough. Sweeping 0.12 g to 0.40 g of lateral needs speed to rise
	 * by sqrt(0.40/0.12), about 1.8x, which sits inside a single ratio. Default 2 is
	 * second: at a 35 km/h entry that is near 3000 rpm, leaving room to pull to
	 * roughly 5400 without meeting the limiter.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "1"))
	int32 PowerOnGear = 2;

	/** Seconds held on the circle at entry speed before the throttle goes in. */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "0.5"))
	float PowerOnSettleSeconds = 2.5f;

	/** Seconds over which the TARGET longitudinal g is wound from zero to the dose. */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "0.1"))
	float PowerOnThrottleRampSeconds = 3.f;

	/**
	 * THE DOSE: longitudinal acceleration the power phase drives the car at, in g.
	 *
	 * This test exists to be read against the skidpad's gradient, and the difference
	 * between the two is supposed to be the drivetrain. That subtraction is only valid
	 * if every car was given the SAME amount of power - and a throttle fraction is not
	 * the same amount of power. 0.6 throttle in second gear is 0.180 g in the saloon
	 * and 0.340 g in the sports car, so the two gradients were never differences of
	 * drivetrain layout; they were differences of how hard each car happened to be
	 * pushed. The sports car's fit then failed outright (R^2 0.02) because at nearly
	 * twice the dose it crossed the fitted band before the band could be sampled.
	 *
	 * So the bench closes the loop on the measured value instead: throttle is whatever
	 * it takes to hold this acceleration, which is what ISO's power-on procedures
	 * specify and for exactly this reason. A powerful car gets less pedal and a heavy
	 * one gets more, and both are then doing the same thing.
	 *
	 * 0.18 g by default because that is what the saloon was already doing at the
	 * hand-tuned 0.6 throttle, and that run fitted well - so the car whose number is
	 * already trusted keeps it, and the others move onto its dose rather than it
	 * moving onto theirs.
	 *
	 * A car that cannot reach this saturates the throttle at PowerOnMaxThrottle and
	 * says so; its gradient is still measured, at the honest lower dose it managed.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "0.02", ClampMax = "1.0"))
	float PowerOnTargetLongG = 0.18f;

	/**
	 * Gains for the dose controller, throttle per g and throttle per g-second.
	 *
	 * Sized off the plant rather than tuned by hand: full throttle buys roughly 0.35 g
	 * in second, so throttle-per-g is about 3 and a proportional term of 1.5 is half of
	 * that - deliberately under, because the integral is what holds the value and the
	 * proportional term only has to stop it hunting. The integrator closes the rest
	 * inside about a second, which fits well within PowerOnThrottleRampSeconds.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On|Advanced", meta = (ClampMin = "0.0"))
	float PowerOnDoseGainP = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Bench|Power-On|Advanced", meta = (ClampMin = "0.0"))
	float PowerOnDoseGainI = 3.0f;

	/**
	 * Ceiling on the throttle the dose controller may command, 0..1.
	 *
	 * A safety stop, not the dose - PowerOnTargetLongG decides how hard the car is
	 * driven, the same way PowerOnEntryLateralG rather than PowerOnSteerInput decides
	 * the circle. This only bites on a car that cannot reach the target at all, and
	 * that is reported rather than hidden.
	 *
	 * It used to BE the dose, at a hand-picked 0.6 that suited one car and mis-dosed
	 * every other. See PowerOnTargetLongG.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "0.05", ClampMax = "1.0"))
	float PowerOnMaxThrottle = 1.0f;

	/** Seconds held at that throttle after the ramp finishes. */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "1.0"))
	float PowerOnHoldSeconds = 8.f;

	/**
	 * Ceiling on the lateral g this test will fit, and it is LOWER than the skidpad's.
	 *
	 * The two tests used the same band on the argument that their gradients only
	 * compare if they are fitted over the same part of the handling curve. That
	 * argument is right and the conclusion was wrong: a car under power leaves its
	 * linear range at LOWER lateral g than the same car coasting, because longitudinal
	 * force is already using part of the friction ellipse and there is less left for
	 * cornering. Sharing a band therefore guarantees that one of the two tests is
	 * fitting a bend.
	 *
	 * It was. Measured across the fleet, holding the floor and sweeping this ceiling,
	 * the intercept - which has to be zero, see Intercept() - behaves like this:
	 *
	 *     ceiling      saloon    sports     rally
	 *       0.30       -0.000     0.143    -0.080
	 *       0.35       -0.005     0.185    -0.140
	 *       0.40       -0.017     0.215    -0.237     <- sports and rally leave
	 *       0.60       -0.047     0.270    -0.262
	 *
	 * 0.35 is the widest ceiling at which every car that can be fitted still fits
	 * through the origin, and it keeps 260-380 samples per car with R^2 of 0.90 to
	 * 0.99. Above it the numbers do not get noisier, they get WRONG - which is the
	 * failure the intercept row exists to expose.
	 *
	 * The offroad car passes at no ceiling in either direction: at 13.7 deg/g of roll
	 * its curve bends inside any window. That is a property of the vehicle, not of
	 * this number, and the intercept row is what reports it.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Power-On", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float PowerOnLinearCeilingG = 0.35f;

	// ---------------- Step steer ----------------

	UPROPERTY(EditAnywhere, Category = "Bench|Step Steer", meta = (ClampMin = "10.0"))
	float StepSteerSpeedKph = 80.f;

	UPROPERTY(EditAnywhere, Category = "Bench|Step Steer", meta = (ClampMin = "0.05", ClampMax = "1.0"))
	float StepSteerInput = 0.3f;

	/** Seconds of response recorded after the step. */
	UPROPERTY(EditAnywhere, Category = "Bench|Step Steer", meta = (ClampMin = "0.5"))
	float StepSteerHoldSeconds = 4.f;

	// ---------------- Articulation ----------------

	/** Height of the block the near-side wheels are driven over, cm. */
	UPROPERTY(EditAnywhere, Category = "Bench|Articulation", meta = (ClampMin = "5.0", Units = "cm"))
	float ArticulationObstacleHeightCm = 35.f;

	UPROPERTY(EditAnywhere, Category = "Bench|Articulation", meta = (ClampMin = "1.0"))
	float ArticulationApproachKph = 8.f;

	// ---------------- Straight line ----------------

	UPROPERTY(EditAnywhere, Category = "Bench|Straight Line", meta = (ClampMin = "10.0"))
	float StraightLineTargetKph = 100.f;

	/**
	 * Samples the peak-acceleration window averages over. Odd numbers centre cleanly.
	 *
	 * Guards the peak against aliasing in the frame-differenced acceleration rather
	 * than against anything the car does - see MeasureStraightLine. Five samples is
	 * about 80 ms at 60 Hz: far shorter than any real acceleration event, far longer
	 * than the one-frame spikes.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench|Straight Line", meta = (ClampMin = "1", ClampMax = "31"))
	int32 PeakAccelSmoothingSamples = 5;

	/** Runs the configured test. Safe to call from Blueprint or the console. */
	UFUNCTION(BlueprintCallable, Category = "Bench", meta = (CallInEditor = "true"))
	void RunBench();

	/** Stops a running test and reports whatever it managed to measure. */
	UFUNCTION(BlueprintCallable, Category = "Bench")
	void AbortBench();

	/**
	 * Predicts, from parameters alone, whether the wheel spin / tyre carcass mode is
	 * stable at the substep rate. No run required - it is arithmetic.
	 */
	void LogWheelSpinStability() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	// ---------------- Run state ----------------

	enum class EPhase : uint8 { Idle, Settle, Running, Between, Done };

	EPhase Phase = EPhase::Idle;

	/**
	 * Corner loads with the car at rest on its springs, captured at the end of
	 * Settle. FR, FL, RR, RL.
	 *
	 * The static weight split used to be averaged out of DRIVING samples filtered on
	 * |Ay| < 0.03 and speed > 8 m/s. Nothing in that window excludes longitudinal
	 * acceleration, and the run-up to 50 km/h satisfies it for its whole second half
	 * - so 184 of 195 qualifying samples came from a car accelerating at 0.22 g, and
	 * the metric reported the rearward load transfer of the launch rather than the
	 * weight distribution. It read 53.6% where the settled car reads 56.2%, and it
	 * moved between runs of the SAME car as the mix of accelerating to cruising
	 * samples changed, which quietly made it useless for comparing variants.
	 */
	float SettledLoadN[4] = { 0.f, 0.f, 0.f, 0.f };
	bool bSettledLoadValid = false;

	/** Kinematics is waiting out the settling transient before it reads its live values. */
	bool bKinematicsAwaitingSettle = false;
	float KinematicsPrevTravelCm[4] = { 0.f, 0.f, 0.f, 0.f };
	float KinematicsSettleTookSeconds = 0.f;
	bool bKinematicsSettleConverged = false;
	bool bStartPending = false;

	/** Live-readout state, independent of any run. */
	FVector LiveLastVelocity = FVector::ZeroVector;
	float LiveLatG = 0.f;
	float LiveLongG = 0.f;
	void DrawLiveTelemetry(float DeltaSeconds);
	float PhaseSeconds = 0.f;
	int32 VariantIndex = 0;
	int32 VariantCount = 1;

	TArray<FBenchRun> Runs;

	/** Per-sample telemetry for the current run. */
	struct FSample
	{
		float Time = 0.f;
		float SpeedMPerSec = 0.f;
		float LateralAccelG = 0.f;
		float LongAccelG = 0.f;
		float YawRateDegPerSec = 0.f;
		float RollDeg = 0.f;
		float PitchDeg = 0.f;
		float SteerInput = 0.f;
		/** Front ROAD-WHEEL angle, deg - what the gradient is actually computed from. */
		float SteerAngleDeg = 0.f;
		/** Taken while the driveline was still settling after a gear change. */
		bool bShiftSettling = false;
		float LoadN[4] = { 0.f, 0.f, 0.f, 0.f };
		float TravelCm[4] = { 0.f, 0.f, 0.f, 0.f };
		float CamberDeg[4] = { 0.f, 0.f, 0.f, 0.f };
		/** Per-wheel longitudinal slip and force - which axle is doing the driving. */
		float Kappa[4] = { 0.f, 0.f, 0.f, 0.f };
		float FxN[4] = { 0.f, 0.f, 0.f, 0.f };

		// SLIP ANGLE AND LATERAL FORCE, which is what tells you WHICH AXLE gives up
		// first. The report's understeer gradient is the net result of that; these are
		// the cause. Without them, asking why a car understeers means guessing at
		// parameters instead of reading which end is sliding.
		float AlphaDeg[4] = { 0.f, 0.f, 0.f, 0.f };
		float FyN[4] = { 0.f, 0.f, 0.f, 0.f };

	/** Substep-rate kappa span - see UPacejkaTyreComponent::KappaSpanMin. */
	float KappaSpan[4] = { 0.f, 0.f, 0.f, 0.f };

	/**
	 * Wheel spin, rad/s, straight off the shaft.
	 *
	 * The consistency check that kappa alone cannot give: a free-rolling wheel must
	 * sit at v/R0, and kappa must equal (Omega*R0 - Vcx)/|Vcx|. If Omega says the
	 * wheel is rolling cleanly while kappa says it is spinning 20% fast, then kappa
	 * is not coming from the kinematics and the transient state is the culprit
	 * rather than the drivetrain.
	 */
	float OmegaRadPerSec[4] = { 0.f, 0.f, 0.f, 0.f };

	/** The omega and Vcx kappa was formed from - see UPacejkaTyreComponent::GetOmegaUsed. */
	float OmegaUsed[4] = { 0.f, 0.f, 0.f, 0.f };
	float VcxUsed[4] = { 0.f, 0.f, 0.f, 0.f };
		float DistanceM = 0.f;
	};
	TArray<FSample> Samples;

	FVector PreviousVelocity = FVector::ZeroVector;
	FTransform StartTransform;
	float DistanceTravelledM = 0.f;

	/** Bar rates as the vehicle was authored, restored when the sweep finishes. */
	TArray<float> SavedAntiRollRates;
	uint8 SavedDrivetrainType = 0;
	bool bConfigSaved = false;

	/** Spawned obstacle for the articulation test, destroyed on completion. */
	UPROPERTY()
	AActor* SpawnedObstacle = nullptr;

	// ---------------- Flow ----------------

	bool ResolveTarget();

	/** Freeze the steer limit the first time a test commands steer. */
	void EnsureSteerLimitLocked();
	void SaveVehicleConfig();
	void RestoreVehicleConfig();
	void ApplyVariant(int32 Index, FString& OutLabel);
	void BeginVariant();
	void FinishVariant();
	void FinishBench();

	void ResetVehicleToStart();
	void RecordSample(float DeltaSeconds);

	/** Per-test driving. Returns true when the manoeuvre is over. */
	bool DriveSkidpad(float ElapsedSeconds);
	bool DrivePowerOn(float ElapsedSeconds);
	bool DriveStepSteer(float ElapsedSeconds);
	bool DriveArticulation(float ElapsedSeconds);
	bool DriveStraightLine(float ElapsedSeconds);

	/**
	 * PI speed hold. Returns the throttle/brake pair.
	 *
	 * Proportional alone cannot hold a target against a constant disturbance: it
	 * settles wherever its own output happens to balance the drag, which is BELOW
	 * the target by however much throttle that balance needs. This one parked at
	 * 46.5 km/h against a target of 50 - 3.5 km/h of error asking for 28% throttle,
	 * and 28% throttle being exactly what the car needed to hold 46.5. Adding
	 * rolling resistance widened that gap enough to matter, and the skidpad's steer
	 * ramp waits on a speed that was therefore never arriving.
	 */
	void SpeedController(float TargetKph, float& OutThrottle, float& OutBrake);

	/** Accumulated speed error, km/h*s. Cleared per run. */
	float SpeedIntegral = 0.f;

	/**
	 * Shifts on engine speed, so a run can actually reach its target.
	 *
	 * Without it the car stays in whatever gear it started in: it either hits the
	 * limiter and stops accelerating, or bogs and never gets there - and the run
	 * ends early having measured a speed sweep that stopped sweeping.
	 */
	void AutoShift(float DeltaSeconds);

	/** Seconds since the last shift, so the box is not hunting every frame. */
	float ShiftCooldown = 0.f;

	/**
	 * Seconds after a gear change during which samples are excluded from the
	 * steady-state regressions.
	 *
	 * The gearbox has no clutch yet, so a shift locks the new ratio up instantly and
	 * dumps a torque step into the driven axle. The tyres spend that step doing
	 * longitudinal work, which is grip they are not spending sideways, so the car
	 * yaws - and a steady-state measurement reads it as the balance moving. It is
	 * loudest on RWD, where the axle carrying the step is also the one holding the
	 * back of the car in line.
	 *
	 * Excluding the window measures the car rather than the shift. Zero to keep
	 * everything and see the transients.
	 */
	UPROPERTY(EditAnywhere, Category = "Bench", meta = (ClampMin = "0.0", Units = "s"))
	float ShiftSettleSeconds = 1.2f;

	/** Best lateral g seen this run, and when - the skidpad's stop condition. */
	float BestLateralG = 0.f;
	float TimeOfBestG = 0.f;

	/** When the last gear change happened, for the settle mask. Negative = none. */
	float LastShiftTime = -1000.f;

	/** When the steer sweep began, or negative while still getting up to speed. */
	float SteerRampStartTime = -1.f;

	/** Run time at which steer reached its hold value, and at which power went in. */
	float PowerOnSteerHeldTime = -1.f;
	float PowerOnRampStartTime = -1.f;

	/** The steer angle the entry ramp stopped at, held for the whole power phase. */
	float PowerOnFrozenSteer = 0.f;

	/** Gear actually engaged when power went in - reported, so it can be compared. */
	int32 PowerOnHeldGear = -1;

	/** Dose controller: integral term, last output, and how often it hit the ceiling. */
	float PowerOnThrottleIntegral = 0.f;
	float PowerOnLastThrottle = 0.f;
	int32 PowerOnSaturatedSamples = 0;
	int32 PowerOnDoseSamples = 0;

	/** Simulated-time reading when this run began, so the run's own share can be told. */
	double SubstepTimeAtRunStart = 0.0;

	/** Throttles the once-a-second "still waiting for speed" report. */
	float WaitReportTimer = 0.f;

	// ---------------- Measurement ----------------

	void MeasureKinematics(FBenchRun& Run) const;
	void MeasureSkidpad(FBenchRun& Run) const;
	void MeasurePowerOn(FBenchRun& Run) const;
	void MeasureStepSteer(FBenchRun& Run) const;

	/** Reports the fit's intercept against its own standard error, and when that fails,
	 *  the gradient either side of the band's midpoint. Shared by both gradient tests. */
	void ReportInterceptAndHalves(FBenchRun& Run, const TArray<float>& Ay,
		const TArray<float>& SteerMinusAckermann) const;
	void MeasureArticulation(FBenchRun& Run) const;
	void MeasureStraightLine(FBenchRun& Run) const;

	// ---------------- Output ----------------

	void ReportRuns() const;
	void WriteCsv() const;
	FString TestName() const;

	float SprungMassKg() const;
	float TotalMassKg() const;
};
