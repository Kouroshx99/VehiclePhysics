#pragma once

#include "CoreMinimal.h"
#include "Shaft.h" // Your FShaft struct
#include "Connection.generated.h"

USTRUCT(BlueprintType)
struct FConnection
{
	GENERATED_USTRUCT_BODY()
	enum class EConnectionType : uint8
	{
		Locked UMETA(DisplayName = "ECT_Locked"),
		Brake UMETA(DisplayName = "ECT_Brake"),
	};
	EConnectionType ConnectionType = EConnectionType::Locked;
	FShaft* Shaft1; // Input shaft
	FShaft* Shaft2; // Output shaft
	float Ratio = 1.f; // Gear ratio R = omega1 / omega2 ( >1 for reduction)
	float AccumJ = 0;
	float DeltaTime = 0.f;
	float MaxCorrectionTorque = 0.f;
	float ConnectionStrength = 0.f;

	FConnection() : Shaft1(nullptr), Shaft2(nullptr) {}
	// Take ownership of already-allocated shafts
	FConnection(FShaft* InS1, FShaft* InS2, float R, EConnectionType InECT)
		: Shaft1(InS1), Shaft2(InS2), Ratio(R), ConnectionType(InECT) {}
	
	FConnection(FShaft* InS1, FShaft* InS2, float R, float InMaxCorrectionTorque, EConnectionType InECT)
		: Shaft1(InS1), Shaft2(InS2), Ratio(R), MaxCorrectionTorque(InMaxCorrectionTorque), ConnectionType(InECT) {}
};