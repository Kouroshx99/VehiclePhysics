#pragma once

#include "WheelSolverData.generated.h"

USTRUCT(BlueprintType)
struct FWheelSolverData
{
	GENERATED_USTRUCT_BODY()

	float LongitudinalSlip = 0.f;
	float LateralSlip = 0.f;
	float SuspensionLenght = 0.f;
	float LastSuspensionLenght = 0.f;
	float AngularVelocity = 0.f;
	float AngularAcceleration = 0.f;

	FWheelSolverData operator+(const FWheelSolverData& Other) const
	{
		FWheelSolverData C;
		C.LongitudinalSlip = LongitudinalSlip + Other.LongitudinalSlip;
		C.LateralSlip = LateralSlip + Other.LateralSlip;
		C.SuspensionLenght = SuspensionLenght + Other.SuspensionLenght;
		C.LastSuspensionLenght = LastSuspensionLenght + Other.LastSuspensionLenght;
		C.AngularVelocity = AngularVelocity + Other.AngularVelocity;
		C.AngularAcceleration = AngularAcceleration + Other.AngularAcceleration;
		return C;
	}

	FWheelSolverData operator*(float B) const
	{
		FWheelSolverData C;
		C.LongitudinalSlip = LongitudinalSlip * B;
		C.LateralSlip = LateralSlip * B;
		C.SuspensionLenght = SuspensionLenght * B;
		C.LastSuspensionLenght = LastSuspensionLenght * B;
		C.AngularVelocity = AngularVelocity * B;
		C.AngularAcceleration = AngularAcceleration * B;
		return C;
	}

	FWheelSolverData operator/(float B) const
	{
		FWheelSolverData C;
		C.LongitudinalSlip = LongitudinalSlip / B;
		C.LateralSlip = LateralSlip / B;
		C.SuspensionLenght = SuspensionLenght / B;
		C.LastSuspensionLenght = LastSuspensionLenght / B;
		C.AngularVelocity = AngularVelocity / B;
		C.AngularAcceleration = AngularAcceleration / B;
		return C;
	}
	
};
