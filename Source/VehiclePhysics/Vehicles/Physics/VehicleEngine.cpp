// Fill out your copyright notice in the Description page of Project Settings.



#include "VehicleEngine.h"
#include "Curves/CurveFloat.h"


UVehicleEngine::UVehicleEngine()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVehicleEngine::BeginPlay()
{
	Super::BeginPlay();

}

void UVehicleEngine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	float NormalizedRPM = FMath::Clamp(CurrentEngineRPM/MaxRPM, 0.f, 1.f);
	float NormalizedTorque = TorqueCurve->GetFloatValue(CurrentEngineRPM);

	float ThrottleChoker = 0.f;
	if(NormalizedRPM == 1.0f)
	{
		bIsBurning = true;
		ThrottleChoker = 0.f;
	}
	if(!bIsBurning)
		ThrottleChoker = 1.f;
	if(NormalizedRPM <= RedLineRPMRatio)
	{
		bIsBurning = false;
		ThrottleChoker = 1.f;
	}
	
	Torque = NormalizedTorque * MaxEngineTorque * Throttle * ThrottleChoker;
	float EngineBrakingTorque = -EngineBrakingCoefficient * NormalizedRPM;
	float EngineBrakingCoef = Throttle * ThrottleChoker == 0 ? 1.f : 0.f;
	EngineBrakingCoef = NormalizedRPM > RedLineRPMRatio
	? 10.f
	: EngineBrakingCoef;

	NetTorque = Torque + EngineBrakingTorque * EngineBrakingCoef;
	//if (CurrentEngineRPM >= MaxRPM)
	//	NetTorque = FMath::Clamp(NetTorque, FLT_MIN, 0.f);
	//ApplyCurrentImpulse(DeltaTime);
	CurrentEngineRPM = Shaft.GetAngularVelocity() * 60 / (PI * 2);
	if(CurrentEngineRPM < 0.f)
	{
		Shaft.SetAngularVelocity(0.f);
		CurrentEngineRPM = Shaft.GetAngularVelocity() * 60 / (PI * 2);
	}
	if(CurrentEngineRPM > MaxRPM)
	{
		Shaft.SetAngularVelocity(MaxRPM * (2 * PI) / 60);
		CurrentEngineRPM = Shaft.GetAngularVelocity() * 60 / (PI * 2);
	}
}