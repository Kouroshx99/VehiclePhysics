// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/WheelSolverData.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWheelSolverData() {}
// Cross Module References
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FWheelSolverData();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
class UScriptStruct* FWheelSolverData::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern VEHICLEPHYSICS_API uint32 Get_Z_Construct_UScriptStruct_FWheelSolverData_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FWheelSolverData, Z_Construct_UPackage__Script_VehiclePhysics(), TEXT("WheelSolverData"), sizeof(FWheelSolverData), Get_Z_Construct_UScriptStruct_FWheelSolverData_Hash());
	}
	return Singleton;
}
template<> VEHICLEPHYSICS_API UScriptStruct* StaticStruct<FWheelSolverData>()
{
	return FWheelSolverData::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FWheelSolverData(FWheelSolverData::StaticStruct, TEXT("/Script/VehiclePhysics"), TEXT("WheelSolverData"), false, nullptr, nullptr);
static struct FScriptStruct_VehiclePhysics_StaticRegisterNativesFWheelSolverData
{
	FScriptStruct_VehiclePhysics_StaticRegisterNativesFWheelSolverData()
	{
		UScriptStruct::DeferCppStructOps<FWheelSolverData>(FName(TEXT("WheelSolverData")));
	}
} ScriptStruct_VehiclePhysics_StaticRegisterNativesFWheelSolverData;
	struct Z_Construct_UScriptStruct_FWheelSolverData_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FWheelSolverData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelSolverData.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FWheelSolverData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FWheelSolverData>();
	}
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FWheelSolverData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
		nullptr,
		&NewStructOps,
		"WheelSolverData",
		sizeof(FWheelSolverData),
		alignof(FWheelSolverData),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FWheelSolverData_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FWheelSolverData_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FWheelSolverData()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FWheelSolverData_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_VehiclePhysics();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("WheelSolverData"), sizeof(FWheelSolverData), Get_Z_Construct_UScriptStruct_FWheelSolverData_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FWheelSolverData_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FWheelSolverData_Hash() { return 3245849476U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
