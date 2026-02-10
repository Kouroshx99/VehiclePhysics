// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/ECU.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeECU() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UECU_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UECU();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
	void UECU::StaticRegisterNativesUECU()
	{
	}
	UClass* Z_Construct_UClass_UECU_NoRegister()
	{
		return UECU::StaticClass();
	}
	struct Z_Construct_UClass_UECU_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bUseTCS_MetaData[];
#endif
		static void NewProp_bUseTCS_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bUseTCS;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TCSLimit_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_TCSLimit;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bUseABS_MetaData[];
#endif
		static void NewProp_bUseABS_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bUseABS;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ABSSlipTarget_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ABSSlipTarget;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ABSReduceRate_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ABSReduceRate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ABSIncreaseRate_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ABSIncreaseRate;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UECU_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UECU_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "Vehicles/Physics/ECU.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/ECU.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UECU_Statics::NewProp_bUseTCS_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Electronics" },
		{ "ModuleRelativePath", "Vehicles/Physics/ECU.h" },
	};
#endif
	void Z_Construct_UClass_UECU_Statics::NewProp_bUseTCS_SetBit(void* Obj)
	{
		((UECU*)Obj)->bUseTCS = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UECU_Statics::NewProp_bUseTCS = { "bUseTCS", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UECU), &Z_Construct_UClass_UECU_Statics::NewProp_bUseTCS_SetBit, METADATA_PARAMS(Z_Construct_UClass_UECU_Statics::NewProp_bUseTCS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::NewProp_bUseTCS_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UECU_Statics::NewProp_TCSLimit_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Electronics" },
		{ "ModuleRelativePath", "Vehicles/Physics/ECU.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UECU_Statics::NewProp_TCSLimit = { "TCSLimit", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UECU, TCSLimit), METADATA_PARAMS(Z_Construct_UClass_UECU_Statics::NewProp_TCSLimit_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::NewProp_TCSLimit_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UECU_Statics::NewProp_bUseABS_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Electronics" },
		{ "ModuleRelativePath", "Vehicles/Physics/ECU.h" },
	};
#endif
	void Z_Construct_UClass_UECU_Statics::NewProp_bUseABS_SetBit(void* Obj)
	{
		((UECU*)Obj)->bUseABS = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UECU_Statics::NewProp_bUseABS = { "bUseABS", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(UECU), &Z_Construct_UClass_UECU_Statics::NewProp_bUseABS_SetBit, METADATA_PARAMS(Z_Construct_UClass_UECU_Statics::NewProp_bUseABS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::NewProp_bUseABS_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UECU_Statics::NewProp_ABSSlipTarget_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Electronics" },
		{ "ModuleRelativePath", "Vehicles/Physics/ECU.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UECU_Statics::NewProp_ABSSlipTarget = { "ABSSlipTarget", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UECU, ABSSlipTarget), METADATA_PARAMS(Z_Construct_UClass_UECU_Statics::NewProp_ABSSlipTarget_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::NewProp_ABSSlipTarget_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UECU_Statics::NewProp_ABSReduceRate_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Electronics" },
		{ "Comment", "// Optimal slip ratio\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/ECU.h" },
		{ "ToolTip", "Optimal slip ratio" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UECU_Statics::NewProp_ABSReduceRate = { "ABSReduceRate", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UECU, ABSReduceRate), METADATA_PARAMS(Z_Construct_UClass_UECU_Statics::NewProp_ABSReduceRate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::NewProp_ABSReduceRate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UECU_Statics::NewProp_ABSIncreaseRate_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Electronics" },
		{ "Comment", "// How fast to reduce torque\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/ECU.h" },
		{ "ToolTip", "How fast to reduce torque" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UECU_Statics::NewProp_ABSIncreaseRate = { "ABSIncreaseRate", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UECU, ABSIncreaseRate), METADATA_PARAMS(Z_Construct_UClass_UECU_Statics::NewProp_ABSIncreaseRate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::NewProp_ABSIncreaseRate_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UECU_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UECU_Statics::NewProp_bUseTCS,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UECU_Statics::NewProp_TCSLimit,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UECU_Statics::NewProp_bUseABS,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UECU_Statics::NewProp_ABSSlipTarget,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UECU_Statics::NewProp_ABSReduceRate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UECU_Statics::NewProp_ABSIncreaseRate,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UECU_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UECU>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UECU_Statics::ClassParams = {
		&UECU::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UECU_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UECU_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UECU_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UECU()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UECU_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UECU, 886491676);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UECU>()
	{
		return UECU::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UECU(Z_Construct_UClass_UECU, &UECU::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UECU"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UECU);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
