// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/WheelCollider.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWheelCollider() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UWheelCollider_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UWheelCollider();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UTireParamsDataAsset_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FShaft();
// End Cross Module References
	void UWheelCollider::StaticRegisterNativesUWheelCollider()
	{
	}
	UClass* Z_Construct_UClass_UWheelCollider_NoRegister()
	{
		return UWheelCollider::StaticClass();
	}
	struct Z_Construct_UClass_UWheelCollider_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TireParams_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TireParams;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WheelRadius_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_WheelRadius;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SteeringResponsiveness_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SteeringResponsiveness;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AngVelItersSlip_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_AngVelItersSlip;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AngVelItersNoSlip_MetaData[];
#endif
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_AngVelItersNoSlip;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SpringStiffness_NM_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SpringStiffness_NM;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DamperCoefficient_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_DamperCoefficient;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AntiRollStiffness_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_AntiRollStiffness;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RestLenght_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RestLenght;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MinSpringTravel_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_MinSpringTravel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaxSpringTravel_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_MaxSpringTravel;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CasterAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_CasterAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_StaticCamberAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_StaticCamberAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ToeAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ToeAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_KingPinAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_KingPinAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HubOffset_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_HubOffset;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SATC_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SATC;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bDebug_MetaData[];
#endif
		static void NewProp_bDebug_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bDebug;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WheelShaft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_WheelShaft;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWheelCollider_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_USceneComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "HideCategories", "Trigger PhysicsVolume" },
		{ "IncludePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_TireParams_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Wheel" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_TireParams = { "TireParams", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, TireParams), Z_Construct_UClass_UTireParamsDataAsset_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_TireParams_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_TireParams_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelRadius_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Wheel" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "Units", "cm" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelRadius = { "WheelRadius", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, WheelRadius), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelRadius_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelRadius_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_SteeringResponsiveness_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Wheel" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_SteeringResponsiveness = { "SteeringResponsiveness", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, SteeringResponsiveness), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_SteeringResponsiveness_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_SteeringResponsiveness_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersSlip_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Wheel" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersSlip = { "AngVelItersSlip", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, AngVelItersSlip), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersSlip_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersSlip_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersNoSlip_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Wheel" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersNoSlip = { "AngVelItersNoSlip", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, AngVelItersNoSlip), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersNoSlip_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersNoSlip_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_SpringStiffness_NM_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "/// Spring stiffness [N/m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Spring stiffness [N/m]" },
		{ "Units", "Newtons" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_SpringStiffness_NM = { "SpringStiffness_NM", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, SpringStiffness_NM), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_SpringStiffness_NM_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_SpringStiffness_NM_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_DamperCoefficient_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "/// Damper coefficient [N\xc2\xb7s/m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Damper coefficient [N\xc2\xb7s/m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_DamperCoefficient = { "DamperCoefficient", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, DamperCoefficient), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_DamperCoefficient_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_DamperCoefficient_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_AntiRollStiffness_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "/// Anti-roll bar stiffness [N/m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Anti-roll bar stiffness [N/m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_AntiRollStiffness = { "AntiRollStiffness", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, AntiRollStiffness), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_AntiRollStiffness_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_AntiRollStiffness_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_RestLenght_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "/// Distance from the strut mount at which the suspension exerts no force [cm]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Distance from the strut mount at which the suspension exerts no force [cm]" },
		{ "Units", "cm" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_RestLenght = { "RestLenght", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, RestLenght), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_RestLenght_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_RestLenght_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_MinSpringTravel_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "/// Maximum compression of suspension [cm]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Maximum compression of suspension [cm]" },
		{ "Units", "cm" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_MinSpringTravel = { "MinSpringTravel", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, MinSpringTravel), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_MinSpringTravel_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_MinSpringTravel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_MaxSpringTravel_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "/// Maximum compression of suspension [cm]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Maximum compression of suspension [cm]" },
		{ "Units", "cm" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_MaxSpringTravel = { "MaxSpringTravel", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, MaxSpringTravel), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_MaxSpringTravel_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_MaxSpringTravel_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_CasterAngle_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "Units", "deg" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_CasterAngle = { "CasterAngle", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, CasterAngle), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_CasterAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_CasterAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_StaticCamberAngle_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "Units", "deg" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_StaticCamberAngle = { "StaticCamberAngle", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, StaticCamberAngle), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_StaticCamberAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_StaticCamberAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_ToeAngle_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "/// Degrees, positive = top tilted out\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Degrees, positive = top tilted out" },
		{ "Units", "deg" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_ToeAngle = { "ToeAngle", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, ToeAngle), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_ToeAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_ToeAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_KingPinAngle_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "// Degrees, positive = toe-in\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Degrees, positive = toe-in" },
		{ "Units", "deg" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_KingPinAngle = { "KingPinAngle", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, KingPinAngle), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_KingPinAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_KingPinAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_HubOffset_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Suspension" },
		{ "Comment", "// Degrees, typical 5-15, positive inward tilt\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
		{ "ToolTip", "Degrees, typical 5-15, positive inward tilt" },
		{ "Units", "cm" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_HubOffset = { "HubOffset", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, HubOffset), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_HubOffset_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_HubOffset_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_SATC_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Electronics" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_SATC = { "SATC", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, SATC), METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_SATC_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_SATC_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_bDebug_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
	void Z_Construct_UClass_UWheelCollider_Statics::NewProp_bDebug_SetBit(void* Obj)
	{
		((UWheelCollider*)Obj)->bDebug = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_bDebug = { "bDebug", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UWheelCollider), &Z_Construct_UClass_UWheelCollider_Statics::NewProp_bDebug_SetBit, METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_bDebug_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_bDebug_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelShaft_MetaData[] = {
		{ "ModuleRelativePath", "Vehicles/Physics/WheelCollider.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelShaft = { "WheelShaft", nullptr, (EPropertyFlags)0x0040000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UWheelCollider, WheelShaft), Z_Construct_UScriptStruct_FShaft, METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelShaft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelShaft_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWheelCollider_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_TireParams,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelRadius,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_SteeringResponsiveness,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersSlip,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_AngVelItersNoSlip,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_SpringStiffness_NM,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_DamperCoefficient,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_AntiRollStiffness,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_RestLenght,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_MinSpringTravel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_MaxSpringTravel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_CasterAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_StaticCamberAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_ToeAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_KingPinAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_HubOffset,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_SATC,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_bDebug,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWheelCollider_Statics::NewProp_WheelShaft,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWheelCollider_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWheelCollider>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UWheelCollider_Statics::ClassParams = {
		&UWheelCollider::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWheelCollider_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UWheelCollider_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWheelCollider_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWheelCollider()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UWheelCollider_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UWheelCollider, 121549156);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UWheelCollider>()
	{
		return UWheelCollider::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UWheelCollider(Z_Construct_UClass_UWheelCollider, &UWheelCollider::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UWheelCollider"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWheelCollider);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
