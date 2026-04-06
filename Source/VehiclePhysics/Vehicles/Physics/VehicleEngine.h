// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shaft.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "VehicleEngine.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class VEHICLEPHYSICS_API UVehicleEngine : public UActorComponent
{
	GENERATED_BODY()

public:
	UVehicleEngine();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetThrottle(float InputThrottle) {Throttle = InputThrottle;}
	FORCEINLINE float GetTorque() const {return FMath::Clamp(Torque, 0.f, FLT_MAX);}
	FORCEINLINE float GetThrottle() const {return Throttle;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Engine")
	float CurrentEngineRPM = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Engine")
	float RedLineRPMRatio = 0.95f;
	
	FORCEINLINE FShaft* GetShaft() {return &Shaft;}
	float NetTorque;

	FORCEINLINE void ApplyCurrentImpulse(float DeltaTime)
	{
		Shaft.ApplyImpulse(NetTorque * DeltaTime);
	}

	FORCEINLINE float GetCurrentTorque() const {return NetTorque;}
protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float MaxRPM = 8000.f;
	
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float IdleRPM = 800.f;

	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	UCurveFloat* TorqueCurve;
	
	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float MaxEngineTorque = 250.f;

	UPROPERTY(EditAnywhere, Category="Engine", meta = (AllowPrivateAccess = true))
	float EngineBrakingCoefficient = 0.f;

private:
	UPROPERTY(EditAnywhere, Category="Engine",meta=(AllowPrivateAccess=true))
	FShaft Shaft;

	float Throttle = 0.f;
	float Torque = 0.f;
	bool bIsBurning = false;

};