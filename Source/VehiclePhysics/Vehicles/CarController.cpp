// Fill out your copyright notice in the Description page of Project Settings.


#include "CarController.h"

#include "Physics/Car.h"

void ACarController::BeginPlay()
{
	Super::BeginPlay();

	// Kept as a backstop only. SetPawn has almost always run by now - possession happens
	// before BeginPlay on the starting pawn - but this costs nothing and covers a
	// controller that is handed a pawn by some route that skips it.
	ResolveCar();
}

void ACarController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	ACar* const NewCar = Cast<ACar>(InPawn);
	const bool bChanged = (Car.Get() != NewCar);

	Car = NewCar;

	// Guarded so an unpossess/repossess of the SAME car does not make a dashboard rebuild
	// itself for nothing.
	if (bChanged)
	{
		OnPossessedCarChanged.Broadcast(NewCar);
	}
}

ACar* ACarController::ResolveCar()
{
	// Self-healing, on purpose. SetPawn is the intended path and normally the cache is
	// already right; this catches the case where something changed the pawn without
	// going through it, so a stale cache degrades to a lookup rather than to a zero on
	// the dashboard.
	if (!Car.IsValid())
	{
		Car = Cast<ACar>(GetPawn());
	}

	return Car.Get();
}

ACar* ACarController::GetCar()
{
	return ResolveCar();
}

void ACarController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

int32 ACarController::GetSpeedKMH()
{
	if (const ACar* const Current = ResolveCar())
	{
		return Current->GetSpeedKMH();
	}
	return 0;
}

float ACarController::GetEngineRPM()
{
	if (const ACar* const Current = ResolveCar())
	{
		return Current->GetEngineRPM();
	}
	return 0.f;
}

int32 ACarController::GetGear()
{
	if (const ACar* const Current = ResolveCar())
	{
		return Current->GetGear();
	}
	return 1;
}
