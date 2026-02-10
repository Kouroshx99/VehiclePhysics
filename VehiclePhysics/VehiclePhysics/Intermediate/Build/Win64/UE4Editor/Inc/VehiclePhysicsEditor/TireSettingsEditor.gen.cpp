// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysicsEditor/PhysicsEditor/TireSettingsEditor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTireSettingsEditor() {}
// Cross Module References
	VEHICLEPHYSICSEDITOR_API UClass* Z_Construct_UClass_UTireEditorSettings_NoRegister();
	VEHICLEPHYSICSEDITOR_API UClass* Z_Construct_UClass_UTireEditorSettings();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysicsEditor();
// End Cross Module References
	void UTireEditorSettings::StaticRegisterNativesUTireEditorSettings()
	{
	}
	UClass* Z_Construct_UClass_UTireEditorSettings_NoRegister()
	{
		return UTireEditorSettings::StaticClass();
	}
	struct Z_Construct_UClass_UTireEditorSettings_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SlipRatio_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SlipRatio;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SlipAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SlipAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CamberAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_CamberAngle;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTireEditorSettings_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysicsEditor,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireEditorSettings_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "PhysicsEditor/TireSettingsEditor.h" },
		{ "ModuleRelativePath", "PhysicsEditor/TireSettingsEditor.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipRatio_MetaData[] = {
		{ "Category", "Editor Preview" },
		{ "ModuleRelativePath", "PhysicsEditor/TireSettingsEditor.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipRatio = { "SlipRatio", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireEditorSettings, SlipRatio), METADATA_PARAMS(Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipRatio_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipRatio_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipAngle_MetaData[] = {
		{ "Category", "Editor Preview" },
		{ "ModuleRelativePath", "PhysicsEditor/TireSettingsEditor.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipAngle = { "SlipAngle", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireEditorSettings, SlipAngle), METADATA_PARAMS(Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_CamberAngle_MetaData[] = {
		{ "Category", "Editor Preview" },
		{ "ModuleRelativePath", "PhysicsEditor/TireSettingsEditor.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_CamberAngle = { "CamberAngle", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireEditorSettings, CamberAngle), METADATA_PARAMS(Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_CamberAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_CamberAngle_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UTireEditorSettings_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipRatio,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_SlipAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireEditorSettings_Statics::NewProp_CamberAngle,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTireEditorSettings_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTireEditorSettings>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTireEditorSettings_Statics::ClassParams = {
		&UTireEditorSettings::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UTireEditorSettings_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UTireEditorSettings_Statics::PropPointers),
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTireEditorSettings_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTireEditorSettings_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTireEditorSettings()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTireEditorSettings_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTireEditorSettings, 3708401142);
	template<> VEHICLEPHYSICSEDITOR_API UClass* StaticClass<UTireEditorSettings>()
	{
		return UTireEditorSettings::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTireEditorSettings(Z_Construct_UClass_UTireEditorSettings, &UTireEditorSettings::StaticClass, TEXT("/Script/VehiclePhysicsEditor"), TEXT("UTireEditorSettings"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTireEditorSettings);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
