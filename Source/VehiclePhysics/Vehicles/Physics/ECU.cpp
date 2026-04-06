// Fill out your copyright notice in the Description page of Project Settings.


#include "ECU.h"
#include "Drivetrain.h"
#include "Connection.h"


// Sets default values for this component's properties
UECU::UECU()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UECU::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UECU::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UECU::ApplyBrake(float BrakeAmount) const
{
	for (FConnection* C : Drivetrain->BrakeConnections)
	{
		C->ConnectionStrength = BrakeAmount;
	}
}

/*void UECU::HandleTCS(bool bIsSlipping)
{
	// If TCS is active, reduce wheel torque and apply braking when slip exceeds the allowed limit.
	if (!bUseTCS)
		return;
	
	if(bIsSlipping)
	{
		// Slip too high: smoothly apply braking and cut torque.
		BrakeAmount = FMath::FInterpTo(BrakeAmount, 1.f, PhysicsDeltaTime, 10.f);
		TCSTorqueControl = FMath::FInterpTo(TCSTorqueControl, 0.f, PhysicsDeltaTime, 40.f);
	}
	else
	{
		// Within traction limit: release brakes and restore full torque.
		BrakeAmount = 0.f;
		TCSTorqueControl = FMath::FInterpTo(TCSTorqueControl, 1.f, PhysicsDeltaTime, 4.f);
		TCSTorqueControl = 1.f;
	}
}

float UECU::ApplyABS(
	float BrakeTorque,
	float LongitudinalSlip,
	float RelativeVelocity)
{
	if (!bUseABS || BrakeAmount <= 0.0f)
	{
		return BrakeTorque;  // No ABS if disabled or no braking
	}

	// ABS logic: Modulate brake torque to keep slip ratio near ABSSlipTarget (e.g., 0.15)
	const float AbsSlip = FMath::Abs(LongitudinalSlip);
	const float Kappa = PacejkaResult.Kappa;
	const float SignKappa = FMath::Sign(Kappa);

	// Estimate target slip based on whether wheel is near peak grip
	const float TargetSlip = (FMath::Abs(PacejkaResult.Ux) < 0.9f * FMath::Abs(PacejkaResult.Ux0))
		? Kappa - SignKappa * 0.01f
		: Kappa + SignKappa * 0.01f; // Optimal slip for peak grip

	// Start from current torque (saves one assignment)
	float TargetBrake = FMath::Abs(CurrentBrakeTorque);
	
	// Check if wheel is locking or slipping too much
	if (RelativeVelocity < 0.f && AbsSlip > TargetSlip)  // Too much slip (e.g., > 0.2)
	{
		if (AbsSlip > TargetSlip)
		{
			// Too much slip: ease off the brakes
			TargetBrake -= WheelBrakeTorque * 0.1f;
		}
		// Reduce brake torque to lower slip
		TargetBrake -= CurrentBrakeTorque - WheelBrakeTorque * 0.1f;  // Reduce by 20%
	}
	else if (AbsSlip < TargetSlip - 0.05f && TargetBrake <= WheelBrakeTorque)  // Too little slip (e.g., < 0.1)
	{
		// Increase brake torque to use more grip
		TargetBrake = CurrentBrakeTorque + WheelBrakeTorque * 0.1f;  // Increase by 20%
	}

	// Smooth modulation toward target
	const float Rate = (AbsSlip > TargetSlip) ? ABSReduceRate : ABSIncreaseRate;
	CurrentBrakeTorque = FMath::FInterpTo(CurrentBrakeTorque, TargetBrake, PhysicsDeltaTime, Rate);

	// Keep within sane limits
	CurrentBrakeTorque = FMath::Clamp(CurrentBrakeTorque, 0.0f, WheelBrakeTorque);

	return CurrentBrakeTorque * FMath::Sign(BrakeTorque);
}*/