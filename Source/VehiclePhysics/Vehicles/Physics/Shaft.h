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

private:
	float AngularVelocity = 0.0f;
	float AccumulatedImpulse = 0.f;

	/**
	 * Inertia the tyre lends this shaft for the duration of one substep, kg*m^2.
	 *
	 * A tyre carcass resists having its slip changed, and over a step that
	 * resistance behaves exactly as inertia on the wheel - see the implicit note in
	 * UPacejkaTyreComponent::SolveSubstep, where it is derived and set.
	 *
	 * It lives HERE, in the shaft, rather than being folded into the tyre's own
	 * impulse, because every torque on this shaft has to see it. Dividing only the
	 * tyre's impulse while the engine and the drivetrain constraints stay at full
	 * strength lets the wheel spin up unopposed - worst at low speed, where the
	 * added inertia is largest, which is why the car could not pull away at all
	 * with the transient model off.
	 *
	 * NOT a UPROPERTY: it is rebuilt from the current operating point every substep
	 * and means nothing outside one.
	 */
	float AddedInertia = 0.f;

public:
	float Angle = 0.0f;
	//float DeltaTime = 1.f;
	// Effective, not raw - an impulse applied for real divides by the same thing.
	// Currently unused; left consistent so it cannot become a subtle bug later.
	float PredictAngularVelocity(float Impulse) const { return AngularVelocity + (Impulse * GetInvInertia()); }

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

	/** The shaft's OWN inertia, without anything the tyre is lending it. */
	FORCEINLINE float GetInertia() const {return Inertia;}
	FORCEINLINE void SetInertia(float InInertia) { Inertia = InInertia; }

	FORCEINLINE void SetAddedInertia(float InAddedInertia)
	{
		AddedInertia = FMath::Max(0.f, InAddedInertia);
	}
	FORCEINLINE float GetAddedInertia() const { return AddedInertia; }
	FORCEINLINE float GetEffectiveInertia() const { return Inertia + AddedInertia; }

	/**
	 * Inverse of the EFFECTIVE inertia, which is what every impulse divides by.
	 *
	 * No longer cached. AddedInertia is rebuilt each substep from the current
	 * operating point, so a stored inverse would be a step stale on every shaft a
	 * tyre touches - and stale in exactly the direction that made the old explicit
	 * coupling unstable.
	 */
	FORCEINLINE float GetInvInertia() const
	{
		return 1.f / FMath::Max(Inertia + AddedInertia, KINDA_SMALL_NUMBER);
	}
	FORCEINLINE float GetAccumImpulse() const {return AccumulatedImpulse;}
	FORCEINLINE float GetAngularVelocity() const {return AngularVelocity;}
	FORCEINLINE float GetAngle() const {return Angle;}
	FORCEINLINE void SetAngularVelocity(float InAngularVelocity) {AngularVelocity = InAngularVelocity;}

	float CurrentTorque = 0.f;
};