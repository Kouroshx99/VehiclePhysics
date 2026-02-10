// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/BodyState.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBodyState() {}
// Cross Module References
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FBodyState();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
class UScriptStruct* FBodyState::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern VEHICLEPHYSICS_API uint32 Get_Z_Construct_UScriptStruct_FBodyState_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FBodyState, Z_Construct_UPackage__Script_VehiclePhysics(), TEXT("BodyState"), sizeof(FBodyState), Get_Z_Construct_UScriptStruct_FBodyState_Hash());
	}
	return Singleton;
}
template<> VEHICLEPHYSICS_API UScriptStruct* StaticStruct<FBodyState>()
{
	return FBodyState::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FBodyState(FBodyState::StaticStruct, TEXT("/Script/VehiclePhysics"), TEXT("BodyState"), false, nullptr, nullptr);
static struct FScriptStruct_VehiclePhysics_StaticRegisterNativesFBodyState
{
	FScriptStruct_VehiclePhysics_StaticRegisterNativesFBodyState()
	{
		UScriptStruct::DeferCppStructOps<FBodyState>(FName(TEXT("BodyState")));
	}
} ScriptStruct_VehiclePhysics_StaticRegisterNativesFBodyState;
	struct Z_Construct_UScriptStruct_FBodyState_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FBodyState_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/BodyState.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FBodyState_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FBodyState>();
	}
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FBodyState_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
		nullptr,
		&NewStructOps,
		"BodyState",
		sizeof(FBodyState),
		alignof(FBodyState),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FBodyState_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FBodyState_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FBodyState()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FBodyState_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_VehiclePhysics();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("BodyState"), sizeof(FBodyState), Get_Z_Construct_UScriptStruct_FBodyState_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FBodyState_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FBodyState_Hash() { return 4216072361U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
