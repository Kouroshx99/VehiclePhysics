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
	EDrivetrainType DrivetrainType = EDrivetrainType::EDT_FrontWheelDrive;

	UPROPERTY(EditAnywhere, Category="Drivetrain")
	UDifferential* FrontDifferential;

	UPROPERTY(EditAnywhere, Category="Drivetrain")
	UDifferential* RearDifferential;

	UPROPERTY(EditAnywhere, Category="Drivetrain", meta = (ClampMin="0.0", ClampMax="1.0"))
	float FrontRearSplit = 0.5f;
	
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

	TArray<FConnection> Connections;
	TArray<FConnection*> BrakeConnections;
	TArray<FConnection> CopyConnections;

private:

	///Torque from drive train (N.m)
	float Torque;

	float a = 0.f;
	float t = 0.f;

	int32 SolverIterations = 10;

	void InitializeShafts();
public:
	void SolveConstraints(TArray<FConnection>& Conns, float DT);
	FORCEINLINE TArray<FConnection> GetConnections() const
	{
		TArray<FConnection> Copy;
		Copy.Reserve(Connections.Num());

		for (const FConnection& Conn : Connections)
		{
			FConnection NewConn;

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