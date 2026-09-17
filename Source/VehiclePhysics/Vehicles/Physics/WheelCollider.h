// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shaft.h"
#include "PacejkaResult.h"
#include "WheelHit.h"
#include "WheelSolverData.h"
#include "TransientState.h"
#include "Components/SceneComponent.h"
#include "WheelCollider.generated.h"

struct FBodyState;
class UDrivetrain;
class UTireParamsDataAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UWheelCollider : public USceneComponent
{
	GENERATED_BODY()
	static constexpr float MToCm = 100.f;
	static constexpr float CmToM = 0.01f;
	static constexpr float CmToM_Squared = 0.0001f;

public:
	UWheelCollider();
	
	void CacheRelativeWheelLocation(FVector BodyLocation, FQuat BodyRotation);

	/**
	* Per-frame update for the wheel collider.
	*
	* Handles:
	*  - Steering interpolation (visual & logical)
	*/
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/**
 	* Performs a full physics solve for a single wheel each frame.
 	*
 	* @param BodyInstance   The owning body’s physics instance (used for applying forces and getting velocity).
 	* @param BodyInfo       Cached world-space transform, velocity, and angular velocity of the vehicle body.
 	* @param WheelHit       Output struct containing contact, forces, and suspension data computed in TraceWheelCollider().
 	* @param DeltaTime      Simulation time step for this physics update.
 	* @param Drivetrain     Drivetrain reference for transmitting torque.
 	*
 	* Steps:
 	*  1. Computes suspension trace start/end in world space.
 	*  2. Runs the raycast and tire model solve via TraceWheelCollider().
 	*/
	void Solve(FBodyInstance* BodyInstance,
		const FBodyState& BodyInfo,
		FWheelHit& WheelHit,
		float DeltaTime,
		UDrivetrain* Drivetrain);
	
	FORCEINLINE FRotator GetWheelRotLocal() const{ return  RelativeWheelRotation.Rotator(); }
	FORCEINLINE float GetWheelRadius() const { return WheelRadius; }
	FORCEINLINE float GetRestLength() const { return RestLength; }
	FORCEINLINE float GetAngularVelocityRad() const { return WheelShaft.GetAngularVelocity(); }
	FORCEINLINE float GetAngularVelocityInDegrees() const { return FMath::RadiansToDegrees(GetAngularVelocityRad()); }
	FORCEINLINE float GetWheelZPosition() const
	{
#if WITH_EDITOR
		// Editor preview world (not simulating): no valid suspension solve yet.
		if (GIsEditor && GetWorld() && !GetWorld()->IsGameWorld())
			return 0.f;
#endif
		return RestLength - CurrentWheelHit.SuspensionLength;
	}
	// Suspension compression [cm], positive when compressed. Single source of truth for the anti-roll
	// term: ComputeSuspensionForce uses it locally and it is fed to the opposite wheel via
	// SetOtherWheelSuspensionCompression, so both sides of that difference share one definition.
	FORCEINLINE float GetSuspensionCompressionCm() const { return CurrentRestLength - SolverSuspensionLength; }

	// --- Tire compliance diagnostics. All read zero unless bUseTireCompliance is on. ---
	FORCEINLINE bool IsTireComplianceEnabled() const { return bUseTireCompliance; }
	/** Tire radial deflection, cm. Pacejka's rho_z. Zero for a rigid tire. */
	FORCEINLINE float GetTyreDeflectionCm() const { return TyreDeflectionCm; }
	/** Normal load from the tire's own spring (4.E68), N. Equals the suspension
	 *  force only for a massless wheel - see GetSuspensionCompressionCm's sibling,
	 *  the suspension force, via GetCurrentWheelHit().SuspensionForce. */
	FORCEINLINE float GetTyreLoadN() const { return TyreLoadN; }
	/** Unsprung mass velocity along the strut axis, cm/s. Positive = extending. */
	FORCEINLINE float GetUnsprungVelocityCmPerSec() const { return UnsprungVelocityCmPerSec; }
	FORCEINLINE FVector GetWheelPosition() const { return WheelWorldPositionCm;}
	FORCEINLINE float GetCamberAngleRad() const { return CamberAngleRad; }
	FORCEINLINE float GetCurrentWheelAngle() const {return CurrentSteerAngle; }
	FORCEINLINE void SetOtherWheelSuspensionCompression(float Compression) {OtherWheelSuspensionCompression = Compression; }
	// Cached wheel-hit result: whatever the most recent trace produced.
	//
	// It used to be timing-dependent - the per-iteration hit during a solve, replaced by a
	// Heun-averaged one afterwards - and that caveat is gone with the integrator. There are
	// no solve iterations to average over now, so there is only ever the latest value.
	FORCEINLINE const FWheelHit& GetCurrentWheelHit() const { return CurrentWheelHit; }
	FORCEINLINE UTireParamsDataAsset* GetParams() const { return TireParams; }
	// The FShaft lives inside this component; the returned pointer must outlive any cache of it
	// (e.g. the drivetrain caches it per substep) and is invalidated if the component is reconstructed.
	FORCEINLINE FShaft* GetShaft() { return &WheelShaft; }
	FORCEINLINE const FPacejkaResult& GetPacejkaResult() const { return PacejkaResult; }

	/**
	* Applies all per-wheel forces and torques to the physics body (Unreal integrates the motion).
	*
	* @param WheelHit        Struct containing all resolved forces, torques, and application points from the solver.
	* @param BodyInstance    The physics body to apply these forces and torques to.
	*
	* Applies:
	*  - Suspension force (vertical support)
	*  - Longitudinal force (traction or braking)
	*  - Lateral force (cornering)
	*  - Self-aligning torque (tire restoring torque)
	*
	* Notes:
	*  - Forces are applied in world space.
	*  - Scaling for torque (×100) assumes Unreal units (cm, N·cm → N·m).
	*  - No smoothing or substepping applied here; integrate at physics tick rate.
	*/
	void ApplyForcesToBody(FWheelHit& WheelHit, FBodyInstance* BodyInstance);

	FORCEINLINE const FTransientTireState& GetTransientState() const { return TransientState; }
	FORCEINLINE float GetTargetSteerAngleDeg() const { return TargetSteerAngleDeg; }
	FORCEINLINE void SetTargetSteerAngleDeg(float InSteerAngleDeg) { TargetSteerAngleDeg = InSteerAngleDeg; }
	FORCEINLINE float GetForwardContactVelocityMPerSec() const { return ForwardContactVelocityMPerSec; }
	FORCEINLINE FWheelSolverData GetSolverData() const
	{
		FWheelSolverData SolverData;
		SolverData.AngularVelocity = WheelShaft.GetAngularVelocity();
		SolverData.SuspensionLength = SolverSuspensionLength;
		SolverData.LastSuspensionLength = SolverPreviousFrameSuspensionLength;
		return SolverData;
	}

protected:
	virtual void BeginPlay() override;
	void ComputeSteeringAxis();

private:

	// Internal: recompute the wheel world position from the current suspension solve.
	FORCEINLINE FVector GetWheelWorldPositionInternal() const
	{
		FVector Position = CurrentWheelHit.SuspensionForceApplication -
			SuspensionDirWS * SolverSuspensionLength;
		return Position;
	}

	/// Pacejka tire parameters + transient relaxation model for this wheel (must be assigned).
	UPROPERTY(EditAnywhere, Category = "Wheel", meta = (AllowPrivateAccess = true))
	UTireParamsDataAsset* TireParams;

	/// Use the transient (relaxation-length) tire model in addition to the steady-state solve.
	UPROPERTY(EditAnywhere, Category = "Wheel", meta = (AllowPrivateAccess = true))
	bool bUseTransients = false;
	
	/// Wheel radius [cm]
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true, Units="cm"))
	float WheelRadius = 35.f;

	/// Steering interpolation speed (higher = snappier)
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	float SteeringResponsiveness = 10.f;

	/// Angular-velocity solver substeps near the slip limit
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	int32 SlipSubstepCount = 10;

	/// Angular-velocity solver substeps away from the slip limit
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	int32 NoSlipSubstepCount = 2;
	
	
	/// Spring stiffness [N/m] (no UE Units enum for N/m, so documented here)
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true))
	float SpringStiffness = 50000.f;

	/// Damper coefficient [N·s/m]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true))
	float DamperCoefficient = 2500.f;

	/// Anti-roll bar stiffness [N/m]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true))
	float AntiRollStiffness = 10000.f;

	/**
	 * Gives the tire its own vertical spring instead of treating it as rigid.
	 *
	 * OFF (default) is the original model: the wheel centre is pinned exactly one
	 * radius above the ground, so all vertical give lives in the suspension and the
	 * tire load equals the suspension force by definition.
	 *
	 * ON introduces a real unsprung mass between the two springs. The suspension
	 * force and the tire load then genuinely differ (by the unsprung weight plus
	 * m*a), the tire deflection rho_z becomes a real quantity that Pacejka's
	 * (4.E68) turns into Fz, and the corner gains a wheel-hop mode. Requires a tire
	 * asset - falls back to rigid without one.
	 *
	 * This changes handling and will need retuning; it is not a drop-in.
	 */
	UPROPERTY(EditAnywhere, Category="Suspension|Tire Compliance", meta=(AllowPrivateAccess=true))
	bool bUseTireCompliance = false;

	/**
	 * Unsprung mass of the corner [kg]: wheel, tire, hub, brake and roughly half
	 * the suspension links. Only used when bUseTireCompliance is on - it is the
	 * mass that sits between the suspension spring and the tire spring, and its
	 * size sets the wheel-hop frequency.
	 */
	UPROPERTY(EditAnywhere, Category="Suspension|Tire Compliance", meta=(AllowPrivateAccess=true, ClampMin="1.0"))
	float UnsprungMassKg = 40.f;

	/**
	 * Tire radial damping [N.s/m] - the carcass's own hysteresis, not the shock's.
	 * (4.E68) is a pure spring with no damping term, so without this the wheel-hop
	 * mode is essentially undamped and rings.
	 */
	UPROPERTY(EditAnywhere, Category="Suspension|Tire Compliance", meta=(AllowPrivateAccess=true, ClampMin="0.0"))
	float TireRadialDampingNsPerM = 2000.f;

	/// Distance from the strut mount at which the suspension exerts no force [cm]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	float RestLength = 50.f;

	/// Minimum spring travel [cm]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	float MinSpringTravel = 10.f;

	/// Maximum spring travel [cm]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	float MaxSpringTravel = 10.f;

	/// Caster angle [deg]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float CasterAngle = 6.0f;

	/// Static camber [deg], positive = top tilted out
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float StaticCamberAngle = -2.0f;

	/// Toe angle [deg], positive = toe-in
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float ToeAngle = 0.2f;

	/// Kingpin angle [deg], typical 5-15, positive = inward tilt
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float KingPinAngle = 10.0f;

	/// Hub lateral offset from wheel center [cm]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	FVector HubOffset = FVector(0.f, 10.f, 0.f);

	/// Self-aligning torque scale (dimensionless)
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true))
	float SelfAligningTorqueScale = 1.f;
	
	// --- Setup (owned components & geometry cached once in BeginPlay/CacheRelativeWheelLocation) ---
	UPROPERTY()
	FShaft WheelShaft;
	FVector RelativeWheelLocation = FVector::ZeroVector;
	float MinSuspensionLength = 0.f;
	float MaxSuspensionLength = 100.f;
	// Steering axis pointing up from wheel center to strut mount in local space
	FVector SteeringAxisLocal = FVector::UpVector;
	// HubOffset with the left/right side sign applied (set in BeginPlay); avoids mutating the EditAnywhere HubOffset.
	FVector SideAdjustedHubOffset = FVector::ZeroVector;

	// --- Per-solve scratch / solver state (rewritten during Solve) ---
	float PhysicsDeltaTime = 0.f;
	float CamberAngleRad = 0.f;
	// Opposite wheel's suspension compression from the previous solve (pushed in each iteration via
	// SetOtherWheelSuspensionCompression); drives the anti-roll term in ComputeSuspensionForce.
	// Initialized to 0 because the first solve reads it before the first setter call. [cm]
	float OtherWheelSuspensionCompression = 0.f;
	float NormalForceUU = 0.f;
	float CurrentSteerAngle = 0.f;
	FQuat RelativeWheelRotation = FQuat::Identity;
	float TargetSteerAngleDeg = 0.f;
	FTransientTireState TransientState;
	float SolverSuspensionLength = 0.f;
	float SolverPreviousFrameSuspensionLength = 0.f;
	float CurrentRestLength = 0.f;

	// --- Tire compliance state (only meaningful when bUseTireCompliance) ---
	// With compliance off, SolverSuspensionLength is derived from the raycast every
	// solve. With it on, SolverSuspensionLength becomes an integrated state - the
	// unsprung mass's position along the strut axis - and these track the rest.
	// Rate of change of the suspension length [cm/s], positive = extending.
	float UnsprungVelocityCmPerSec = 0.f;
	// Tire radial deflection [cm], positive in compression. Pacejka's rho_z.
	float TyreDeflectionCm = 0.f;
	// Normal load reported by the tire's own spring, (4.E68) [N]. This is what the
	// Magic Formula gets as Fz when compliance is on; the body still gets the
	// suspension force.
	float TyreLoadN = 0.f;
	// Set once the integrated state has been seeded from a settled rest pose.
	bool bUnsprungStateInitialised = false;
	FVector BodyVelocityAtContact = FVector::ZeroVector;
	// Lateral contact velocity (wheel-local +Y) [cm/s]; produced by ComputeTireStateAndForces, consumed by FillContactWheelHit.
	float LateralContactVelocityCmPerSec = 0.f;
	// Per-solve angular-velocity substep count and loop dt; produced by ComputeTireStateAndForces, consumed by FillContactWheelHit.
	int32 SolverSubstepCount = 1;
	float SolverLoopDeltaTime = 0.f;
	FVector SuspensionDirWS = FVector::UpVector;

	// --- Cached output (read by getters / HUD / anim) ---
	FPacejkaResult PacejkaResult;
	FWheelHit CurrentWheelHit;
	float ForwardContactVelocityMPerSec = 0.f;
	// Wheel world position, exposed via GetWheelPosition() for animation [cm]
	FVector WheelWorldPositionCm = FVector::ZeroVector;

	/**
    * Trace the wheel and compute contact/suspension/tire forces for this frame.
    *
    * @param StartLocation       Trace start in world space.
    * @param EndLocation         Trace end in world space.
    * @param BodyInfo            Vehicle body state (Location, Rotation, Velocity, AngularVelocity) — world space.
    * @param OutWheelHit         Output structure filled with forces, contact points and slips.
    */
	void TraceWheelCollider(
		FVector StartLocation,
		FVector EndLocation,
		const FBodyState& BodyInfo,
		FWheelHit& OutWheelHit);

	// Resolves suspension + tire forces once a ground contact is confirmed (the contact branch of TraceWheelCollider).
	void ResolveContact(
		const FHitResult& Hit,
		const FBodyState& BodyInfo,
		FWheelHit& OutWheelHit);

	void BuildNoContactHit(
		const FVector& EndLocation,
		const FBodyState& BodyInfo,
		FWheelHit& OutWheelHit);
	
	void UpdateContactKinematics(
		const FHitResult& Hit,
		const FBodyState& BodyInfo,
		const FQuat& WheelRotation);
	
	void ComputeTireStateAndForces(
		const FQuat& WheelRotation);

	void ApplyTireImpulseToShaft(
		const FVector& TractionForce,
		float LoopDeltaTime);

	void FillContactWheelHit(
		const FHitResult& Hit,
		const FBodyState& BodyInfo,
		const FQuat& WheelRotation,
		FWheelHit& OutWheelHit);

	// Sub-steps the wheel shaft under longitudinal traction (LongDir = unit longitudinal direction in
	// the contact plane) and returns the averaged longitudinal force [UU]. Mutates PacejkaResult,
	// TransientState, and the wheel shaft.
	FVector IntegrateWheelShaftUnderTraction(const FVector& LongDir);

	/**
	* Calculates the wheel rotation in local space.
	* Combines steering (including toe) and camber rotations relative to the body.
	* 
	* @return Wheel rotation in relative to the body.
	*/
	FQuat ComputeRelativeWheelRotation() const;

	/**
	* Updates the suspension length for this frame based on hit distance.
	* Clamps the suspension length within min/max bounds.
	*
	* @param HitDistance  Distance from strut mount to contact point [cm].
	*/
	void UpdateSuspensionLength(float HitDistance);

	/**
	 * Advances the unsprung mass one step along the strut axis, with the suspension
	 * spring above it and the tire's own spring (4.E68) below. Only used when
	 * bUseTireCompliance is on; otherwise the wheel is pinned to the ground and the
	 * tire never deflects.
	 */
	void IntegrateUnsprungMass(float HitDistance);

	/**
	* Calculates the suspension force in world space [kg·cm/s²].
	* Includes spring, damper, and anti-roll contributions.
	*
	* @return Suspension force [kg·cm/s²].
	*/
	float ComputeSuspensionForce() const;
};