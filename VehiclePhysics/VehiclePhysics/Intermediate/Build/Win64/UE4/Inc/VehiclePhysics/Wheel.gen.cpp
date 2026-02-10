// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Types/Wheel.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWheel() {}
// Cross Module References
	VEHICLEPHYSICS_API UEnum* Z_Construct_UEnum_VehiclePhysics_EWheel();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
	static UEnum* EWheel_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_VehiclePhysics_EWheel, Z_Construct_UPackage__Script_VehiclePhysics(), TEXT("EWheel"));
		}
		return Singleton;
	}
	template<> VEHICLEPHYSICS_API UEnum* StaticEnum<EWheel>()
	{
		return EWheel_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EWheel(EWheel_StaticEnum, TEXT("/Script/VehiclePhysics"), TEXT("EWheel"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_VehiclePhysics_EWheel_Hash() { return 3001932220U; }
	UEnum* Z_Construct_UEnum_VehiclePhysics_EWheel()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_VehiclePhysics();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EWheel"), 0, Get_Z_Construct_UEnum_VehiclePhysics_EWheel_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EWheel::EW_FrontRight", (int64)EWheel::EW_FrontRight },
				{ "EWheel::EW_FrontLeft", (int64)EWheel::EW_FrontLeft },
				{ "EWheel::EW_RearRight", (int64)EWheel::EW_RearRight },
				{ "EWheel::EW_RearLeft", (int64)EWheel::EW_RearLeft },
				{ "EWheel::EW_MAX", (int64)EWheel::EW_MAX },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "EW_FrontLeft.DisplayName", "FrontLeft" },
				{ "EW_FrontLeft.Name", "EWheel::EW_FrontLeft" },
				{ "EW_FrontRight.DisplayName", "FrontRight" },
				{ "EW_FrontRight.Name", "EWheel::EW_FrontRight" },
				{ "EW_MAX.DisplayName", "DefaultMax" },
				{ "EW_MAX.Name", "EWheel::EW_MAX" },
				{ "EW_RearLeft.DisplayName", "RearLeft" },
				{ "EW_RearLeft.Name", "EWheel::EW_RearLeft" },
				{ "EW_RearRight.DisplayName", "RearRight" },
				{ "EW_RearRight.Name", "EWheel::EW_RearRight" },
				{ "ModuleRelativePath", "Vehicles/Types/Wheel.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_VehiclePhysics,
				nullptr,
				"EWheel",
				"EWheel",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
