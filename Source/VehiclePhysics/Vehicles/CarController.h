// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CarController.generated.h"

class ACar;

/** Fired whenever the possessed car changes, so a dashboard can rebind to the new one. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossessedCarChanged, ACar*, NewCar);

UCLASS()
class VEHICLEPHYSICS_API ACarController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * Broadcast on every possession change, including a roster switch.
	 *
	 * Only needed by a UI that keeps its own reference to the car rather than asking
	 * through the getters below. If a dashboard casts the pawn once and stores it, that
	 * stored pointer dies with the car it came from - bind here and re-read.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Vehicle")
	FOnPossessedCarChanged OnPossessedCarChanged;

	/** The car being driven right now, or null between possessions. */
	UFUNCTION(BlueprintPure, Category = "Vehicle")
	ACar* GetCar();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	int32 GetSpeedKMH();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	float GetEngineRPM();

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	int32 GetGear();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * THE ONE THAT MATTERS. Every possession and unpossession comes through here, so it
	 * is where the cached car is refreshed.
	 *
	 * It used to be cached in BeginPlay alone, which is correct exactly once - for the
	 * car the player starts in. After a roster switch the pointer referred to a car that
	 * had been destroyed, every getter fell through its IsValid check, and the dashboard
	 * read zero while the car it was meant to be reading drove past perfectly well.
	 *
	 * SetPawn rather than OnPossess deliberately: OnPossess is the server-side half of
	 * the handshake and is not the only route a pawn arrives by, whereas AController
	 * funnels all of them through SetPawn.
	 */
	virtual void SetPawn(APawn* InPawn) override;

private:
	/** Returns the cached car, re-resolving from the pawn if the cache went stale. */
	ACar* ResolveCar();

	TWeakObjectPtr<ACar> Car;
};
