

#include "WheelCollider.h"
#include "BodyState.h"
#include "Car.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "TireParamsDataAsset.h"

UWheelCollider::UWheelCollider()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	CurrentWheelHit.SuspensionLength = RestLenght;
	bTickInEditor = true;
}

void UWheelCollider::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* CarMesh = Cast<USkeletalMeshComponent>(GetAttachParent());
	FBodyInstance* CarBodyInstance = CarMesh->GetBodyInstance();
	FTransform BodyTransform = CarBodyInstance->GetUnrealWorldTransform_AssumesLocked();

	MinSuspensionLength = RestLenght - MinSpringTravel;
	MaxSuspensionLength = RestLenght + MaxSpringTravel;
	CurrentWheelHit.SuspensionLength = MaxSuspensionLength;
	CurrentWheelHit.LastSuspensionLength = MaxSuspensionLength;
	RelativeWheelLocation = BodyTransform.InverseTransformPosition(GetComponentLocation());

	// New: Determine side sign for KPI (positive for right wheel, negative for left)
	float SideSign = FMath::Sign(RelativeWheelLocation.Y);
	float CasterRad = FMath::DegreesToRadians(CasterAngle);
	float KPIRad = FMath::DegreesToRadians(KingPinAngle);
	
	SteeringAxisLocal = FVector(
		-FMath::Sin(CasterRad) * FMath::Cos(KPIRad),
		-SideSign * FMath::Sin(KPIRad), 
		FMath::Cos(CasterRad) * FMath::Cos(KPIRad));

	CasterOffsetLocal = FVector(
	-FMath::Sin(CasterRad) * FMath::Cos(KPIRad) * RestLenght,
	-SideSign * FMath::Sin(KPIRad) * RestLenght,
	RestLenght * (1.f - FMath::Cos(CasterRad) * FMath::Cos(KPIRad)));

	// Optional: Compute mechanical trail if not set manually (approx for tuning)
	MechanicalTrail = RestLenght * FMath::Tan(CasterRad) * TO_SI;
	CurrentRestLenght = RestLenght;

	HubOffset.Y *= SideSign;
}

void UWheelCollider::Initialize(
	FVector BodyLocation,
	FQuat BodyRotation)
{
	RelativeWheelLocation = BodyRotation.UnrotateVector(GetComponentLocation() - BodyLocation);
}

void UWheelCollider::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Smooth steering interpolation (avoid instant snapping)
	CurrentSteerAngle = FMath::Lerp(CurrentSteerAngle, SteerInput, DeltaTime * SteeringResponsiveness);
}

void UWheelCollider::Solve(
	FBodyInstance* BodyInstance,
	const FBodyState& BodyInfo,
	FWheelHit& WheelHit,
	float DeltaTime,
	int32 Substeps,
	UDrivetrain* Drivetrain)
{
	if (!BodyInstance)
		return;

	// Cache simulation step values
	StepCount = Substeps;
	PhysicsDeltaTime = DeltaTime;

	// Compute suspension ray start in world space
	// Components:
	//  - RelativeLocation: Wheel-base point relative to body
	//  - RestLength: default suspension rest position
	//  - HubOffset: wheel hub displacement
	//  - SteeringAxisLocal: defines the direction of suspension compression relative to the body
	FVector StartLocation = BodyInfo.Location +
		BodyInfo.Rotation.RotateVector((RelativeWheelLocation
		+ FVector::DownVector * RestLenght +
		-HubOffset +
		SteeringAxisLocal * RestLenght));
	//UE_LOG(LogTemp, Warning, TEXT("------- %s -------"), *GetName());
	// Compute end location (bottom of suspension travel + wheel radius)
	SuspensionDirWS = BodyInfo.Rotation.RotateVector(SteeringAxisLocal);

	FVector EndLocation = StartLocation -
		(WheelRadius + MaxSuspensionLength) * SuspensionDirWS;

	// Perform collision trace + suspension + tire force resolution
	TraceWheelCollider(StartLocation,
	                   EndLocation,
	                   BodyInfo,
	                   WheelHit,
	                   Drivetrain);
}

void UWheelCollider::TraceWheelCollider(FVector StartLocation, FVector EndLocation, const FBodyState& BodyInfo,
	FWheelHit& OutWheelHit, UDrivetrain* Drivetrain)
{
	check(PhysicsDeltaTime > 0.f); // Make sure PhysicsDeltaTime/DeltaTime is valid upstream
	// Defensive: make sure wheel radius and inertia are sane
	ensureMsgf(WheelRadius > 0.f, TEXT("WheelRadius must be > 0"));
	ensureMsgf(WheelShaft.GetInertia() > 0.f, TEXT("WheelShaft inertia invalid"));
	
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceWheelCollider), /*bTraceComplex*/ false);
	Params.AddIgnoredActor(GetOwner());
	FVector WheelOffset = HubOffset.RotateAngleAxis(CurrentSteerAngle, SteeringAxisLocal);
	CurrentRestLenght = RestLenght - WheelOffset.Z;
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, Params);

	if(!bHit)
	{
		OutWheelHit.AngularVelocity = WheelShaft.GetAngularVelocity();

		// Extend suspension toward max (a small recovery rate)
		SolverSuspensionLength = FMath::Clamp(SolverSuspensionLength + PhysicsDeltaTime * 30.f, MinSuspensionLength, MaxSuspensionLength);
		SolverPreviousFrameSuspensionLength = SolverSuspensionLength;

		OutWheelHit.SuspensionForce = FVector::ZeroVector;
		OutWheelHit.SuspensionLength = SolverSuspensionLength;
		OutWheelHit.LastSuspensionLength = SolverPreviousFrameSuspensionLength;

		OutWheelHit.LongitudinalForce = FVector::ZeroVector;
		OutWheelHit.LateralForce = FVector::ZeroVector;
		OutWheelHit.LongitudinalForceApplication = EndLocation;
		OutWheelHit.LateralForceApplication = EndLocation;
		OutWheelHit.SuspensionForceApplication = BodyInfo.Location + BodyInfo.Rotation.RotateVector(RelativeWheelLocation); // use your non-u name
		OutWheelHit.LongitudinalSlip = 0;
		OutWheelHit.LateralSlip = 0;
		OutWheelHit.BodyVelocityAtContact = FVector::ZeroVector;
		OutWheelHit.bIsInAir = true;
		CurrentWheelHit = OutWheelHit;
		TransientState.Reset();
		Pos = GetWheelPositionInt();

		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Hit Normal: %s"), *Hit.ImpactNormal.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Hit location %s"), *(Hit.ImpactPoint).ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Hit Component: %s"), *Hit.GetComponent()->GetName());
	// --- Contact case ----------------------------------------------------

	// Update wheel geometry/suspension
	// Compute relative-wheel rotation (local/relative) and transform to world once
	RelativeWheelRotation = ComputeRelativeWheelRotation(); // pure, no side effect
	// Wheel-ortation in world space
	const FQuat WheelRotation = BodyInfo.Rotation * RelativeWheelRotation;

	// Save previous suspension length BEFORE we interpolate; CalculateSuspensionForce uses previous value.
	SolverPreviousFrameSuspensionLength = SolverSuspensionLength;
	// Update (interpolate/clamp) suspension length based on hit distance
	UpdateSuspensionLength(Hit.Distance);

	// Calculate normal (suspension) force (function should be pure and use SolverSuspensionLength + SolverPreviousFrameSuspensionLength)
	NormalForce = ComputeSuspensionForce();
	// Store world-space suspension force vector and application point
	OutWheelHit.SuspensionForce = NormalForce * SuspensionDirWS;
	const FVector SuspensionApplicationPoint = BodyInfo.Location + BodyInfo.Rotation.RotateVector(RelativeWheelLocation);
	OutWheelHit.SuspensionForceApplication = SuspensionApplicationPoint;

	OutWheelHit.SuspensionLength = SolverSuspensionLength;
	OutWheelHit.LastSuspensionLength = SolverPreviousFrameSuspensionLength;

	// Contact point kinematics
	FVector r = Hit.ImpactPoint - BodyInfo.Location;
	BodyVelocityAtContact = BodyInfo.Velocity + FVector::CrossProduct(BodyInfo.AngularVelocity, r);

	// Wheel forward velocity at contact (in wheel-local forward axis)
	// NOTE: GetForwardVector/GetRightVector are in world space
	SolverWheelForwardVelocityAtContact = WheelRotation.Rotator().UnrotateVector(BodyVelocityAtContact).X;
	//UE_LOG(LogTemp, Warning, TEXT("%s : %f - %f"), *GetName(), 
	//	SolverWheelForwardVelocityAtContact * TO_SI,
	//	GetAngularVelocityRad());

	// Camber angle: signed angle between wheel up and contact normal (in radians)
	CamberAngle = -FMath::Acos(FVector::DotProduct(Hit.Normal, WheelRotation.GetUpVector()));
	float Sign = FVector::DotProduct(
	WheelRotation.GetRightVector(),
	BodyInfo.Rotation.GetUpVector());
	Sign = FMath::Sign(Sign);
	CamberAngle = FMath::Acos(
	FVector::DotProduct(
		WheelRotation.GetUpVector(),
		BodyInfo.Rotation.GetUpVector()
	)) * Sign;
	//CamberAngle *= FMath::Sign(RelativeWheelLocation.Y);
	//UE_LOG(LogTemp, Warning, TEXT("%s Camber %f"), *GetName(), FMath::RadiansToDegrees(CamberAngle));

	// Compute Pacejka once for the chosen substep dt; ensure LoopDeltaTime derived after Pacejka if you need Ux/Ux0 to choose iteration count.
	// First compute Pacejka with full PhysicsDeltaTime for a baseline, or compute with tentative small dt and then recompute if needed.
	// Here we compute with a representative substep dt after deciding NumIterations based on initial slip guess.
	// To be safe, we compute Pacejka initially with PhysicsDeltaTime then decide iterations and recompute with LoopDeltaTime.
    /*********** CAMBER *************/
	float LateralVelocity = WheelRotation.UnrotateVector(BodyVelocityAtContact).Y;
	// Initial tire computation (uses PhysicsDeltaTime as conservative baseline)
	{
		PacejkaResult = TireParams->ComputeTireForces(
			NormalForce * TO_SI,                                 // N
			CamberAngle,
			BodyVelocityAtContact.Size() * TO_SI,               // m/s
			LateralVelocity * TO_SI, // m/s lateral
			SolverWheelForwardVelocityAtContact * TO_SI,        // m/s forward
			GetAngularVelocityRad()
		);
	}

	const int NumIterations = (FMath::Abs(PacejkaResult.Ux) < 0.9f * FMath::Abs(PacejkaResult.Ux0)) ? AngVelItersSlip : AngVelItersNoSlip;
	float LoopDeltaTime = PhysicsDeltaTime/(NumIterations);

	// Update transient state
	//if (false)
	TransientState = TireParams->UpdateTransientState(
		BodyVelocityAtContact.Size() * TO_SI,
		SolverWheelForwardVelocityAtContact * TO_SI,
		LateralVelocity * TO_SI,
		CamberAngle,
		GetAngularVelocityRad(),
		PacejkaResult.Fx * TO_SI, 
		PacejkaResult.Fy * TO_SI,
		NormalForce * TO_SI,
		LoopDeltaTime,
		TransientState);

	// Pacejka expects SI units. Convert as your tire model requires. TO_SI should convert cm->m or Unreal force -> N appropriately.
	//if (false)
	PacejkaResult = TireParams->ComputeTireForcesOverriden(
	   NormalForce * TO_SI,                              // force -> N
	   TransientState.kappa_prime,
	   TransientState.alpha_prime,
	   TransientState.gamma_prime * 0.f,                                      // rad
	   BodyVelocityAtContact.Size() * TO_SI,             // m/s
	   LateralVelocity * TO_SI, // lateral vel (m/s)
	   SolverWheelForwardVelocityAtContact * TO_SI,      // forward vel (m/s)
	   GetAngularVelocityRad());
	
	OutWheelHit.LongitudinalSlip = PacejkaResult.Kappa;
	OutWheelHit.LateralSlip = PacejkaResult.Alpha;

	// Compute contact directions projected onto contact plane
	const FVector LongDir = FVector::VectorPlaneProject(WheelRotation.GetForwardVector(), Hit.ImpactNormal).GetSafeNormal();
	const FVector LatDir  = FVector::VectorPlaneProject(WheelRotation.GetRightVector(), Hit.ImpactNormal).GetSafeNormal();

	// Instantaneous forces (may be updated by iteration below)
	FVector LatForce = PacejkaResult.Fy * LatDir;
	
	OutWheelHit.LateralForce = LatForce;
	OutWheelHit.Mz = PacejkaResult.Mz;
	

	// --- Iterative angular velocity update (sub-stepping the drivetrain/shaft) ---
	FVector LongForceAccum = FVector::ZeroVector;
	FVector LatForceAccum = FVector::ZeroVector;
	
	for(int i = 0; i < NumIterations; i++)
	{
		// Traction force direction (same as InstantLongForce direction)
		FVector TractionForce = PacejkaResult.Fx * LongDir;
		LongForceAccum += TractionForce;
		LatForceAccum += PacejkaResult.Fy * LatDir;

		// Impulse to drivetrain/shaft from traction (converted correctly)
		float ImpulseTraction = -TractionForce.Size() * FMath::Sign(PacejkaResult.Kappa) * WheelRadius * TO_SIPO2 * LoopDeltaTime;
		WheelShaft.ApplyImpulse(ImpulseTraction);

		// Brake impulse to shaft (BrakeAmount scales the brake torque)
		//const float BrakeImopulse = BrakeAmount * CurrentBrakeTorque * FMath::Sign(-WheelShaft.GetAngularVelocity()) * LoopDeltaTime;
		//WheelShaft.ApplyImpulse(BrakeImopulse);

		// Solve drivetrain constraints for this substep if provided
		//if (Drivetrain)
		//	Drivetrain->SolveConstraints(Drivetrain->Connections ,LoopDeltaTime);

		// Do not recompute PacejkaResult here unless angular velocity changed between substeps affects slip.
		// If you need to update Pacejka per substep, recompute it here using the updated angular velocity and shortened dt.
		if (i < NumIterations - 1)
		{
			// Optionally recompute tire forces for the reduced dt:
			PacejkaResult = TireParams->ComputeTireForcesOverriden(
	   			NormalForce * TO_SI,                              // force -> N
	   			TransientState.kappa_prime,
	   			TransientState.alpha_prime,
	   			TransientState.gamma_prime,                                      // rad
	   			BodyVelocityAtContact.Size() * TO_SI,             // m/s
	   			LateralVelocity * TO_SI, // lateral vel (m/s)
	   			SolverWheelForwardVelocityAtContact * TO_SI,      // forward vel (m/s)
	   			GetAngularVelocityRad());
			TransientState = TireParams->UpdateTransientState(
				BodyVelocityAtContact.Size() * TO_SI,
				SolverWheelForwardVelocityAtContact * TO_SI,
				LateralVelocity * TO_SI,
				CamberAngle,
				GetAngularVelocityRad(),
				PacejkaResult.Fx * TO_SI, 
				PacejkaResult.Fy * TO_SI,
				NormalForce * TO_SI,
				LoopDeltaTime,
				TransientState);
		}
	}

	// Average accumulated longitudinal force over substeps
	OutWheelHit.LongitudinalForce = LongForceAccum/NumIterations;
	//OutWheelHit.LateralForce = LatForceAccum/NumIterations;

	// Final angular velocity for the wheel
	OutWheelHit.AngularVelocity = GetAngularVelocityRad();

	// Force application points (pneumatic trail point)
	const FVector PneumaticTrailLocation = Hit.ImpactPoint + WheelRotation.GetForwardVector() * PacejkaResult.pneumaticTrail;
	OutWheelHit.LateralForceApplication = PneumaticTrailLocation;
	OutWheelHit.LongitudinalForceApplication = PneumaticTrailLocation;
	OutWheelHit.LateralForceApplication = Hit.ImpactPoint;
	OutWheelHit.LongitudinalForceApplication = Hit.ImpactPoint;
	//UE_LOG(LogTemp, Warning, TEXT("Sus len %f"), Hit.Distance);
	// Self-aligning torque vector (world axis)
	OutWheelHit.SelfAligningTorque = PacejkaResult.Mz * SATC * BodyInfo.Rotation.GetAxisZ();
	
	OutWheelHit.BodyVelocityAtContact = BodyVelocityAtContact;
	OutWheelHit.bIsInAir = false;
	CurrentWheelHit = OutWheelHit;
	Pos = GetWheelPositionInt();
	//DrawDebugCapsule(GetWorld(), GetWheelPositionInt(), 40.f, GetWheelRadius(), 
	//FRotationMatrix::MakeFromZ(WheelRotation.GetRightVector()).ToQuat(), FColor::Orange);
}

FQuat UWheelCollider::ComputeRelativeWheelRotation() const
{

    // Steering rotation around local steering axis
	// Should the Toe angle be added or neglected to find wheel rotation (Right wheel increases, Left wheel reduces)
	const float ToeSign = FMath::Sign(RelativeWheelLocation.Y);
	const float SteeringAngleRad = FMath::DegreesToRadians(CurrentSteerAngle + ToeSign * ToeAngle);
	const FQuat SteeringRotation(SteeringAxisLocal, SteeringAngleRad);

    // Camber rotation around wheel forward vector
	// Should the Camber angle be added or neglected to find wheel rotation (Right wheel reduces, Left wheel increases)
	const float CamberSign = FMath::Sign(-RelativeWheelLocation.Y);
	const float CamberAngleRad = FMath::DegreesToRadians(StaticCamberAngle) * CamberSign;
	const FQuat CamberRotation(FVector::ForwardVector, CamberAngleRad);
	
	// Combine steering and camber
	return SteeringRotation * CamberRotation;

	// Wheel rotation in world space
	/*if(bDebug)
	{
		DrawDebugCircle(GetWorld(), GetComponentLocation(), WheelRadius, 32, FColor::Yellow,
			false,
			-1,
			0,
			1.f,
			WheelRotWorld.GetForwardVector(),
			WheelRotWorld.GetUpVector(),
			true);
		DrawDebugLine(GetWorld(), GetComponentLocation(),
			GetComponentLocation() + WheelRotWorld.GetForwardVector() * 100.f,
			FColor::Red);
		DrawDebugLine(GetWorld(), GetComponentLocation(),
			GetComponentLocation() + WheelRotWorld.GetRightVector() * 100.f,
			FColor::Green);
		DrawDebugLine(GetWorld(), GetComponentLocation(),
			GetComponentLocation() + WheelRotWorld.GetUpVector() * 100.f,
			FColor::Blue);
		DrawDebugLine(GetWorld(), GetComponentLocation(),
			GetComponentLocation() + WheelRotWorld.GetForwardVector() * 200.f,
			FColor::Magenta);

		// Normalize
		FVector V = WheelRotWorld.GetUpVector().GetSafeNormal();
		FVector N = FVector::UpVector;

		// Angle between vector and normal
		float AngleToNormal = FMath::RadiansToDegrees(acosf(FVector::DotProduct(V, N)));

	}*/
}

void UWheelCollider::UpdateSuspensionLength(float HitDistance)
{
	// Clamp the current suspension length between min and max
	SolverSuspensionLength = FMath::Clamp(HitDistance - WheelRadius, MinSuspensionLength, MaxSuspensionLength);
}

float UWheelCollider::ComputeSuspensionForce() const
{
	// Compute spring velocity for damping
	float SpringVelocity = (SolverSuspensionLength - SolverPreviousFrameSuspensionLength)/(PhysicsDeltaTime);

	// Suspension compression (positive when compressed)
	float SuspensionCompression = CurrentRestLenght - SolverSuspensionLength;
	
	// Anti-roll contribution
	float AntiRollCompression = SuspensionCompression - OtherWheelSuspensionCompression;

	// Damper and spring forces
	float DamperForce = DamperCoefficient * -SpringVelocity;
	// Because 1 m = 100 cm and 1 N = 100 (kg·cm/s^2), the conversions cancel:
	// F_unreal = SpringStiffness_N_per_m * Compression_cm
	float SpringForce = SpringStiffness_NM * SuspensionCompression + AntiRollCompression * AntiRollStiffness + DamperForce; // result is in Unreal force units (kg·cm/s^2)

	// Clamp to non-negative
	SpringForce = FMath::Max(SpringForce, 0.f);

	return SpringForce;
}

void UWheelCollider::Integrate(FWheelHit& WheelHit, FBodyInstance* BodyInstance)
{
	// Validate input pointers to avoid crashes under async physics conditions
	if (!BodyInstance)
	{
		return;
	}
	// Cache the most recent wheel contact result for debugging or post-processing
	CurrentWheelHit = WheelHit;
	BodyInstance->AddForceAtPosition(
		WheelHit.SuspensionForce,
		WheelHit.SuspensionForceApplication,
		false);
	BodyInstance->AddForceAtPosition(
		WheelHit.LongitudinalForce,
		WheelHit.LongitudinalForceApplication,
		false);
	BodyInstance->AddForceAtPosition(
		WheelHit.LateralForce,
		WheelHit.LateralForceApplication,
		false);
	//BodyInstance->AddTorqueInRadians(
	//	WheelHit.SelfAligningTorque * TO_UNREAL_UNIT,
	//	false);

}