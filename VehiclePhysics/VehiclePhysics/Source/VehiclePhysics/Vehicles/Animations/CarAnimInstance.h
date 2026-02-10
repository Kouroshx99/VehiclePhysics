// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CarAnimInstance.generated.h"

class ACar;
/**
 * 
 */
UCLASS()
class VEHICLEPHYSICS_API UCarAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACar* Car;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearRightRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearLeftRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightAngle = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftAngle = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightCenterZ = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftCenterZ = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearRightCenterZ = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearLeftCenterZ = 0.f;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearRightRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearLeftRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator FrontRight = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator FrontLeft = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator RearRight = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator RearLeft = FRotator::ZeroRotator;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosFR;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosFL;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosRR;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosRL;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestFR = 50.f;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestFL = 50.f;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestRR = 50.f;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestRL = 50.f;
};