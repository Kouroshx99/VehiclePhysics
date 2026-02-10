// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/Clutch.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeClutch() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UClutch_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UClutch();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
	void UClutch::StaticRegisterNativesUClutch()
	{
	}
	UClass* Z_Construct_UClass_UClutch_NoRegister()
	{
		return UClutch::StaticClass();
	}
	struct Z_Construct_UClass_UClutch_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaxTorque_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_MaxTorque;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UClutch_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UClutch_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Vehicles/Physics/Clutch.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Clutch.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UClutch_Statics::NewProp_MaxTorque_MetaData[] = {
		{ "Category", "Clutch" },
		{ "Comment", "/// The maximum torque the clutch can handle (N\xc2\xb7m).\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/Clutch.h" },
		{ "ToolTip", "The maximum torque the clutch can handle (N\xc2\xb7m)." },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UClutch_Statics::NewProp_MaxTorque = { "MaxTorque", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UClutch, MaxTorque), METADATA_PARAMS(Z_Construct_UClass_UClutch_Statics::NewProp_MaxTorque_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UClutch_Statics::NewProp_MaxTorque_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UClutch_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UClutch_Statics::NewProp_MaxTorque,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UClutch_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UClutch>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UClutch_Statics::ClassParams = {
		&UClutch::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UClutch_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UClutch_Statics::PropPointers),
		0,
		0x001010A0u,
		METADATA_PARAMS(Z_Construct_UClass_UClutch_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UClutch_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UClutch()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UClutch_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UClutch, 622575563);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UClutch>()
	{
		return UClutch::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UClutch(Z_Construct_UClass_UClutch, &UClutch::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UClutch"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UClutch);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
