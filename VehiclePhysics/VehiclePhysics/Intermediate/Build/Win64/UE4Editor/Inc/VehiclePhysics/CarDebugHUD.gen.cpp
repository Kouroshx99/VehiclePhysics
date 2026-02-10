// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/CarDebugHUD.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCarDebugHUD() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_ACarDebugHUD_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_ACarDebugHUD();
	ENGINE_API UClass* Z_Construct_UClass_AHUD();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
	void ACarDebugHUD::StaticRegisterNativesACarDebugHUD()
	{
	}
	UClass* Z_Construct_UClass_ACarDebugHUD_NoRegister()
	{
		return ACarDebugHUD::StaticClass();
	}
	struct Z_Construct_UClass_ACarDebugHUD_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ACarDebugHUD_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AHUD,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACarDebugHUD_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Rendering Actor Input Replication" },
		{ "IncludePath", "Vehicles/CarDebugHUD.h" },
		{ "ModuleRelativePath", "Vehicles/CarDebugHUD.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ACarDebugHUD_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACarDebugHUD>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ACarDebugHUD_Statics::ClassParams = {
		&ACarDebugHUD::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ACarDebugHUD_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ACarDebugHUD_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ACarDebugHUD()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ACarDebugHUD_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ACarDebugHUD, 1654549024);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<ACarDebugHUD>()
	{
		return ACarDebugHUD::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ACarDebugHUD(Z_Construct_UClass_ACarDebugHUD, &ACarDebugHUD::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("ACarDebugHUD"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ACarDebugHUD);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
