// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/Connection.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeConnection() {}
// Cross Module References
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FConnection();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
class UScriptStruct* FConnection::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern VEHICLEPHYSICS_API uint32 Get_Z_Construct_UScriptStruct_FConnection_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FConnection, Z_Construct_UPackage__Script_VehiclePhysics(), TEXT("Connection"), sizeof(FConnection), Get_Z_Construct_UScriptStruct_FConnection_Hash());
	}
	return Singleton;
}
template<> VEHICLEPHYSICS_API UScriptStruct* StaticStruct<FConnection>()
{
	return FConnection::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FConnection(FConnection::StaticStruct, TEXT("/Script/VehiclePhysics"), TEXT("Connection"), false, nullptr, nullptr);
static struct FScriptStruct_VehiclePhysics_StaticRegisterNativesFConnection
{
	FScriptStruct_VehiclePhysics_StaticRegisterNativesFConnection()
	{
		UScriptStruct::DeferCppStructOps<FConnection>(FName(TEXT("Connection")));
	}
} ScriptStruct_VehiclePhysics_StaticRegisterNativesFConnection;
	struct Z_Construct_UScriptStruct_FConnection_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FConnection_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Connection.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FConnection_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FConnection>();
	}
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FConnection_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
		nullptr,
		&NewStructOps,
		"Connection",
		sizeof(FConnection),
		alignof(FConnection),
		nullptr,
		0,
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FConnection_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FConnection_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FConnection()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FConnection_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_VehiclePhysics();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("Connection"), sizeof(FConnection), Get_Z_Construct_UScriptStruct_FConnection_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FConnection_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FConnection_Hash() { return 549224276U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
