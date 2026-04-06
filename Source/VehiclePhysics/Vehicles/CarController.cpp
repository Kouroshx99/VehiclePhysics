// Fill out your copyright notice in the Description page of Project Settings.


#include "CarController.h"

#include "Physics/Car.h"

void ACarController::BeginPlay()
{
	Super::BeginPlay();
	Car = Cast<ACar>(GetPawn());
}

void ACarController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

int32 ACarController::GetSpeedKMH()
{
	
	if(Car.IsValid())
		return Car->GetSpeedKMH();
	return 0;
}

float ACarController::GetEngineRPM()
{
	if(Car.IsValid())
		return Car->GetEngineRPM();
	return 0;
}

int32 ACarController::GetGear()
{
	if(Car.IsValid())
		return Car->GetGear();
	return 1;
}
