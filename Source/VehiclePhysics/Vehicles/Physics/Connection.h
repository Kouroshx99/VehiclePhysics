#pragma once

#include "CoreMinimal.h"
#include "Shaft.h" // Your FShaft struct
#include "Connection.generated.h"

UENUM()
enum class EDifferentialType : uint8
{
	Open UMETA(DisplayName = "EDT_Open"),
	Locked UMETA(DisplayName = "EDT_Locked"),
	LimitedSlip UMETA(DisplayName = "EDT_LimitedSlip"),
};

UENUM()
enum class EConstraintKind : uint8
{
	Gear UMETA(DisplayName = "ECK_Gear"),
	DiffCarrier UMETA(DisplayName = "ECK_DiffCarrier"),
	LimitedSlip UMETA(DisplayName = "ECK_LimitedSlip"),
	Brake UMETA(DisplayName = "ECK_Brake"),
};

USTRUCT()
struct FConstraintRef
{
	GENERATED_BODY()

	UPROPERTY()
	EConstraintKind Kind = EConstraintKind::Gear;

	UPROPERTY()
	int32 Index = INDEX_NONE;

	FConstraintRef() = default;

	FConstraintRef(EConstraintKind InKind, int32 InIndex)
		: Kind(InKind)
		, Index(InIndex)
	{
	}
};

USTRUCT(BlueprintType)
struct FGearConstraint
{
	GENERATED_USTRUCT_BODY()
	
	FShaft* Shaft1; // Input shaft
	FShaft* Shaft2; // Output shaft
	
	float Ratio = 1.f; // Gear ratio R = omega1 / omega2 ( >1 for reduction)
	float AccumJ = 0;
	float DeltaTime = 0.f;

	FGearConstraint() = default;
	
	// Take ownership of already-allocated shafts
	FGearConstraint(FShaft* InS1, FShaft* InS2, float R) :
		Shaft1(InS1), 
		Shaft2(InS2), 
		Ratio(R)
	{}
	
};

USTRUCT(BlueprintType)
struct FBrakeConstraint
{
	GENERATED_USTRUCT_BODY()
	
	FShaft* Shaft;
	
	float Ratio = 1.f; // Gear ratio R = omega1 / omega2 ( >1 for reduction)
	float AccumJ = 0;
	float DeltaTime = 0.f;
	float MaxCorrectionTorque = 0.f;
	float ConnectionStrength = 0.f;

	FBrakeConstraint() = default;
	// Take ownership of already-allocated shafts
	
	FBrakeConstraint(FShaft* InS, float R, float InMaxCorrectionTorque) :
		Shaft(InS), 
		Ratio(R), 
		MaxCorrectionTorque(InMaxCorrectionTorque)
	{}
};

USTRUCT(BlueprintType)
struct FDiffCarrierConstraint
{
	GENERATED_USTRUCT_BODY()
	
	FShaft* Input = nullptr;
	FShaft* Left = nullptr;
	FShaft* Right = nullptr;

	float Ratio = 1.f;

	float PreloadTorque = 0.f;     // for LSD
	float LockStrength = 0.f;      // for LSD/lock
	float MaxBiasRatio = 2.5f;     // for later Torsen-like behavior

	float AccumJ = 0.f;
	float DeltaTime = 0.f;
	
	FDiffCarrierConstraint() = default;
	
	FDiffCarrierConstraint(FShaft* InS1, FShaft* InS2, FShaft* InS3, float R)            
	: Input(InS1), Right(InS2), Left(InS3), Ratio(R) {}
};

USTRUCT()
struct FLimitedSlipConstraint
{
	GENERATED_BODY()

	FShaft* Left = nullptr;
	FShaft* Right = nullptr;

	int32 CarrierConstraintIndex = INDEX_NONE;

	float PreloadTorque = 0.f;
	float PowerLockCoeff = 0.f;
	float CoastLockCoeff = 0.f;
	float MaxCorrectionTorque = 0.f;

	float AccumJ = 0.f;
	float DeltaTime = 0.f;

	FLimitedSlipConstraint() = default;

	FLimitedSlipConstraint(
		FShaft* InLeft,
		FShaft* InRight,
		int32 InCarrierConstraintIndex,
		float InPreloadTorque,
		float InPowerLockCoeff,
		float InCoastLockCoeff,
		float InMaxCorrectionTorque)
		: Left(InLeft)
		, Right(InRight)
		, CarrierConstraintIndex(InCarrierConstraintIndex)
		, PreloadTorque(InPreloadTorque)
		, PowerLockCoeff(InPowerLockCoeff)
		, CoastLockCoeff(InCoastLockCoeff)
		, MaxCorrectionTorque(InMaxCorrectionTorque)
	{
	}
};