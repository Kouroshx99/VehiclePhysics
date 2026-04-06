// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Differential.h"
#include "Connection.h"
#include "Components/ActorComponent.h"
#include "Drivetrain.generated.h"

class UVehicleEngine;
class UDifferential;

UENUM(BlueprintType)
enum class EDrivetrainType: uint8
{
	EDT_FrontWheelDrive UMETA(DisplayName="FrontWheelDrive"),
	EDT_RearWheelDrive UMETA(DisplayName="RearWheelDrive"),
	EDT_AllWheelDrive UMETA(DisplayName="AllWheelDrive")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UDrivetrain : public UActorComponent
{
	GENERATED_BODY()

public:
	UDrivetrain();
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	int32 SolverIterations = 10;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	EDrivetrainType DrivetrainType = EDrivetrainType::EDT_FrontWheelDrive;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	EDifferentialType FrontDifferentialType = EDifferentialType::Open;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	EDifferentialType RearDifferentialType = EDifferentialType::Open;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential", meta=(unit="N.m"))
	float PreloadTorque = 50;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential")
	float PowerLockCoeff = 0.35f;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential")
	float CoastLockCoeff = 0.15f;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential", meta=(unit="N.m"))
	float MaxCorrectionTorque = 700;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	float FinalDriveRatio = 3.15f;

	UPROPERTY(EditAnywhere, Category="Drivetrain")
	TArray<float> GearRatios {3.71, 5.25f, 3.36f, 2.17f, 1.72f, 1.32f, 1.f, 0.82f, 0.64f};
	
	int32 CurrentGear = 1;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void GearUp();
	void GearDown();

	//Shafts for the chain
	UPROPERTY()
	UVehicleEngine* VehicleEngine;
	
	FShaft* EngineShaft;
	FShaft* ClutchShaft;
	FShaft* TransmissionShaft;
	FShaft* TransferCaseShaft;
	FShaft* FrontDiffShaft;
	FShaft* RearDiffShaft;
	FShaft* FrontRightWheelShaft;
	FShaft* FrontLeftWheelShaft;
	FShaft* RearRightWheelShaft;
	FShaft* RearLeftWheelShaft;

	FShaft* EngineCopy;
	FShaft* ClutchCopy;
	FShaft* TransmissionCopy;
	FShaft* TransferCaseCopy;
	FShaft* FrontDiffCopy;
	FShaft* RearDiffCopy;
	FShaft* FRCopy;
	FShaft* FLCopy;
	FShaft* RRCopy;
	FShaft* RLCopy;

	TArray<FGearConstraint> Connections;
	TArray<FDiffCarrierConstraint> DifferentialConstraints;
	TArray<FGearConstraint> CopyConnections;
	
	TArray<FGearConstraint> GearConstraints;
	TArray<FBrakeConstraint> BrakeConstraints;
	TArray<FDiffCarrierConstraint> DiffCarrierConstraints;
	TArray<FLimitedSlipConstraint> LimitedSlipConstraints;
	TArray<FConstraintRef> SolveOrder;

private:

	///Torque from drive train (N.m)
	float Torque;

	float a = 0.f;
	float t = 0.f;


	void InitializeShafts();
	
	int32 AddGearConstraint(const FGearConstraint& Constraint)
	{
		const int32 Index = GearConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::Gear, Index));
		return Index;
	}
	
	int32 AddBrakeConstraint(const FBrakeConstraint& Constraint)
	{
		const int32 Index = BrakeConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::Brake, Index));
		return Index;
	}

	int32 AddDiffCarrierConstraint(const FDiffCarrierConstraint& Constraint)
	{
		const int32 Index = DiffCarrierConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::DiffCarrier, Index));
		return Index;
	}

	int32 AddLimitedSlipConstraint(const FLimitedSlipConstraint& Constraint)
	{
		const int32 Index = LimitedSlipConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::LimitedSlip, Index));
		return Index;
	}
public:
	void PreSolveConstraints(float DT);
	void SolveConstraints(TArray<FGearConstraint>& Conns, float DT);
	void SolveGearConstraint(FGearConstraint& Constraint, float DeltaTime);
	void SolveBrakeConstraint(FBrakeConstraint& Constraint, float DeltaTime);
	void SolveDiffCarrierConstraint(FDiffCarrierConstraint& Diffs, float DeltaTime);
	void SolveLimitedSlipConstraint(FLimitedSlipConstraint& LSD);
	FORCEINLINE TArray<FGearConstraint> GetConnections() const
	{
		TArray<FGearConstraint> Copy;
		Copy.Reserve(Connections.Num());

		for (const FGearConstraint& Conn : Connections)
		{
			FGearConstraint NewConn;

			if (Conn.Shaft1)
			{
				NewConn.Shaft1 = new FShaft(*Conn.Shaft1); // deep copy
			}

			if (Conn.Shaft2)
			{
				NewConn.Shaft2 = new FShaft(*Conn.Shaft2); // deep copy
			}

			Copy.Add(NewConn);
		}

		return Copy;
	}

	FORCEINLINE void CopyAngVelToOG()
	{
		EngineShaft->SetAngularVelocity(EngineCopy->GetAngularVelocity());
		ClutchShaft->SetAngularVelocity(ClutchCopy->GetAngularVelocity());
		TransmissionShaft->SetAngularVelocity(TransmissionCopy->GetAngularVelocity());
		TransferCaseShaft->SetAngularVelocity(TransferCaseCopy->GetAngularVelocity());
		FrontDiffShaft->SetAngularVelocity(FrontDiffCopy->GetAngularVelocity());
		RearDiffShaft->SetAngularVelocity(RearDiffCopy->GetAngularVelocity());
		FrontRightWheelShaft->SetAngularVelocity(FRCopy->GetAngularVelocity());
		FrontLeftWheelShaft->SetAngularVelocity(FLCopy->GetAngularVelocity());
		RearRightWheelShaft->SetAngularVelocity(RRCopy->GetAngularVelocity());
		RearLeftWheelShaft->SetAngularVelocity(RLCopy->GetAngularVelocity());
	}
	
};