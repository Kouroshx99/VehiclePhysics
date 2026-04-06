#pragma once

#include "CoreMinimal.h"
#include "Shaft.generated.h"

USTRUCT(BlueprintType)
struct FShaft
{
	GENERATED_USTRUCT_BODY()

	FShaft(){};

public:
	///Rotational inertia (kg*m^2).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DriveShaft", meta = (AllowPrivateAccess = "true"))
	float Inertia = 1.0f;

	///Inverse of rotational inertia (1 / Inertia).
private:
	float InvInertia = -1.0f;
	float AngularVelocity = 0.0f;
	float AccumulatedImpulse = 0.f;

public:
	float Angle = 0.0f;
	//float DeltaTime = 1.f;
	float PredictAngularVelocity(float Impulse) { return AngularVelocity + Impulse / Inertia; }

	//FORCEINLINE void ApplyTorque(float Torque, float DeltaTime)
	//{
	//	AngularVelocity += Torque * DeltaTime /Inertia;
	//	CurrentTorque = Torque;
	//}

	///Applies an impulse (N·m·s) to the driveshaft, changing its angular velocity.
	FORCEINLINE void ApplyImpulse(float Impulse) {
		AngularVelocity += GetInvInertia() * Impulse;
		AccumulatedImpulse += Impulse; // Track total impulse applied
		} // 1/I * IdW = delta W

	/**
	 * Advances the rotation of the driveshaft over time, based on current angular velocity.
	 * @param DeltaTime Time step in seconds.
	 */
	//FORCEINLINE void UpdateAngle(float DeltaTime) { Angle += AngularVelocity * DeltaTime; }
	//FORCEINLINE float GetNetTorque()
	//{
	//	return AccumulatedImpulse / DeltaTime;
	//}
	FORCEINLINE void ResetAccumImpulse() {	AccumulatedImpulse = 0.f; }

	FORCEINLINE float GetInertia() const {return Inertia;}
	FORCEINLINE void SetInertia(float InInertia)
	{
		Inertia = InInertia;
		InvInertia = 1.0f / InInertia;
	}
	FORCEINLINE float GetInvInertia()
	{
		if (InvInertia == -1.f)
			InvInertia = 1.f / Inertia;
		return InvInertia;
	}
	FORCEINLINE float GetAngularVelocity() const {return AngularVelocity;}
	FORCEINLINE void SetAngularVelocity(float InAngularVelocity) {AngularVelocity = InAngularVelocity;}

	float CurrentTorque = 0.f;
};