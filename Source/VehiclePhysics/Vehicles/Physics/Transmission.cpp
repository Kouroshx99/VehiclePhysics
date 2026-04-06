// Fill out your copyright notice in the Description page of Project Settings.


#include "Transmission.h"

#include "Clutch.h"
#include "Shaft.h"
#include "Drivetrain.h"

UTransmission::UTransmission()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTransmission::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Drivetrain->SetDrivetrainTorque(GearRatios[CurrentGear] * DriveShaft.CurrentTorque * TransmissionEfficiency);
	//DriveShaft.AngularVelocity = Drivetrain->GetDrivetrainAngularVelocity() * GearRatios[CurrentGear];
}