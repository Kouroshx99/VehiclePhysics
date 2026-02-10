// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/Shaft.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeShaft() {}
// Cross Module References
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FShaft();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
class UScriptStruct* FShaft::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern VEHICLEPHYSICS_API uint32 Get_Z_Construct_UScriptStruct_FShaft_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FShaft, Z_Construct_UPackage__Script_VehiclePhysics(), TEXT("Shaft"), sizeof(FShaft), Get_Z_Construct_UScriptStruct_FShaft_Hash());
	}
	return Singleton;
}
template<> VEHICLEPHYSICS_API UScriptStruct* StaticStruct<FShaft>()
{
	return FShaft::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FShaft(FShaft::StaticStruct, TEXT("/Script/VehiclePhysics"), TEXT("Shaft"), false, nullptr, nullptr);
static struct FScriptStruct_VehiclePhysics_StaticRegisterNativesFShaft
{
	FScriptStruct_VehiclePhysics_StaticRegisterNativesFShaft()
	{
		UScriptStruct::DeferCppStructOps<FShaft>(FName(TEXT("Shaft")));
	}
} ScriptStruct_VehiclePhysics_StaticRegisterNativesFShaft;
	struct Z_Construct_UScriptStruct_FShaft_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Inertia_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Inertia;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FShaft_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Shaft.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FShaft_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FShaft>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FShaft_Statics::NewProp_Inertia_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "DriveShaft" },
		{ "Comment", "///Rotational inertia (kg*m^2).\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/Shaft.h" },
		{ "ToolTip", "Rotational inertia (kg*m^2)." },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FShaft_Statics::NewProp_Inertia = { "Inertia", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FShaft, Inertia), METADATA_PARAMS(Z_Construct_UScriptStruct_FShaft_Statics::NewProp_Inertia_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FShaft_Statics::NewProp_Inertia_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FShaft_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FShaft_Statics::NewProp_Inertia,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FShaft_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
		nullptr,
		&NewStructOps,
		"Shaft",
		sizeof(FShaft),
		alignof(FShaft),
		Z_Construct_UScriptStruct_FShaft_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FShaft_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FShaft_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FShaft_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FShaft()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FShaft_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_VehiclePhysics();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("Shaft"), sizeof(FShaft), Get_Z_Construct_UScriptStruct_FShaft_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FShaft_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FShaft_Hash() { return 2501330168U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
