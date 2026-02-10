#pragma once

#include "WheelHit.generated.h"

USTRUCT(BlueprintType)
struct FWheelHit
{
	GENERATED_USTRUCT_BODY()

	FWheelHit() {} // Default constructor is needed

	FWheelHit(float InAngularVelocity, float InSuspensionLength, float InLastSuspensionLength,
		FVector InSuspensionForce, FVector InSuspensionForceApplication,
		FVector InLongitudinalForce, FVector InLongitudinalForceApplication,
		FVector InLateralForce, FVector InLateralForceApplication,
		FVector InSelfAligningTorque, FVector InBodyVelocityAtContact,
		float InLongitudinalSlip,
		float InLateralSlip,
		float InTractionImpulse,
		float InMz,
		bool InIsInAir)
		: AngularVelocity(InAngularVelocity), SuspensionLength(InSuspensionLength),
		LastSuspensionLength(InLastSuspensionLength), SuspensionForce(InSuspensionForce),
		SuspensionForceApplication(InSuspensionForceApplication), LongitudinalForce(InLongitudinalForce),
		LongitudinalForceApplication(InLongitudinalForceApplication), LateralForce(InLateralForce),
		LateralForceApplication(InLateralForceApplication), SelfAligningTorque(InSelfAligningTorque),
		BodyVelocityAtContact(InBodyVelocityAtContact),
		LongitudinalSlip(InLongitudinalSlip), LateralSlip(InLateralSlip)
	, TractionImpulse(InTractionImpulse), Mz(InMz), bIsInAir(InIsInAir){}

	
	float AngularVelocity = 0.f;
	//Cashed variable
	float SuspensionLength = 0.f;
	//Cashed variable
	float LastSuspensionLength = 0.f;
	FVector SuspensionForce = FVector::ZeroVector;
	FVector SuspensionForceApplication = FVector::ZeroVector;
	FVector LongitudinalForce = FVector::ZeroVector;
	FVector LongitudinalForceApplication = FVector::ZeroVector;
	FVector LateralForce = FVector::ZeroVector;
	FVector LateralForceApplication = FVector::ZeroVector;
	FVector SelfAligningTorque = FVector::ZeroVector;
	FVector BodyVelocityAtContact = FVector::ZeroVector;
	float LongitudinalSlip = 0.f;
	float LateralSlip = 0.f;
	float Brx = 0.f;
	float Bry = 0.f;
	float TractionImpulse = 0.f;
	float Mz = 0.f;
	bool bIsInAir = false;

	FVector CalculateTotalForce() const
	{
		return LongitudinalForce + LateralForce + SuspensionForce;
	}

	FVector CalculateTotalTorque(FVector ReferencePosition) const
	{
		FVector TotalTorque = FVector::CrossProduct(SuspensionForceApplication - ReferencePosition, SuspensionForce); //Torque from suspension
		TotalTorque += FVector::CrossProduct(LongitudinalForceApplication - ReferencePosition, LongitudinalForce); //Torque from longitudinal force
		TotalTorque += FVector::CrossProduct(LateralForceApplication - ReferencePosition, LateralForce); //Torque from lateral force
		TotalTorque += SelfAligningTorque; //Torque from self alignment torque
		return TotalTorque;
	}

	FWheelHit operator+(const FWheelHit& Other) const
	{
		return FWheelHit(
			AngularVelocity + Other.AngularVelocity,
			SuspensionLength + Other.SuspensionLength,
			LastSuspensionLength + Other.LastSuspensionLength,
			SuspensionForce + Other.SuspensionForce,
			SuspensionForceApplication + Other.SuspensionForceApplication,
			LongitudinalForce + Other.LongitudinalForce,
			LongitudinalForceApplication + Other.LongitudinalForceApplication,
			LateralForce + Other.LateralForce,
			LateralForceApplication + Other.LateralForceApplication,
			SelfAligningTorque + Other.SelfAligningTorque,
			BodyVelocityAtContact + Other.BodyVelocityAtContact,
			LongitudinalSlip + Other.LongitudinalSlip,
			LateralSlip + Other.LateralSlip,
			TractionImpulse + Other.TractionImpulse,
			Mz + Other.Mz,
			bIsInAir || Other.bIsInAir
		);
	}

	FWheelHit& operator+=(const FWheelHit& Other)
	{
		AngularVelocity += Other.AngularVelocity;
		SuspensionLength += Other.SuspensionLength;
		LastSuspensionLength += Other.LastSuspensionLength;
		SuspensionForce += Other.SuspensionForce;
		SuspensionForceApplication += Other.SuspensionForceApplication;
		LongitudinalForce += Other.LongitudinalForce;
		LongitudinalForceApplication += Other.LongitudinalForceApplication;
		LateralForce += Other.LateralForce;
		LateralForceApplication += Other.LateralForceApplication;
		SelfAligningTorque += Other.SelfAligningTorque;
		BodyVelocityAtContact += Other.BodyVelocityAtContact;
		LongitudinalSlip += Other.LongitudinalSlip;
		LateralSlip += Other.LateralSlip;
		Brx += Other.Brx;
		Bry += Other.Bry;
		TractionImpulse += Other.TractionImpulse;
		Mz += Other.Mz;

		return *this;
	}

	// Overloaded division operator (scalar division)
	FWheelHit operator/(float Scalar) const
	{
		if (FMath::IsNearlyZero(Scalar)) return *this; // Prevent division by zero
		Scalar = 1 / Scalar;
		return FWheelHit(
			AngularVelocity * Scalar,
			SuspensionLength * Scalar,
			LastSuspensionLength * Scalar,
			SuspensionForce * Scalar,
			SuspensionForceApplication * Scalar,
			LongitudinalForce * Scalar,
			LongitudinalForceApplication * Scalar,
			LateralForce * Scalar,
			LateralForceApplication * Scalar,
			SelfAligningTorque * Scalar,
			BodyVelocityAtContact * Scalar,
			LongitudinalSlip * Scalar,
			LateralSlip * Scalar,
			TractionImpulse * Scalar,
			Mz * Scalar,
			bIsInAir
		);
	}

	// Overloaded multiplication operator (scalar multiplication)
	FWheelHit operator*(float Scalar) const
	{
		return FWheelHit(
			AngularVelocity * Scalar,
			SuspensionLength * Scalar,
			LastSuspensionLength * Scalar,
			SuspensionForce * Scalar,
			SuspensionForceApplication * Scalar,
			LongitudinalForce * Scalar,
			LongitudinalForceApplication * Scalar,
			LateralForce * Scalar,
			LateralForceApplication * Scalar,
			SelfAligningTorque * Scalar,
			BodyVelocityAtContact * Scalar,
			LongitudinalSlip * Scalar,
			LateralSlip * Scalar,
			TractionImpulse * Scalar,
			Mz * Scalar,
			bIsInAir
		);
	}
};