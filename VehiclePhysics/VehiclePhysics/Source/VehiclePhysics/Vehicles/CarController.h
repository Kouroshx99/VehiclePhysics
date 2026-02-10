// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CarController.generated.h"

class ACar;
UCLASS()
class VEHICLEPHYSICS_API ACarController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category="Vehicle")
	int32 GetSpeedKMH();
	
	UFUNCTION(BlueprintCallable, Category="Vehicle")
	float GetEngineRPM();
	
	UFUNCTION(BlueprintCallable, Category="Vehicle")
	int32 GetGear();
	
	TWeakObjectPtr<ACar> Car;
	
};
