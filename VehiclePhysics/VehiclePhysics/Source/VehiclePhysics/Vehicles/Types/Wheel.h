#pragma once

UENUM(BlueprintType)
enum class EWheel: uint8
{

	EW_FrontRight UMETA(DisplayName="FrontRight"),
	EW_FrontLeft UMETA(DisplayName="FrontLeft"),
	EW_RearRight UMETA(DisplayName="RearRight"),
	EW_RearLeft UMETA(DisplayName="RearLeft"),

	EW_MAX UMETA(DisplayName="DefaultMax")
	
};
