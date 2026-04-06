// Fill out your copyright notice in the Description page of Project Settings.


#include "Differential.h"

#include "Clutch.h"
#include "DrawDebugHelpers.h"


UDifferential::UDifferential()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDifferential::BeginPlay()
{
	Super::BeginPlay();
}

void UDifferential::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	return;

	float LTorque = Torque * 0.5f;
	float RTorque = Torque * 0.5f;

	// Get estimated max traction torque for each wheel (from previous frame or WheelCollider data)
	// Assuming you add getters to UWheelCollider for these (see below for details)
	//float RightTractionTorque = RightWheel->GetTractionTorque();
	//float LeftTractionTorque = LeftWheel->GetTractionTorque();
	float RightTractionTorque = 0.f;
	float LeftTractionTorque = 0.f;  
	float DeltaOmega = LeftWheel->GetAngularVelocityRad() - RightWheel->GetAngularVelocityRad();

	if(bIsLocked)
	{
		// Combine wheel inertias
		float TotalInertia = LeftWheel->GetShaft()->GetInertia() + RightWheel->GetShaft()->GetInertia();

		// Input impulse from drivetrain (after final drive ratio)
		float InputImpulse = Torque * FinalDriveRatio * DeltaTime;

		// Traction impulses (oppose rotation, capped by grip)
		LeftTractionTorque = FMath::Min(LTorque, LeftTractionTorque);
		RightTractionTorque = FMath::Min(RTorque, RightTractionTorque);
		float LeftTractionImpulse = -LeftTractionTorque * DeltaTime; // Negative: opposes input
		float RightTractionImpulse = -RightTractionTorque * DeltaTime;

		// Total impulse: Input - traction losses
		float TotalImpulse = InputImpulse + LeftTractionImpulse + RightTractionImpulse;

		// Previous shared velocity (assume wheels are synced)
		float PrevAngularVelocity = LeftWheel->GetShaft()->GetAngularVelocity();

		// New shared angular velocity: ω = ω_prev + Δω, where Δω = TotalImpulse / TotalInertia
		float SharedAngularVelocity = PrevAngularVelocity + TotalImpulse / TotalInertia;
		SharedAngularVelocity = FMath::Max(SharedAngularVelocity, 0.0f); // Prevent negative rotation

		// Set torques (equal, capped by traction)
		LTorque = LeftTractionTorque;
		RTorque = RightTractionTorque;

		// Apply shared velocity to both wheels
		//LeftWheel->GetShaft()->AngularVelocity = SharedAngularVelocity;
		//RightWheel->GetShaft()->AngularVelocity = SharedAngularVelocity;
	}
	
	if(bIsTorsen && DeltaOmega != 0.0f)
	{
		// Infer which wheel has lower traction (faster = slipping = low traction)
		bool bLeftIsLowTraction = (LeftTractionTorque < RightTractionTorque);  // Left faster -> left low traction
		float LowTraction = bLeftIsLowTraction ? LeftTractionTorque : RightTractionTorque;
		float HighTraction = bLeftIsLowTraction ? RightTractionTorque : LeftTractionTorque;

		// Dynamic bias based on slip amount
		float Alpha = FMath::Clamp(FMath::Abs(DeltaOmega/TorsenSlipLimit), 0.f, 1.f);
		float DynamicBias = FMath::Lerp(1.f, BiasRatio, Alpha);

		// Ideal Torsen split (high gets more)
		float LowTorque = Torque / (DynamicBias + 1.f);  // e.g., for Bias=3, low=Torque/4
		float HighTorque = Torque - LowTorque;           // high=Torque*Bias/(Bias+1), e.g., 3Torque/4


		// Limit low torque by its traction (can't apply more than wheel can handle)
		LowTorque = FMath::Min(LowTorque, LowTraction);

		// High torque = up to Bias * low torque, clamped by its own traction
		HighTorque = FMath::Min(DynamicBias * LowTorque, HighTraction);

		// Handle zero traction with preload
		if (LowTraction <= 0.0f)
		{
			// Apply preload: hanging wheel gets most, grounded gets some
			LowTorque = TorsenPreLoad; // Hanging wheel spins
			HighTorque = FMath::Min(DynamicBias * TorsenPreLoad, HighTraction); // Grounded wheel gets limited torque
		}

		// Total torque can't exceed input (scale if needed)
		float TotalApplied = LowTorque + HighTorque;
		if (TotalApplied > Torque)
		{
			float Scale = Torque / TotalApplied;
			LowTorque *= Scale;
			HighTorque *= Scale;
		}

		// Assign to wheels
		if (bLeftIsLowTraction)
		{
			LTorque = LowTorque;
			RTorque = HighTorque;
		}
		else
		{
			LTorque = HighTorque;
			RTorque = LowTorque;
		}
			
	}
	
	float ClutchStrength = 0.5f; // Dynamic locking (0-1, scales with input torque)
    float SlipLimit = TorsenSlipLimit; // Reuse for wheel speed diff sensitivity

    if (bIsLimitedSlip)
    {
        // Check for zero traction case
    	if(LeftTractionTorque <= 0.f && RightTractionTorque <= 0.f)
    	{
    		LTorque = Torque * 0.5f;
    		RTorque = Torque * 0.5f;
    	}
        else if (LeftTractionTorque <= 0.0f)
        {
            LTorque = FMath::Min(Torque, TorsenPreLoad); // Hanging wheel spins
            RTorque = FMath::Min(Torque - LTorque, RightTractionTorque); // Grounded wheel gets rest
        }
        else if (RightTractionTorque <= 0.0f)
        {
            RTorque = FMath::Min(Torque, TorsenPreLoad); // Hanging wheel spins
            LTorque = FMath::Min(Torque - RTorque, LeftTractionTorque); // Grounded wheel gets rest
        }
        else
        {
            // Clutch-type LSD: Lock based on speed diff and input torque
            float SlipFactor = FMath::Clamp(FMath::Abs(DeltaOmega / SlipLimit), 0.f, 1.f);

            // Locking torque: Preload + dynamic (scales with input torque)
            float LockTorque = TorsenPreLoad + ClutchStrength * Torque * SlipFactor;

            // Determine which wheel is slipping (faster wheel)
            bool bLeftIsSlipping = (DeltaOmega > 0.0f);
            float& SlippingTorque = bLeftIsSlipping ? LTorque : RTorque;
            float& GrippingTorque = bLeftIsSlipping ? RTorque : LTorque;
            float SlippingMaxTraction = bLeftIsSlipping ? LeftTractionTorque : RightTractionTorque;
            float GrippingMaxTraction = bLeftIsSlipping ? RightTractionTorque : LeftTractionTorque;

            // Reduce slipping wheel torque, increase gripping wheel
            SlippingTorque = FMath::Min(Torque * 0.5f - LockTorque, SlippingMaxTraction);
            GrippingTorque = FMath::Min(Torque * 0.5f + LockTorque, GrippingMaxTraction);

            // Total torque can't exceed input
            float TotalApplied = LTorque + RTorque;
            if (TotalApplied > Torque)
            {
                float Scale = Torque / TotalApplied;
                LTorque *= Scale;
                RTorque *= Scale;
            }
        }
    }

	//ApplyFinalDriveRatio
	LTorque *= FinalDriveRatio;
	RTorque *= FinalDriveRatio;



	//Set to wheels
	//RightWheel->Torque = RTorque;
	//LeftWheel->Torque = LTorque;
}