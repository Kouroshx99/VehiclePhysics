#pragma once

#include "Clutch.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class VEHICLEPHYSICS_API UClutch : public UObject
{
	GENERATED_BODY()

public:

	UClutch() : A(nullptr), B(nullptr)
	{}

	void Initialize(FShaft* Other, FShaft* TheOther)
	{
		A = Other;
		B = TheOther;
	}

	void Update(float DeltaTime)
	{
		if(A == nullptr || B == nullptr)
			return;
		if(A && B)
		{
			float DeltaOmega = A->GetAngularVelocity() - B->GetAngularVelocity();
			float Tanh = DeltaOmega/(1.0f + FMath::Abs(DeltaOmega));
			float Torque = MaxTorque * ClutchEngagement * Tanh;
			float DeltaOmegaPrediction = A->PredictAngularVelocity(Torque * DeltaTime) - B->PredictAngularVelocity(Torque * DeltaTime);
			float TanhPrediction = DeltaOmegaPrediction/(1.f + FMath::Abs(DeltaOmegaPrediction));
			float TorquePrediction = MaxTorque * ClutchEngagement * TanhPrediction;
			//UE_LOG(LogTemp, Warning, TEXT("Clutch Torque %f"), A->AngularVelocity);
			//A->ApplyTorque(-(Torque + TorquePrediction)/2.f, DeltaTime);
			//B->ApplyTorque((Torque + TorquePrediction)/2.f, DeltaTime);
			UE_LOG(LogTemp, Warning, TEXT("The other torque %f"), Torque);
		}
	}

	/// The maximum torque the clutch can handle (N·m).
	UPROPERTY(EditAnywhere, Category="Clutch", BlueprintReadWrite)
	float MaxTorque = 1000.0f;
	
	FORCEINLINE void SetClutchEngagement(float Value) { ClutchEngagement = FMath::Clamp(Value, 0.0f, 1.0f); }
	
	FORCEINLINE float GetPosition() const { return ClutchEngagement; }
	
	FORCEINLINE float GetTorqueCapacity() const { return MaxTorque * ClutchEngagement; }

protected:
	
	float ClutchEngagement = 1.0f;

private:
	FShaft* A;
	FShaft* B;
	
};