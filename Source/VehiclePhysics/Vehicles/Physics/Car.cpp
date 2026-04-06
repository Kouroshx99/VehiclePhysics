// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Car.h"

#include "Engine/Engine.h"
#include "BodyState.h"
#include "DrawDebugHelpers.h"
#include "Drivetrain.h"
#include "Curves/CurveFloat.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "ECU.h"
#include "Widgets/SOverlay.h"
#include "Transmission.h"
#include "VehicleEngine.h"
#include "Widgets/SOverlay.h"
#include "WheelCollider.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Widgets/Layout/SBox.h"
#include "VehiclePhysics/Vehicles/CarDebugHUD.h"
#include "VehiclePhysics/Vehicles/STireGraphWidget.h"

///Cleaned
ACar::ACar()
{
	PrimaryActorTick.bCanEverTick = true;

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
	TransmissionComponent = CreateDefaultSubobject<UTransmission>(TEXT("Transmission"));
	Drivetrain = CreateDefaultSubobject<UDrivetrain>(TEXT("Drivetrain"));
	ECU = CreateDefaultSubobject<UECU>(TEXT("Electronic Control Unit"));
	TransmissionComponent->Drivetrain = Drivetrain;



	FrontRight = CreateDefaultSubobject<UWheelCollider>(TEXT("FrontRight"));
	FrontLeft = CreateDefaultSubobject<UWheelCollider>(TEXT("FrontLeft"));
	RearRight = CreateDefaultSubobject<UWheelCollider>(TEXT("BackRight"));
	RearLeft = CreateDefaultSubobject<UWheelCollider>(TEXT("BackLeft"));
	
	FrontRight->SetupAttachment(RootComponent);
	FrontLeft->SetupAttachment(RootComponent);
	RearRight->SetupAttachment(RootComponent);
	RearLeft->SetupAttachment(RootComponent);

	WheelColliders = {FrontRight, FrontLeft, RearRight, RearLeft};

	Drivetrain->EngineShaft = Engine->GetShaft();
	Drivetrain->ClutchShaft = &ClutchShaft;
	Drivetrain->TransmissionShaft = TransmissionComponent->GetShaft();
	Drivetrain->TransferCaseShaft = &TransferCaseShaft;

	Drivetrain->FrontDifferential->RightWheel = FrontRight;
	Drivetrain->FrontDifferential->LeftWheel = FrontLeft;
	Drivetrain->RearDifferential->RightWheel = RearRight;
	Drivetrain->RearDifferential->LeftWheel = RearLeft;
	Drivetrain->FrontRightWheelShaft = FrontRight->GetShaft();
	Drivetrain->FrontLeftWheelShaft = FrontLeft->GetShaft();
	Drivetrain->RearRightWheelShaft = RearRight->GetShaft();
	Drivetrain->RearLeftWheelShaft = RearLeft->GetShaft();
	SetReplicates(true);
	CarMesh->SetIsReplicated(true);
	ClutchShaft.Inertia = 0.2f;
	TransferCaseShaft.Inertia = 0.3f;
	Drivetrain->VehicleEngine = Engine;
	ECU->Drivetrain = Drivetrain;
	
}

float ACar::GetWheelSpeed(EWheel Wheel)
{
	switch(Wheel)
	{
		case EWheel::EW_FrontRight: return FrontRight->GetAngularVelocityInDegrees();
		case EWheel::EW_FrontLeft: return FrontLeft->GetAngularVelocityInDegrees();
		case EWheel::EW_RearRight: return RearRight->GetAngularVelocityInDegrees();
		case EWheel::EW_RearLeft: return RearLeft->GetAngularVelocityInDegrees();
		default: return 0;
	}
}

float ACar::GetWheelZPosition(EWheel Wheel)
{
	switch(Wheel)
	{
		case EWheel::EW_FrontRight: return FrontRight->GetWheelZPosition() - FrontRight->GetRestLenght();
		case EWheel::EW_FrontLeft: return FrontLeft->GetWheelZPosition() - FrontLeft->GetRestLenght();
		case EWheel::EW_RearRight: return RearRight->GetWheelZPosition() - RearRight->GetRestLenght();
		case EWheel::EW_RearLeft: return RearLeft->GetWheelZPosition() - RearLeft->GetRestLenght();
		default: return 0;
	}
}

FRotator ACar::GetWheelRotation(EWheel Wheel)
{
	switch (Wheel)
	{
	case EWheel::EW_FrontRight: return FrontRight->GetWheelRotLocal();
	case EWheel::EW_FrontLeft: return FrontLeft->GetWheelRotLocal();
	case EWheel::EW_RearRight: return RearRight->GetWheelRotLocal();
	case EWheel::EW_RearLeft: return RearLeft->GetWheelRotLocal();
	default: return FRotator::ZeroRotator;
	}
}

FVector ACar::GetWheelPosition(EWheel Wheel)
{
	switch (Wheel)
	{
	case EWheel::EW_FrontRight: return FrontRight->GetWheelPosition();
	case EWheel::EW_FrontLeft: return FrontLeft->GetWheelPosition();
	case EWheel::EW_RearRight: return RearRight->GetWheelPosition();
	case EWheel::EW_RearLeft: return RearLeft->GetWheelPosition();
	default: return FVector::ZeroVector;
	}
}

float ACar::GetWheelAngle(EWheel Wheel)
{
	switch(Wheel)
	{
	case EWheel::EW_FrontRight: return FrontRight->GetCurrentWheelAngle();
	case EWheel::EW_FrontLeft: return FrontLeft->GetCurrentWheelAngle();
	default: return 0;
	}
}

///Cleaned
void ACar::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine && GEngine->GameViewport)
	{
		TSharedRef<STireGraphWidget> TireGraph = SNew(STireGraphWidget)
			.Asset(RearRight->GetParams())
			.bIsInteractive(false); // Your UTireParamsDataAsset
		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			.Padding(10.f)
			[
				SNew(SBox)
				.WidthOverride(700.f)
				.HeightOverride(700.f)
				[
					TireGraph
				]
			],
			100 // Z-order
		);
		TireGraph->Zoom = 250.f;
		TireGraphWidget = TireGraph;
	}
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
	SetReplicates(true);
	CarMesh->SetIsReplicated(true);
	OGSpringArmRotation = SpringArm->GetRelativeRotation();

	CustomPhysics.BindUObject(this, &ACar::SubstepPhysics);

	FBodyInstance* BodyInstance = CarMesh->GetBodyInstance();
	//Assuming the pivot is at center
	float Y = BodyInstance->GetUnrealWorldTransform_AssumesLocked().InverseTransformPosition(BodyInstance->GetCOMPosition()).Y;
	UE_LOG(LogTemp, Warning, TEXT("Y is %f"), Y);
	FVector NewCOMNudge = FVector(0, -Y, 0) + COMOffset;
	BodyInstance->COMNudge = NewCOMNudge;
	BodyInstance->UpdateMassProperties();

	FVector COMPos = BodyInstance->GetCOMPosition();
	FQuat Rotation = BodyInstance->GetUnrealWorldTransform_AssumesLocked().GetRotation();
	FrontRight->Initialize(COMPos, Rotation);
	FrontLeft->Initialize(COMPos, Rotation);
	RearRight->Initialize(COMPos, Rotation);
	RearLeft->Initialize(COMPos, Rotation);

	TrackWidth = FMath::Abs((FrontRight->GetRelativeLocation() - FrontLeft->GetRelativeLocation()).Y);
	WheelBase = FMath::Abs((FrontRight->GetRelativeLocation() - RearLeft->GetRelativeLocation()).X);

	DefaultLocation = GetActorLocation();
	DefaultRotation = GetActorRotation();

}

void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bLogCarData)
		UE_LOG(LogTemp, Warning, TEXT("----------  New Frame ---------"));

	CarMesh->GetBodyInstance()->AddCustomPhysics(CustomPhysics);
	
	Engine->SetThrottle(Throttle);
	ECU->ApplyBrake(Brake);
	float Velocity = CarMesh->GetPhysicsLinearVelocity().Size() * TO_SI;

	if(bLogAcceleration)
	{
		Acceleration = (Velocity - LastVelocity)/DeltaTime;
		UE_LOG(LogTemp,Warning,TEXT("Acceleration m/s: %f"), Acceleration);
		LastVelocity = CarMesh->GetPhysicsLinearVelocity().Size() * TO_SI;
	}

	SpringArm->AddRelativeRotation(FRotator(MouseYSpeed, MouseXSpeed, 0.f));
	
	if (bLogCarData)
		UE_LOG(LogTemp, Display, TEXT("Steer Angle: %f, Throttle: %f, Brake: %f"), 
			CurrentSteer * MaxSteerAngle,
			Throttle,
			Brake);
	
	auto LogWheel = [](const TCHAR* Name, UWheelCollider* Wheel)
	{
		const auto& Hit = Wheel->GetCurrentWheelHit();

		UE_LOG(LogTemp, Display,
			TEXT("%s slip ratio: %f, slip angle: %f, normal force(N): %f, Long force(N): %f, "
				 "Lat Force(N): %f, Wheel omega(Rad/s): %f, Contact forward velocity (m/s): %f"),
			Name,
			Hit.LongitudinalSlip,
			FMath::RadiansToDegrees(Hit.LateralSlip),
			Hit.SuspensionForce.Size() * 0.01f,
			Hit.LongitudinalForce.Size() * 0.01f,
			Hit.LateralForce.Size() * 0.01f,
			Wheel->GetShaft()->GetAngularVelocity(),
			Wheel->SolverWheelForwardVelocityAtContact * 0.01f);
	};
	
	if (bLogWheelData)
	{
		LogWheel(TEXT("FR"), FrontRight);
		LogWheel(TEXT("FL"), FrontLeft);
		LogWheel(TEXT("RR"), RearRight);
		LogWheel(TEXT("RL"), RearLeft);
	}
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Throttle"), this, &ThisClass::SetThrottle);
	PlayerInputComponent->BindAxis(FName("Steering"), this, &ThisClass::SetSteering);
	PlayerInputComponent->BindAxis(FName("Horizontal"), this, &ThisClass::SetMouseX);
	PlayerInputComponent->BindAxis(FName("Vertical"), this, &ThisClass::SetMouseY);
	PlayerInputComponent->BindAxis(FName("Brake"), this, &ThisClass::SetBrake);
	PlayerInputComponent->BindAction(FName("GearUp"), IE_Pressed, this, &ThisClass::GearUp);
	PlayerInputComponent->BindAction(FName("GearDown"), IE_Pressed, this, &ThisClass::GearDown);
	PlayerInputComponent->BindAction(FName("Reset"), IE_Pressed, this, &ThisClass::ResetCar);

}

///Cleaned
void ACar::SubstepPhysics(float DeltaTime, FBodyInstance* BodyInstance)
{
	FVector uBodyVelocity = BodyInstance->GetUnrealWorldVelocity_AssumesLocked();
	VelocityKMH = uBodyVelocity.Size() * TO_SI * MPSTOKMPH;
	ForwardVelocityKMH = FVector::DotProduct(uBodyVelocity * TO_SI * MPSTOKMPH,
		BodyInstance->GetUnrealWorldTransform_AssumesLocked().GetUnitAxis(EAxis::X));
	
	if(bLogDeltaTime)
		UE_LOG(LogTemp, Display, TEXT("--- Delta Time : %f ---"), DeltaTime);
	if(bLogVelocityKMH)
		UE_LOG(LogTemp, Display, TEXT("Velocity KM/H: %d"), VelocityKMH);
	if(bVisualizeCOM)
		DrawCOM(BodyInstance);

	FBodyState CurrentState(BodyInstance->GetCOMPosition(),
		BodyInstance->GetUnrealWorldTransform_AssumesLocked().GetRotation(),
		uBodyVelocity,
		BodyInstance->GetUnrealWorldAngularVelocityInRadians_AssumesLocked());

	Heun(DeltaTime, BodyInstance, CurrentState);
	Engine->GetShaft()->ApplyImpulse(Engine->GetCurrentTorque() * DeltaTime);
	Drivetrain->SolveConstraints(Drivetrain->Connections, DeltaTime);
	
	if (bLogAngVelocity)
		UE_LOG(LogTemp, Display, TEXT("Vehicle Angular Velocity (Rad/s): %s"), 
			*BodyInstance->GetUnrealWorldAngularVelocityInRadians_AssumesLocked().ToString());
}

void ACar::Heun(float DeltaTime, FBodyInstance* BodyInstance, FBodyState CurrentBodyInfo)
{

	const float Count = FMath::Max(static_cast<float>(HeunNumIterations), 1.f);
	float DT = DeltaTime/Count;

	// Accumulators for each wheel
	TArray<FWheelHit> HitAccum;
	HitAccum.SetNumZeroed(WheelColliders.Num());
	
	TArray<FWheelHit> FirstWheelHits;
	FirstWheelHits.SetNum(WheelColliders.Num());

	FBodyState PredictedBodyInfo = CurrentBodyInfo;

    for (int32 Iter = 0; Iter < HeunNumIterations; ++Iter)
    {
        TArray<FWheelHit> WheelHits;
        WheelHits.SetNum(WheelColliders.Num());
    	//Engine->ApplyCurrentImpulse(DT);
        ComputeWheelHits(PredictedBodyInfo, WheelHits, BodyInstance, DT);
    	Drivetrain->SolveConstraints(Drivetrain->Connections, DT);

        // Update cross-wheel suspension
        WheelColliders[0]->SetOtherWheelSuspensionCompression(WheelColliders[1]->GetWheelZPosition());
        WheelColliders[1]->SetOtherWheelSuspensionCompression(WheelColliders[0]->GetWheelZPosition());
        WheelColliders[2]->SetOtherWheelSuspensionCompression(WheelColliders[3]->GetWheelZPosition());
        WheelColliders[3]->SetOtherWheelSuspensionCompression(WheelColliders[2]->GetWheelZPosition());

        if (Iter == 0)
            FirstWheelHits = WheelHits;

    	// Accumulate hits for averaging
        for (int32 i = 0; i < WheelColliders.Num(); ++i)
        {
            HitAccum[i] += WheelHits[i];
        }
    	if (Iter == HeunNumIterations - 1 && false)
    	{
    		Drivetrain->FRCopy->ApplyImpulse(WheelHits[0].TractionImpulse);
    		Drivetrain->FLCopy->ApplyImpulse(WheelHits[1].TractionImpulse);
    		Drivetrain->RRCopy->ApplyImpulse(WheelHits[2].TractionImpulse);
    		Drivetrain->RLCopy->ApplyImpulse(WheelHits[3].TractionImpulse);
    	}
        FBodyState TempBodyState = PredictedBodyInfo;
        PredictBodyState(BodyInstance, WheelHits, TempBodyState, PredictedBodyInfo, DT);
    }
	
	// Average hits
	for (int32 i = 0; i < WheelColliders.Num() ; i++)
	{
		HitAccum[i] = HitAccum[i] / Count;

		// Preserve ForceApplication from first iteration
		HitAccum[i].SuspensionForceApplication = FirstWheelHits[i].SuspensionForceApplication;
		HitAccum[i].LongitudinalForceApplication = FirstWheelHits[i].LongitudinalForceApplication;
		HitAccum[i].LateralForceApplication = FirstWheelHits[i].LateralForceApplication;
		WheelColliders[i]->Integrate(HitAccum[i], BodyInstance);

	}
	//Drivetrain->FRCopy->ApplyImpulse(HitAccum[0].TractionImpulse * HeunNumIterations);
	//Drivetrain->FLCopy->ApplyImpulse(HitAccum[1].TractionImpulse * HeunNumIterations);
	//Drivetrain->RRCopy->ApplyImpulse(HitAccum[2].TractionImpulse * HeunNumIterations);
	//Drivetrain->RLCopy->ApplyImpulse(HitAccum[3].TractionImpulse * HeunNumIterations);
	
    // Update cross-wheel suspension one last time
    //WheelColliders[0]->SetOtherWheelSuspensionCompression(WheelColliders[1]->GetWheelZPosition());
    //WheelColliders[1]->SetOtherWheelSuspensionCompression(WheelColliders[0]->GetWheelZPosition());
    //WheelColliders[2]->SetOtherWheelSuspensionCompression(WheelColliders[3]->GetWheelZPosition());
    //WheelColliders[3]->SetOtherWheelSuspensionCompression(WheelColliders[2]->GetWheelZPosition());

	/*FDirectInputModule& DirectInputModule = FDirectInputModule::Get();
	if (DirectInputModule.IsAvailable())
	{
		TSharedPtr<IDInputDevice> DirectInputDevice = DirectInputModule.GetDirectInputDevice();
		if (DirectInputDevice.IsValid())
		{
			float diff = (CurrentSteer - LastSteer)/DeltaTime;
			// Test with a simple pulse
			DirectInputDevice->SetChannelValue(0, FForceFeedbackChannelType::LEFT_LARGE,
				(HitAccum[0].Mz + HitAccum[1].Mz) * -10.f + diff * 2000.f * FMath::Lerp(1.f, 0.f, FMath::Abs(CurrentSteer)));
			//UE_LOG(LogTemp, Warning, TEXT("Mz : %f"), -(HitAccum[0].Mz + HitAccum[1].Mz));
			//DirectInputDevice->SetChannelValue(0, FForceFeedbackChannelType::RIGHT_SMALL, -2000.f);
			// Wait a bit
			//FPlatformProcess::Sleep(0.5f);
			// Stop the force
			//DirectInputDevice->SetChannelValue(0, FForceFeedbackChannelType::LEFT_LARGE, 0.0f);
		}
	}*/
	if(TireGraphWidget.IsValid())
		TireGraphWidget->CurrentSlipPoint = FVector(WheelColliders[2]->TransientState.kappa_prime,
			WheelColliders[2]->TransientState.alpha_prime, /**TO DO*/ WheelColliders[2]->CamberAngle);
	ACarDebugHUD* DebugHUD = Cast<ACarDebugHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(DebugHUD)
	{
		DebugHUD->AddDebugValue(RearRight->GetPacejkaResult().Alpha,
			0,
			FString(TEXT("RR slip")));
		DebugHUD->AddDebugValue(RearLeft->GetPacejkaResult().Alpha,
			1,
			FString(TEXT("RL slip")));
		DebugHUD->AddDebugValue(FrontRight->GetPacejkaResult().Alpha,
			2,
			FString(TEXT("AngularVelDelta")));
	}

}

///Optimize later to compare performance
void ACar::ComputeWheelHits(const FBodyState& State,
	TArray<FWheelHit>& Hits,
	FBodyInstance* BodyInstance, 
	float DeltaTime)
{
	Hits.Empty();

	for(UWheelCollider* WheelCollider : WheelColliders)
	{
		FWheelHit Hit;
		WheelCollider->Solve(BodyInstance, State, Hit, DeltaTime, HeunNumIterations, Drivetrain);
		Hits.Add(Hit);
	}
}

void ACar::PredictBodyState(
	FBodyInstance* BodyInstance,
	TArray<FWheelHit>& WheelHits,
	const FBodyState CurrentBodyInfo,
	FBodyState& PredictedBodyInfo,
	float PredictionTime)
{
	// --- Linear Force Integration ---
	FVector GravityForce = BodyInstance->GetBodyMass() * GetWorld()->GetGravityZ() * FVector::UpVector;
	
	FVector TotalForce = GravityForce;
	FVector TotalTorque = FVector::ZeroVector;
	for (const FWheelHit& Hit : WheelHits)
	{
		TotalForce += Hit.CalculateTotalForce();
		TotalTorque += Hit.CalculateTotalTorque(CurrentBodyInfo.Location);
	}
	
	FVector PredictedAcceleration = TotalForce / BodyInstance->GetBodyMass();
	PredictedBodyInfo.Velocity = CurrentBodyInfo.Velocity + PredictedAcceleration * PredictionTime;
	PredictedBodyInfo.Location = CurrentBodyInfo.Location + PredictedBodyInfo.Velocity * PredictionTime;
	//Maybe use average of current and predicted velocity
	//Or vt + 1/2 at^2

	const FVector InertiaTensor = BodyInstance->GetBodyInertiaTensor();//Returned in local space
	FVector LocalTotalTorque = CurrentBodyInfo.Rotation.UnrotateVector(TotalTorque);
	FVector LocalAngularAcceleration = FVector::ZeroVector;
	
	// Compute angular acceleration (component-wise: α = Torque / Inertia).
	if (!InertiaTensor.IsNearlyZero())
	{
		LocalAngularAcceleration.X = (FMath::Abs(InertiaTensor.X) > KINDA_SMALL_NUMBER) ? (LocalTotalTorque.X / InertiaTensor.X) : 0.f;
		LocalAngularAcceleration.Y = (FMath::Abs(InertiaTensor.Y) > KINDA_SMALL_NUMBER) ? (LocalTotalTorque.Y / InertiaTensor.Y) : 0.f;
		LocalAngularAcceleration.Z = (FMath::Abs(InertiaTensor.Z) > KINDA_SMALL_NUMBER) ? (LocalTotalTorque.Z / InertiaTensor.Z) : 0.f;
	}
	// Predict the new angular velocity: ω' = ω + α * t
	FVector AngularAcceleration = CurrentBodyInfo.Rotation.RotateVector(LocalAngularAcceleration);
	PredictedBodyInfo.AngularVelocity = CurrentBodyInfo.AngularVelocity + AngularAcceleration * PredictionTime;

	// To update rotation, a simple method is to integrate angular velocity.
	// For better accuracy with acceleration, you might use the average angular velocity over the time step.
	const FVector AverageAngularVelocity =
		0.5f * (CurrentBodyInfo.AngularVelocity + PredictedBodyInfo.AngularVelocity);//Maybe not use average?
	// The total rotation angle over the time step is: θ = |ω_avg| * t.
	const float RotationAngle = AverageAngularVelocity.Size() * PredictionTime;
	
	FQuat DeltaRotation = FQuat::Identity;
	if (RotationAngle > KINDA_SMALL_NUMBER)
	{
		DeltaRotation = FQuat(AverageAngularVelocity.GetSafeNormal(), RotationAngle);
	}
	PredictedBodyInfo.Rotation = DeltaRotation * CurrentBodyInfo.Rotation;
	PredictedBodyInfo.Rotation.Normalize();
}

void ACar::SetSteering(float Steering)
{
	if (FMath::IsNearlyZero(Steering)) {
		if (FrontRight) FrontRight->SteerInput = 0.0f;
		if (FrontLeft) FrontLeft->SteerInput = 0.0f;
		LastSteer = CurrentSteer;
		CurrentSteer = 0.f;
		return;
	}

	LastSteer = CurrentSteer;
	CurrentSteer = Steering;
	// Steering input (-1 to 1), scaled by speed limit
	float AbsSteering = FMath::Abs(Steering);
	float SteerSign = FMath::Sign(Steering); // >0 for left turn, <0 for right

	float SteerLimit = SteerCurve->GetFloatValue(ForwardVelocityKMH);
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

	// Apply to wheels
	if (FrontLeft) FrontLeft->SteerInput = LeftInput;
	if (FrontRight) FrontRight->SteerInput = RightInput;
	
	if(bLogWheelSteerAngle)
		UE_LOG(LogTemp, Display, TEXT("LeftAngle : %f, RightAngle : %f"), LeftInput * MaxSteerAngle, RightInput * MaxSteerAngle);
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

void ACar::DrawCOM(FBodyInstance* BodyInstance)
{
	FVector COMPos = BodyInstance->GetCOMPosition();
	FTransform BodyTransform = BodyInstance->GetUnrealWorldTransform_AssumesLocked();
	DrawDebugLine(GetWorld(), COMPos, COMPos + BodyTransform.GetUnitAxis(EAxis::Y) * 300.f,FColor::Green);
	DrawDebugLine(GetWorld(), COMPos, COMPos + BodyTransform.GetUnitAxis(EAxis::Z) * 300.f,FColor::Blue);
}