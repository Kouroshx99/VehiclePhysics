

#include "WheelCollider.h"
#include "BodyState.h"
#include "Car.h"
#include "Components/SkeletalMeshComponent.h"
#include "TireParamsDataAsset.h"

UWheelCollider::UWheelCollider()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	bTickInEditor = true;
}

void UWheelCollider::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* CarMesh = Cast<USkeletalMeshComponent>(GetAttachParent());
	if (!CarMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("UWheelCollider '%s' requires a USkeletalMeshComponent parent; skipping setup."), *GetName());
		return;
	}
	FBodyInstance* CarBodyInstance = CarMesh->GetBodyInstance();
	FTransform BodyTransform = CarBodyInstance->GetUnrealWorldTransform_AssumesLocked();

	MinSuspensionLength = RestLength - MinSpringTravel;
	MaxSuspensionLength = RestLength + MaxSpringTravel;
	CurrentWheelHit.SuspensionLength = MaxSuspensionLength;
	CurrentWheelHit.LastSuspensionLength = MaxSuspensionLength;

	// Side sign for KPI (positive for right wheel, negative for left)
	float SideSign = FMath::Sign(BodyTransform.InverseTransformPosition(GetComponentLocation()).Y);

	CurrentRestLength = RestLength;

	// Apply the side sign to a copy so the EditAnywhere HubOffset is never mutated (keeps BeginPlay idempotent).
	SideAdjustedHubOffset = HubOffset;
	SideAdjustedHubOffset.Y *= SideSign;
}

void UWheelCollider::ComputeSteeringAxis()
{
	// Side sign for KPI (positive for right wheel, negative for left)
	float SideSign = FMath::Sign(RelativeWheelLocation.Y);
	float CasterRad = FMath::DegreesToRadians(CasterAngle);
	float KPIRad = FMath::DegreesToRadians(KingPinAngle);

	SteeringAxisLocal = FVector(
		-FMath::Sin(CasterRad) * FMath::Cos(KPIRad),
		-SideSign * FMath::Sin(KPIRad),
		FMath::Cos(CasterRad) * FMath::Cos(KPIRad));
}

void UWheelCollider::CacheRelativeWheelLocation(
	FVector BodyLocation,
	FQuat BodyRotation)
{
	RelativeWheelLocation = BodyRotation.UnrotateVector(GetComponentLocation() - BodyLocation);
	
	ComputeSteeringAxis();
}

void UWheelCollider::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Smooth steering interpolation (avoid instant snapping)
	CurrentSteerAngle = FMath::Lerp(CurrentSteerAngle, TargetSteerAngleDeg, DeltaTime * SteeringResponsiveness);
}

void UWheelCollider::Solve(
	FBodyInstance* BodyInstance,
	const FBodyState& BodyInfo,
	FWheelHit& WheelHit,
	float DeltaTime,
	UDrivetrain* Drivetrain)
{
	if (!BodyInstance)
		return;

	PhysicsDeltaTime = DeltaTime;

	// StartLocation = the strut mount (top of suspension) in world space.
	// NOTE: temporary geometry. Kingpin/caster tilt SteeringAxisLocal toward the vehicle
	// center, so the strut mount is not directly above the wheel-base point. We drop straight
	// DOWN by RestLength to approximate the ground contact patch, then walk back UP the (tilted)
	// steering axis by RestLength to recover the strut-mount location. The two RestLength terms
	// nearly cancel in Z by design. Proper suspension geometry will replace this later.
	FVector StartLocation = BodyInfo.Location +
		BodyInfo.Rotation.RotateVector((RelativeWheelLocation
		+ FVector::DownVector * RestLength +
		-SideAdjustedHubOffset +
		SteeringAxisLocal * RestLength));

	// Compute end location (bottom of suspension travel + wheel radius)
	SuspensionDirWS = BodyInfo.Rotation.RotateVector(SteeringAxisLocal);

	FVector EndLocation = StartLocation -
		(WheelRadius + MaxSuspensionLength) * SuspensionDirWS;

	// Perform collision trace + suspension + tire force resolution
	TraceWheelCollider(StartLocation,
	                   EndLocation,
	                   BodyInfo,
	                   WheelHit);
}

void UWheelCollider::TraceWheelCollider(FVector StartLocation, FVector EndLocation, const FBodyState& BodyInfo,
	FWheelHit& OutWheelHit)
{
	check(PhysicsDeltaTime > 0.f); // Make sure PhysicsDeltaTime/DeltaTime is valid upstream
	// Defensive: make sure wheel radius and inertia are sane
	ensureMsgf(WheelRadius > 0.f, TEXT("WheelRadius must be > 0"));
	ensureMsgf(WheelShaft.GetInertia() > 0.f, TEXT("WheelShaft inertia invalid"));
	
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TraceWheelCollider), /*bTraceComplex*/ false);
	Params.AddIgnoredActor(GetOwner());
	// Hub displacement produced by steering about the (tilted) steering axis. Its Z component is the
	// kingpin/caster-induced hub lift, which shortens the effective rest length while steering.
	const FVector SteerHubOffsetLocal = SideAdjustedHubOffset.RotateAngleAxis(CurrentSteerAngle, SteeringAxisLocal);
	CurrentRestLength = RestLength - SteerHubOffsetLocal.Z;

	// Trace channel assumption: drivable ground is on the Visibility channel (ECC_Visibility).
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		Params);

	if(!bHit)
	{
		BuildNoContactHit(EndLocation, 
			BodyInfo, 
			OutWheelHit);
		return;
	}

	ResolveContact(Hit, BodyInfo, OutWheelHit);
}

void UWheelCollider::ResolveContact(const FHitResult& Hit, const FBodyState& BodyInfo, FWheelHit& OutWheelHit)
{
	// --- Contact case ----------------------------------------------------

	// Update wheel geometry/suspension
	// Compute relative-wheel rotation (local/relative) and transform to world once
	RelativeWheelRotation = ComputeRelativeWheelRotation(); // pure, no side effect
	// Wheel-orientation in world space
	const FQuat WheelRotation = BodyInfo.Rotation * RelativeWheelRotation;

	// Save previous suspension length BEFORE we interpolate; ComputeSuspensionForce uses previous value.
	SolverPreviousFrameSuspensionLength = SolverSuspensionLength;
	// Update (interpolate/clamp) suspension length based on hit distance
	UpdateSuspensionLength(Hit.Distance);

	// Normal (suspension) force from SolverSuspensionLength + SolverPreviousFrameSuspensionLength
	NormalForceUU = ComputeSuspensionForce();
	// Store world-space suspension force vector and application point
	OutWheelHit.SuspensionForce = NormalForceUU * SuspensionDirWS;
	const FVector SuspensionApplicationPoint = BodyInfo.Location +
		BodyInfo.Rotation.RotateVector(RelativeWheelLocation);
	OutWheelHit.SuspensionForceApplication = SuspensionApplicationPoint;

	OutWheelHit.SuspensionLength = SolverSuspensionLength;
	OutWheelHit.LastSuspensionLength = SolverPreviousFrameSuspensionLength;
	if (!TireParams)
		return;
	UpdateContactKinematics(Hit, BodyInfo, WheelRotation);

	// Tire solve. ComputeTireStateAndForces computes the tire forces and the per-solve substep
	// count / loop dt, storing them on the component (LateralContactVelocityCmPerSec,
	// SolverSubstepCount, SolverLoopDeltaTime); FillContactWheelHit then consumes those.
	ComputeTireStateAndForces(WheelRotation);
	FillContactWheelHit(Hit, BodyInfo, WheelRotation, OutWheelHit);
}

void UWheelCollider::BuildNoContactHit(const FVector& EndLocation, const FBodyState& BodyInfo, FWheelHit& OutWheelHit)
{
	OutWheelHit.AngularVelocity = WheelShaft.GetAngularVelocity();
	// Extend suspension toward max at a temporary droop-recovery rate (30 cm/s).
	// TODO: this ad-hoc rate goes away once unsprung mass is modelled.
	SolverSuspensionLength = FMath::Clamp(
		SolverSuspensionLength + PhysicsDeltaTime * 30.f,
		MinSuspensionLength,
		MaxSuspensionLength);

	SolverPreviousFrameSuspensionLength = SolverSuspensionLength;

	OutWheelHit.SuspensionForce = FVector::ZeroVector;
	OutWheelHit.SuspensionLength = SolverSuspensionLength;
	OutWheelHit.LastSuspensionLength = SolverPreviousFrameSuspensionLength;
	OutWheelHit.LongitudinalForce = FVector::ZeroVector;
	OutWheelHit.LateralForce = FVector::ZeroVector;
	OutWheelHit.LongitudinalForceApplication = EndLocation;
	OutWheelHit.LateralForceApplication = EndLocation;
	OutWheelHit.SuspensionForceApplication =
		BodyInfo.Location + BodyInfo.Rotation.RotateVector(RelativeWheelLocation);
	OutWheelHit.LongitudinalSlip = 0.f;
	OutWheelHit.LateralSlip = 0.f;
	OutWheelHit.BodyVelocityAtContact = FVector::ZeroVector;
	OutWheelHit.bIsInAir = true;

	CurrentWheelHit = OutWheelHit;
	// Airborne: reset the tire's transient (relaxation-length) state so stale slip history
	// doesn't carry into the next ground contact.
	TransientState.Reset();
	WheelWorldPositionCm = GetWheelWorldPositionInternal();
}

void UWheelCollider::UpdateContactKinematics(const FHitResult& Hit, const FBodyState& BodyInfo,
	const FQuat& WheelRotation)
{
	const FVector ContactOffsetFromCoM = Hit.ImpactPoint - BodyInfo.Location;

	BodyVelocityAtContact =
		BodyInfo.Velocity +
		FVector::CrossProduct(BodyInfo.AngularVelocity, ContactOffsetFromCoM);

	// Wheel forward velocity at contact (wheel-local +X). Direct quaternion unrotate — no
	// quat->rotator->unrotate round-trip.
	ForwardContactVelocityMPerSec =
		WheelRotation.UnrotateVector(BodyVelocityAtContact).X * CmToM;

	// Camber sign convention: sign of dot(wheel right axis, body up). It disambiguates the
	// unsigned acos below into a signed camber angle (which way the wheel leans).
	const float CamberSign = FMath::Sign(
		FVector::DotProduct(
			WheelRotation.GetRightVector(),
			BodyInfo.Rotation.GetUpVector()));

	// Camber angle: signed angle between wheel up and contact normal.
	// Clamp the dot to [-1,1] before acos — float error can push it slightly outside on flat
	// ground, which would make acos() return NaN.
	const float CamberCos = FMath::Clamp(
		FVector::DotProduct(WheelRotation.GetUpVector(), Hit.Normal), -1.f, 1.f);
	CamberAngleRad = FMath::Acos(CamberCos) * CamberSign;
}

void UWheelCollider::ComputeTireStateAndForces(const FQuat& WheelRotation)
{
	// Cache SI conversions once; the tire model works in SI (N, m, m/s, rad).
	//
	// Which load the tire sees depends on whether the tire has its own spring:
	//  - Rigid tire: the carrier is massless and pinned to the ground, so the tire
	//    load IS the suspension force - there is nothing between them to differ.
	//  - Compliant tire: the unsprung mass sits between the two springs, so the
	//    tire's load comes from its own deflection via (4.E68) and differs from the
	//    suspension force by the unsprung weight plus m*a.
	const float Fz_N = (bUseTireCompliance && TireParams != nullptr)
		? TyreLoadN
		: NormalForceUU * CmToM;                                       // UU -> N
	const float SpeedMPerSec = BodyVelocityAtContact.Size() * CmToM; // cm/s -> m/s
	LateralContactVelocityCmPerSec = WheelRotation.UnrotateVector(BodyVelocityAtContact).Y;
	const float LatVelMPerSec = LateralContactVelocityCmPerSec * CmToM;
	
	// Steady-state tire forces first.
	PacejkaResult = TireParams->ComputeTireForces(
		Fz_N,
		CamberAngleRad,
		SpeedMPerSec,
		LatVelMPerSec,
		ForwardContactVelocityMPerSec,
		GetAngularVelocityRad());

	// More angular-velocity substeps near the grip limit: when |Ux| is within 90% of the peak
	// |Ux0|, the wheel is close to slipping and needs finer sub-stepping for stability.
	SolverSubstepCount =
		(FMath::Abs(PacejkaResult.Ux) < 0.9f * FMath::Abs(PacejkaResult.Ux0))
			? SlipSubstepCount
			: NoSlipSubstepCount;

	SolverSubstepCount = FMath::Max(SolverSubstepCount, 1);
	SolverLoopDeltaTime = PhysicsDeltaTime / SolverSubstepCount;

	if (bUseTransients)
	{
		// ...then override the steady-state result with the transient (relaxation-length) state.
		TransientState = TireParams->UpdateTransientState(
			SpeedMPerSec,
			ForwardContactVelocityMPerSec,
			LatVelMPerSec,
			CamberAngleRad,
			GetAngularVelocityRad(),
			PacejkaResult.Fx * CmToM,
			PacejkaResult.Fy * CmToM,
			Fz_N,
			SolverLoopDeltaTime,
			TransientState);
		PacejkaResult = TireParams->ComputeTireForcesOverriden(
			Fz_N,
			TransientState.kappa_prime,
			TransientState.alpha_prime,
			TransientState.gamma_prime,
			SpeedMPerSec,
			LatVelMPerSec,
			ForwardContactVelocityMPerSec,
			GetAngularVelocityRad());
	}
	else
	{
		TransientState.kappa_prime = PacejkaResult.Kappa;
		TransientState.alpha_prime = PacejkaResult.Alpha;
		TransientState.gamma_prime = CamberAngleRad;
	}
}

void UWheelCollider::ApplyTireImpulseToShaft(
	const FVector& TractionForce,
	float LoopDeltaTime)
{
	// Convert the longitudinal tire force into an angular impulse on the wheel shaft:
	//   impulse [N·m·s] = -|F| * sign(Kappa) * radius * dt
	// - sign(Kappa): the traction force opposes the slip direction, so we use the slip sign as a
	//   proxy for the sign of Fx (avoids projecting the force onto the wheel's longitudinal axis).
	// - CmToM_Squared = (cm->m)^2 = 0.0001: folds the UU->N force conversion and the cm->m radius-arm
	//   conversion into one factor, yielding SI torque·time (N·m·s).
	const float ImpulseTraction =
		-TractionForce.Size()
		* FMath::Sign(PacejkaResult.Kappa)
		* WheelRadius
		* CmToM_Squared
		* LoopDeltaTime;

	WheelShaft.ApplyImpulse(ImpulseTraction);
}

void UWheelCollider::FillContactWheelHit(const FHitResult& Hit, const FBodyState& BodyInfo, const FQuat& WheelRotation,
	FWheelHit& OutWheelHit)
{
	OutWheelHit.LongitudinalSlip = PacejkaResult.Kappa;
	OutWheelHit.LateralSlip = PacejkaResult.Alpha;

	// Compute contact directions projected onto contact plane
	const FVector LongDir =
		FVector::VectorPlaneProject(WheelRotation.GetForwardVector(), Hit.Normal).GetSafeNormal();

	const FVector LatDir =
		FVector::VectorPlaneProject(WheelRotation.GetRightVector(), Hit.Normal).GetSafeNormal();

	OutWheelHit.LateralForce = PacejkaResult.Fy * LatDir;
	OutWheelHit.Mz = PacejkaResult.Mz;

	// Sub-step the wheel shaft under traction; returns the averaged longitudinal force.
	OutWheelHit.LongitudinalForce = IntegrateWheelShaftUnderTraction(LongDir);
	// Final angular velocity for the wheel
	OutWheelHit.AngularVelocity = GetAngularVelocityRad();
	
	// Force application points (pneumatic trail point)
	const FVector PneumaticTrailLocation = Hit.ImpactPoint + WheelRotation.GetForwardVector() * PacejkaResult.pneumaticTrail * MToCm;
	OutWheelHit.LateralForceApplication = PneumaticTrailLocation;
	OutWheelHit.LongitudinalForceApplication = PneumaticTrailLocation;
	
	// Self-aligning torque vector (world axis)
	OutWheelHit.SelfAligningTorque = PacejkaResult.Mz * SelfAligningTorqueScale * BodyInfo.Rotation.GetAxisZ();

	OutWheelHit.BodyVelocityAtContact = BodyVelocityAtContact;
	OutWheelHit.bIsInAir = false;

	CurrentWheelHit = OutWheelHit;
	WheelWorldPositionCm = GetWheelWorldPositionInternal();
}

FVector UWheelCollider::IntegrateWheelShaftUnderTraction(const FVector& LongDir)
{
	// SI inputs for the tire model (fixed across substeps).
	const float Fz_N          = NormalForceUU * CmToM;                 // UU -> N
	const float SpeedMPerSec  = BodyVelocityAtContact.Size() * CmToM;  // cm/s -> m/s
	const float LatVelMPerSec = LateralContactVelocityCmPerSec * CmToM;

	// --- Iterative angular velocity update (sub-stepping the drivetrain/shaft) ---
	FVector LongForceAccum = FVector::ZeroVector;

	for (int32 Iteration = 0; Iteration < SolverSubstepCount; ++Iteration)
	{
		const FVector TractionForce = PacejkaResult.Fx * LongDir;
		LongForceAccum += TractionForce;

		ApplyTireImpulseToShaft(TractionForce, SolverLoopDeltaTime);

		// Recompute the tire state with the just-updated wheel speed so the NEXT substep's traction
		// force reflects it. Skipped on the final substep because there is no next iteration to
		// consume it. Consequence: the PacejkaResult read after the loop (self-aligning torque,
		// pneumatic trail) is from the second-to-last substep, not a post-final-impulse recompute.
		if (Iteration < SolverSubstepCount - 1)
		{
			PacejkaResult = TireParams->ComputeTireForcesOverriden(
				Fz_N,
				TransientState.kappa_prime,
				TransientState.alpha_prime,
				TransientState.gamma_prime,
				SpeedMPerSec,
				LatVelMPerSec,
				ForwardContactVelocityMPerSec,
				GetAngularVelocityRad());

			if (bUseTransients)
			{
				TransientState = TireParams->UpdateTransientState(
					SpeedMPerSec,
					ForwardContactVelocityMPerSec,
					LatVelMPerSec,
					CamberAngleRad,
					GetAngularVelocityRad(),
					PacejkaResult.Fx * CmToM,
					PacejkaResult.Fy * CmToM,
					Fz_N,
					SolverLoopDeltaTime,
					TransientState);
			}
		}
	}

	// Average accumulated longitudinal force over substeps
	return LongForceAccum / SolverSubstepCount;
}

FQuat UWheelCollider::ComputeRelativeWheelRotation() const
{
	// --- Steering (includes static toe) about the local steering axis ---
	// Toe convention: positive ToeAngle = toe-IN (wheel fronts point toward the vehicle centreline).
	// The SAME ToeAngle is entered for both wheels; the side sign makes each toe symmetrically:
	// a positive steer angle rotates the wheel's forward toward +Y, so to toe IN the right wheel
	// (Y>0) needs a negative offset and the left wheel (Y<0) a positive one -> sign(-Y).
	const float ToeSign = FMath::Sign(-RelativeWheelLocation.Y);
	const float SteeringAngleRad = FMath::DegreesToRadians(CurrentSteerAngle + ToeSign * ToeAngle);
	const FQuat SteeringRotation(SteeringAxisLocal, SteeringAngleRad);

	// --- Static camber about the wheel forward axis ---
	// Convention: positive StaticCamberAngle = top tilted OUT, negative = top tilted in (negative
	// camber). The side sign (sign(-Y)) applies it symmetrically so both wheels lean the same sense.
	const float CamberSign = FMath::Sign(-RelativeWheelLocation.Y);
	const float StaticCamberAngleRad = FMath::DegreesToRadians(StaticCamberAngle) * CamberSign;
	const FQuat CamberRotation(FVector::ForwardVector, StaticCamberAngleRad);

	// Combine steering and camber
	return SteeringRotation * CamberRotation;
}

void UWheelCollider::UpdateSuspensionLength(float HitDistance)
{
	if (bUseTireCompliance && TireParams != nullptr)
	{
		IntegrateUnsprungMass(HitDistance);
		return;
	}

	// Rigid tire: the wheel centre is pinned exactly one radius above the ground, so
	// the suspension absorbs every bit of the travel and the tire never deflects.
	SolverSuspensionLength = FMath::Clamp(HitDistance - WheelRadius, MinSuspensionLength, MaxSuspensionLength);
	TyreDeflectionCm = 0.f;
	TyreLoadN = 0.f;
}

void UWheelCollider::IntegrateUnsprungMass(float HitDistance)
{
	// Two springs in series along the strut axis, with the unsprung mass between:
	//
	//     chassis --[suspension spring/damper]-- wheel carrier --[tire]-- ground
	//
	// SolverSuspensionLength (L) is the carrier's position: distance from the strut
	// mount down to the wheel centre. It is an integrated STATE here, not something
	// read off the raycast - that is the whole difference from the rigid model.
	//
	// Geometry along the axis:  L + (R0 - rho_z) = HitDistance
	//   =>  rho_z = R0 - HitDistance + L
	const float UnloadedRadiusCm = WheelRadius;

	// Seed from a settled rest pose rather than letting the corner drop and bounce
	// on the first frame. Solves both springs for the deflection that balances them.
	if (!bUnsprungStateInitialised)
	{
		SolverSuspensionLength = FMath::Clamp(HitDistance - UnloadedRadiusCm, MinSuspensionLength, MaxSuspensionLength);
		UnsprungVelocityCmPerSec = 0.f;
		bUnsprungStateInitialised = true;
	}

	TyreDeflectionCm = UnloadedRadiusCm - HitDistance + SolverSuspensionLength;

	// Tire load from Pacejka (4.E68). Fx/Fy come from the previous solve; the
	// equation is implicit in them, and lagging by one step is the standard
	// resolution (same explicit coupling a penalty spring already has).
	TyreLoadN = TireParams->ComputeNormalLoad(
		FMath::Max(TyreDeflectionCm, 0.f) * CmToM,
		CamberAngleRad,
		GetAngularVelocityRad(),
		PacejkaResult.Fx * CmToM,
		PacejkaResult.Fy * CmToM);

	// Radial damping. (4.E68) is a pure spring, so without this the hop mode rings.
	// The carrier approaches the ground as L grows, hence the sign.
	const float ApproachSpeedMPerSec = UnsprungVelocityCmPerSec * CmToM;
	const float TyreDampingN = (TyreDeflectionCm > 0.f)
		? TireRadialDampingNsPerM * ApproachSpeedMPerSec
		: 0.f;

	// Tire pushes the carrier back up the axis; never pulls it down.
	const float TyreForceUU = FMath::Max(TyreLoadN + TyreDampingN, 0.f) * MToCm;

	// Suspension pushes the carrier down the axis by exactly what it pushes the
	// body up by (Newton's third law), which is what ComputeSuspensionForce returns.
	const float SuspensionForceUU = ComputeSuspensionForce();

	// Gravity resolved onto the strut axis. SuspensionDirWS points from the mount
	// toward the wheel, i.e. downward, so this comes out positive.
	const float GravityAlongAxis = GetWorld()->GetGravityZ() * SuspensionDirWS.Z;

	// Semi-implicit Euler. Units are consistent in UU throughout: kg * cm/s^2.
	const float AccelCmPerSec2 =
		((SuspensionForceUU - TyreForceUU) / FMath::Max(UnsprungMassKg, KINDA_SMALL_NUMBER))
		+ GravityAlongAxis;

	UnsprungVelocityCmPerSec += AccelCmPerSec2 * PhysicsDeltaTime;
	SolverSuspensionLength += UnsprungVelocityCmPerSec * PhysicsDeltaTime;

	// Travel stops: clamp and kill the inbound rate so the carrier cannot tunnel
	// through the bump stop or drop past full droop.
	if (SolverSuspensionLength <= MinSuspensionLength)
	{
		SolverSuspensionLength = MinSuspensionLength;
		UnsprungVelocityCmPerSec = FMath::Max(UnsprungVelocityCmPerSec, 0.f);
	}
	else if (SolverSuspensionLength >= MaxSuspensionLength)
	{
		SolverSuspensionLength = MaxSuspensionLength;
		UnsprungVelocityCmPerSec = FMath::Min(UnsprungVelocityCmPerSec, 0.f);
	}

	// Recompute the deflection at the new position so Fz and the reported rho_z
	// match the state the rest of the solve will see.
	TyreDeflectionCm = FMath::Max(UnloadedRadiusCm - HitDistance + SolverSuspensionLength, 0.f);
}

float UWheelCollider::ComputeSuspensionForce() const
{
	// Rate of change of suspension length (cm/s); drives the damper term.
	float SpringVelocity = (SolverSuspensionLength - SolverPreviousFrameSuspensionLength)/(PhysicsDeltaTime);

	// Suspension compression, positive when compressed [cm]. Same accessor is fed to the opposite
	// wheel (see Car anti-roll update), so both sides of the difference below use one definition.
	float SuspensionCompression = GetSuspensionCompressionCm();

	// Anti-roll contribution: reacts to the genuine compression difference vs the opposite wheel.
	float AntiRollCompression = SuspensionCompression - OtherWheelSuspensionCompression;

	// Unit cancellation (applies to ALL THREE terms below): since 1 m = 100 cm and 1 N = 100
	// (kg·cm/s^2), the cm<->m and N<->UU factors cancel, so multiplying SI coefficients by cm
	// quantities yields Unreal force units directly (no explicit conversion needed):
	//   spring:    N/m   * cm    -> UU
	//   anti-roll: N/m   * cm    -> UU
	//   damper:    N·s/m * cm/s  -> UU
	float DamperForce = DamperCoefficient * -SpringVelocity;
	float SpringForce = SpringStiffness * SuspensionCompression + AntiRollCompression * AntiRollStiffness + DamperForce; // result is in Unreal force units (kg·cm/s^2)

	// Strut can push but not pull: clamp to >= 0. Note this also drops rebound damping whenever
	// the spring term can't offset a negative damper force (a known simplification).
	SpringForce = FMath::Max(SpringForce, 0.f);

	return SpringForce;
}

void UWheelCollider::ApplyForcesToBody(FWheelHit& WheelHit, FBodyInstance* BodyInstance)
{
	// Validate input pointers to avoid crashes under async physics conditions
	if (!BodyInstance)
		return;

	// DISABLED, AND NOT BY ACCIDENT - but it used to look like one.
	//
	// UWheelCollider is the retained raycast path; UWishboneSuspension applies its own
	// reactions and nothing calls into this any more. It was switched off with a bare
	// `return;` on the line below, which left the entire body of the function dead while
	// reading exactly like a debugging leftover somebody forgot to remove.
	//
	// UE 4.27 compiled that silently. 5.8 makes unreachable code an error, which is how
	// it surfaced. The body is kept under #if 0 rather than deleted, because the comments
	// inside it are the only remaining description of how this path fed the body, and the
	// class is deliberately retained.
	return;

#if 0 // Retained as documentation of the raycast path - see the note above.
	// Re-cache the wheel hit. NOTE: this receives the Heun-averaged hit (averaged across solve
	// iterations), so CurrentWheelHit is overwritten with the averaged result to seed the next
	// frame. Consequence: GetCurrentWheelHit() returns the per-iteration solve value during the
	// solve and the averaged value after this call.
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
	BodyInstance->AddTorqueInRadians(
		WheelHit.SelfAligningTorque * MToCm,
		false);
#endif
}