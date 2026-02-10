// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/Drivetrain.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDrivetrain() {}
// Cross Module References
	VEHICLEPHYSICS_API UEnum* Z_Construct_UEnum_VehiclePhysics_EDrivetrainType();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UDrivetrain_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UDrivetrain();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UDifferential_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UVehicleEngine_NoRegister();
// End Cross Module References
	static UEnum* EDrivetrainType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_VehiclePhysics_EDrivetrainType, Z_Construct_UPackage__Script_VehiclePhysics(), TEXT("EDrivetrainType"));
		}
		return Singleton;
	}
	template<> VEHICLEPHYSICS_API UEnum* StaticEnum<EDrivetrainType>()
	{
		return EDrivetrainType_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EDrivetrainType(EDrivetrainType_StaticEnum, TEXT("/Script/VehiclePhysics"), TEXT("EDrivetrainType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_VehiclePhysics_EDrivetrainType_Hash() { return 4161002985U; }
	UEnum* Z_Construct_UEnum_VehiclePhysics_EDrivetrainType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_VehiclePhysics();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EDrivetrainType"), 0, Get_Z_Construct_UEnum_VehiclePhysics_EDrivetrainType_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EDrivetrainType::EDT_FrontWheelDrive", (int64)EDrivetrainType::EDT_FrontWheelDrive },
				{ "EDrivetrainType::EDT_RearWheelDrive", (int64)EDrivetrainType::EDT_RearWheelDrive },
				{ "EDrivetrainType::EDT_AllWheelDrive", (int64)EDrivetrainType::EDT_AllWheelDrive },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "EDT_AllWheelDrive.DisplayName", "AllWheelDrive" },
				{ "EDT_AllWheelDrive.Name", "EDrivetrainType::EDT_AllWheelDrive" },
				{ "EDT_FrontWheelDrive.DisplayName", "FrontWheelDrive" },
				{ "EDT_FrontWheelDrive.Name", "EDrivetrainType::EDT_FrontWheelDrive" },
				{ "EDT_RearWheelDrive.DisplayName", "RearWheelDrive" },
				{ "EDT_RearWheelDrive.Name", "EDrivetrainType::EDT_RearWheelDrive" },
				{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_VehiclePhysics,
				nullptr,
				"EDrivetrainType",
				"EDrivetrainType",
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
	void UDrivetrain::StaticRegisterNativesUDrivetrain()
	{
	}
	UClass* Z_Construct_UClass_UDrivetrain_NoRegister()
	{
		return UDrivetrain::StaticClass();
	}
	struct Z_Construct_UClass_UDrivetrain_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_DrivetrainType_Underlying;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DrivetrainType_MetaData[];
#endif
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_DrivetrainType;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontDifferential_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_FrontDifferential;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearDifferential_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_RearDifferential;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontRearSplit_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontRearSplit;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FinalDriveRatio_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FinalDriveRatio;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_GearRatios_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_GearRatios_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_GearRatios;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_VehicleEngine_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_VehicleEngine;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UDrivetrain_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Vehicles/Physics/Drivetrain.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
	};
#endif
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_DrivetrainType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::NewProp_DrivetrainType_MetaData[] = {
		{ "Category", "Drivetrain" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
	};
#endif
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_DrivetrainType = { "DrivetrainType", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDrivetrain, DrivetrainType), Z_Construct_UEnum_VehiclePhysics_EDrivetrainType, METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::NewProp_DrivetrainType_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::NewProp_DrivetrainType_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontDifferential_MetaData[] = {
		{ "Category", "Drivetrain" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontDifferential = { "FrontDifferential", nullptr, (EPropertyFlags)0x0010000000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDrivetrain, FrontDifferential), Z_Construct_UClass_UDifferential_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontDifferential_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontDifferential_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::NewProp_RearDifferential_MetaData[] = {
		{ "Category", "Drivetrain" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_RearDifferential = { "RearDifferential", nullptr, (EPropertyFlags)0x0010000000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDrivetrain, RearDifferential), Z_Construct_UClass_UDifferential_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::NewProp_RearDifferential_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::NewProp_RearDifferential_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontRearSplit_MetaData[] = {
		{ "Category", "Drivetrain" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontRearSplit = { "FrontRearSplit", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDrivetrain, FrontRearSplit), METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontRearSplit_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontRearSplit_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::NewProp_FinalDriveRatio_MetaData[] = {
		{ "Category", "Drivetrain" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_FinalDriveRatio = { "FinalDriveRatio", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDrivetrain, FinalDriveRatio), METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::NewProp_FinalDriveRatio_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::NewProp_FinalDriveRatio_MetaData)) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_GearRatios_Inner = { "GearRatios", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::NewProp_GearRatios_MetaData[] = {
		{ "Category", "Drivetrain" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_GearRatios = { "GearRatios", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDrivetrain, GearRatios), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::NewProp_GearRatios_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::NewProp_GearRatios_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UDrivetrain_Statics::NewProp_VehicleEngine_MetaData[] = {
		{ "Comment", "//Shafts for the chain\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Drivetrain.h" },
		{ "ToolTip", "Shafts for the chain" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UDrivetrain_Statics::NewProp_VehicleEngine = { "VehicleEngine", nullptr, (EPropertyFlags)0x0010000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UDrivetrain, VehicleEngine), Z_Construct_UClass_UVehicleEngine_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::NewProp_VehicleEngine_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::NewProp_VehicleEngine_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UDrivetrain_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_DrivetrainType_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_DrivetrainType,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontDifferential,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_RearDifferential,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_FrontRearSplit,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_FinalDriveRatio,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_GearRatios_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_GearRatios,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UDrivetrain_Statics::NewProp_VehicleEngine,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UDrivetrain_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDrivetrain>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UDrivetrain_Statics::ClassParams = {
		&UDrivetrain::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UDrivetrain_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UDrivetrain_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UDrivetrain_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UDrivetrain()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UDrivetrain_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UDrivetrain, 2579072162);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UDrivetrain>()
	{
		return UDrivetrain::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UDrivetrain(Z_Construct_UClass_UDrivetrain, &UDrivetrain::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UDrivetrain"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UDrivetrain);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
