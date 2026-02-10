// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/Differential.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDifferential() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UDifferential_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UDifferential();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UWheelCollider_NoRegister();
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FShaft();
// End Cross Module References
	void UDifferential::StaticRegisterNativesUDifferential()
	{
	}
	UClass* Z_Construct_UClass_UDifferential_NoRegister()
	{
		return UDifferential::StaticClass();
	}
	struct Z_Construct_UClass_UDifferential_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RightWheel_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_RightWheel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LeftWheel_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_LeftWheel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Shaft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Shaft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bIsLocked_MetaData[];
#endif
		static void NewProp_bIsLocked_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIsLocked;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bIsTorsen_MetaData[];
#endif
		static void NewProp_bIsTorsen_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIsTorsen;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FinalDriveRatio_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FinalDriveRatio;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BiasRatio_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_BiasRatio;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TorsenSlipLimit_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TorsenSlipLimit;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TorsenPreLoad_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TorsenPreLoad;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bIsLimitedSlip_MetaData[];
#endif
		static void NewProp_bIsLimitedSlip_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bIsLimitedSlip;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UDifferential_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Vehicles/Physics/Differential.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_RightWheel_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_RightWheel = { "RightWheel", nullptr, (EPropertyFlags)0x0010000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDifferential, RightWheel), Z_Construct_UClass_UWheelCollider_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_RightWheel_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_RightWheel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_LeftWheel_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_LeftWheel = { "LeftWheel", nullptr, (EPropertyFlags)0x0010000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDifferential, LeftWheel), Z_Construct_UClass_UWheelCollider_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_LeftWheel_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_LeftWheel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_Shaft_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_Shaft = { "Shaft", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDifferential, Shaft), Z_Construct_UScriptStruct_FShaft, METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_Shaft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_Shaft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLocked_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	void Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLocked_SetBit(void* Obj)
	{
		((UDifferential*)Obj)->bIsLocked = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLocked = { "bIsLocked", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UDifferential), &Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLocked_SetBit, METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLocked_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLocked_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_bIsTorsen_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	void Z_Construct_UClass_UDifferential_Statics::NewProp_bIsTorsen_SetBit(void* Obj)
	{
		((UDifferential*)Obj)->bIsTorsen = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_bIsTorsen = { "bIsTorsen", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UDifferential), &Z_Construct_UClass_UDifferential_Statics::NewProp_bIsTorsen_SetBit, METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_bIsTorsen_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_bIsTorsen_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_FinalDriveRatio_MetaData[] = {
		{ "AloowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_FinalDriveRatio = { "FinalDriveRatio", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDifferential, FinalDriveRatio), METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_FinalDriveRatio_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_FinalDriveRatio_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_BiasRatio_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_BiasRatio = { "BiasRatio", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDifferential, BiasRatio), METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_BiasRatio_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_BiasRatio_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenSlipLimit_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenSlipLimit = { "TorsenSlipLimit", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDifferential, TorsenSlipLimit), METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenSlipLimit_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenSlipLimit_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenPreLoad_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenPreLoad = { "TorsenPreLoad", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDifferential, TorsenPreLoad), METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenPreLoad_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenPreLoad_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLimitedSlip_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Differential" },
		{ "ModuleRelativePath", "Vehicles/Physics/Differential.h" },
	};
#endif
	void Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLimitedSlip_SetBit(void* Obj)
	{
		((UDifferential*)Obj)->bIsLimitedSlip = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLimitedSlip = { "bIsLimitedSlip", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UDifferential), &Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLimitedSlip_SetBit, METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLimitedSlip_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLimitedSlip_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UDifferential_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_RightWheel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_LeftWheel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_Shaft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLocked,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_bIsTorsen,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_FinalDriveRatio,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_BiasRatio,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenSlipLimit,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_TorsenPreLoad,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDifferential_Statics::NewProp_bIsLimitedSlip,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UDifferential_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDifferential>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UDifferential_Statics::ClassParams = {
		&UDifferential::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UDifferential_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UDifferential_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UDifferential_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UDifferential()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UDifferential_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UDifferential, 2211009815);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UDifferential>()
	{
		return UDifferential::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UDifferential(Z_Construct_UClass_UDifferential, &UDifferential::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UDifferential"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UDifferential);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
