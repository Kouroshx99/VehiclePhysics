// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Car.h"
#include "VehiclePhysics/Vehicles/VehiclePhysicsCompat.h"
#include "VehiclePhysics/Vehicles/VehiclePhysicsCompat.h"

#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/BodySetup.h"
// USkeletalBodySetup is declared in PhysicsAsset.h in 4.27, not a header of its own.
#include "PhysicsEngine/PhysicsAsset.h"
#include "Drivetrain.h"
#include "Curves/CurveFloat.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "ECU.h"
#include "PacejkaTyreComponent.h"
#include "Widgets/SOverlay.h"
#include "VehicleEngine.h"
#include "Widgets/SOverlay.h"
#include "WishboneSuspension.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
// Chaos particle access, and USkeletalBodySetup's own header. Neither exists on 4.27,
// where USkeletalBodySetup is declared in PhysicsAsset.h - included unconditionally
// below, since 5.x has that header too.
#if VP_HAS_ASYNC_PHYSICS
#include "Chaos/ParticleHandle.h"
#include "Chaos/Particle/ParticleUtilities.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#endif
#include "PhysicsEngine/PhysicsAsset.h"
#include "Widgets/Layout/SBox.h"
#include "VehiclePhysics/Vehicles/CarDebugHUD.h"
#include "VehiclePhysics/Vehicles/STireGraphWidget.h"
#include "VehiclePhysics/Vehicles/VehicleRoster.h"

namespace
{
	static constexpr float MToCm = 100.f;
	static constexpr float CmToM = 0.01f;
	static constexpr float CmToMPO2 = 0.0001f;
	static constexpr float MPerSecToKMPerHour = 3.6f;
}

/**
 * Placeholder rear-right hardpoints. NOT measured off a real rear rig - none
 * exists in this project's assets, so this is a designed baseline, honestly
 * distinct from a translated copy of the front corner:
 *
 *  - Lower arm and both chassis-side coilover points keep the front corner's
 *    exact shape, shifted back by a 265 cm wheelbase.
 *  - The upper arm's sweep radius is deliberately 0.75x the lower arm's (31.87
 *    vs 42.49 cm), so this corner has real camber/caster/KPI gain through
 *    travel - unlike the front baseline, which is a near-parallelogram with
 *    almost none. Verified: kingpin length 35.54 cm, sweep ratio exactly 0.750,
 *    coilover length unchanged by the shift (53.73 cm, since translation is rigid).
 *
 * Retune freely - this exists so a fresh vehicle has four visually distinct,
 * kinematically valid corners rather than four overlapping copies of one.
 */
static FWishboneHardpoints BuildRearHardpoints()
{
	FWishboneHardpoints Rear;
	Rear.LCA_F = FVector(-121.753342f, 57.374817f, 4.712109f);
	Rear.LCA_B = FVector(-150.456505f, 57.408588f, 4.699930f);
	Rear.LCA_U = FVector(-132.725143f, 99.871216f, 4.239235f);
	Rear.UCA_F = FVector(-121.754883f, 57.373707f, 38.508877f);
	Rear.UCA_B = FVector(-150.458031f, 57.407295f, 38.496670f);
	Rear.UCA_U = FVector(-132.737622f, 89.250063f, 38.151779f);
	Rear.DamperMount = FVector(-136.929558f, 85.290191f, 8.898479f);
	Rear.CoiloverTop = FVector(-138.719994f, 62.784325f, 57.652046f);
	Rear.WheelCentre = FVector(-132.725143f, 106.000000f, 21.136772f);
	return Rear;
}

///Cleaned
ACar::ACar()
{
	PrimaryActorTick.bCanEverTick = true;

#if VP_HAS_ASYNC_PHYSICS
	// HERE, NOT IN BeginPlay. AActor::RegisterAllActorTickFunctions reads this flag to
	// decide whether to call FPhysScene_Chaos::RegisterAsyncPhysicsTickActor, and that
	// runs at registration - before BeginPlay. Set it later and AsyncPhysicsTickActor is
	// simply never called, with no error: the car sits there while every force the
	// suspension computes goes nowhere.
	bAsyncPhysicsTickEnabled = true;
#endif

	CarMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetSimulatePhysics(true);
	CarMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CarMesh->SetCollisionObjectType(ECC_Vehicle);
	CarMesh->SetCollisionProfileName(FName("BlockAll"));

	SetRootComponent(CarMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->TargetArmLength = 650.f;
	SpringArm->bInheritRoll = false;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	SpringArm->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeRotation(FRotator(5.f, 0.f, 0.f));

	Engine = CreateDefaultSubobject<UVehicleEngine>(TEXT("Engine"));
	Drivetrain = CreateDefaultSubobject<UDrivetrain>(TEXT("Drivetrain"));
	ECU = CreateDefaultSubobject<UECU>(TEXT("Electronic Control Unit"));

	FrontRightSuspension = CreateDefaultSubobject<UWishboneSuspension>(TEXT("FrontRightSuspension"));
	FrontRightSuspension->SetMirrorAcrossCentreline(/*bMirror*/ false);

	FrontLeftSuspension = CreateDefaultSubobject<UWishboneSuspension>(TEXT("FrontLeftSuspension"));
	FrontLeftSuspension->SetMirrorAcrossCentreline(/*bMirror*/ true);

	RearRightSuspension = CreateDefaultSubobject<UWishboneSuspension>(TEXT("RearRightSuspension"));
	RearRightSuspension->SetMirrorAcrossCentreline(/*bMirror*/ false);
	RearRightSuspension->Hardpoints = BuildRearHardpoints();

	RearLeftSuspension = CreateDefaultSubobject<UWishboneSuspension>(TEXT("RearLeftSuspension"));
	RearLeftSuspension->SetMirrorAcrossCentreline(/*bMirror*/ true);
	RearLeftSuspension->Hardpoints = BuildRearHardpoints();

	FrontRightTire = CreateDefaultSubobject<UPacejkaTyreComponent>(TEXT("FrontRightTyre"));
	FrontLeftTire  = CreateDefaultSubobject<UPacejkaTyreComponent>(TEXT("FrontLeftTyre"));
	RearRightTire  = CreateDefaultSubobject<UPacejkaTyreComponent>(TEXT("RearRightTyre"));
	RearLeftTire   = CreateDefaultSubobject<UPacejkaTyreComponent>(TEXT("RearLeftTyre"));

	// Paired at the point of construction, so a corner and its tyre are written on
	// one line and cannot drift apart. FR, FL, RR, RL.
	Corners = {
		{ FrontRightSuspension, FrontRightTire },
		{ FrontLeftSuspension,  FrontLeftTire  },
		{ RearRightSuspension,  RearRightTire  },
		{ RearLeftSuspension,   RearLeftTire   }
	};

	Drivetrain->EngineShaft = Engine->GetShaft();
	Drivetrain->ClutchShaft = &ClutchShaft;
	
	Drivetrain->FrontRightWheelShaft = FrontRightTire->GetShaft();
	Drivetrain->FrontLeftWheelShaft = FrontLeftTire->GetShaft();
	Drivetrain->RearRightWheelShaft = RearRightTire->GetShaft();
	Drivetrain->RearLeftWheelShaft = RearLeftTire->GetShaft();
	Drivetrain->FrontDiffShaft = &FrontDiffShaft;
	Drivetrain->RearDiffShaft = &RearDiffShaft;
	SetReplicates(true);
	CarMesh->SetIsReplicated(true);
	ClutchShaft.Inertia = 0.2f;

	// Steering limit against speed, km/h -> fraction of MaxSteerAngle. Full lock while
	// manoeuvring, tapering so that a flick of lock at speed is not a spin. Only used
	// when no SteerCurve asset is assigned, which is the state every freshly made
	// vehicle Blueprint starts in.
	if (FRichCurve* Steer = InlineSteerCurve.GetRichCurve())
	{
		Steer->Reset();
		Steer->AddKey(0.f,   1.00f);
		Steer->AddKey(40.f,  1.00f);
		Steer->AddKey(80.f,  0.75f);
		Steer->AddKey(120.f, 0.55f);
		Steer->AddKey(160.f, 0.42f);
		Steer->AddKey(200.f, 0.35f);
		Steer->AddKey(260.f, 0.30f);
	}
	Drivetrain->VehicleEngine = Engine;
	ECU->Drivetrain = Drivetrain;
	
}

float ACar::GetWheelSpeed(EWheel Wheel)
{
	switch(Wheel)
	{
		case EWheel::EW_FrontRight: return FMath::RadiansToDegrees(FrontRightTire->GetShaft()->GetAngularVelocity());
		case EWheel::EW_FrontLeft: return FMath::RadiansToDegrees(FrontLeftTire->GetShaft()->GetAngularVelocity());
		case EWheel::EW_RearRight: return FMath::RadiansToDegrees(RearRightTire->GetShaft()->GetAngularVelocity());
		case EWheel::EW_RearLeft: return FMath::RadiansToDegrees(RearLeftTire->GetShaft()->GetAngularVelocity());
		default: return 0;
	}
}

float ACar::GetWheelZPosition(EWheel Wheel)
{
	switch(Wheel)
	{
		case EWheel::EW_FrontRight: return 0.f;
		case EWheel::EW_FrontLeft: return 0.f;
		case EWheel::EW_RearRight: return 0.f;
		case EWheel::EW_RearLeft: return 0.f;
		default: return 0;
	}
}

FRotator ACar::GetWheelRotation(EWheel Wheel)
{
	switch (Wheel)
	{
	case EWheel::EW_FrontRight: return FRotator::ZeroRotator;
	case EWheel::EW_FrontLeft: return FRotator::ZeroRotator;
	case EWheel::EW_RearRight: return FRotator::ZeroRotator;
	case EWheel::EW_RearLeft: return FRotator::ZeroRotator;
	default: return FRotator::ZeroRotator;
	}
}

FVector ACar::GetWheelPosition(EWheel Wheel)
{
	switch (Wheel)
	{
	case EWheel::EW_FrontRight: return FVector::ZeroVector;
	case EWheel::EW_FrontLeft: return FVector::ZeroVector;
	case EWheel::EW_RearRight: return FVector::ZeroVector;
	case EWheel::EW_RearLeft: return FVector::ZeroVector;
	default: return FVector::ZeroVector;
	}
}

float ACar::GetWheelAngle(EWheel Wheel)
{
	switch(Wheel)
	{
	case EWheel::EW_FrontRight: return 0.f;
	case EWheel::EW_FrontLeft: return 0.f;
	default: return 0;
	}
}

int32 ACar::GetGear() const
{
	return Drivetrain->CurrentGear;
}

///Cleaned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine && GEngine->GameViewport && bDrawGraphs)
	{
		auto BuildTireGraph = [&](float HPadding, float VPadding, UTireParamsDataAsset* TireParams)
		{
			TSharedRef<STireGraphWidget> TireGraph = SNew(STireGraphWidget)
				.Asset(TireParams)
				.bIsInteractive(false);
			
			GEngine->GameViewport->AddViewportWidgetContent(
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Bottom)
				.Padding(HPadding, VPadding)
				[
					SNew(SBox)
					.WidthOverride(350.f)
					.HeightOverride(350.f)
					[
						TireGraph
					]
				],
				100 // Z-order
			);
			
			TireGraph->Zoom = 250.f;
			return TireGraph;
		};
		
		// Named loudly rather than left to an empty graph: a missing tyre asset also
		// means that corner has no vertical spring and no grip, so the car sags and
		// slides, and the cause is a blank field rather than anything in the physics.
		for (int32 i = 0; i < Corners.Num(); ++i)
		{
			if (Corners[i].Tyre && Corners[i].Tyre->GetTireParams() == nullptr)
			{
				UE_LOG(LogTemp, Error,
					TEXT("%s: corner %d (%s) has NO TireParams asset assigned. That corner will "
						 "produce no grip and carry no load. Assign one on the tyre component."),
					*GetName(), i, *Corners[i].Tyre->GetName());
			}
		}

		TireGraphWidgetFrontRight = BuildTireGraph(360.f, 360.f, FrontRightTire->GetTireParams());
		TireGraphWidgetFrontLeft = BuildTireGraph(10.f, 360.f, FrontLeftTire->GetTireParams());
		TireGraphWidgetRearRight = BuildTireGraph(360.f, 10.f, RearRightTire->GetTireParams());
		TireGraphWidgetRearLeft = BuildTireGraph(10.f, 10.f, RearLeftTire->GetTireParams());
	}
	
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
	SetReplicates(true);
	CarMesh->SetIsReplicated(true);
	OGSpringArmRotation = SpringArm->GetRelativeRotation();

	CustomPhysics.BindUObject(this, &ACar::SubstepPhysics);

	FBodyInstance* BodyInstance = CarMesh ? CarMesh->GetBodyInstance() : nullptr;

	// NO BODY, NO CAR - and reported rather than crashed on.
	//
	// GetBodyInstance returns null whenever the skeletal mesh has no physics asset,
	// which most often means no mesh was set at all. Everything below this point reads
	// the body: the COM nudge, the mass update, the cached COM offset. The next line
	// used to dereference it unconditionally, so spawning a car without a mesh took the
	// editor down with an access violation instead of naming the setup mistake.
	//
	// The C++ car classes carry the simulation and no assets - they exist to be
	// subclassed by a Blueprint that supplies the mesh - so they are exactly the case
	// that reaches here.
	if (BodyInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s has no physics body and will not be simulated. A car needs a skeletal "
				 "mesh with a physics asset assigned; without one there is nothing for the "
				 "suspension to push against. If this is one of the C++ car classes, use a "
				 "Blueprint subclass that supplies the mesh instead."),
			*GetName());
		return;
	}

	//Assuming the pivot is at center
	float Y = BodyInstance->GetUnrealWorldTransform_AssumesLocked().InverseTransformPosition(BodyInstance->GetCOMPosition()).Y;
	UE_LOG(LogTemp, Warning, TEXT("Y is %f"), Y);
	FVector NewCOMNudge = FVector(0, -Y, 0) + COMOffset;
	BodyInstance->COMNudge = NewCOMNudge;
	BodyInstance->UpdateMassProperties();

	FVector COMPos = BodyInstance->GetCOMPosition();
	FQuat Rotation = BodyInstance->GetUnrealWorldTransform_AssumesLocked().GetRotation();

	// Cached after the nudge above, so it is the COM the solver will actually see.
	// GetSubstepBodyToWorld needs it to recover the component transform from the
	// live body state mid-substep.
	COMLocalToMesh = CarMesh->GetComponentTransform().InverseTransformPosition(COMPos);

	// Pairing checks. Every one of these is a setup mistake that is invisible in
	// motion - the car just handles wrong - so each is named loudly once, here,
	// rather than left to be inferred from behaviour.
	for (int32 i = 0; i < Corners.Num(); ++i)
	{
		UWishboneSuspension* Corner = Corners[i].Suspension;
		UPacejkaTyreComponent* Tyre = Corners[i].Tyre;
		if (Corner == nullptr)
		{
			continue;
		}

		// A corner with no tyre carries NO LOAD - the vertical spring lives in the
		// tyre - so it settles to full droop while the others hold the car up. That
		// reads as a broken spring rate or a bad hardpoint rather than a missing
		// component, so name the corner and say it loudly.
		if (Tyre == nullptr)
		{
			UE_LOG(LogTemp, Error,
				TEXT("%s: corner %d (%s) has no tyre component. It has no vertical spring of "
					 "its own, so it will carry no load and hang at full droop."),
				*GetName(), i, *Corner->GetName());
			continue;
		}

		// The corner's radius and the tyre asset's R0 are the SAME physical
		// quantity, held twice because the corner has to place a contact patch in
		// the editor with no tyre attached. The corner measures the deflection and
		// R0 prices it - via rho_z/R0 in 4.E68 - so a disagreement is a systematic
		// error in both Fz and slip ratio, and nothing about it is visible in
		// motion. Cheap to check, silent to ship.
		const float TyreR0Cm = Tyre->GetUnloadedRadiusCm();
		if (TyreR0Cm > KINDA_SMALL_NUMBER
			&& !FMath::IsNearlyEqual(Corner->GetWheelRadiusCm(), TyreR0Cm, 0.1f))
		{
			UE_LOG(LogTemp, Error,
				TEXT("%s: corner %d (%s) WheelRadiusCm %.2f cm disagrees with its tyre's R0 "
					 "%.2f cm. Deflection is measured against the first and priced against the "
					 "second, so Fz and slip ratio both carry the %.1f%% difference."),
				*GetName(), i, *Corner->GetName(), Corner->GetWheelRadiusCm(), TyreR0Cm,
				100.f * FMath::Abs(Corner->GetWheelRadiusCm() - TyreR0Cm) / TyreR0Cm);
		}
	}

	// The two corners of an axle are two ends of ONE bar, so they have to be set
	// to the same rate - otherwise it pushes harder on one side than the other
	// and puts net vertical force into the car out of nowhere. Nothing about
	// that is visible in motion; it just reads as a car that sits or rolls
	// oddly. See UWishboneSuspension::AntiRollRateNPerM.
	for (int32 Axle = 0; Axle + 1 < Corners.Num(); Axle += 2)
	{
		const UWishboneSuspension* Right = Corners[Axle].Suspension;
		const UWishboneSuspension* Left = Corners[Axle + 1].Suspension;
		if (Right == nullptr || Left == nullptr) { continue; }

		if (!FMath::IsNearlyEqual(Right->GetAntiRollRateNPerM(), Left->GetAntiRollRateNPerM(), 1.f))
		{
			UE_LOG(LogTemp, Error,
				TEXT("%s: %s axle anti-roll rates disagree - %s %.0f N/m vs %s %.0f N/m. They are "
					 "two ends of one bar; unequal rates make it push harder on one side and "
					 "add net vertical force to the car."),
				*GetName(), Axle == 0 ? TEXT("front") : TEXT("rear"),
				*Right->GetName(), Right->GetAntiRollRateNPerM(),
				*Left->GetName(), Left->GetAntiRollRateNPerM());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("COM is %s"), *COMPos.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Loc is %s"), *BodyInstance->GetUnrealWorldTransform_AssumesLocked().GetLocation().ToString());
	// Safe here: AActor::BeginPlay dispatches Component->BeginPlay for every
	// component (Actor.cpp:3556), and Super::BeginPlay ran at the top of this
	// function - so InitialiseGeometry has already resolved the rest geometry.
	//
	// Single components rather than a 3D distance, matching what this replaced:
	// track is lateral, wheelbase longitudinal, and neither should pick up a
	// ride-height or cross-axis difference between corners.
	if (FrontRightSuspension && FrontLeftSuspension && RearRightSuspension)
	{
		TrackWidth = FMath::Abs(FrontRightSuspension->GetWheelCentreRest().Y
							  - FrontLeftSuspension->GetWheelCentreRest().Y);
		WheelBase  = FMath::Abs(FrontRightSuspension->GetWheelCentreRest().X
							  - RearRightSuspension->GetWheelCentreRest().X);

		// Zero track kills Ackermann silently - both front wheels would take the
		// inner angle - so say so rather than let it look like a handling quirk.
		if (TrackWidth < KINDA_SMALL_NUMBER || WheelBase < KINDA_SMALL_NUMBER)
		{
			UE_LOG(LogTemp, Error,
				TEXT("%s: degenerate vehicle dimensions - track %.2f cm, wheelbase %.2f cm. "
					 "Ackermann steering needs both. Check the corner hardpoints and their mirror flags."),
				*GetName(), TrackWidth, WheelBase);
		}
	}

	DefaultLocation = GetActorLocation();
	DefaultRotation = GetActorRotation();

}

void ACar::ApplyDriverInput(float InThrottle, float InBrake, float InSteer)
{
	// Through the same setters the input bindings call, so a bench run and a
	// lap driven by hand exercise identical code - including the speed-dependent
	// steer limit and Ackermann split, which a bench writing steer angles
	// straight to the corners would silently skip.
	SetThrottle(FMath::Clamp(InThrottle, 0.f, 1.f));
	SetBrake(FMath::Clamp(InBrake, 0.f, 1.f));
	SetSteering(FMath::Clamp(InSteer, -1.f, 1.f));
}

UWishboneSuspension* ACar::GetCornerSuspension(EWheel Wheel) const
{
	switch (Wheel)
	{
	case EWheel::EW_FrontRight: return FrontRightSuspension;
	case EWheel::EW_FrontLeft:  return FrontLeftSuspension;
	case EWheel::EW_RearRight:  return RearRightSuspension;
	case EWheel::EW_RearLeft:   return RearLeftSuspension;
	default:                    return nullptr;
	}
}

void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Zeroed HERE rather than at the end of the callback, because the callback does not
	// know it is the last one. Tick runs pre-physics, so by the time anything at
	// TG_PostPhysics reads these they hold this frame's complete total.
	SubstepCountThisFrame = 0;
	SubstepTimeSumThisFrame = 0.f;

	// THE SEAM. 4.27 dispatches the solve through PhysX substepping; 5.x must not,
	// because Chaos's AddCustomPhysics is a one-line stub that calls the delegate inline
	// with the frame delta - keeping it there would run the integrator at frame rate.
	// On 5.x AsyncPhysicsTickActor drives it instead, at a fixed rate.
#if !VP_HAS_ASYNC_PHYSICS
	CarMesh->GetBodyInstance()->AddCustomPhysics(CustomPhysics);
#endif
	
	Engine->SetThrottle(Throttle);
	ECU->ApplyBrake(Brake);
	float Velocity = CarMesh->GetPhysicsLinearVelocity().Size() * CmToM;
	SpringArm->AddRelativeRotation(FRotator(MouseYSpeed, MouseXSpeed, 0.f));

	UpdateTireGraphs();

	// Debug drawing is NOT done here - see FCarDebugDrawTickFunction. This tick runs
	// at TG_PrePhysics, so anything drawn from it is a frame stale.

	if (!bLogCarData)
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("----------  New Frame ---------"));
	if(bLogAcceleration)
	{
		Acceleration = (Velocity - LastVelocity)/DeltaTime;
		UE_LOG(LogTemp,Warning,TEXT("Acceleration m/s: %f"), Acceleration);
		LastVelocity = CarMesh->GetPhysicsLinearVelocity().Size() * CmToM;
	}

	
	if (bLogCarData)
		UE_LOG(LogTemp, Display, TEXT("Steer Angle: %f, Throttle: %f, Brake: %f"), 
			CurrentSteer * MaxSteerAngle,
			Throttle,
			Brake);
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Throttle"), this, &ThisClass::SetThrottle);
	PlayerInputComponent->BindAxis(FName("Steering"), this, &ThisClass::SetSteeringFromPlayer);
	PlayerInputComponent->BindAxis(FName("Horizontal"), this, &ThisClass::SetMouseX);
	PlayerInputComponent->BindAxis(FName("Vertical"), this, &ThisClass::SetMouseY);
	PlayerInputComponent->BindAxis(FName("Brake"), this, &ThisClass::SetBrake);
	PlayerInputComponent->BindAction(FName("GearUp"), IE_Pressed, this, &ThisClass::GearUp);
	PlayerInputComponent->BindAction(FName("GearDown"), IE_Pressed, this, &ThisClass::GearDown);
	PlayerInputComponent->BindAction(FName("Reset"), IE_Pressed, this, &ThisClass::ResetCar);
	PlayerInputComponent->BindAction(FName("NextCar"), IE_Pressed, this, &ThisClass::NextCar);
	PlayerInputComponent->BindAction(FName("PrevCar"), IE_Pressed, this, &ThisClass::PreviousCar);

}

FCarBodyState ACar::ReadBodyState(FBodyInstance* BodyInstance, bool bAllowGameThreadRead) const
{
	FCarBodyState State;
	if (BodyInstance == nullptr)
	{
		return State;
	}

#if VP_HAS_ASYNC_PHYSICS
	// The physics-thread particle. Null until the proxy has been registered with the
	// solver, which takes a couple of ticks after BeginPlay.
	if (FPhysicsActorHandle Handle = BodyInstance->GetPhysicsActor())
	{
		if (const Chaos::FRigidBodyHandle_Internal* Internal = Handle->GetPhysicsThreadAPI())
		{
			// FParticleUtilitiesGT is named for its accessor pair (X and R), not for a
			// thread - handed the internal particle it reads the live physics-thread pose.
			State.Rotation           = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(Internal).GetRotation();
			State.COMWS              = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(Internal);
			State.LinearVelocityWS   = Internal->V();
			State.AngularVelocityRad = Internal->W();
			State.bFromPhysicsThread = true;
			State.bValid             = true;
			return State;
		}
	}
#endif

	// NO GAME-THREAD FALLBACK FROM THE PHYSICS THREAD, and this is not caution - it was a
	// bug, and a loud one.
	//
	// Every FBodyInstance accessor ensures on Chaos's IsInGameThreadContext(). Falling
	// back to them from the async tick therefore produced a handled ensure per call, each
	// with a stack walk, which is a visible hitch on startup and a wall of red in the log.
	// Worse than the noise: what they return at that moment is the SPAWN pose. The car is
	// dropped in a metre above the ground, so the first solves ran against a body a metre
	// up, and the suspension began from a state the car was never in. That showed as the
	// vehicle settling several millimetres off its design ride height for the rest of the
	// run, which then moved static camber, ride frequency and every dynamic number after
	// them.
	//
	// So when there is no particle yet there is nothing to read, and the honest answer is
	// to say so and let the caller skip the step. One or two skipped steps at BeginPlay
	// cost nothing; solving against the wrong pose costs the whole run.
	if (!bAllowGameThreadRead)
	{
		return State;   // bValid stays false
	}

	State.Rotation           = BodyInstance->GetUnrealWorldTransform_AssumesLocked().GetRotation();
	State.COMWS              = BodyInstance->GetCOMPosition();
	State.LinearVelocityWS   = BodyInstance->GetUnrealWorldVelocity_AssumesLocked();
	State.AngularVelocityRad = BodyInstance->GetUnrealWorldAngularVelocityInRadians_AssumesLocked();
	State.bValid             = true;
	return State;
}

#if VP_HAS_ASYNC_PHYSICS
void ACar::AsyncPhysicsTickActor(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickActor(DeltaTime, SimTime);

	if (CarMesh == nullptr)
	{
		return;
	}
	if (FBodyInstance* BodyInstance = CarMesh->GetBodyInstance())
	{
		SubstepPhysics(DeltaTime, BodyInstance);
	}
}
#endif

///Cleaned
void ACar::SubstepPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	// Count the callback and the simulated time it was given. See
	// ACar::GetSubstepCountThisFrame.
	++SubstepCountThisFrame;
	SubstepTimeSumThisFrame += DeltaTime;
	SubstepTimeTotalSeconds += double(DeltaTime);

	// ONCE per solve, and ONLY from the physics thread. Everything below reads this
	// rather than the body, because the game-thread copy does not move between the steps
	// of a frame and is not safe to touch from here - see FCarBodyState.
	// The flag is the seam here, not an #if. On 4.27 this runs inside a PhysX substep
	// callback where FBodyInstance IS the authoritative copy, so reading it is both
	// correct and safe. On 5.x the game-thread copy is a frame stale and unsafe to touch
	// from the async tick, so a missing particle means skip the step - see ReadBodyState.
	const FCarBodyState State = ReadBodyState(BodyInstance, /*bAllowGameThreadRead*/ !VP_HAS_ASYNC_PHYSICS);
	if (!State.bValid)
	{
		// The solver has not adopted this body yet. Skipping is correct: there is no pose
		// to solve against, and the alternative is solving against the spawn pose.
		UE_LOG(LogTemp, Verbose,
			TEXT("[Car] async tick with no physics particle yet - step skipped"));
		return;
	}

	const FVector uBodyVelocity = State.LinearVelocityWS;
	VelocityKMH = uBodyVelocity.Size() * CmToM * MPerSecToKMPerHour;
	ForwardVelocityKMH = FVector::DotProduct(uBodyVelocity * CmToM * MPerSecToKMPerHour,
		State.Rotation.GetAxisX());
	
	if(bLogDeltaTime)
		UE_LOG(LogTemp, Display, TEXT("--- Delta Time : %f ---"), DeltaTime);
	if(bLogVelocityKMH)
		UE_LOG(LogTemp, Display, TEXT("Velocity KM/H: %d"), VelocityKMH);
	if(bVisualizeCOM)
		DrawCOM(BodyInstance);

	// The corners and their tyres, on the engine's substep cadence. This is the
	// suspension - there is no second path to order against any more.
	SolveWishboneCorners(DeltaTime, BodyInstance, State);

	// The gearbox gets first call on the throttle: during a downshift re-engagement it
	// blips the engine up to meet the faster gearbox side. -1 means it does not want
	// it and the driver keeps the pedal.
	//
	// Both this and the torque now happen per SUBSTEP. Torque used to be worked out
	// once a frame in the engine's own tick, from a speed sampled at the end of the
	// previous one, and the same stale value was then applied to every substep. Fine
	// while the engine was rigidly geared to the driveline; an oscillator once the
	// clutch could decouple it.
	const float ShiftThrottle = Drivetrain ? Drivetrain->GetShiftThrottleOverride() : -1.f;
	Engine->SetThrottle(ShiftThrottle >= 0.f ? ShiftThrottle : Throttle);

	Engine->UpdateTorque();
	Engine->GetShaft()->ApplyImpulse(Engine->GetCurrentTorque() * DeltaTime);
	Drivetrain->SolveConstraints(Drivetrain->Connections, DeltaTime);
	
	if (bLogAngVelocity)
		UE_LOG(LogTemp, Display, TEXT("Vehicle Angular Velocity (Rad/s): %s"), 
			*State.AngularVelocityRad.ToString());
}

FTransform ACar::GetSubstepBodyToWorld(const FCarBodyState& State) const
{
	const FQuat Rotation = State.Rotation;
	const FVector COMPos = State.COMWS;

	// Walk back from the live COM to where the component origin must be. Reading
	// CarMesh->GetComponentTransform() here instead would be a frame stale, which at
	// speed is tens of centimetres.
	return FTransform(Rotation, COMPos - Rotation.RotateVector(COMLocalToMesh));
}

void ACar::ExchangeAntiRollTravel()
{
	// Corners is FR, FL, RR, RL, so the axles are the two adjacent pairs. Anti-roll
	// pairing is left-right on ONE axle - vehicle-level knowledge, which is why it
	// lives here and not in the corner.
	for (int32 Axle = 0; Axle + 1 < Corners.Num(); Axle += 2)
	{
		UWishboneSuspension* Right = Corners[Axle].Suspension;
		UWishboneSuspension* Left = Corners[Axle + 1].Suspension;
		if (Right == nullptr || Left == nullptr) { continue; }

		// Both travels read BEFORE either corner is stepped, so the pair sees one
		// consistent state and the bar stays antisymmetric. Swap them one at a time
		// and the second corner would answer to a partner that had already moved -
		// the two forces would no longer be equal and opposite, and the bar would
		// quietly inject net vertical force into the car.
		const float RightTravelCm = Right->GetWheelTravelCm();
		const float LeftTravelCm = Left->GetWheelTravelCm();

		Right->SetAntiRollPartnerTravelCm(LeftTravelCm);
		Left->SetAntiRollPartnerTravelCm(RightTravelCm);
	}
}

void ACar::SolveWishboneCorners(float DeltaTime, FBodyInstance* BodyInstance,
	const FCarBodyState& State)
{
	if (BodyInstance == nullptr)
		return;

	// Sampled once for all four corners: they all hang off the same body, and the
	// body cannot move until this substep's forces are integrated (see the class
	// comment on UWishboneSuspension::SolveSubstep - PhysX does not advance the
	// body mid-callback, so re-sampling more than once per call would be wasted
	// work, not extra freshness). This IS called fresh every actual engine
	// substep, since AddCustomPhysics invokes SubstepPhysics once per substep -
	// the granularity that matters is MaxSubstepDeltaTime (Project Settings >
	// Physics), not anything cached here.
	const FTransform BodyToWorld = GetSubstepBodyToWorld(State);
	const FVector BodyCOMWS = State.COMWS;
	
	const FVector BodyVelWS = State.LinearVelocityWS;
	const FVector BodyAngVelWS = State.AngularVelocityRad;

	ExchangeAntiRollTravel();

	for (const FCornerAssembly& Assembly : Corners)
	{
		UWishboneSuspension* Corner = Assembly.Suspension;
		UPacejkaTyreComponent* Tyre = Assembly.Tyre;
		if (Corner == nullptr) { continue; }

		// Suspension first: it consumes LAST substep's ground force and produces this
		// substep's deflection and contact frame.
		Corner->SolveSubstep(DeltaTime, BodyToWorld, BodyCOMWS);
		Corner->ApplyReactionToBody(BodyInstance);

		if (Tyre != nullptr)
		{
			Tyre->SolveSubstep(
				Corner->GetContactFrame(BodyToWorld, BodyVelWS, BodyAngVelWS, BodyCOMWS), DeltaTime);

			// The WHOLE ground force, normal load included - the corner has no
			// vertical spring of its own any more. It reaches the body through the
			// suspension's single wrench rather than being applied here, so nothing
			// can be counted twice.
			Corner->SetTyreContactForceN(Tyre->GetContactForceN());
			Corner->SetWheelSpinAngleRad(Tyre->GetShaft()->GetAngle());

			// Mz is a pure couple, so it has no place in a two-point wrench. Handed to the
			// body as the angular impulse it is, together with the window it was accumulated
			// over, because how that lands differs by engine - see VehiclePhysicsBody.
			VehiclePhysicsBody::ApplyAngularReaction(BodyInstance,
				Corner->GetGroundNormalWorld() * (Tyre->GetAligningMomentNm() * 100.f * 100.f * DeltaTime),
				DeltaTime);
		}
	}
}

void ACar::DrawDebugHUD()
{
	ACarDebugHUD* DebugHUD = Cast<ACarDebugHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(DebugHUD)
	{
		// Tire-compliance verification, FR corner. With bUseTireCompliance on, slot 0
		// and slot 1 should sit apart by roughly the unsprung weight (392 N with the
		// 40 kg default) and slot 1 should visibly wiggle faster than slot 0 - that
		// wiggle is the wheel-hop mode, which does not exist at all with compliance
		// off (slots 0 and 1 would be identical every frame in that case).
		DebugHUD->AddDebugValue(0.01f,
		                        0,
		                        FString(TEXT("FR F_suspension (N)")));
		DebugHUD->AddDebugValue(0.f,
		                        1,
		                        FString(TEXT("FR F_tire (N)")));
		DebugHUD->AddDebugValue(0.f,
		                        2,
		                        FString(TEXT("FR tire deflection (cm)")));
	}
}

void ACar::UpdateTireGraphs()
{
	// Once per FRAME, not per substep - these drive on-screen widgets, and the
	// transient state they read is whatever the last substep left behind either way.
	//
	// Sourced from the tyre components now. They were previously updated inside
	// Heun off UWheelCollider, which is why they went dark when that path was
	// disconnected; FTransientTireState is the same struct on both, so this is a
	// repoint rather than a rewrite.
	auto UpdateGraph = [](TSharedPtr<STireGraphWidget>& Widget, const UPacejkaTyreComponent* Tyre,
		const UWishboneSuspension* Corner)
	{
		if (!Widget.IsValid() || Tyre == nullptr || Corner == nullptr)
		{
			return;
		}

		const FTransientTireState& State = Tyre->GetTransientState();
		Widget->CurrentSlipPoint = FVector(State.kappa_prime, State.alpha_prime, State.gamma_prime);
	};

	// Same order the corners are built in.
	if (Corners.Num() >= 4)
	{
		UpdateGraph(TireGraphWidgetFrontRight, Corners[0].Tyre, Corners[0].Suspension);
		UpdateGraph(TireGraphWidgetFrontLeft,  Corners[1].Tyre, Corners[1].Suspension);
		UpdateGraph(TireGraphWidgetRearRight,  Corners[2].Tyre, Corners[2].Suspension);
		UpdateGraph(TireGraphWidgetRearLeft,   Corners[3].Tyre, Corners[3].Suspension);
	}
}

void ACar::GearUp()
{
	Drivetrain->GearUp();
}

void ACar::GearDown()
{
	Drivetrain->GearDown();
}

float ACar::GetRedlineRPM() const
{
	return Engine != nullptr ? Engine->MaxRPM * Engine->RedLineRPMRatio : 0.f;
}

void ACar::SetSteeringFromPlayer(float Steering)
{
	const float Mag = FMath::Min(FMath::Abs(Steering), 1.f);

	// Faded in by speed, so low-speed manoeuvring keeps the linear mapping it wants.
	const float Alpha = FMath::Clamp(
		(ForwardVelocityKMH - SteerGammaOnsetKMH)
			/ FMath::Max(SteerGammaFullKMH - SteerGammaOnsetKMH, 1.f), 0.f, 1.f);
	const float Gamma = FMath::Lerp(1.f, FMath::Max(SteerInputGamma, 1.f), Alpha);

	// Sign kept out of the power so the curve is symmetric about centre; Sign(0) is 0
	// and Pow(0, g) is 0, so dead centre stays dead centre.
	SetSteering(FMath::Sign(Steering) * FMath::Pow(Mag, Gamma));
}

void ACar::SetSteering(float Steering)
{
	if (FMath::IsNearlyZero(Steering)) {
		if (FrontRightSuspension) FrontRightSuspension->SetTargetSteerAngleDeg(0.0f);
		if (FrontLeftSuspension)  FrontLeftSuspension->SetTargetSteerAngleDeg(0.0f);
		LastSteer = CurrentSteer;
		CurrentSteer = 0.f;
		return;
	}

	LastSteer = CurrentSteer;
	CurrentSteer = Steering;
	// Steering input (-1 to 1), scaled by speed limit
	float AbsSteering = FMath::Abs(Steering);
	float SteerSign = FMath::Sign(Steering); // >0 for left turn, <0 for right

	// GUARDED, and with something usable behind the guard. This line dereferenced
	// SteerCurve unconditionally and crashed any Blueprint that had not been given one.
	// Falling back to 1.0 alone would trade a crash for full lock at 250 km/h, so the
	// inline curve carries a real speed taper - see InlineSteerCurve.
	float SteerLimit = 1.f;
	if (SteerLimitOverride >= 0.f)
	{
		// Frozen by LockSteerLimitAtCurrentSpeed. Everything below this - Ackermann, the
		// max-angle clamp, the setters themselves - still runs exactly as it does for a
		// driver; only the speed term is held still.
		SteerLimit = SteerLimitOverride;
	}
	else if (SteerCurve != nullptr)
	{
		SteerLimit = SteerCurve->GetFloatValue(ForwardVelocityKMH);
	}
	else if (const FRichCurve* InlineSteer = InlineSteerCurve.GetRichCurveConst())
	{
		if (InlineSteer->GetNumKeys() > 0)
		{
			SteerLimit = InlineSteer->Eval(ForwardVelocityKMH);
		}
	}
	SteerLimit = FMath::Clamp(SteerLimit, 0.f, 1.f);
	// Compute inner wheel angle (degrees), capped by MaxSteerAngle
	float InnerAngle = FMath::Min(AbsSteering * MaxSteerAngle * SteerLimit, MaxSteerAngle);

	// Compute turn radius for inner wheel
	float InnerRadius = WheelBase / FMath::Tan(FMath::DegreesToRadians(InnerAngle));
	InnerRadius = FMath::Max(InnerRadius, 0.01f); // Prevent division by zero or negative radii

	// Compute outer wheel angle based on Ackerman geometry
	float EffectiveTrack = (AckermanPercent / 100.0f) * TrackWidth;
	float OuterRadius = InnerRadius + EffectiveTrack; // No SteerSign here; handle direction later
	OuterRadius = FMath::Max(OuterRadius, 0.01f); // Prevent negative or zero radius
	float OuterAngle = FMath::RadiansToDegrees(FMath::Atan(WheelBase / OuterRadius));

	// Clamp outer angle to MaxSteerAngle for safety
	OuterAngle = FMath::Min(OuterAngle, MaxSteerAngle);

	// Assign angles based on turn direction
	float LeftInput, RightInput;
	if (SteerSign < 0.0f) // Left turn: left wheel is inner
	{
		LeftInput = -InnerAngle;  // Positive, normalized [0..1]
		RightInput = -OuterAngle; // Positive, smaller
	}
	else // Right turn: right wheel is inner
	{
		LeftInput = OuterAngle;  // Negative, smaller
		RightInput = InnerAngle; // Negative, larger
	}
	
	if (FrontRightSuspension) FrontRightSuspension->SetTargetSteerAngleDeg(RightInput);
	if (FrontLeftSuspension)  FrontLeftSuspension->SetTargetSteerAngleDeg(LeftInput);
	
	if(bLogWheelSteerAngle)
		UE_LOG(LogTemp, Display, TEXT("LeftAngle : %f, RightAngle : %f"), LeftInput * MaxSteerAngle, RightInput * MaxSteerAngle);
}

void ACar::LockSteerLimitAtCurrentSpeed()
{
	float Limit = 1.f;
	if (SteerCurve != nullptr)
	{
		Limit = SteerCurve->GetFloatValue(ForwardVelocityKMH);
	}
	else if (const FRichCurve* InlineSteer = InlineSteerCurve.GetRichCurveConst())
	{
		if (InlineSteer->GetNumKeys() > 0)
		{
			Limit = InlineSteer->Eval(ForwardVelocityKMH);
		}
	}
	SteerLimitOverride = FMath::Clamp(Limit, 0.f, 1.f);
}

void ACar::UnlockSteerLimit()
{
	SteerLimitOverride = -1.f;
}

void ACar::CycleCar(int32 Delta)
{
	// Null whenever the level contains no AVehicleRoster, which is the ordinary case and
	// not an error: car switching is opt-in per level, and a level without a roster simply
	// does not use it. Doing nothing silently is the correct response.
	if (AVehicleRoster* Roster = AVehicleRoster::Find(GetWorld()))
	{
		Roster->CycleCar(Delta);
	}
}

void ACar::ResetCar()
{
	if (CarMesh)
	{
		// Create the new transform
		FTransform NewTransform(DefaultRotation.Quaternion(), DefaultLocation);

		// Set the world transform of the mesh
		CarMesh->SetWorldTransform(NewTransform, true, nullptr, ETeleportType::TeleportPhysics);

		// Reset velocities to stop motion
		if (FBodyInstance* BodyInstance = CarMesh->GetBodyInstance())
		{
			BodyInstance->SetLinearVelocity(FVector::ZeroVector, false);
			BodyInstance->SetAngularVelocityInRadians(FVector::ZeroVector, false);
		}

		SpringArm->SetRelativeRotation(OGSpringArmRotation);
	}
}

void FCarDebugDrawTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType,
	ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	if (IsValid(Target))
	{
		Target->DrawDebugPass(DeltaTime);
	}
}

FString FCarDebugDrawTickFunction::DiagnosticMessage()
{
	return (Target != nullptr ? Target->GetFullName() : FString(TEXT("<null>"))) + TEXT("[CarDebugDraw]");
}

void ACar::RegisterActorTickFunctions(bool bRegister)
{
	Super::RegisterActorTickFunctions(bRegister);

	if (bRegister)
	{
		DebugDrawTick.bCanEverTick = true;
		DebugDrawTick.TickGroup = TG_PostUpdateWork;
		DebugDrawTick.bStartWithTickEnabled = true;
		DebugDrawTick.bTickEvenWhenPaused = false;
		DebugDrawTick.Target = this;
		DebugDrawTick.SetTickFunctionEnable(true);
		DebugDrawTick.RegisterTickFunction(GetLevel());
	}
	else if (DebugDrawTick.IsTickFunctionRegistered())
	{
		DebugDrawTick.UnRegisterTickFunction();
	}
}

void ACar::DrawDebugPass(float DeltaSeconds)
{
	if (bDrawBodyMotion)
	{
		DrawBodyMotion(DeltaSeconds);
	}
	if (bDrawBodyCollision)
	{
		DrawBodyCollision();
	}
}

FVector ACar::ShapeScaleForBone(FName BoneName) const
{
	// The scale UE built the physics shapes at.
	//
	// NOT the component scale, which is the trap: USkeletalMeshComponent creates its
	// bodies from the BONE transform, and that carries the reference skeleton's scale
	// as well as the component's. A mesh authored in metres whose root bone holds the
	// hundredfold gets correctly sized shapes while the component still reports 1.
	//
	// Nor the body's own transform, which always reports 1 - physics never carries
	// scale on a transform, it bakes it into the geometry at creation.
	if (CarMesh != nullptr && !BoneName.IsNone())
	{
		const int32 BoneIndex = CarMesh->GetBoneIndex(BoneName);
		if (BoneIndex != INDEX_NONE)
		{
			return CarMesh->GetBoneTransform(BoneIndex).GetScale3D();
		}
	}
	return CarMesh != nullptr ? CarMesh->GetComponentTransform().GetScale3D() : FVector::OneVector;
}

int32 ACar::DrawAggGeom(const FKAggregateGeom& Geom, const FTransform& At, const FColor& Colour)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return 0;
	}

	// SCALE. The shape dimensions in a body setup are the UNSCALED authored ones,
	// and the component scale is baked in separately when the physics shapes are
	// created - so the collision is right at runtime while these numbers are not,
	// unless the scale is put back. On this car the difference is a hundredfold: the
	// mesh was authored in metres, so the chassis box reads 2.5 x 1.2 x 0.5 and the
	// real thing is 250 x 120 x 50 cm.
	//
	// It cannot be read from the body's own transform either - FBodyInstance's world
	// transform always reports a scale of 1, because physics does not carry scale on
	// a transform; it bakes it into the geometry instead. So it comes from the
	// component, which is where it was applied.
	const FVector Scale = At.GetScale3D();
	const float UniformScale = FMath::Min3(FMath::Abs(Scale.X), FMath::Abs(Scale.Y), FMath::Abs(Scale.Z));

	for (const FKBoxElem& Box : Geom.BoxElems)
	{
		const FTransform T = Box.GetTransform() * At;
		DrawDebugBox(World, T.GetLocation(), FVector(Box.X, Box.Y, Box.Z) * 0.5f * Scale,
			T.GetRotation(), Colour, false, -1.f, 0, 1.5f);
	}
	for (const FKSphereElem& Sph : Geom.SphereElems)
	{
		// A sphere cannot be non-uniformly scaled and stay a sphere, so UE takes the
		// smallest axis. Matching that here keeps the drawing honest about what the
		// physics engine actually built.
		DrawDebugSphere(World, At.TransformPosition(Sph.Center), Sph.Radius * UniformScale,
			16, Colour, false, -1.f, 0, 1.5f);
	}
	for (const FKSphylElem& Cap : Geom.SphylElems)
	{
		const FTransform T = Cap.GetTransform() * At;
		const float R = Cap.Radius * FMath::Min(FMath::Abs(Scale.X), FMath::Abs(Scale.Y));
		const float HalfLen = (Cap.Length * 0.5f * FMath::Abs(Scale.Z)) + R;
		DrawDebugCapsule(World, T.GetLocation(), HalfLen, R,
			T.GetRotation(), Colour, false, -1.f, 0, 1.5f);
	}

	// Convex hulls as their own triangle edges. Every shared edge gets drawn twice,
	// which costs a little and is worth not caring about - the alternative is
	// building an edge set every frame to save lines that land on top of each other.
	for (const FKConvexElem& Hull : Geom.ConvexElems)
	{
		const FTransform T = Hull.GetTransform() * At;
		const int32 TriCount = Hull.IndexData.Num() / 3;
		for (int32 Tri = 0; Tri < TriCount; ++Tri)
		{
			const int32 I0 = Hull.IndexData[(Tri * 3) + 0];
			const int32 I1 = Hull.IndexData[(Tri * 3) + 1];
			const int32 I2 = Hull.IndexData[(Tri * 3) + 2];
			if (!Hull.VertexData.IsValidIndex(I0) || !Hull.VertexData.IsValidIndex(I1)
				|| !Hull.VertexData.IsValidIndex(I2))
			{
				continue;
			}
			const FVector V0 = T.TransformPosition(Hull.VertexData[I0]);
			const FVector V1 = T.TransformPosition(Hull.VertexData[I1]);
			const FVector V2 = T.TransformPosition(Hull.VertexData[I2]);
			DrawDebugLine(World, V0, V1, Colour, false, -1.f, 0, 1.f);
			DrawDebugLine(World, V1, V2, Colour, false, -1.f, 0, 1.f);
			DrawDebugLine(World, V2, V0, Colour, false, -1.f, 0, 1.f);
		}
	}

	return Geom.BoxElems.Num() + Geom.SphereElems.Num()
		+ Geom.SphylElems.Num() + Geom.ConvexElems.Num();
}

void ACar::DrawBodyCollision()
{
	UWorld* World = GetWorld();
	if (World == nullptr || CarMesh == nullptr)
	{
		return;
	}

	const FColor Shell(90, 170, 255);
	int32 ShapeCount = 0;
	int32 BodyCount = 0;
	const TCHAR* Route = TEXT("none");

	// THREE routes, tried in order, because a car that visibly has a boxy hull came
	// back empty from the obvious one twice. A debug draw that silently shows nothing
	// is worse than no debug draw at all, so this tries every place the shapes can
	// legitimately live and then says which one had them.
	//
	// 1. The simulated bodies - correct when the mesh is articulated, which is what
	//    a physics asset is normally authored for.
	for (FBodyInstance* B : CarMesh->Bodies)
	{
		if (B == nullptr || B->GetBodySetup() == nullptr)
		{
			continue;
		}
		// The body's world transform with the scale put back on it - taken from the
		// BONE, not the component.
		//
		// UE builds a skeletal mesh's physics shapes from the bone transform, and
		// that carries the reference skeleton's own scale as well as the component's.
		// A mesh authored in metres with the hundredfold living on the root bone
		// therefore produces correctly sized shapes while the COMPONENT still reports
		// a scale of 1 - which is exactly this car, and why reading the component
		// scale here changed nothing.
		FTransform BodyTM = B->GetUnrealWorldTransform();
		BodyTM.SetScale3D(ShapeScaleForBone(B->GetBodySetup()->BoneName));

		ShapeCount += DrawAggGeom(B->GetBodySetup()->AggGeom, BodyTM, Shell);
		++BodyCount;
		Route = TEXT("simulated bodies");
	}

	// 2. Straight off the physics ASSET, each setup at its own bone. This survives a
	//    mesh that is not articulated: Bodies is empty, and the shapes still exist
	//    and still collide.
	if (ShapeCount == 0)
	{
		if (UPhysicsAsset* PhysAsset = CarMesh->GetPhysicsAsset())
		{
			for (USkeletalBodySetup* Setup : PhysAsset->SkeletalBodySetups)
			{
				if (Setup == nullptr)
				{
					continue;
				}
				const int32 BoneIndex = CarMesh->GetBoneIndex(Setup->BoneName);
				FTransform At = (BoneIndex != INDEX_NONE)
					? CarMesh->GetBoneTransform(BoneIndex)
					: CarMesh->GetComponentTransform();
				At.SetScale3D(ShapeScaleForBone(Setup->BoneName));
				ShapeCount += DrawAggGeom(Setup->AggGeom, At, Shell);
				++BodyCount;
				Route = TEXT("physics asset");
			}
		}
	}

	// 3. The component's own body setup, for a mesh carrying collision with no
	//    physics asset at all.
	if (ShapeCount == 0)
	{
		if (FBodyInstance* Root = CarMesh->GetBodyInstance())
		{
			if (UBodySetup* Setup = Root->GetBodySetup())
			{
				FTransform RootTM = Root->GetUnrealWorldTransform();
				RootTM.SetScale3D(ShapeScaleForBone(Setup->BoneName));
				ShapeCount += DrawAggGeom(Setup->AggGeom, RootTM, Shell);
				BodyCount = 1;
				Route = TEXT("component body setup");
			}
		}
	}

	// Said ONCE, and said with numbers. Two attempts at this drew nothing and gave no
	// reason, which cost more than the feature is worth - so if it is still empty,
	// this says which routes were tried and what each one actually held.
	if (!bLoggedCollisionSetup)
	{
		bLoggedCollisionSetup = true;
		UPhysicsAsset* PhysAsset = CarMesh->GetPhysicsAsset();
		FBodyInstance* Root = CarMesh->GetBodyInstance();
		// Every shape's actual numbers. "1 shape found" and "no box on screen" can
		// only both be true if the shape is not where or what it is assumed to be,
		// and that is answered by printing it rather than by reasoning about it.
		for (FBodyInstance* B : CarMesh->Bodies)
		{
			UBodySetup* BS = B ? B->GetBodySetup() : nullptr;
			if (BS == nullptr) { continue; }
			const FTransform BTM = B->GetUnrealWorldTransform();

			// GROUND TRUTH. Everything else here is an authored number that has to
			// have a scale guessed onto it; this is the world-space extent of the
			// shape physics actually built, straight from the body. If this says the
			// hull is a few centimetres then the collision really IS a few
			// centimetres and the car has effectively no body collision at all -
			// which is a setup bug, not a drawing one. If it says metres, then the
			// scale is real and only the drawing was reading it from the wrong place.
			const FBox RealBounds = B->GetBodyBounds();
			const FVector RealSize = RealBounds.GetSize();

			UE_LOG(LogTemp, Warning,
				TEXT("[Car]   body '%s' at %s | ACTUAL simulated bounds %.1f x %.1f x %.1f cm"),
				*BS->GetName(), *BTM.GetLocation().ToCompactString(),
				RealSize.X, RealSize.Y, RealSize.Z);
			UE_LOG(LogTemp, Warning,
				TEXT("[Car]     component scale %s | body xform scale %s | boxes %d spheres %d sphyls %d convex %d"),
				*CarMesh->GetComponentTransform().GetScale3D().ToCompactString(),
				*BTM.GetScale3D().ToCompactString(),
				BS->AggGeom.BoxElems.Num(), BS->AggGeom.SphereElems.Num(),
				BS->AggGeom.SphylElems.Num(), BS->AggGeom.ConvexElems.Num());
			for (const FKBoxElem& Bx : BS->AggGeom.BoxElems)
			{
				const FTransform T = Bx.GetTransform() * BTM;
				const FVector CompScale = ShapeScaleForBone(BS->BoneName);
				UE_LOG(LogTemp, Warning,
					TEXT("[Car]     BOX  authored %.2f x %.2f x %.2f  x component scale %s")
					TEXT("  =  %.1f x %.1f x %.1f cm  at %s"),
					Bx.X, Bx.Y, Bx.Z, *CompScale.ToCompactString(),
					Bx.X * CompScale.X, Bx.Y * CompScale.Y, Bx.Z * CompScale.Z,
					*T.GetLocation().ToCompactString());
			}
			for (const FKSphylElem& Cp : BS->AggGeom.SphylElems)
			{
				UE_LOG(LogTemp, Warning, TEXT("[Car]     SPHYL r %.1f len %.1f centre %s"),
					Cp.Radius, Cp.Length, *Cp.Center.ToCompactString());
			}
			for (const FKSphereElem& Sp : BS->AggGeom.SphereElems)
			{
				UE_LOG(LogTemp, Warning, TEXT("[Car]     SPHERE r %.1f centre %s"),
					Sp.Radius, *Sp.Center.ToCompactString());
			}
		}

		UE_LOG(LogTemp, Warning,
			TEXT("[Car] collision draw: route '%s', %d bodies, %d shapes. ")
			TEXT("Bodies=%d  PhysicsAsset=%s (%d setups)  component setup=%s  simulating=%d"),
			Route, BodyCount, ShapeCount,
			CarMesh->Bodies.Num(),
			PhysAsset ? *PhysAsset->GetName() : TEXT("NONE"),
			PhysAsset ? PhysAsset->SkeletalBodySetups.Num() : 0,
			(Root && Root->GetBodySetup()) ? TEXT("yes") : TEXT("no"),
			CarMesh->IsSimulatingPhysics() ? 1 : 0);
	}

	// Centre of mass, and a spoke out to each corner's chassis pickups. The corners
	// are drawn by the suspension debug as four rigs floating in space; these are the
	// lines that say they are all bolted to one thing, and that the thing is here.
	const FVector ComWS = CarMesh->GetCenterOfMass();
	DrawDebugSphere(World, ComWS, 8.f, 12, FColor(255, 210, 60), false, -1.f, 0, 2.f);

	for (const FCornerAssembly& C : Corners)
	{
		if (C.Suspension == nullptr)
		{
			continue;
		}

		// The CHASSIS pickups, not the wheel. These are body-fixed - they do not move
		// with suspension travel - so the rest pose gives them correctly whatever the
		// car is doing.
		const FWishbonePose Rest = C.Suspension->GetPose(0.f, 0.f);
		DrawDebugLine(World, ComWS, Rest.LcaFront, FColor(255, 210, 60), false, -1.f, 0, 1.f);
		DrawDebugLine(World, ComWS, Rest.UcaFront, FColor(255, 210, 60), false, -1.f, 0, 1.f);

		DrawDebugLine(World, Rest.LcaFront, C.Suspension->GetWheelCentreWorld(),
			FColor(150, 120, 40), false, -1.f, 0, 0.6f);
	}

	DrawDebugString(World, ComWS + FVector(0.f, 0.f, 150.f),
		FString::Printf(TEXT("chassis: %d bodies, %d shapes (%s), %.0f kg"),
			BodyCount, ShapeCount, Route, CarMesh->GetMass()),
		nullptr, ShapeCount > 0 ? FColor(90, 170, 255) : FColor::Red, 0.f, true, 1.0f);
}

void ACar::DrawBodyMotion(float DeltaSeconds)
{
	if (CarMesh == nullptr || DeltaSeconds <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	// Vertical acceleration in the WORLD frame, not the body frame. A body-frame
	// reading rotates with the car, so a hard corner would colour the trail red for
	// leaning rather than for being hit - and being hit is the whole question.
	const FVector VelocityWS = CarMesh->GetPhysicsLinearVelocity();
	const float VerticalG =
		((VelocityWS.Z - BodyTrailLastVelocity.Z) / DeltaSeconds) * CmToM / 9.81f;
	BodyTrailLastVelocity = VelocityWS;

	// Gravity is a constant offset on a car in free flight, so subtracting it means
	// an airborne body reads ZERO rather than -1 g, and the trail only colours for
	// what the road did. The launch itself still shows: the kick that threw the car
	// is a spike, and the landing is a bigger one.
	const float RoadG = VerticalG + 1.f;

	const FVector ComWS = CarMesh->GetCenterOfMass();
	// A teleport is not an acceleration. Spawning, a bench reset or any SetActorLocation
	// shows up here as a velocity difference over one frame with no force behind it, and
	// differencing that gives tens or hundreds of g - which then latches into the peak
	// and sits there for the rest of the session claiming the car saw 110 g.
	//
	// Nothing a tyre can do reaches 20 g, so anything past it is a discontinuity rather
	// than a reading. Dropped from the peak AND from the trail, since a trail segment
	// drawn across a teleport is a line through space the car never travelled.
	const bool bPlausible = FMath::Abs(RoadG) < 20.f;
	if (bPlausible)
	{
		BodyTrail.Add({ ComWS, RoadG });
		BodyTrailPeakG = FMath::Max(BodyTrailPeakG, FMath::Abs(RoadG));
	}
	else
	{
		BodyTrail.Reset();
	}

	// Trimmed by TIME rather than by count, so the history behind the car is the same
	// few seconds whatever the frame rate is doing.
	const int32 MaxPoints = FMath::Max(FMath::RoundToInt(BodyTrailSeconds / DeltaSeconds), 2);
	while (BodyTrail.Num() > MaxPoints)
	{
		BodyTrail.RemoveAt(0, 1, VP_NO_SHRINK);
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	for (int32 i = 1; i < BodyTrail.Num(); ++i)
	{
		const float T = FMath::Clamp(
			FMath::Abs(BodyTrail[i].VerticalG) / FMath::Max(BodyTrailRedAtG, 0.01f), 0.f, 1.f);

		// Green to red through yellow. Deliberately NOT a smooth blend of the two
		// endpoints, which passes through a muddy olive that reads as "slightly bad"
		// across most of its range; going via yellow keeps the middle legible.
		const FColor Colour = (T < 0.5f)
			? FColor(FMath::RoundToInt(255.f * (T * 2.f)), 255, 40)
			: FColor(255, FMath::RoundToInt(255.f * (1.f - ((T - 0.5f) * 2.f))), 40);

		DrawDebugLine(World, BodyTrail[i - 1].PosWS, BodyTrail[i].PosWS, Colour,
			false, -1.f, 0, 2.f + (T * 4.f));
	}

	// Body axes at the centre of mass, so roll and pitch are readable against the
	// trail rather than guessed from the mesh.
	const FTransform BodyToWorld = CarMesh->GetComponentTransform();
	DrawDebugLine(World, ComWS, ComWS + BodyToWorld.GetUnitAxis(EAxis::X) * 150.f, FColor(255, 80, 80), false, -1.f, 0, 2.f);
	DrawDebugLine(World, ComWS, ComWS + BodyToWorld.GetUnitAxis(EAxis::Y) * 150.f, FColor(80, 255, 80), false, -1.f, 0, 2.f);
	DrawDebugLine(World, ComWS, ComWS + BodyToWorld.GetUnitAxis(EAxis::Z) * 150.f, FColor(120, 160, 255), false, -1.f, 0, 2.f);

	// How many wheels are actually on the ground. The single most useful number when
	// a car is being thrown about, and the one thing the trail cannot show: a body
	// arcing smoothly through the air draws a perfectly smooth green line.
	int32 InContact = 0;
	for (const FCornerAssembly& C : Corners)
	{
		if (C.Suspension && C.Suspension->IsInContact())
		{
			++InContact;
		}
	}

	// The trail is EMPTY on any frame the reading was rejected as a teleport, because
	// that path clears it - so Last() is not safe here, and asserted the first time a
	// car was respawned with the trail on.
	const float LatestG = BodyTrail.Num() > 0 ? BodyTrail.Last().VerticalG : 0.f;

	DrawDebugString(World, ComWS + FVector(0.f, 0.f, 180.f),
		FString::Printf(TEXT("vertical %+.2f g   peak %.2f g   wheels down %d/4"),
			LatestG, BodyTrailPeakG, InContact),
		nullptr, InContact < 4 ? FColor::Orange : FColor::White, 0.f, true, 1.1f);
}

void ACar::DrawCOM(FBodyInstance* BodyInstance)
{
	FVector COMPos = BodyInstance->GetCOMPosition();
	FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform_AssumesLocked();
	DrawDebugLine(GetWorld(), COMPos, COMPos + BodyTransform.GetUnitAxis(EAxis::Y) * 300.f,FColor::Green);
	DrawDebugLine(GetWorld(), COMPos, COMPos + BodyTransform.GetUnitAxis(EAxis::Z) * 300.f,FColor::Blue);
}