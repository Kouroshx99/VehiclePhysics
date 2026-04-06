// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shaft.h"
#include "Drivetrain.h"
#include "Components/ActorComponent.h"
#include "Transmission.generated.h"


struct FShaft;
class UDrivetrain;
class UClutch;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UTransmission : public UActorComponent
{
	GENERATED_BODY()

public:
	UTransmission();
	
	UPROPERTY(EditAnywhere, Category="Transmission", meta=(AllowPrivateAccess=true))
	FShaft Shaft;

	UPROPERTY()
	UDrivetrain* Drivetrain;

	FORCEINLINE void SetTorque(float InputTorque) {Torque = InputTorque;}
	FORCEINLINE void SetLoadTorque(float InputLoad) {LoadTorque = InputLoad;}
	//FORCEINLINE float GetShaftAngularVelocity() const { return Drivetrain? Drivetrain->GetDrivetrainAngularVelocity() * GearRatios[CurrentGear] : 0.f; }
	FORCEINLINE FShaft* GetShaft() { return &Shaft; }

	UPROPERTY(EditAnywhere, Category="Transmission", meta=(AllowPrivateAccess=true, Tooltip = "First ratio in array is reverse gear"))
	TArray<float> GearRatios {3.71, 5.25f, 3.36f, 2.17f, 1.72f, 1.32f, 1.f, 0.82f, 0.64f};
	int32 CurrentGear;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere, Category="Transmission", meta=(AllowPrivateAccess=true))
	float TransmissionEfficiency = 0.94f;

	float Torque = 0.f;
	float LoadTorque = 0.f;

};