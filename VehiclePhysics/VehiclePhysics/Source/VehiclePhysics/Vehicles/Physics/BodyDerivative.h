#pragma once

#include "BodyDerivative.generated.h"

USTRUCT(BlueprintType)
struct FBodyDerivative
{
	GENERATED_USTRUCT_BODY()
	
	FVector dPosition;        // Velocity
	FVector dVelocity;        // Acceleration
	FQuat dRotation;       // Orientation derivative
	FVector dAngularVelocity; // Angular acceleration

	FBodyDerivative operator+(const FBodyDerivative& Other) const
	{
		FBodyDerivative Result;
		Result.dPosition = dPosition + Other.dPosition;
		Result.dVelocity = dVelocity + Other.dVelocity;
		Result.dRotation = dRotation + Other.dRotation;
		Result.dAngularVelocity = dAngularVelocity + Other.dAngularVelocity;
		return Result;
	}

	FBodyDerivative operator*(float Scalar) const
	{
		FBodyDerivative Result;
		Result.dPosition = dPosition * Scalar;
		Result.dVelocity = dVelocity * Scalar;
		Result.dRotation = dRotation * Scalar;
		Result.dAngularVelocity = dAngularVelocity * Scalar;
		return Result;
	}
};