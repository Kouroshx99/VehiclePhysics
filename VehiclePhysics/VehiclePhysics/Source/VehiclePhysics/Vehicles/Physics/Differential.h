// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheelCollider.h"
#include "Components/ActorComponent.h"
#include "Differential.generated.h"


class UClutch;
class UWheelCollider;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UDifferential : public UActorComponent
{
	GENERATED_BODY()

public:
	UDifferential();

	UPROPERTY()
	UWheelCollider* RightWheel;

	UPROPERTY()
	UWheelCollider* LeftWheel;
	
	void BeginPlay() override;
	
	void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetTorque(float InputTorque) {Torque = InputTorque;}
	FORCEINLINE float GetLoadTorque() const {return (RightWheel->GetWheelLoad() + LeftWheel->GetWheelLoad())/FinalDriveRatio;}
	FORCEINLINE float GetAngularVelocity() const {return (RightWheel->GetAngularVelocityRad() + LeftWheel->GetAngularVelocityRad()) * FinalDriveRatio; }
	FORCEINLINE FShaft* GetShaft() {return &Shaft; }

private:

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AllowPrivateAccess=true))
	FShaft Shaft;

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AllowPrivateAccess=true))
	bool bIsLocked;

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AllowPrivateAccess = true))
	bool bIsTorsen = false;

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AloowPrivateAccess = true))
	float FinalDriveRatio = 3.15f;

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AllowPrivateAccess = true))
	float BiasRatio = 2.5;

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AllowPrivateAccess = true))
	float TorsenSlipLimit = 1.f;

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AllowPrivateAccess = true))
	float TorsenPreLoad = 30.f;

	UPROPERTY(EditAnywhere, Category="Differential", meta=(AllowPrivateAccess = true))
	bool bIsLimitedSlip = false;
	
	float Torque = 0.f;

};