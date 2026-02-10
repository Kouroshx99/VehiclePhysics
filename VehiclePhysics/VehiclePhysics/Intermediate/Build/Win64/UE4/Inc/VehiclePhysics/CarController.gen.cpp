// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/CarController.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCarController() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_ACarController_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_ACarController();
	ENGINE_API UClass* Z_Construct_UClass_APlayerController();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
// End Cross Module References
	DEFINE_FUNCTION(ACarController::execGetGear)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=P_THIS->GetGear();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ACarController::execGetEngineRPM)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetEngineRPM();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ACarController::execGetSpeedKMH)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=P_THIS->GetSpeedKMH();
		P_NATIVE_END;
	}
	void ACarController::StaticRegisterNativesACarController()
	{
		UClass* Class = ACarController::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetEngineRPM", &ACarController::execGetEngineRPM },
			{ "GetGear", &ACarController::execGetGear },
			{ "GetSpeedKMH", &ACarController::execGetSpeedKMH },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ACarController_GetEngineRPM_Statics
	{
		struct CarController_eventGetEngineRPM_Parms
		{
			float ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CarController_eventGetEngineRPM_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::Function_MetaDataParams[] = {
		{ "Category", "Vehicle" },
		{ "ModuleRelativePath", "Vehicles/CarController.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ACarController, nullptr, "GetEngineRPM", nullptr, nullptr, sizeof(CarController_eventGetEngineRPM_Parms), Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04040401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ACarController_GetEngineRPM()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_ACarController_GetEngineRPM_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ACarController_GetGear_Statics
	{
		struct CarController_eventGetGear_Parms
		{
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ACarController_GetGear_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CarController_eventGetGear_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ACarController_GetGear_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ACarController_GetGear_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ACarController_GetGear_Statics::Function_MetaDataParams[] = {
		{ "Category", "Vehicle" },
		{ "ModuleRelativePath", "Vehicles/CarController.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_ACarController_GetGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ACarController, nullptr, "GetGear", nullptr, nullptr, sizeof(CarController_eventGetGear_Parms), Z_Construct_UFunction_ACarController_GetGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ACarController_GetGear_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04040401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ACarController_GetGear_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ACarController_GetGear_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ACarController_GetGear()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_ACarController_GetGear_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics
	{
		struct CarController_eventGetSpeedKMH_Parms
		{
			int32 ReturnValue;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(CarController_eventGetSpeedKMH_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::Function_MetaDataParams[] = {
		{ "Category", "Vehicle" },
		{ "ModuleRelativePath", "Vehicles/CarController.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ACarController, nullptr, "GetSpeedKMH", nullptr, nullptr, sizeof(CarController_eventGetSpeedKMH_Parms), Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04040401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ACarController_GetSpeedKMH()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_ACarController_GetSpeedKMH_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_ACarController_NoRegister()
	{
		return ACarController::StaticClass();
	}
	struct Z_Construct_UClass_ACarController_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ACarController_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerController,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_ACarController_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ACarController_GetEngineRPM, "GetEngineRPM" }, // 651615800
		{ &Z_Construct_UFunction_ACarController_GetGear, "GetGear" }, // 996122621
		{ &Z_Construct_UFunction_ACarController_GetSpeedKMH, "GetSpeedKMH" }, // 528337778
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACarController_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "Vehicles/CarController.h" },
		{ "ModuleRelativePath", "Vehicles/CarController.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ACarController_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACarController>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ACarController_Statics::ClassParams = {
		&ACarController::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x009002A4u,
		METADATA_PARAMS(Z_Construct_UClass_ACarController_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ACarController_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ACarController()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ACarController_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ACarController, 2251986616);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<ACarController>()
	{
		return ACarController::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ACarController(Z_Construct_UClass_ACarController, &ACarController::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("ACarController"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ACarController);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
