// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Drivetrain.h"
#include "Differential.h"
#include "Transmission.h"

UDrivetrain::UDrivetrain()
{
	PrimaryComponentTick.bCanEverTick = true;

	//FrontDifferential = CreateDefaultSubobject<UDifferential>(TEXT("Front Differential"));
	//RearDifferential = CreateDefaultSubobject<UDifferential>(TEXT("Rear Differential"));

	//FrontDiffShaft = FrontDifferential->GetShaft();
	//RearDiffShaft = RearDifferential->GetShaft();
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
	if (DifferentialConstraints.Num() > 0 && false)
	UE_LOG(LogTemp, Warning, TEXT("Diff %f, R %f, L %f"), 
		DifferentialConstraints[0].Input->GetAngularVelocity() / DifferentialConstraints[0].Ratio,
		DifferentialConstraints[0].Right->GetAngularVelocity(),
		DifferentialConstraints[0].Left->GetAngularVelocity())
	
	if (DifferentialConstraints.Num() > 0)
		UE_LOG(LogTemp, Warning, TEXT("-------------------- Diff %f"), 
			DifferentialConstraints[0].Right->GetAngularVelocity() - DifferentialConstraints[0].Left->GetAngularVelocity())
}

void UDrivetrain::GearUp()
{
	CurrentGear = FMath::Clamp(++CurrentGear, 0, GearRatios.Num()-1);
	switch (DrivetrainType)
	{
	case EDrivetrainType::EDT_RearWheelDrive:
		GearConstraints[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_FrontWheelDrive:
		GearConstraints[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_AllWheelDrive:
		GearConstraints[2].Ratio = GearRatios[CurrentGear];
		GearConstraints[3].Ratio = GearRatios[CurrentGear];
		break;
	}
}

void UDrivetrain::GearDown()
{
	CurrentGear = FMath::Clamp(--CurrentGear, 0, GearRatios.Num()-1);
	switch (DrivetrainType)
	{
	case EDrivetrainType::EDT_RearWheelDrive:
		GearConstraints[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_FrontWheelDrive:
		GearConstraints[2].Ratio = GearRatios[CurrentGear];
		break;
	case EDrivetrainType::EDT_AllWheelDrive:
		GearConstraints[2].Ratio = GearRatios[CurrentGear];
		GearConstraints[3].Ratio = GearRatios[CurrentGear];
		break;
	}
}

void UDrivetrain::InitializeShafts()
{
	// Define connections (from input to output, ratio >1 for reduction)
	AddGearConstraint(FGearConstraint(EngineShaft, ClutchShaft, 1.f));
	AddGearConstraint(FGearConstraint(ClutchShaft, TransmissionShaft, 1.0f)); // Clutch to trans
	//Connections.Add(FGearConstraint(EngineShaft, ClutchShaft, 1.0f)); // Engine to clutch
	//Connections.Add(FGearConstraint(ClutchShaft, TransmissionShaft, 1.0f)); // Clutch to trans
	
	if (DrivetrainType == EDrivetrainType::EDT_RearWheelDrive)
	{
		AddGearConstraint(FGearConstraint(TransmissionShaft, RearDiffShaft, GearRatios[CurrentGear]));
		//Connections.Add(FGearConstraint(TransmissionShaft, RearDiffShaft, GearRatios[CurrentGear]));
		
		const int32 RearDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				RearDiffShaft,
				RearLeftWheelShaft,
				RearRightWheelShaft,
				FinalDriveRatio));
		
		if (RearDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(RearRightWheelShaft,
				RearLeftWheelShaft,
				1.f));
		
		if (RearDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(RearLeftWheelShaft, RearRightWheelShaft,
				RearDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
	}
	
	if (DrivetrainType == EDrivetrainType::EDT_FrontWheelDrive)
	{
		AddGearConstraint(FGearConstraint(TransmissionShaft, FrontDiffShaft, GearRatios[CurrentGear]));
		
		const int32 FrontDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				FrontDiffShaft,
				FrontLeftWheelShaft,
				FrontRightWheelShaft,
				FinalDriveRatio));
		
		if (FrontDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(FrontRightWheelShaft,
				FrontLeftWheelShaft,
				1.f));
		
		if (FrontDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(FrontLeftWheelShaft, FrontRightWheelShaft,
				FrontDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
	}
	
	if (DrivetrainType == EDrivetrainType::EDT_AllWheelDrive)
	{
		AddGearConstraint(FGearConstraint(TransmissionShaft, RearDiffShaft, GearRatios[CurrentGear]));
		
		const int32 RearDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				RearDiffShaft,
				RearLeftWheelShaft,
				RearRightWheelShaft,
				FinalDriveRatio));
		
		if (RearDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(RearRightWheelShaft,
				RearLeftWheelShaft,
				1.f));
		
		if (RearDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(RearLeftWheelShaft, RearRightWheelShaft,
				RearDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
		
		AddGearConstraint(FGearConstraint(TransmissionShaft, FrontDiffShaft, GearRatios[CurrentGear]));
		
		const int32 FrontDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				FrontDiffShaft,
				FrontLeftWheelShaft,
				FrontRightWheelShaft,
				FinalDriveRatio));
		
		if (FrontDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(FrontRightWheelShaft,
				FrontLeftWheelShaft,
				1.f));
		
		if (FrontDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(FrontLeftWheelShaft, FrontRightWheelShaft,
				FrontDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
	}
	//FBrakeConstraint FRBrake = FBrakeConstraint(
	//	FrontRightWheelShaft,
	//	1.f,
	//	3000);
	//FBrakeConstraint FLBrake = FBrakeConstraint(
	//	FrontLeftWheelShaft,
	//	1.f,
	//	3000);
	//FBrakeConstraint RRBrake = FBrakeConstraint(
	//	RearRightWheelShaft,
	//	1.f,
	//	3000);
	//FBrakeConstraint RLBrake = FBrakeConstraint(
	//	RearLeftWheelShaft,
	//	1.f,
	//	3000);
	//Connections.Add(FRBrake);
	AddBrakeConstraint(FBrakeConstraint(
		FrontRightWheelShaft,
		1.f,
		3000));
	AddBrakeConstraint(FBrakeConstraint(
		FrontLeftWheelShaft,
		1.f,
		3000));
	AddBrakeConstraint(FBrakeConstraint(
		RearRightWheelShaft,
		1.f,
		3000));
	AddBrakeConstraint(FBrakeConstraint(
		RearLeftWheelShaft,
		1.f,
		3000));
	//BrakeConstraints.Add(&GearConstraints[Connections.Num() - 1]);
	//Connections.Add(FLBrake);
	//BrakeConstraints.Add(GearConstraints[Connections.Num() - 1]);
	//Connections.Add(RRBrake);
	//BrakeConstraints.Add(&GearConstraints[Connections.Num() - 1]);
	//Connections.Add(RLBrake);
	//BrakeConstraints.Add(&GearConstraints[Connections.Num() - 1]);

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

void UDrivetrain::PreSolveConstraints(float DT)
{
	for (FGearConstraint& C : GearConstraints)
	{
		C.AccumJ = 0.f;
		C.DeltaTime = DT;
	}

	for (FDiffCarrierConstraint& C : DiffCarrierConstraints)
	{
		C.AccumJ = 0.f;
		C.DeltaTime = DT;
	}

	for (FLimitedSlipConstraint& C : LimitedSlipConstraints)
	{
		C.AccumJ = 0.f;
		C.DeltaTime = DT;
	}
}

void UDrivetrain::SolveConstraints(TArray<FGearConstraint>& Conns, float DT)
{
	PreSolveConstraints(DT);
	for(FGearConstraint& Connection : GearConstraints)
	{
		Connection.Shaft1->ResetAccumImpulse();
		Connection.Shaft2->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for(FBrakeConstraint& Connection : BrakeConstraints)
	{
		Connection.Shaft->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for(FDiffCarrierConstraint& Connection : DiffCarrierConstraints)
	{
		Connection.Input->ResetAccumImpulse();
		Connection.Left->ResetAccumImpulse();
		Connection.Right->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for(FLimitedSlipConstraint& Connection : LimitedSlipConstraints)
	{
		Connection.Left->ResetAccumImpulse();
		Connection.Right->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for (int32 Iter = 0; Iter < SolverIterations; ++Iter)
	{
		const int32 NumRefs = SolveOrder.Num();
		if (NumRefs == 0)
			return;

		const int32 Offset = Iter % NumRefs;

		for (int32 j = 0; j < NumRefs; ++j)
		{
			const int32 RefIndex = (j + Offset) % NumRefs;
			const FConstraintRef& Ref = SolveOrder[RefIndex];

			switch (Ref.Kind)
			{
			case EConstraintKind::Gear:
				SolveGearConstraint(GearConstraints[Ref.Index], DT);
				break;

			case EConstraintKind::DiffCarrier:
				SolveDiffCarrierConstraint(DiffCarrierConstraints[Ref.Index], DT);
				break;

			case EConstraintKind::LimitedSlip:
				SolveLimitedSlipConstraint(LimitedSlipConstraints[Ref.Index]);
				break;

			case EConstraintKind::Brake:
				SolveBrakeConstraint(BrakeConstraints[Ref.Index], DT);
				break;
			}
		}
	}
	return;
	for (int32 Iter = 0; Iter < SolverIterations; ++Iter)
	{
		offset = offset % Conns.Num();
		//SolveGearConstraint(GearConstraints, DT);
		//SolveDiffCarrierConstraint(DifferentialConstraints, DT);
		offset++;
	}
}

void UDrivetrain::SolveGearConstraint(FGearConstraint&Constraint, float DeltaTime)
{
	const float R = Constraint.Ratio;

	float Win = Constraint.Shaft1->GetAngularVelocity();
	float Wout = Constraint.Shaft2->GetAngularVelocity();

	float DeltaOmega = Win - Wout * R;

	if (FMath::Abs(DeltaOmega) < KINDA_SMALL_NUMBER)
		return;

	float InvIin = Constraint.Shaft1->GetInvInertia();
	float InvIout = Constraint.Shaft2 ? Constraint.Shaft2->GetInvInertia() * FMath::Square(R) : 0.f;

	float InvISum = InvIin + InvIout;

	if (FMath::Abs(InvISum) < KINDA_SMALL_NUMBER)
		return;

	float J = DeltaOmega / InvISum;

	Constraint.AccumJ += J;

	Constraint.Shaft1->ApplyImpulse(-J);
	Constraint.Shaft2->ApplyImpulse(J * R);
}

void UDrivetrain::SolveBrakeConstraint(FBrakeConstraint& Constraint, float DeltaTime)
{
	const float R = Constraint.Ratio;

	float WheelAngularVelocity = Constraint.Shaft->GetAngularVelocity();

	float DeltaOmega = WheelAngularVelocity;

	if (FMath::Abs(DeltaOmega) < KINDA_SMALL_NUMBER)
		return;

	float InvI = Constraint.Shaft->GetInvInertia();
		
	if (FMath::Abs(InvI) < KINDA_SMALL_NUMBER)
		return;

	float J = DeltaOmega / InvI;
	J *= Constraint.ConnectionStrength;

	Constraint.AccumJ += J;

	Constraint.Shaft->ApplyImpulse(-J);
}

void UDrivetrain::SolveDiffCarrierConstraint(FDiffCarrierConstraint& Constraint, float DeltaTime)
{
	const float R = Constraint.Ratio;

	float Win = Constraint.Input->GetAngularVelocity();
	float Wl  = Constraint.Left->GetAngularVelocity();
	float Wr  = Constraint.Right->GetAngularVelocity();
		
	float Cdot = Win - (Wl + Wr) * R * 0.5f;

	float InvIin = Constraint.Input->GetInvInertia();
	float InvIl  = Constraint.Left->GetInvInertia() * FMath::Square(R * 0.5f);
	float InvIr  = Constraint.Right->GetInvInertia() * FMath::Square(R * 0.5f);

	float InvISum = InvIin + InvIl + InvIr;
	if (FMath::Abs(InvISum) < KINDA_SMALL_NUMBER)
		return;

	float J = Cdot / InvISum;
	Constraint.AccumJ += J;
	Constraint.Input->ApplyImpulse(-J);
	Constraint.Left->ApplyImpulse(J * R * 0.5f);
	Constraint.Right->ApplyImpulse(J * R * 0.5f);
}

void UDrivetrain::SolveLimitedSlipConstraint(FLimitedSlipConstraint& LSD)
{
	float Wl = LSD.Left->GetAngularVelocity();
	float Wr = LSD.Right->GetAngularVelocity();

	float DeltaOmega = Wl - Wr;
	if (FMath::Abs(DeltaOmega) < KINDA_SMALL_NUMBER)
		return;

	float InvIl = LSD.Left->GetInvInertia();
	float InvIr = LSD.Right->GetInvInertia();

	float InvISum = InvIl + InvIr;
	if (FMath::Abs(InvISum) < KINDA_SMALL_NUMBER)
		return;

	float J = DeltaOmega / InvISum;

	float CarrierTorque = 0.f;
	if (DiffCarrierConstraints.IsValidIndex(LSD.CarrierConstraintIndex))
	{
		const FDiffCarrierConstraint& Carrier = DiffCarrierConstraints[LSD.CarrierConstraintIndex];
		CarrierTorque = Carrier.AccumJ != 0.f ? FMath::Abs(Carrier.AccumJ / LSD.DeltaTime) : 0.f;
	}

	float AllowedTorque = LSD.PreloadTorque + LSD.PowerLockCoeff * CarrierTorque;
	AllowedTorque = FMath::Min(AllowedTorque, LSD.MaxCorrectionTorque);

	float MaxJ = AllowedTorque * LSD.DeltaTime / SolverIterations;
	J = FMath::Clamp(J, -MaxJ, MaxJ);

	LSD.AccumJ += J;
	LSD.Left->ApplyImpulse(-J);
	LSD.Right->ApplyImpulse(J);
}