// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/VehicleEngine.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleEngine() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UVehicleEngine_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UVehicleEngine();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FShaft();
// End Cross Module References
	void UVehicleEngine::StaticRegisterNativesUVehicleEngine()
	{
	}
	UClass* Z_Construct_UClass_UVehicleEngine_NoRegister()
	{
		return UVehicleEngine::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleEngine_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CurrentEngineRPM_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_CurrentEngineRPM;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RedLineRPMRatio_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RedLineRPMRatio;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaxRPM_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_MaxRPM;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_IdleRPM_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_IdleRPM;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TorqueCurve_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TorqueCurve;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaxEngineTorque_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_MaxEngineTorque;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_EngineBrakingCoefficient_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_EngineBrakingCoefficient;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Shaft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Shaft;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVehicleEngine_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Vehicles/Physics/VehicleEngine.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_CurrentEngineRPM_MetaData[] = {
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_CurrentEngineRPM = { "CurrentEngineRPM", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, CurrentEngineRPM), METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_CurrentEngineRPM_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_CurrentEngineRPM_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_RedLineRPMRatio_MetaData[] = {
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_RedLineRPMRatio = { "RedLineRPMRatio", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, RedLineRPMRatio), METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_RedLineRPMRatio_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_RedLineRPMRatio_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxRPM_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxRPM = { "MaxRPM", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, MaxRPM), METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxRPM_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxRPM_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_IdleRPM_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_IdleRPM = { "IdleRPM", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, IdleRPM), METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_IdleRPM_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_IdleRPM_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_TorqueCurve_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_TorqueCurve = { "TorqueCurve", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, TorqueCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_TorqueCurve_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_TorqueCurve_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxEngineTorque_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxEngineTorque = { "MaxEngineTorque", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, MaxEngineTorque), METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxEngineTorque_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxEngineTorque_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_EngineBrakingCoefficient_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_EngineBrakingCoefficient = { "EngineBrakingCoefficient", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, EngineBrakingCoefficient), METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_EngineBrakingCoefficient_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_EngineBrakingCoefficient_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngine_Statics::NewProp_Shaft_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Engine" },
		{ "ModuleRelativePath", "Vehicles/Physics/VehicleEngine.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleEngine_Statics::NewProp_Shaft = { "Shaft", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UVehicleEngine, Shaft), Z_Construct_UScriptStruct_FShaft, METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_Shaft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::NewProp_Shaft_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleEngine_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_CurrentEngineRPM,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_RedLineRPMRatio,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxRPM,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_IdleRPM,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_TorqueCurve,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_MaxEngineTorque,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_EngineBrakingCoefficient,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngine_Statics::NewProp_Shaft,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleEngine_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleEngine>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UVehicleEngine_Statics::ClassParams = {
		&UVehicleEngine::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UVehicleEngine_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UVehicleEngine_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngine_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UVehicleEngine()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UVehicleEngine_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UVehicleEngine, 3039600951);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UVehicleEngine>()
	{
		return UVehicleEngine::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UVehicleEngine(Z_Construct_UClass_UVehicleEngine, &UVehicleEngine::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UVehicleEngine"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleEngine);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
