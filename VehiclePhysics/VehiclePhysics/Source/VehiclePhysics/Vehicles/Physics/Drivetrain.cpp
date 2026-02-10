// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Drivetrain.h"
#include "Differential.h"
#include "Transmission.h"

UDrivetrain::UDrivetrain()
{
	PrimaryComponentTick.bCanEverTick = true;

	FrontDifferential = CreateDefaultSubobject<UDifferential>(TEXT("Front Differential"));
	RearDifferential = CreateDefaultSubobject<UDifferential>(TEXT("Rear Differential"));

	FrontDiffShaft = FrontDifferential->GetShaft();
	RearDiffShaft = RearDifferential->GetShaft();
}

void UDrivetrain::BeginPlay()
{
	Super::BeginPlay();
	
	EngineCopy = new FShaft(*EngineShaft);
	ClutchCopy = new FShaft(*ClutchShaft);
	TransmissionCopy = new FShaft(*TransmissionShaft);
	TransferCaseCopy = new FShaft(*TransferCaseShaft);
	FrontDiffCopy = new FShaft(*FrontDiffShaft);
	RearDiffCopy = new FShaft(*RearDiffShaft);
	FRCopy = new FShaft(*FrontRightWheelShaft);
	FLCopy = new FShaft(*RearRightWheelShaft);
	RRCopy = new FShaft(*RearRightWheelShaft);
	RLCopy = new FShaft(*RearLeftWheelShaft);

	InitializeShafts();
}

void UDrivetrain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FrontDifferential->SetTorque(Torque * FMath::Abs(1 - FrontRearSplit));
	//RearDifferential->SetTorque(Torque * FMath::Abs(0 - FrontRearSplit));
	
	if (EngineShaft->GetAngularVelocity() * 60 / (PI * 2) < 1000)
		t = GetWorld()->TimeSeconds;
	a = EngineShaft->GetAngularVelocity();
}

void UDrivetrain::GearUp()
{
	CurrentGear = FMath::Clamp(++CurrentGear, 0, GearRatios.Num()-1);
	switch (DrivetrainType)
	{
	case EDrivetrainType::EDT_RearWheelDrive:
		Connections[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_FrontWheelDrive:
		Connections[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_AllWheelDrive:
		Connections[2].Ratio = GearRatios[CurrentGear];
		Connections[3].Ratio = GearRatios[CurrentGear];
		break;
	}
}

void UDrivetrain::GearDown()
{
	CurrentGear = FMath::Clamp(--CurrentGear, 0, GearRatios.Num()-1);
	switch (DrivetrainType)
	{
	case EDrivetrainType::EDT_RearWheelDrive:
		Connections[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_FrontWheelDrive:
		Connections[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_AllWheelDrive:
		Connections[2].Ratio = GearRatios[CurrentGear];
		Connections[3].Ratio = GearRatios[CurrentGear];
		break;
	}
}

void UDrivetrain::InitializeShafts()
{
	// Define connections (from input to output, ratio >1 for reduction)
	Connections.Add(FConnection(EngineShaft, ClutchShaft, 1.0f, FConnection::EConnectionType::Locked)); // Engine to clutch
	Connections.Add(FConnection(ClutchShaft, TransmissionShaft, 1.0f, FConnection::EConnectionType::Locked)); // Clutch to trans
	
	if (DrivetrainType == EDrivetrainType::EDT_RearWheelDrive)
	{
		Connections.Add(FConnection(TransmissionShaft, RearDiffShaft, GearRatios[CurrentGear], FConnection::EConnectionType::Locked)); // Trans to transfer
		Connections.Add(FConnection(RearDiffShaft, 
		RearRightWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
		Connections.Add(FConnection(RearDiffShaft, 
		RearLeftWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
	}
	
	if (DrivetrainType == EDrivetrainType::EDT_FrontWheelDrive)
	{
		Connections.Add(FConnection(TransmissionShaft, FrontDiffShaft, GearRatios[CurrentGear], FConnection::EConnectionType::Locked)); // Transfer to front diff
		Connections.Add(FConnection(FrontDiffShaft, 
			FrontRightWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
		Connections.Add(FConnection(FrontDiffShaft,
			FrontLeftWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
		Connections.Add(FConnection(RearDiffShaft, 
			FrontDiffShaft, 1.f, FConnection::EConnectionType::Locked)); // Transfer to front diff
	}
	if (DrivetrainType == EDrivetrainType::EDT_AllWheelDrive)
	{
		Connections.Add(FConnection(TransmissionShaft, RearDiffShaft, GearRatios[CurrentGear], FConnection::EConnectionType::Locked)); // Trans to transfer

		Connections.Add(FConnection(TransmissionShaft, FrontDiffShaft, GearRatios[CurrentGear], FConnection::EConnectionType::Locked)); // Transfer to front diff
		Connections.Add(FConnection(RearDiffShaft, 
		RearRightWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
		Connections.Add(FConnection(RearDiffShaft, 
		RearLeftWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
		Connections.Add(FConnection(FrontDiffShaft, 
			FrontRightWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
		Connections.Add(FConnection(FrontDiffShaft,
			FrontLeftWheelShaft, FinalDriveRatio, FConnection::EConnectionType::Locked));
		Connections.Add(FConnection(RearDiffShaft, 
			FrontDiffShaft, 1.f, FConnection::EConnectionType::Locked));
		
		
	}
	FConnection FRBrake = FConnection(
		FrontRightWheelShaft, 
		FrontRightWheelShaft,
		1.f,
		3000,
		FConnection::EConnectionType::Brake);
	FConnection FLBrake = FConnection(
		FrontLeftWheelShaft,
		FrontLeftWheelShaft,
		1.f,
		3000,
		FConnection::EConnectionType::Brake);
	FConnection RRBrake = FConnection(
		RearRightWheelShaft,
		RearRightWheelShaft,
		1.f,
		3000,
		FConnection::EConnectionType::Brake);
	FConnection RLBrake = FConnection(
		RearLeftWheelShaft,
		RearLeftWheelShaft,
		1.f,
		3000,
		FConnection::EConnectionType::Brake);
	Connections.Add(FRBrake);
	BrakeConnections.Add(&Connections[Connections.Num() - 1]);
	Connections.Add(FLBrake);
	BrakeConnections.Add(&Connections[Connections.Num() - 1]);
	Connections.Add(RRBrake);
	BrakeConnections.Add(&Connections[Connections.Num() - 1]);
	Connections.Add(RLBrake);
	BrakeConnections.Add(&Connections[Connections.Num() - 1]);

	// Define connections (from input to output, ratio >1 for reduction)
	//CopyConnections.Add(FConnection(EngineCopy, ClutchCopy, 1.0f)); // Engine to clutch
	//CopyConnections.Add(FConnection(ClutchCopy, TransmissionCopy, 1.0f)); // Clutch to trans
	//CopyConnections.Add(FConnection(TransmissionCopy, RearDiffCopy, 5.25)); // Trans to transfer
	//CopyConnections.Add(FConnection(TransferCaseCopy, FrontDiffCopy, 1.0f)); // Transfer to front diff
	//CopyConnections.Add(FConnection(TransferCaseCopy, RearDiffCopy, 1.0f)); // Transfer to rear diff
	//CopyConnections.Add(FConnection(FrontDiffCopy, FRCopy, FinalDriveRatio)); // Front diff to right wheel
	//CopyConnections.Add(FConnection(FrontDiffCopy, FLCopy, FinalDriveRatio)); // Front diff to left wheel
	//CopyConnections.Add(FConnection(RearDiffCopy, RRCopy, FinalDriveRatio)); // Rear diff to right wheel
	//CopyConnections.Add(FConnection(RearDiffCopy, RLCopy, FinalDriveRatio)); // Rear diff to left wheel

}
int32 offset = 0;
void UDrivetrain::SolveConstraints(TArray<FConnection>& Conns, float DT)
{
	for(FConnection& Connection : Conns)
	{
		//Connection.Shaft1->DeltaTime = DT;
		//Connection.Shaft2->DeltaTime = DT;
		Connection.Shaft1->ResetAccumImpulse();
		Connection.Shaft2->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for (int32 Iter = 0; Iter < SolverIterations; ++Iter)
	{
		offset = offset % Conns.Num();
		for (int32 j = 0; j < Conns.Num(); j++)
		{
			int32 jj = j + offset;
			jj = jj % Conns.Num();
			FConnection& Conn = Conns[jj];
			//if (Conn.ConnectionType == FConnection::EConnectionType::Brake)
			//	UE_LOG(LogTemp, Display, TEXT("Brake"))
			//else
			//	UE_LOG(LogTemp, Display, TEXT("Locked"))
			float Omega1 = Conn.Shaft1->GetAngularVelocity();
			float Omega2 = 0.f;
			if (Conn.ConnectionType == FConnection::EConnectionType::Locked)
				Omega2 = Conn.Shaft2->GetAngularVelocity();
			float ABSRatio = FMath::Abs(Conn.Ratio);
			float DeltaOmega = Omega1 - Conn.Ratio * Omega2;
			if (FMath::Abs(DeltaOmega) < KINDA_SMALL_NUMBER)
			{
				continue;
			}
			float InvI1 = Conn.Shaft1->GetInvInertia();
			float InvI2 = Conn.Shaft2->GetInvInertia() * FMath::Square(ABSRatio);
			float InvISum = InvI1 + InvI2;
			if (FMath::Abs(InvISum) < KINDA_SMALL_NUMBER)
			{
				continue;
			}
			float J = DeltaOmega / InvISum;
			//if (Conn.ConnectionType == FConnection::EConnectionType::Brake)
			//	UE_LOG(LogTemp, Display, TEXT("J %f"), J)
			//else
			//	UE_LOG(LogTemp, Display, TEXT("Not"))
			
			if (Conn.ConnectionType == FConnection::EConnectionType::Brake)
			{
				float Max = FMath::Abs(Conn.MaxCorrectionTorque * Conn.ConnectionStrength * FMath::Sign(DeltaOmega)
					* DT /SolverIterations);
				J = FMath::Clamp(J, -Max, Max);
			}
			//if (Conn.ConnectionType == FConnection::EConnectionType::Brake)
			//	UE_LOG(LogTemp, Display, TEXT("J after %f"), J);
			Conn.AccumJ += J;
			Conn.Shaft1->ApplyImpulse(-J);
			if (Conn.ConnectionType == FConnection::EConnectionType::Locked)
				Conn.Shaft2->ApplyImpulse(J * Conn.Ratio);
		}
		offset++;
	}
}