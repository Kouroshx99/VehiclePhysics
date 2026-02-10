// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Animations/CarAnimInstance.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCarAnimInstance() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UCarAnimInstance_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UCarAnimInstance();
	ENGINE_API UClass* Z_Construct_UClass_UAnimInstance();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_ACar_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
// End Cross Module References
	void UCarAnimInstance::StaticRegisterNativesUCarAnimInstance()
	{
	}
	UClass* Z_Construct_UClass_UCarAnimInstance_NoRegister()
	{
		return UCarAnimInstance::StaticClass();
	}
	struct Z_Construct_UClass_UCarAnimInstance_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Car_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Car;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontRightRotation_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontRightRotation;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontLeftRotation_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontLeftRotation;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearRightRotation_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RearRightRotation;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearLeftRotation_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RearLeftRotation;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontRightAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontRightAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontLeftAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontLeftAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontRightCenterZ_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontRightCenterZ;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontLeftCenterZ_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontLeftCenterZ;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearRightCenterZ_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RearRightCenterZ;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearLeftCenterZ_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RearLeftCenterZ;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontRightRoll_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontRightRoll;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontLeftRoll_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FrontLeftRoll;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearRightRoll_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RearRightRoll;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearLeftRoll_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RearLeftRoll;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontRight_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_FrontRight;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontLeft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_FrontLeft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearRight_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_RearRight;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearLeft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_RearLeft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PosFR_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_PosFR;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PosFL_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_PosFL;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PosRR_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_PosRR;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PosRL_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_PosRL;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RestFR_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RestFR;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RestFL_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RestFL;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RestRR_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RestRR;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RestRL_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RestRL;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCarAnimInstance_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAnimInstance,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "AnimInstance" },
		{ "IncludePath", "Vehicles/Animations/CarAnimInstance.h" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_Car_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_Car = { "Car", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, Car), Z_Construct_UClass_ACar_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_Car_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_Car_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRotation_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRotation = { "FrontRightRotation", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontRightRotation), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRotation_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRotation_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRotation_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRotation = { "FrontLeftRotation", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontLeftRotation), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRotation_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRotation_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRotation_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRotation = { "RearRightRotation", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearRightRotation), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRotation_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRotation_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRotation_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRotation = { "RearLeftRotation", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearLeftRotation), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRotation_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRotation_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightAngle_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightAngle = { "FrontRightAngle", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontRightAngle), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftAngle_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftAngle = { "FrontLeftAngle", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontLeftAngle), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightCenterZ_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightCenterZ = { "FrontRightCenterZ", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontRightCenterZ), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightCenterZ_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightCenterZ_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftCenterZ_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftCenterZ = { "FrontLeftCenterZ", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontLeftCenterZ), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftCenterZ_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftCenterZ_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightCenterZ_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightCenterZ = { "RearRightCenterZ", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearRightCenterZ), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightCenterZ_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightCenterZ_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftCenterZ_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftCenterZ = { "RearLeftCenterZ", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearLeftCenterZ), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftCenterZ_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftCenterZ_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRoll_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRoll = { "FrontRightRoll", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontRightRoll), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRoll_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRoll_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRoll_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRoll = { "FrontLeftRoll", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontLeftRoll), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRoll_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRoll_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRoll_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRoll = { "RearRightRoll", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearRightRoll), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRoll_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRoll_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRoll_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRoll = { "RearLeftRoll", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearLeftRoll), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRoll_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRoll_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRight_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRight = { "FrontRight", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontRight), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRight_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRight_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeft_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeft = { "FrontLeft", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, FrontLeft), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRight_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRight = { "RearRight", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearRight), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRight_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRight_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeft_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeft = { "RearLeft", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RearLeft), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFR_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFR = { "PosFR", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, PosFR), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFR_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFR_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFL_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFL = { "PosFL", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, PosFL), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFL_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFL_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRR_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRR = { "PosRR", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, PosRR), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRR_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRR_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRL_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRL = { "PosRL", nullptr, (EPropertyFlags)0x0040000000030015, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, PosRL), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRL_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRL_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFR_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFR = { "RestFR", nullptr, (EPropertyFlags)0x0040000000030001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RestFR), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFR_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFR_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFL_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFL = { "RestFL", nullptr, (EPropertyFlags)0x0040000000030001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RestFL), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFL_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFL_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRR_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRR = { "RestRR", nullptr, (EPropertyFlags)0x0040000000030001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RestRR), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRR_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRR_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRL_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Animations/CarAnimInstance.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRL = { "RestRL", nullptr, (EPropertyFlags)0x0040000000030001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCarAnimInstance, RestRL), METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRL_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRL_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCarAnimInstance_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_Car,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRotation,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRotation,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRotation,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRotation,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightCenterZ,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftCenterZ,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightCenterZ,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftCenterZ,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRightRoll,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeftRoll,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRightRoll,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeftRoll,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontRight,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_FrontLeft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearRight,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RearLeft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFR,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosFL,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRR,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_PosRL,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFR,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestFL,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRR,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCarAnimInstance_Statics::NewProp_RestRL,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCarAnimInstance_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCarAnimInstance>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UCarAnimInstance_Statics::ClassParams = {
		&UCarAnimInstance::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UCarAnimInstance_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::PropPointers),
		0,
		0x009000A8u,
		METADATA_PARAMS(Z_Construct_UClass_UCarAnimInstance_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCarAnimInstance_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCarAnimInstance()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UCarAnimInstance_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UCarAnimInstance, 3959160601);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UCarAnimInstance>()
	{
		return UCarAnimInstance::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UCarAnimInstance(Z_Construct_UClass_UCarAnimInstance, &UCarAnimInstance::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UCarAnimInstance"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCarAnimInstance);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
