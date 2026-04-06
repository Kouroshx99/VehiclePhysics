// Fill out your copyright notice in the Description page of Project Settings.


#include "CarAnimInstance.h"

#include "VehiclePhysics/Vehicles/Physics/Car.h"

void UCarAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Car = Cast<ACar>(TryGetPawnOwner());
}

void UCarAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Car == nullptr)
	{
		Car = Cast<ACar>(TryGetPawnOwner());
		if(Car == nullptr)
			return;
	}

	FrontRight = Car->GetWheelRotation(EWheel::EW_FrontRight);
	FrontLeft = Car->GetWheelRotation(EWheel::EW_FrontLeft);
	RearRight = Car->GetWheelRotation(EWheel::EW_RearRight);
	RearLeft = Car->GetWheelRotation(EWheel::EW_RearLeft);

	FrontRightRotation -= Car->GetWheelSpeed(EWheel::EW_FrontRight) * DeltaSeconds;
	FrontLeftRotation -= Car->GetWheelSpeed(EWheel::EW_FrontLeft) * DeltaSeconds;
	RearRightRotation -= Car->GetWheelSpeed(EWheel::EW_RearRight) * DeltaSeconds;
	RearLeftRotation -= Car->GetWheelSpeed(EWheel::EW_RearLeft) * DeltaSeconds;

	FrontRightCenterZ = Car->GetWheelZPosition(EWheel::EW_FrontRight) + RestFR;
	FrontLeftCenterZ = Car->GetWheelZPosition(EWheel::EW_FrontLeft) + RestFR;
	RearRightCenterZ = Car->GetWheelZPosition(EWheel::EW_RearRight) + RestFL;
	RearLeftCenterZ = Car->GetWheelZPosition(EWheel::EW_RearLeft) + RestRL;

	FrontRightAngle = Car->GetWheelAngle(EWheel::EW_FrontRight);
	FrontLeftAngle = Car->GetWheelAngle(EWheel::EW_FrontLeft);

	FrontRightRoll = FrontRight.Roll;
	FrontLeftRoll = FrontLeft.Roll;
	RearRightRoll = RearRight.Roll;
	RearLeftRoll = RearLeft.Roll;
	
	PosFR = Car->GetWheelPosition(EWheel::EW_FrontRight) + RestFR;
	PosFL = Car->GetWheelPosition(EWheel::EW_FrontLeft) + RestFL;
	PosRR = Car->GetWheelPosition(EWheel::EW_RearRight) + RestRR;
	PosRL = Car->GetWheelPosition(EWheel::EW_RearLeft) + RestRL;
	
}