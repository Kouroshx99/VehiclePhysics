// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CarDebugHUD.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLEPHYSICS_API ACarDebugHUD : public AHUD
{
	GENERATED_BODY()

private:
	FDebugFloatHistory DebugHistory1; // Stores historical float values
	FDebugFloatHistory DebugHistory2;
	FDebugFloatHistory DebugHistory3;

	FString DebugString1;
	FString DebugString2;
	FString DebugString3;

public:
	ACarDebugHUD();

	virtual void DrawHUD() override;

	void AddDebugValue(float Value, int32 index, FString Label);
private:
	void DrawGraph(float GraphX, float GraphY, float GraphWidth, float GraphHeight, FDebugFloatHistory DebugHistory, FString Label);
};
