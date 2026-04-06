// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ECU.generated.h"


class UDrivetrain;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UECU : public UActorComponent
{
	GENERATED_BODY()

public:
	UECU();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category="Electronics", meta=(AllowPrivateAccess=true))
	bool bUseTCS = false;

	UPROPERTY(EditAnywhere, Category="Electronics", meta=(AllowPrivateAccess=true))
	float TCSLimit = 0.25f;

	UPROPERTY(EditAnywhere, Category="Electronics", meta=(AllowPrivateAccess=true))
	bool bUseABS = true;

	UPROPERTY(EditAnywhere, Category="Electronics", meta=(AllowPrivateAccess=true))
	float ABSSlipTarget = 0.15f;  // Optimal slip ratio

	UPROPERTY(EditAnywhere, Category="Electronics", meta=(AllowPrivateAccess=true))
	float ABSReduceRate = 10.0f;  // How fast to reduce torque

	UPROPERTY(EditAnywhere, Category="Electronics", meta=(AllowPrivateAccess=true))
	float ABSIncreaseRate = 5.0f;  // How fast to increase torque
	
	TWeakObjectPtr<UDrivetrain> Drivetrain;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	* Applies Traction Control System (TCS) logic to limit wheel slip under acceleration.
	*
	* If TCS is enabled, this function monitors the current longitudinal slip and dynamically
	* adjusts braking and sends a signal to ECU. When the wheel starts slipping,
	* braking force is gradually increased and engine torque is adjusted by ECU.
	* Otherwise, control values are interpolated back toward normal driving conditions.
	*
	* @param bIsSlipping  Is the wheel slipping
	*/
	void HandleTCS(bool bIsSlipping);

	/**
	* Modulates brake torque to prevent wheel lock-up using a simple ABS (Anti-lock Braking System) model.
	*
	* This function dynamically adjusts the applied brake torque based on the difference between the current
	* longitudinal slip and an estimated optimal slip value derived from the tire’s Pacejka response.
	* The modulation keeps the wheel near its peak traction range during braking.
	*
	* @param BrakeTorque        The requested brake torque from the driver or system [N·m].
	* @param LongitudinalSlip   The current slip ratio of the wheel (dimensionless).
	* @param RelativeVelocity   The relative tangential velocity of the tire contact patch [m/s].
	*
	* @return The adjusted brake torque value after ABS modulation [N·m].
	*
	* Behavior:
	* - If ABS is disabled or no braking is requested, returns the input BrakeTorque directly.
	* - Reduces torque when slip exceeds the optimal range (to prevent lock-up).
	* - Increases torque when slip is too low (to maximize braking force).
	* - Smoothly interpolates torque changes using different rise/fall rates.
	*
	* Performance:
	* - Runs once per wheel per physics tick.
	* - Computationally lightweight (no heavy math or branching).
	*/
	void ApplyABS(float BrakeTorque, float LongitudinalSlip, float RelativeVelocity);
	
	/**
	* Calculates the brake torque applied to the wheel.
	* Torque opposes wheel rotation or, if stationary, the forward velocity at the contact point.
	*
	* @param AngularVelocity  Wheel's angular velocity [rad/s].
	* @return Brake torque applied to the wheel [N/m].
	*/
	void ApplyBrake(float BrakeAmount) const;
};
