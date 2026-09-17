// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "Shaft.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "VehicleEngine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class VEHICLEPHYSICS_API UVehicleEngine : public UActorComponent
{
	GENERATED_BODY()

public:
	UVehicleEngine();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetThrottle(float InputThrottle) {Throttle = InputThrottle;}
	FORCEINLINE float GetTorque() const {return FMath::Clamp(Torque, 0.f, FLT_MAX);}
	FORCEINLINE float GetThrottle() const {return Throttle;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Engine")
	float CurrentEngineRPM = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Engine")
	float RedLineRPMRatio = 0.95f;
	
	FORCEINLINE FShaft* GetShaft() {return &Shaft;}
	float NetTorque;

	FORCEINLINE void ApplyCurrentImpulse(float DeltaTime)
	{
		Shaft.ApplyImpulse(NetTorque * DeltaTime);
	}

	FORCEINLINE float GetCurrentTorque() const {return NetTorque;}
	FORCEINLINE float GetIdleRPM() const {return IdleRPM;}

	/**
	 * Samples the shaft, then works out the torque the engine makes at THAT speed.
	 *
	 * THE ONLY PLACE TORQUE IS COMPUTED, and it must be called once per physics
	 * substep by whoever owns the shaft. It used to happen in TickComponent, once a
	 * frame, from an RPM read at the END of the previous tick - so ACar applied one
	 * stale value across every substep of the frame. At MaxSubstepDeltaTime of 5 ms
	 * that is three or four substeps driven by the torque for a speed the engine no
	 * longer has.
	 *
	 * That lag was invisible while the engine was rigidly geared to the driveline:
	 * its effective inertia was the whole driveline's and the tyres damped
	 * everything. The clutch decouples it. A slipping plate leaves the engine a
	 * small, lightly damped mass driven by a torque that depends on its own speed,
	 * one frame late - which is how you build an oscillator, not a powertrain.
	 */
	void UpdateTorque();
protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float MaxRPM = 8000.f;
	
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float IdleRPM = 800.f;

	/**
	 * Torque the idle governor adds at a standstill, N*m.
	 *
	 * An engine does not hold idle on its own - friction would stop it. The ECU
	 * meters in enough air to balance the losses, and this is that torque. Roughly
	 * 10-20% of peak is typical; too little and it will not catch itself, too much
	 * and the car creeps hard against the brakes.
	 */
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true, ClampMin = "0.0"))
	float IdleGovernorTorque = 40.f;

	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	UCurveFloat* TorqueCurve;

	/**
	 * Torque curve carried BY VALUE, used whenever TorqueCurve is unset.
	 *
	 * Exists because a UCurveFloat created in a constructor cannot be referenced by a
	 * saved Blueprint. It lives in the class's own package, so a Blueprint deriving
	 * from that class ends up pointing at a private object in an external package, and
	 * the editor refuses to save it - "Graph is linked to private object(s) in an
	 * external package" - and then crashes. That is exactly what happened, and it is
	 * not fixable by naming or flagging the object: an asset reference has to point at
	 * an asset.
	 *
	 * An inline FRuntimeFloatCurve has no such problem. It serialises by value with
	 * whatever owns it, so a class can define a curve in code, a Blueprint deriving
	 * from it saves cleanly, and the curve is still editable per instance in the
	 * details panel.
	 *
	 * TorqueCurve wins when both are set, so assigning an asset still overrides a
	 * class-defined curve rather than being ignored.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Engine",
		meta = (AllowPrivateAccess = "true"))
	FRuntimeFloatCurve InlineTorqueCurve;
	
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float MaxEngineTorque = 250.f;

	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float EngineBrakingCoefficient = 0.f;

	/**
	 * Engine braking multiplier at MaxRPM, ramped in from RedLineRPMRatio upward.
	 *
	 * Lets the engine lean on the limiter instead of arriving at it. Ramped across
	 * the band rather than stepped at its edge - a sudden multiple of the braking
	 * term, inside a loop that feeds back through engine speed, is a shove rather
	 * than a limit.
	 */
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true, ClampMin = "1.0"))
	float RedlineBrakingMultiplier = 10.f;

private:
	UPROPERTY(EditAnywhere, Category="Engine",meta=(AllowPrivateAccess=true))
	FShaft Shaft;

	float Throttle = 0.f;
	float Torque = 0.f;
	bool bIsBurning = false;

};