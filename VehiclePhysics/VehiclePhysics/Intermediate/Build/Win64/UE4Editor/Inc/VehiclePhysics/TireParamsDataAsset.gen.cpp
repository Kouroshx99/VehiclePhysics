// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/TireParamsDataAsset.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTireParamsDataAsset() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UTireParamsDataAsset_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UTireParamsDataAsset();
	ENGINE_API UClass* Z_Construct_UClass_UDataAsset();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FPacejkaResult();
// End Cross Module References
	DEFINE_FUNCTION(UTireParamsDataAsset::execComputeTireForcesOverriden)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Fz);
		P_GET_PROPERTY(FFloatProperty,Z_Param_kappa);
		P_GET_PROPERTY(FFloatProperty,Z_Param_alpha);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Gamma);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Vc);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Vcy);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Vcx);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Omega);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FPacejkaResult*)Z_Param__Result=P_THIS->ComputeTireForcesOverriden(Z_Param_Fz,Z_Param_kappa,Z_Param_alpha,Z_Param_Gamma,Z_Param_Vc,Z_Param_Vcy,Z_Param_Vcx,Z_Param_Omega);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTireParamsDataAsset::execComputeTireForces)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Fz);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Gamma);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Vc);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Vcy);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Vcx);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Omega);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FPacejkaResult*)Z_Param__Result=P_THIS->ComputeTireForces(Z_Param_Fz,Z_Param_Gamma,Z_Param_Vc,Z_Param_Vcy,Z_Param_Vcx,Z_Param_Omega);
		P_NATIVE_END;
	}
	void UTireParamsDataAsset::StaticRegisterNativesUTireParamsDataAsset()
	{
		UClass* Class = UTireParamsDataAsset::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ComputeTireForces", &UTireParamsDataAsset::execComputeTireForces },
			{ "ComputeTireForcesOverriden", &UTireParamsDataAsset::execComputeTireForcesOverriden },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics
	{
		struct TireParamsDataAsset_eventComputeTireForces_Parms
		{
			float Fz;
			float Gamma;
			float Vc;
			float Vcy;
			float Vcx;
			float Omega;
			FPacejkaResult ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Fz;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Gamma;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Vc;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Vcy;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Vcx;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Omega;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Fz = { "Fz", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForces_Parms, Fz), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Gamma = { "Gamma", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForces_Parms, Gamma), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Vc = { "Vc", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForces_Parms, Vc), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Vcy = { "Vcy", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForces_Parms, Vcy), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Vcx = { "Vcx", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForces_Parms, Vcx), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Omega = { "Omega", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForces_Parms, Omega), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForces_Parms, ReturnValue), Z_Construct_UScriptStruct_FPacejkaResult, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Fz,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Gamma,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Vc,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Vcy,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Vcx,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_Omega,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/// \n/// @param Fz Normal force [N]\n/// @param Gamma Camber angle [Rad]\n/// @param Vc  magnitude of the velocity of the wheel contact center [m/s]\n/// @param Vcy Lateral vehicle motion at the wheel contact patch center [m/s]\n/// @param Vcx Forward vehicle motion at the wheel contact patch center [m/s]\n/// @param Omega Angular velocity of the wheel [Rad/s]\n/// @return \n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "@param Fz Normal force [N]\n@param Gamma Camber angle [Rad]\n@param Vc  magnitude of the velocity of the wheel contact center [m/s]\n@param Vcy Lateral vehicle motion at the wheel contact patch center [m/s]\n@param Vcx Forward vehicle motion at the wheel contact patch center [m/s]\n@param Omega Angular velocity of the wheel [Rad/s]\n@return" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTireParamsDataAsset, nullptr, "ComputeTireForces", nullptr, nullptr, sizeof(TireParamsDataAsset_eventComputeTireForces_Parms), Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x40020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics
	{
		struct TireParamsDataAsset_eventComputeTireForcesOverriden_Parms
		{
			float Fz;
			float kappa;
			float alpha;
			float Gamma;
			float Vc;
			float Vcy;
			float Vcx;
			float Omega;
			FPacejkaResult ReturnValue;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Fz;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_kappa;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_alpha;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Gamma;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Vc;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Vcy;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Vcx;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Omega;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Fz = { "Fz", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, Fz), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_kappa = { "kappa", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, kappa), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_alpha = { "alpha", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, alpha), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Gamma = { "Gamma", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, Gamma), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Vc = { "Vc", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, Vc), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Vcy = { "Vcy", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, Vcy), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Vcx = { "Vcx", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, Vcx), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Omega = { "Omega", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, Omega), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms, ReturnValue), Z_Construct_UScriptStruct_FPacejkaResult, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Fz,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_kappa,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_alpha,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Gamma,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Vc,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Vcy,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Vcx,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_Omega,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/// \n/// @param Fz Normal force [N]\n/// @param kappa \n/// @param alpha \n/// @param Gamma Camber angle [Rad]\n/// @param Vc  magnitude of the velocity of the wheel contact center [m/s]\n/// @param Vcy Lateral vehicle motion at the wheel contact patch center [m/s]\n/// @param Vcx Forward vehicle motion at the wheel contact patch center [m/s]\n/// @param Omega Angular velocity of the wheel [Rad/s]\n/// @param bOverrideSlipRatios \n/// @return \n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "@param Fz Normal force [N]\n@param kappa\n@param alpha\n@param Gamma Camber angle [Rad]\n@param Vc  magnitude of the velocity of the wheel contact center [m/s]\n@param Vcy Lateral vehicle motion at the wheel contact patch center [m/s]\n@param Vcx Forward vehicle motion at the wheel contact patch center [m/s]\n@param Omega Angular velocity of the wheel [Rad/s]\n@param bOverrideSlipRatios\n@return" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTireParamsDataAsset, nullptr, "ComputeTireForcesOverriden", nullptr, nullptr, sizeof(TireParamsDataAsset_eventComputeTireForcesOverriden_Parms), Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x40020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UTireParamsDataAsset_NoRegister()
	{
		return UTireParamsDataAsset::StaticClass();
	}
	struct Z_Construct_UClass_UTireParamsDataAsset_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
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
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Camber_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Camber;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FZ0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_FZ0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_R0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_R0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_pi0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_pi0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LFZ0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LFZ0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LCX_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LCX;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LMUX_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LMUX;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LEX_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LEX;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LHX_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LHX;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LVX_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LVX;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LXAL_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LXAL;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LCY_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LCY;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LMUY_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LMUY;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LEY_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LEY;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LKYAL_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LKYAL;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LKZC_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LKZC;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LHY_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LHY;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LVY_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LVY;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LT_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LT;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LYK_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LYK;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LVYK_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LVYK;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LMUV_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LMUV;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LS_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LS;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_LMR_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_LMR;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_6_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_6;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_7_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_7;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_zeta_8_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_zeta_8;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PCX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PCX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PDX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PDX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PDX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PDX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PDX3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PDX3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEX3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEX3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEX4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEX4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKX3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKX3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PHX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PHX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PHX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PHX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PVX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PVX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PVX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PVX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBX3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBX3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RCX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RCX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_REX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_REX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_REX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_REX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RHX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RHX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPX1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPX1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPX2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPX2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPX3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPX3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPX4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPX4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PCY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PCY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PDY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PDY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PDY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PDY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PDY3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PDY3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEY3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEY3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEY4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEY4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PEY5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PEY5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKY3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKY3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKY4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKY4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKY5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKY5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKY6_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKY6;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PKY7_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PKY7;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PHY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PHY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PHY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PHY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PVY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PVY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PVY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PVY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PVY3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PVY3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PVY4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PVY4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBY3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBY3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBY4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBY4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RCY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RCY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_REY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_REY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_REY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_REY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RHY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RHY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RHY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RHY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RVY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RVY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RVY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RVY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RVY3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RVY3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RVY4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RVY4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RBY5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RBY5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RVY6_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_RVY6;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPY1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPY1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPY2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPY2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPY3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPY3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPY4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPY4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPY5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPY5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QBZ1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QBZ1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QBZ2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QBZ2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QBZ3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QBZ3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QBZ5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QBZ5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QBZ6_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QBZ6;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QBZ9_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QBZ9;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QBZ10_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QBZ10;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QCZ1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QCZ1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QD4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QD4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ6_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ6;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ7_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ7;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ8_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ8;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ9_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ9;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ10_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ10;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QDZ11_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QDZ11;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QEZ1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QEZ1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QEZ2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QEZ2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QEZ3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QEZ3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QEZ4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QEZ4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QEZ5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QEZ5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QHZ1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QHZ1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QHZ2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QHZ2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QHZ3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QHZ3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_QHZ4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_QHZ4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SSZ1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SSZ1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SSZ2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SSZ2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SSZ3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SSZ3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SSZ4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SSZ4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPZ1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPZ1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PPZ2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_PPZ2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_epsilon_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_epsilon;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_sigma_kappa0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_sigma_kappa0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_sigma_alpha0_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_sigma_alpha0;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_C_Fx_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_C_Fx;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_C_Fy_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_C_Fy;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_C_Fgamma_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_C_Fgamma;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_epsilon_F_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_epsilon_F;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_sigma_min_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_sigma_min;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL1_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL1;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL2_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL2;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL3_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL3;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL4_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL4;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL5_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL5;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL6_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL6;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL7_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL7;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_p_NL8_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_p_NL8;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_c_gyr_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_c_gyr;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTireParamsDataAsset_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UDataAsset,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UTireParamsDataAsset_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForces, "ComputeTireForces" }, // 3241292660
		{ &Z_Construct_UFunction_UTireParamsDataAsset_ComputeTireForcesOverriden, "ComputeTireForcesOverriden" }, // 1257024984
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipRatio_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Editor" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipRatio = { "SlipRatio", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, SlipRatio), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipRatio_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipRatio_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipAngle_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Editor" },
		{ "Comment", "// Nominal load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Nominal load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipAngle = { "SlipAngle", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, SlipAngle), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_Camber_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Editor" },
		{ "Comment", "// effective unloaded radius [m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "effective unloaded radius [m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_Camber = { "Camber", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, Camber), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_Camber_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_Camber_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_FZ0_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|General" },
		{ "Comment", "// Nominal (reference) tire inflation pressure\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Nominal (reference) tire inflation pressure" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_FZ0 = { "FZ0", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, FZ0), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_FZ0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_FZ0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_R0_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|General" },
		{ "Comment", "// Nominal load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Nominal load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_R0 = { "R0", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, R0), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_R0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_R0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_pi0_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|General" },
		{ "Comment", "// effective unloaded radius [m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "effective unloaded radius [m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_pi0 = { "pi0", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, pi0), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_pi0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_pi0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LFZ0_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "/* ---------- [SCALING_COEFFICIENTS] ---------- */" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "---------- [SCALING_COEFFICIENTS] ----------" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LFZ0 = { "LFZ0", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LFZ0), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LFZ0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LFZ0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCX_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of nominal (rated) load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of nominal (rated) load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCX = { "LCX", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LCX), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCX_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCX_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUX_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fx shape factor\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fx shape factor" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUX = { "LMUX", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LMUX), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUX_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUX_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEX_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fx peak friction coefficient\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fx peak friction coefficient" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEX = { "LEX", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LEX), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEX_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEX_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHX_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fx curvature factor\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fx curvature factor" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHX = { "LHX", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LHX), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHX_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHX_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVX_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fx horizontal shift\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fx horizontal shift" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVX = { "LVX", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LVX), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVX_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVX_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LXAL_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fx vertical shift\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fx vertical shift" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LXAL = { "LXAL", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LXAL), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LXAL_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LXAL_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCY_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fx vertical shift\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fx vertical shift" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCY = { "LCY", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LCY), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCY_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCY_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUY_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fy shape factor\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fy shape factor" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUY = { "LMUY", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LMUY), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUY_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUY_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEY_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fy peak friction coefficient\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fy peak friction coefficient" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEY = { "LEY", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LEY), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEY_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEY_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKYAL_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fy curvature factor\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fy curvature factor" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKYAL = { "LKYAL", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LKYAL), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKYAL_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKYAL_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKZC_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of cornering stiffness\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of cornering stiffness" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKZC = { "LKZC", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LKZC), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKZC_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKZC_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHY_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of camber moment stiffness\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of camber moment stiffness" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHY = { "LHY", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LHY), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHY_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHY_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVY_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fy horizontal shift\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fy horizontal shift" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVY = { "LVY", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LVY), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVY_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVY_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LT_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of Fy vertical shift\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of Fy vertical shift" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LT = { "LT", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LT), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LT_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LT_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LYK_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of pneumatic trail\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of pneumatic trail" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LYK = { "LYK", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LYK), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LYK_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LYK_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVYK_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of k influence on Fy(a)\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of k influence on Fy(a)" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVYK = { "LVYK", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LVYK), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVYK_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVYK_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUV_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of k induced \xe2\x80\x98ply-steer\xe2\x80\x99 Fy\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of k induced \xe2\x80\x98ply-steer\xe2\x80\x99 Fy" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUV = { "LMUV", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LMUV), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUV_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LS_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of with slip speed Vs decaying friction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of with slip speed Vs decaying friction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LS = { "LS", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LS), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LS_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LS_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMR_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor of moment arm of Fx\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor of moment arm of Fx" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMR = { "LMR", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, LMR), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMR_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMR_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_0_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "Comment", "// Scale factor aligning torque\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Scale factor aligning torque" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_0 = { "zeta_0", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_0), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_1 = { "zeta_1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_2 = { "zeta_2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_3 = { "zeta_3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_4 = { "zeta_4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_5 = { "zeta_5", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_6_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_6 = { "zeta_6", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_6), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_6_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_6_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_7_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_7 = { "zeta_7", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_7), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_7_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_7_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_8_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Scaling Coefficients" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_8 = { "zeta_8", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, zeta_8), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_8_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_8_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "/* ---------- [LONGITUDINAL_COEFFICIENTS] ---------- */" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "---------- [LONGITUDINAL_COEFFICIENTS] ----------" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCX1 = { "PCX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PCX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Shape factor Cfx for longitudinal force\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shape factor Cfx for longitudinal force" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX1 = { "PDX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PDX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Longitudinal friction Mux at Fznom \n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Longitudinal friction Mux at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX2 = { "PDX2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PDX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Variation of friction Mux with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of friction Mux with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX3 = { "PDX3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PDX3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Variation of friction Mux with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of friction Mux with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX1 = { "PEX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Longitudinal curvature Efx at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Longitudinal curvature Efx at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX2 = { "PEX2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Variation of curvature Efx with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of curvature Efx with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX3 = { "PEX3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEX3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Variation of curvature Efx with load squared\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of curvature Efx with load squared" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX4 = { "PEX4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEX4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Factor in curvature Efx while driving\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Factor in curvature Efx while driving" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX1 = { "PKX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Longitudinal slip stiffness Kfx/Fz at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Longitudinal slip stiffness Kfx/Fz at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX2 = { "PKX2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Variation of slip stiffness Kfx/Fz with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of slip stiffness Kfx/Fz with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX3 = { "PKX3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKX3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Exponent in slip stiffness Kfx/Fz with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Exponent in slip stiffness Kfx/Fz with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX1 = { "PHX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PHX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Horizontal shift Shx at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Horizontal shift Shx at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX2 = { "PHX2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PHX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Variation of shift Shx with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Shx with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX1 = { "PVX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PVX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Vertical shift Svx/Fz at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Vertical shift Svx/Fz at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX2 = { "PVX2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PVX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Combined" },
		{ "Comment", "// Variation of shift Svx/Fz with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Svx/Fz with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX1 = { "RBX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Combined" },
		{ "Comment", "// Slope factor for combined slip Fx reduction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Slope factor for combined slip Fx reduction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX2 = { "RBX2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Combined" },
		{ "Comment", "// Variation of slope Fx reduction with kappa\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of slope Fx reduction with kappa" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX3 = { "RBX3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBX3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Combined" },
		{ "Comment", "// Influence of camber on stiffness for Fx combined\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Influence of camber on stiffness for Fx combined" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCX1 = { "RCX1", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RCX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Combined" },
		{ "Comment", "// Shape factor for combined slip Fx reduction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shape factor for combined slip Fx reduction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX1 = { "REX1", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, REX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Combined" },
		{ "Comment", "// Curvature factor of combined Fx\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Curvature factor of combined Fx" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX2 = { "REX2", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, REX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Combined" },
		{ "Comment", "// Curvature factor of combined Fx with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Curvature factor of combined Fx with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHX1 = { "RHX1", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RHX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Shift factor for combined slip Fx reduction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shift factor for combined slip Fx reduction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX1 = { "PPX1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPX1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Linear pressure effect on slip stiffness\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Linear pressure effect on slip stiffness" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX2 = { "PPX2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPX2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Quadratic pressure effect on slip stiffness\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Quadratic pressure effect on slip stiffness" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX3 = { "PPX3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPX3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Linear pressure effect on longitudinal friction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Linear pressure effect on longitudinal friction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX4 = { "PPX4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPX4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "/* ---------- [LATERAL_COEFFICIENTS] ---------- */" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "---------- [LATERAL_COEFFICIENTS] ----------" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCY1 = { "PCY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PCY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Shape factor Cfy for lateral forces\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shape factor Cfy for lateral forces" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY1 = { "PDY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PDY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Lateral friction Muy\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Lateral friction Muy" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY2 = { "PDY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PDY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of friction Muy with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of friction Muy with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY3 = { "PDY3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PDY3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of friction Muy with squared camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of friction Muy with squared camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY1 = { "PEY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Lateral curvature Efy at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Lateral curvature Efy at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY2 = { "PEY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of curvature Efy with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of curvature Efy with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY3 = { "PEY3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEY3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Zero order camber dependency of curvature Efy\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Zero order camber dependency of curvature Efy" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY4 = { "PEY4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEY4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of curvature Efy with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of curvature Efy with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY5 = { "PEY5", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PEY5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Camber curvature Efc\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Camber curvature Efc" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY1 = { "PKY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Maximum value of stiffness Kfy/Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Maximum value of stiffness Kfy/Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY2 = { "PKY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Load at which Kfy reaches maximum value\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Load at which Kfy reaches maximum value" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY3 = { "PKY3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKY3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of Kfy/Fznom with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Kfy/Fznom with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY4 = { "PKY4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKY4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Peak stiffness variation with camber squared\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Peak stiffness variation with camber squared" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY5 = { "PKY5", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKY5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY6_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Lateral stiffness dependency with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Lateral stiffness dependency with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY6 = { "PKY6", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKY6), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY6_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY6_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY7_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Camber stiffness factor\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Camber stiffness factor" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY7 = { "PKY7", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PKY7), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY7_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY7_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Load dependency of camber stiffness factor\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Load dependency of camber stiffness factor" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY1 = { "PHY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PHY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Horizontal shift Shy at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Horizontal shift Shy at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY2 = { "PHY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PHY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of shift Shy with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Shy with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY1 = { "PVY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PVY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Vertical shift in Svy/Fz at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Vertical shift in Svy/Fz at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY2 = { "PVY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PVY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of shift Svy/Fz with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Svy/Fz with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY3 = { "PVY3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PVY3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of shift Svy/Fz with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Svy/Fz with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY4 = { "PVY4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PVY4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of shift Svy/Fz with camber and load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Svy/Fz with camber and load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY1 = { "RBY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Slope factor for combined Fy reduction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Slope factor for combined Fy reduction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY2 = { "RBY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of slope Fy reduction with alpha\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of slope Fy reduction with alpha" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY3 = { "RBY3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBY3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Shift term for alpha in slope Fy reduction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shift term for alpha in slope Fy reduction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY4 = { "RBY4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBY4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Influence of camber on stiffness of Fy combined\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Influence of camber on stiffness of Fy combined" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCY1 = { "RCY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RCY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Shape factor for combined Fy reduction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shape factor for combined Fy reduction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY1 = { "REY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, REY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Curvature factor of combined Fy\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Curvature factor of combined Fy" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY2 = { "REY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, REY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Curvature factor of combined Fy with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Curvature factor of combined Fy with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY1 = { "RHY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RHY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Shift factor for combined Fy reduction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shift factor for combined Fy reduction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY2 = { "RHY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RHY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Shift factor for combined Fy reduction with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Shift factor for combined Fy reduction with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY1 = { "RVY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RVY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Kappa induced side force Svyk/Muy*Fz at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Kappa induced side force Svyk/Muy*Fz at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY2 = { "RVY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RVY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of Svyk/Muy*Fz with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Svyk/Muy*Fz with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY3 = { "RVY3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RVY3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of Svyk/Muy*Fz with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Svyk/Muy*Fz with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY4 = { "RVY4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RVY4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of Svyk/Muy*Fz with alpha\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Svyk/Muy*Fz with alpha" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY5 = { "RBY5", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RBY5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY6_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of Svyk/Muy*Fz with kappa\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Svyk/Muy*Fz with kappa" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY6 = { "RVY6", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, RVY6), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY6_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY6_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Variation of Svyk/Muy*Fz with atan (kappa)\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Svyk/Muy*Fz with atan (kappa)" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY1 = { "PPY1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPY1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Pressure effect on cornering stiffness magnitude\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Pressure effect on cornering stiffness magnitude" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY2 = { "PPY2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPY2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Pressure effect on location of cornering stiffness peak\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Pressure effect on location of cornering stiffness peak" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY3 = { "PPY3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPY3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Linear pressure effect on lateral friction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Linear pressure effect on lateral friction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY4 = { "PPY4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPY4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Lateral" },
		{ "Comment", "// Quadratic pressure effect on lateral friction\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Quadratic pressure effect on lateral friction" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY5 = { "PPY5", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPY5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "/* ---------- [ALIGNING_COEFFICIENTS] ---------- */" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "---------- [ALIGNING_COEFFICIENTS] ----------" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ1 = { "QBZ1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QBZ1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// TTrail slope factor for trail Bpt at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "TTrail slope factor for trail Bpt at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ2 = { "QBZ2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QBZ2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of slope Bpt with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of slope Bpt with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ3 = { "QBZ3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QBZ3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of slope Bpt with load squared\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of slope Bpt with load squared" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ5 = { "QBZ5", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QBZ5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ6_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "//Variation of slope Bpt with absolute camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of slope Bpt with absolute camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ6 = { "QBZ6", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QBZ6), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ6_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ6_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ9_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// //Variation of slope Bpt with camber squared\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of slope Bpt with camber squared" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ9 = { "QBZ9", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QBZ9), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ9_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ9_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ10_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Slope factor Br of residual torque Mzr\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Slope factor Br of residual torque Mzr" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ10 = { "QBZ10", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QBZ10), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ10_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ10_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QCZ1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "//Slope factor Br of residual torque Mzr\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Slope factor Br of residual torque Mzr" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QCZ1 = { "QCZ1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QCZ1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QCZ1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QCZ1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Dr / Dz family (peak/residual amplitude)\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Dr / Dz family (peak/residual amplitude)" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ1 = { "QDZ1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Peak trail Dpt\" = Dpt*(Fz/Fznom*R0)\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Peak trail Dpt\" = Dpt*(Fz/Fznom*R0)" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ2 = { "QDZ2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of peak Dpt with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of peak Dpt with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ3 = { "QDZ3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QD4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of peak Dpt with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of peak Dpt with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QD4 = { "QD4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QD4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QD4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QD4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ6_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of peak Dpt with camber squared\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of peak Dpt with camber squared" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ6 = { "QDZ6", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ6), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ6_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ6_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ7_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Peak residual torque Dmr = Dmr/(Fz*R0)\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Peak residual torque Dmr = Dmr/(Fz*R0)" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ7 = { "QDZ7", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ7), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ7_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ7_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ8_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of peak factor Dmr with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of peak factor Dmr with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ8 = { "QDZ8", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ8), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ8_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ8_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ9_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of peak factor Dmr with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of peak factor Dmr with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ9 = { "QDZ9", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ9), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ9_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ9_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ10_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of peak factor Dmr with camber and load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of peak factor Dmr with camber and load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ10 = { "QDZ10", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ10), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ10_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ10_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ11_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of peak factor Dmr with camber squared\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of peak factor Dmr with camber squared" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ11 = { "QDZ11", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QDZ11), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ11_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ11_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of Dmr with camber squared and load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Dmr with camber squared and load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ1 = { "QEZ1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QEZ1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Trail curvature Ept at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Trail curvature Ept at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ2 = { "QEZ2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QEZ2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of curvature Ept with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of curvature Ept with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ3 = { "QEZ3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QEZ3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of curvature Ept with load squared\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of curvature Ept with load squared" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ4 = { "QEZ4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QEZ4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of curvature Ept with sign of Alpha-t\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of curvature Ept with sign of Alpha-t" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ5 = { "QEZ5", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QEZ5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of Ept with camber and sign Alpha-t\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of Ept with camber and sign Alpha-t" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ1 = { "QHZ1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QHZ1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Trail horizontal shift Sht at Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Trail horizontal shift Sht at Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ2 = { "QHZ2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QHZ2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of shift Sht with load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Sht with load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ3 = { "QHZ3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QHZ3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of shift Sht with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Sht with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ4 = { "QHZ4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, QHZ4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of shift Sht with camber and load\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of shift Sht with camber and load" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ1 = { "SSZ1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, SSZ1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Nominal value of s/R0: effect of Fx on Mz\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Nominal value of s/R0: effect of Fx on Mz" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ2 = { "SSZ2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, SSZ2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of distance s/R0 with Fy/Fznom\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of distance s/R0 with Fy/Fznom" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ3 = { "SSZ3", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, SSZ3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Variation of distance s/R0 with camber\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Variation of distance s/R0 with camber" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ4 = { "SSZ4", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, SSZ4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// pressure sensitivity\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "pressure sensitivity" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ1 = { "PPZ1", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPZ1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Aligning" },
		{ "Comment", "// Linear pressure effect on pneumatic trail\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Linear pressure effect on pneumatic trail" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ2 = { "PPZ2", nullptr, (EPropertyFlags)0x0040000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, PPZ2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Longitudinal" },
		{ "Comment", "// Influence of inflation pressure on residual aligning torque\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Influence of inflation pressure on residual aligning torque" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon = { "epsilon", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, epsilon), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_kappa0_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "/** ----------- Transient model parameters ----------- */" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "----------- Transient model parameters -----------" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_kappa0 = { "sigma_kappa0", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, sigma_kappa0), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_kappa0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_kappa0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_alpha0_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Initial longitudinal relaxation length [m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Initial longitudinal relaxation length [m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_alpha0 = { "sigma_alpha0", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, sigma_alpha0), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_alpha0_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_alpha0_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fx_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Initial lateral relaxation length [m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Initial lateral relaxation length [m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fx = { "C_Fx", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, C_Fx), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fx_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fx_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fy_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Longitudinal carcass stiffness [N/m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Longitudinal carcass stiffness [N/m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fy = { "C_Fy", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, C_Fy), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fy_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fy_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fgamma_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Lateral carcass stiffness [N/m]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Lateral carcass stiffness [N/m]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fgamma = { "C_Fgamma", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, C_Fgamma), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fgamma_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fgamma_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_F_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Camber stiffness [N/rad]\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Camber stiffness [N/rad]" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_F = { "epsilon_F", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, epsilon_F), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_F_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_F_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_min_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Small constant to avoid singularity\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Small constant to avoid singularity" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_min = { "sigma_min", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, sigma_min), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_min_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_min_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL1_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Non-lagging camber parameters\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Non-lagging camber parameters" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL1 = { "p_NL1", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL1), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL1_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL1_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL2_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL2 = { "p_NL2", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL2), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL2_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL2_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL3_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL3 = { "p_NL3", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL3), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL3_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL3_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL4_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL4 = { "p_NL4", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL4), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL4_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL4_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL5_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL5 = { "p_NL5", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL5), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL5_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL5_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL6_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL6 = { "p_NL6", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL6), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL6_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL6_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL7_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL7 = { "p_NL7", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL7), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL7_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL7_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL8_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL8 = { "p_NL8", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, p_NL8), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL8_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL8_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_c_gyr_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Tire|Transient" },
		{ "Comment", "// Gyroscopic couple\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/TireParamsDataAsset.h" },
		{ "ToolTip", "Gyroscopic couple" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_c_gyr = { "c_gyr", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UTireParamsDataAsset, c_gyr), METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_c_gyr_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_c_gyr_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UTireParamsDataAsset_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipRatio,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SlipAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_Camber,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_FZ0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_R0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_pi0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LFZ0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCX,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUX,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEX,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHX,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVX,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LXAL,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LCY,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUY,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LEY,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKYAL,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LKZC,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LHY,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVY,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LT,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LYK,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LVYK,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMUV,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LS,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_LMR,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_6,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_7,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_zeta_8,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDX3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEX4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKX3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBX3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPX4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PCY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PDY3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PEY5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY6,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PKY7,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PHY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PVY4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RCY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_REY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RHY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RBY5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_RVY6,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPY5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ6,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ9,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QBZ10,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QCZ1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QD4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ6,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ7,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ8,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ9,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ10,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QDZ11,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QEZ5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_QHZ4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_SSZ4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_PPZ2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_kappa0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_alpha0,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fx,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fy,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_C_Fgamma,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_epsilon_F,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_sigma_min,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL1,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL2,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL3,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL4,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL5,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL6,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL7,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_p_NL8,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTireParamsDataAsset_Statics::NewProp_c_gyr,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTireParamsDataAsset_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTireParamsDataAsset>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTireParamsDataAsset_Statics::ClassParams = {
		&UTireParamsDataAsset::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UTireParamsDataAsset_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::PropPointers),
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTireParamsDataAsset_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTireParamsDataAsset_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTireParamsDataAsset()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTireParamsDataAsset_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTireParamsDataAsset, 702051842);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<UTireParamsDataAsset>()
	{
		return UTireParamsDataAsset::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTireParamsDataAsset(Z_Construct_UClass_UTireParamsDataAsset, &UTireParamsDataAsset::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("UTireParamsDataAsset"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTireParamsDataAsset);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
