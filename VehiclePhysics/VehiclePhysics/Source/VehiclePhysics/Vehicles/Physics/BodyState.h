#pragma once
#include "BodyDerivative.h"

#include "BodyState.generated.h"

USTRUCT(BlueprintType)
struct FBodyState
{
	GENERATED_USTRUCT_BODY()

	FBodyState() 
		: Location(FVector::ZeroVector), Rotation(FQuat::Identity), Velocity(FVector::ZeroVector), AngularVelocity(FVector::ZeroVector) {}

	FBodyState(FVector InLocation, FQuat InRotation, FVector InVelocity, FVector InAngularVelocity) :
	Location(InLocation), Rotation(InRotation), Velocity(InVelocity), AngularVelocity(InAngularVelocity){}

	/// World Location
	FVector Location = FVector::ZeroVector;

	/// Rotation of the body in world space
	FQuat Rotation = FQuat::Identity;

	
	FVector Velocity = FVector::ZeroVector;
	FVector AngularVelocity = FVector::ZeroVector;

	FBodyState operator+(const FBodyDerivative& Deriv) const
	{
		FBodyState Result;
		Result.Location = Location + Deriv.dPosition;
		Result.Velocity = Velocity + Deriv.dVelocity;
		Result.Rotation = Rotation + Deriv.dRotation;
		Result.Rotation.Normalize();
		Result.AngularVelocity = AngularVelocity + Deriv.dAngularVelocity;
		return Result;
	}

	FBodyState& operator+=(const FBodyDerivative& Deriv)
	{
		Location += Deriv.dPosition;
		Velocity += Deriv.dVelocity;
		Rotation += Deriv.dRotation;
		Rotation.Normalize();
		AngularVelocity += Deriv.dAngularVelocity;
		return *this;
	}

	FBodyState operator*(float Scalar) const
	{
		FBodyState Result;
		Result.Location = Location * Scalar;
		Result.Velocity = Velocity * Scalar;
		// Quaternion multiplication by scalar is not standard, handle with care.
		// For simplicity, this example scales the rotation derivative, which may not be physically accurate.
		Result.Rotation = FQuat::Slerp(FQuat::Identity, Rotation, Scalar);
		Result.AngularVelocity = AngularVelocity * Scalar;
		return Result;
	}
	
};