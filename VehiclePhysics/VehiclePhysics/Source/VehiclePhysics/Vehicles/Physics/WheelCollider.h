// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shaft.h"
#include "PacejkaResult.h"
#include "WheelHit.h"
#include "WheelSolverData.h"
#include "TransientState.h"
#include "Engine/EngineTypes.h"  // For ELevelTick
#include "PhysicsEngine/BodyInstance.h"
#include "Components/SceneComponent.h"
#include "WheelCollider.generated.h"

struct FBodyState;
class UDrivetrain;
class UTireParamsDataAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UWheelCollider : public USceneComponent
{
	GENERATED_BODY()

#define TO_UNREAL_UNIT 100.f
#define TO_SI 0.01f
#define TO_SIPO2 0.0001f
#define MPSTOKMPH 3.6f

public:
	UWheelCollider();
	
	void Initialize(FVector BodyLocation, FQuat BodyRotation);

	/**
	* Per-frame update for the wheel collider.
	*
	* Handles:
	*  - Steering interpolation (visual & logical)
	*  - Audio feedback for slip (pitch/volume)
	*  - Skidmark mesh generation and updates
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
 	* @param Substeps       Number of sub-iterations to improve solver stability (e.g., for drivetrain substepping).
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
		int32 Substeps,
		UDrivetrain* Drivetrain);

	/*FORCEINLINE void SetSolverData(FWheelSolverData SolverData)
	{
		SolverLongitudinalSlip = SolverData.LongitudinalSlip;
		SolverLateralSlip = SolverData.LateralSlip;
		WheelShaft.AngularVelocity = SolverData.AngularVelocity;
		SolverSuspensionLenght = SolverData.SuspensionLenght;
		SolverLastSuspensionLenght = SolverData.LastSuspensionLenght;
	}*/
	float CamberAngle = 0.f;

	FORCEINLINE FRotator GetWheelRotLocal() const{ return  RelativeWheelRotation.Rotator(); }
	FORCEINLINE float GetWheelRadius() const { return WheelRadius; }
	FORCEINLINE float GetRestLenght() const { return RestLenght; }
	FORCEINLINE float GetWheelLoad() const { return 0.f; }
	FORCEINLINE float GetAngularVelocityRad() const { return WheelShaft.GetAngularVelocity(); }
	FORCEINLINE float GetAngularVelocityInDegrees() const { return FMath::RadiansToDegrees(GetAngularVelocityRad()); }
	FORCEINLINE float GetWheelZPosition() const{
		#if WITH_EDITOR
		if(GIsEditor)
		{
			if(GetWorld() && !GetWorld()->IsGameWorld())
				return 0.f;
			return  RestLenght - CurrentWheelHit.SuspensionLength;
		}
#endif
		return  RestLenght - CurrentWheelHit.SuspensionLength;}
	FORCEINLINE FVector GetWheelPosition() { return Pos;}
private:
		FORCEINLINE FVector GetWheelPositionInt() const{
        		FVector Position = CurrentWheelHit.SuspensionForceApplication - 
        			SuspensionDirWS * SolverSuspensionLength;
        		return  Position;}
public:
	FORCEINLINE float GetCurrentWheelAngle() const {return CurrentSteerAngle; }
	FORCEINLINE void SetOtherWheelSuspensionCompression(float Compression) {OtherWheelSuspensionCompression = Compression; }
	FORCEINLINE FWheelHit GetCurrentWheelHit() const { return CurrentWheelHit; }
	FORCEINLINE UTireParamsDataAsset* GetParams() const { return TireParams; }
	FORCEINLINE FShaft* GetShaft() { return &WheelShaft; }
	FORCEINLINE FPacejkaResult GetPacejkaResult() const { return PacejkaResult; }
	FVector Pos = FVector::ZeroVector;
	/**
	* Integrates all per-wheel forces and torques into the physics body.
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
	void Integrate(FWheelHit& WheelHit, FBodyInstance* BodyInstance);

	FTransientTireState TransientState;
	float SteerInput = 0.f;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Wheel", meta = (AllowPrivateAccess = true))
	UTireParamsDataAsset* TireParams;
	
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true, Units="cm"))
	float WheelRadius = 35.f;

	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	float SteeringResponsiveness = 10.f;
	
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	int AngVelItersSlip = 10;

	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	int AngVelItersNoSlip = 2;
	
	
	/// Spring stiffness [N/m]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="Newtons"))
	float SpringStiffness_NM = 50000.f;

	/// Damper coefficient [N·s/m]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true))
	float DamperCoefficient = 2500.f;

	/// Anti-roll bar stiffness [N/m]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true))
	float AntiRollStiffness = 10000.f;

	/// Distance from the strut mount at which the suspension exerts no force [cm]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	float RestLenght = 50.f;

	/// Maximum compression of suspension [cm]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	float MinSpringTravel = 10.f;

	/// Maximum compression of suspension [cm]
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	float MaxSpringTravel = 10.f;

	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float CasterAngle = 6.0f;
	
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float StaticCamberAngle = -2.0f; /// Degrees, positive = top tilted out
	
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float ToeAngle = 0.2f; // Degrees, positive = toe-in

	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="deg"))
	float KingPinAngle = 10.0f; // Degrees, typical 5-15, positive inward tilt
	
	UPROPERTY(EditAnywhere, Category="Suspension", meta=(AllowPrivateAccess=true, Units="cm"))
	FVector HubOffset = FVector(0.f, 10.f, 0.f);
	
	UPROPERTY(EditAnywhere, Category="Electronics", meta=(AllowPrivateAccess=true))
	float SATC = 1.f;

	UPROPERTY(EditAnywhere,Category="Debug", meta=(AllowPrivateAccess=true))
	bool bDebug = false;
	
	
	UPROPERTY()
	FShaft WheelShaft;
	FPacejkaResult PacejkaResult;
	FVector RelativeWheelLocation;
	FWheelHit CurrentWheelHit;
	float PhysicsDeltaTime = 0.f;
	float MinSuspensionLength;
	float MaxSuspensionLength;
	float OtherWheelSuspensionCompression;
	// Steering axis pointing up from wheel center to strut mount in local space
	FVector SteeringAxisLocal;
	// Strut mount location
	FVector CasterOffsetLocal;
	float NormalForce;
	int32 StepCount = 1;
	float MechanicalTrail;
	float CurrentSteerAngle = 0.f;
	FQuat RelativeWheelRotation = FQuat::Identity;
public:
	float SolverWheelForwardVelocityAtContact;
private:
	float SolverSuspensionLength = 0.f;
	float SolverPreviousFrameSuspensionLength = 0.f;
	float CurrentRestLenght = 0.f;
	FVector BodyVelocityAtContact;
	//float LateralVelocity;
	FVector SuspensionDirWS = FVector::UpVector;

	/**
    * Trace the wheel and compute contact/suspension/tire forces for this frame.
    *
    * @param StartLocation       Trace start in world space.
    * @param EndLocation         Trace end in world space.
    * @param BodyInfo            Vehicle body state (Location, Rotation, Velocity, AngularVelocity) — world space.
    * @param OutWheelHit         Output structure filled with forces, contact points and slips.
    * @param Drivetrain          Drivetrain pointer (may be null if no drive).
    */
	void TraceWheelCollider(FVector StartLocation, FVector EndLocation,
	                        const FBodyState& BodyInfo, FWheelHit& OutWheelHit, UDrivetrain* Drivetrain);
	
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
	* Calculates the suspension force in world space [kg·cm/s²].
	* Includes spring, damper, and anti-roll contributions.
	*
	* @return Suspension force [kg·cm/s²].
	*/
	float ComputeSuspensionForce() const;

public:
	FORCEINLINE FWheelSolverData GetSolverData() const
	{
		FWheelSolverData SolverData;
		SolverData.AngularVelocity = WheelShaft.GetAngularVelocity();
		//SolverData.LateralSlip = SolverLateralSlip;
		//SolverData.LongitudinalSlip = SolverLongitudinalSlip;
		SolverData.SuspensionLenght = SolverSuspensionLength;
		SolverData.LastSuspensionLenght = SolverPreviousFrameSuspensionLength;
		return SolverData;
	}
};