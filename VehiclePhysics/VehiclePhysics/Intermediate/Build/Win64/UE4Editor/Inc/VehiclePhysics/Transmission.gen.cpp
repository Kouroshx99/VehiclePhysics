// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/Transmission.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTransmission() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UTransmission_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UTransmission();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FShaft();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UDrivetrain_NoRegister();
// End Cross Module References
	void UTransmission::StaticRegisterNativesUTransmission()
	{
	}
	UClass* Z_Construct_UClass_UTransmission_NoRegister()
	{
		return UTransmission::StaticClass();
	}
	struct Z_Construct_UClass_UTransmission_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Shaft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_Shaft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Drivetrain_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Drivetrain;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_GearRatios_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_GearRatios_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_GearRatios;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TransmissionEfficiency_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TransmissionEfficiency;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTransmission_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTransmission_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Vehicles/Physics/Transmission.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/Transmission.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTransmission_Statics::NewProp_Shaft_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Transmission" },
		{ "ModuleRelativePath", "Vehicles/Physics/Transmission.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UTransmission_Statics::NewProp_Shaft = { "Shaft", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTransmission, Shaft), Z_Construct_UScriptStruct_FShaft, METADATA_PARAMS(Z_Construct_UClass_UTransmission_Statics::NewProp_Shaft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTransmission_Statics::NewProp_Shaft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTransmission_Statics::NewProp_Drivetrain_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Transmission.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UTransmission_Statics::NewProp_Drivetrain = { "Drivetrain", nullptr, (EPropertyFlags)0x0010000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTransmission, Drivetrain), Z_Construct_UClass_UDrivetrain_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UTransmission_Statics::NewProp_Drivetrain_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTransmission_Statics::NewProp_Drivetrain_MetaData)) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTransmission_Statics::NewProp_GearRatios_Inner = { "GearRatios", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTransmission_Statics::NewProp_GearRatios_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Transmission" },
		{ "ModuleRelativePath", "Vehicles/Physics/Transmission.h" },
		{ "Tooltip", "First ratio in array is reverse gear" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UTransmission_Statics::NewProp_GearRatios = { "GearRatios", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTransmission, GearRatios), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UTransmission_Statics::NewProp_GearRatios_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTransmission_Statics::NewProp_GearRatios_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTransmission_Statics::NewProp_TransmissionEfficiency_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Transmission" },
		{ "ModuleRelativePath", "Vehicles/Physics/Transmission.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTransmission_Statics::NewProp_TransmissionEfficiency = { "TransmissionEfficiency", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTransmission, TransmissionEfficiency), METADATA_PARAMS(Z_Construct_UClass_UTransmission_Statics::NewProp_TransmissionEfficiency_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTransmission_Statics::NewProp_TransmissionEfficiency_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UTransmission_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTransmission_Statics::NewProp_Shaft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTransmission_Statics::NewProp_Drivetrain,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTransmission_Statics::NewProp_GearRatios_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTransmission_Statics::NewProp_GearRatios,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTransmission_Statics::NewProp_TransmissionEfficiency,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTransmission_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTransmission>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTransmission_Statics::ClassParams = {
		&UTransmission::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UTransmission_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UTransmission_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UTransmission_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTransmission_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTransmission()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTransmission_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTransmission, 1151804820);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UTransmission>()
	{
		return UTransmission::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTransmission(Z_Construct_UClass_UTransmission, &UTransmission::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UTransmission"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTransmission);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
