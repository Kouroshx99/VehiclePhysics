// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/WheelHit.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWheelHit() {}
// Cross Module References
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FWheelHit();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
class UScriptStruct* FWheelHit::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern VEHICLEPHYSICS_API uint32 Get_Z_Construct_UScriptStruct_FWheelHit_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FWheelHit, Z_Construct_UPackage__Script_VehiclePhysics(), TEXT("WheelHit"), sizeof(FWheelHit), Get_Z_Construct_UScriptStruct_FWheelHit_Hash());
	}
	return Singleton;
}
template<> VEHICLEPHYSICS_API UScriptStruct* StaticStruct<FWheelHit>()
{
	return FWheelHit::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FWheelHit(FWheelHit::StaticStruct, TEXT("/Script/VehiclePhysics"), TEXT("WheelHit"), false, nullptr, nullptr);
static struct FScriptStruct_VehiclePhysics_StaticRegisterNativesFWheelHit
{
	FScriptStruct_VehiclePhysics_StaticRegisterNativesFWheelHit()
	{
		UScriptStruct::DeferCppStructOps<FWheelHit>(FName(TEXT("WheelHit")));
	}
} ScriptStruct_VehiclePhysics_StaticRegisterNativesFWheelHit;
	struct Z_Construct_UScriptStruct_FWheelHit_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FWheelHit_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelHit.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FWheelHit_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FWheelHit>();
	}
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FWheelHit_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
		nullptr,
		&NewStructOps,
		"WheelHit",
		sizeof(FWheelHit),
		alignof(FWheelHit),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FWheelHit_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWheelHit_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FWheelHit()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FWheelHit_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_VehiclePhysics();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("WheelHit"), sizeof(FWheelHit), Get_Z_Construct_UScriptStruct_FWheelHit_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FWheelHit_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FWheelHit_Hash() { return 1926666241U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
