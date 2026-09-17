#pragma once

#include "WheelSolverData.generated.h"

USTRUCT(BlueprintType)
struct FWheelSolverData
{
	GENERATED_USTRUCT_BODY()

	float LongitudinalSlip = 0.f;
	float LateralSlip = 0.f;
	float SuspensionLength = 0.f;
	float LastSuspensionLength = 0.f;
	float AngularVelocity = 0.f;
	float AngularAcceleration = 0.f;

	FWheelSolverData operator+(const FWheelSolverData& Other) const
	{
		FWheelSolverData C;
		C.LongitudinalSlip = LongitudinalSlip + Other.LongitudinalSlip;
		C.LateralSlip = LateralSlip + Other.LateralSlip;
		C.SuspensionLength = SuspensionLength + Other.SuspensionLength;
		C.LastSuspensionLength = LastSuspensionLength + Other.LastSuspensionLength;
		C.AngularVelocity = AngularVelocity + Other.AngularVelocity;
		C.AngularAcceleration = AngularAcceleration + Other.AngularAcceleration;
		return C;
	}

	FWheelSolverData operator*(float B) const
	{
		FWheelSolverData C;
		C.LongitudinalSlip = LongitudinalSlip * B;
		C.LateralSlip = LateralSlip * B;
		C.SuspensionLength = SuspensionLength * B;
		C.LastSuspensionLength = LastSuspensionLength * B;
		C.AngularVelocity = AngularVelocity * B;
		C.AngularAcceleration = AngularAcceleration * B;
		return C;
	}

	FWheelSolverData operator/(float B) const
	{
		FWheelSolverData C;
		C.LongitudinalSlip = LongitudinalSlip / B;
		C.LateralSlip = LateralSlip / B;
		C.SuspensionLength = SuspensionLength / B;
		C.LastSuspensionLength = LastSuspensionLength / B;
		C.AngularVelocity = AngularVelocity / B;
		C.AngularAcceleration = AngularAcceleration / B;
		return C;
	}
	
};
