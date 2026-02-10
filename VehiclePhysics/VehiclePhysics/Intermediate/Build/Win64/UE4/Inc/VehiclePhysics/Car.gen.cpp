// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VehiclePhysics/Vehicles/Physics/Car.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCar() {}
// Cross Module References
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_ACar_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_ACar();
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	UPackage* Z_Construct_UPackage__Script_VehiclePhysics();
	VEHICLEPHYSICS_API UScriptStruct* Z_Construct_UScriptStruct_FShaft();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UWheelCollider_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USkeletalMeshComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USpringArmComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UVehicleEngine_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UTransmission_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UDrivetrain_NoRegister();
	VEHICLEPHYSICS_API UClass* Z_Construct_UClass_UECU_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
// End Cross Module References
	DEFINE_FUNCTION(ACar::execResetCar)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ResetCar();
		P_NATIVE_END;
	}
	void ACar::StaticRegisterNativesACar()
	{
		UClass* Class = ACar::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ResetCar", &ACar::execResetCar },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ACar_ResetCar_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ACar_ResetCar_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_ACar_ResetCar_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ACar, nullptr, "ResetCar", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ACar_ResetCar_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ACar_ResetCar_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ACar_ResetCar()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_ACar_ResetCar_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_ACar_NoRegister()
	{
		return ACar::StaticClass();
	}
	struct Z_Construct_UClass_ACar_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ClutchShaft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_ClutchShaft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TransferCaseShaft_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_TransferCaseShaft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontRight_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_FrontRight;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_FrontLeft_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_FrontLeft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearRight_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_RearRight;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RearLeft_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_RearLeft;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_CarMesh_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_CarMesh;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SpringArm_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SpringArm;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Camera_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Camera;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Engine_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Engine;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TransmissionComponent_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TransmissionComponent;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Drivetrain_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Drivetrain;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ECU_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ECU;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HeunNumIterations_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_HeunNumIterations;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_COMOffset_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_COMOffset;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaxSteerAngle_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_MaxSteerAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WheelAnimZOffset_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_WheelAnimZOffset;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SteerCurve_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_SteerCurve;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AckermanPercent_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_AckermanPercent;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bVisualizeCOM_MetaData[];
#endif
		static void NewProp_bVisualizeCOM_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bVisualizeCOM;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bLogDeltaTime_MetaData[];
#endif
		static void NewProp_bLogDeltaTime_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bLogDeltaTime;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bLogVelocityKMH_MetaData[];
#endif
		static void NewProp_bLogVelocityKMH_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bLogVelocityKMH;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bLogAngVelocity_MetaData[];
#endif
		static void NewProp_bLogAngVelocity_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bLogAngVelocity;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bLogAcceleration_MetaData[];
#endif
		static void NewProp_bLogAcceleration_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bLogAcceleration;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bLogWheelSteerAngle_MetaData[];
#endif
		static void NewProp_bLogWheelSteerAngle_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bLogWheelSteerAngle;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bLogCarData_MetaData[];
#endif
		static void NewProp_bLogCarData_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bLogCarData;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_bLogWheelData_MetaData[];
#endif
		static void NewProp_bLogWheelData_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bLogWheelData;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_WheelColliders_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WheelColliders_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_WheelColliders;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ACar_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APawn,
		(UObject* (*)())Z_Construct_UPackage__Script_VehiclePhysics,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_ACar_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ACar_ResetCar, "ResetCar" }, // 2331766847
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Vehicles/Physics/Car.h" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_ClutchShaft_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_ClutchShaft = { "ClutchShaft", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, ClutchShaft), Z_Construct_UScriptStruct_FShaft, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_ClutchShaft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_ClutchShaft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_TransferCaseShaft_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Car" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_TransferCaseShaft = { "TransferCaseShaft", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, TransferCaseShaft), Z_Construct_UScriptStruct_FShaft, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_TransferCaseShaft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_TransferCaseShaft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_FrontRight_MetaData[] = {
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_FrontRight = { "FrontRight", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, FrontRight), Z_Construct_UClass_UWheelCollider_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_FrontRight_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_FrontRight_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_FrontLeft_MetaData[] = {
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_FrontLeft = { "FrontLeft", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, FrontLeft), Z_Construct_UClass_UWheelCollider_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_FrontLeft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_FrontLeft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_RearRight_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_RearRight = { "RearRight", nullptr, (EPropertyFlags)0x002008000008001d, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, RearRight), Z_Construct_UClass_UWheelCollider_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_RearRight_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_RearRight_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_RearLeft_MetaData[] = {
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_RearLeft = { "RearLeft", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, RearLeft), Z_Construct_UClass_UWheelCollider_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_RearLeft_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_RearLeft_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_CarMesh_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_CarMesh = { "CarMesh", nullptr, (EPropertyFlags)0x002008000008001d, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, CarMesh), Z_Construct_UClass_USkeletalMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_CarMesh_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_CarMesh_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_SpringArm_MetaData[] = {
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_SpringArm = { "SpringArm", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, SpringArm), Z_Construct_UClass_USpringArmComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_SpringArm_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_SpringArm_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_Camera_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_Camera = { "Camera", nullptr, (EPropertyFlags)0x002008000008001d, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, Camera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_Camera_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_Camera_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_Engine_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "SceneComponents" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_Engine = { "Engine", nullptr, (EPropertyFlags)0x002008000008001d, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, Engine), Z_Construct_UClass_UVehicleEngine_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_Engine_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_Engine_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_TransmissionComponent_MetaData[] = {
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_TransmissionComponent = { "TransmissionComponent", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, TransmissionComponent), Z_Construct_UClass_UTransmission_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_TransmissionComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_TransmissionComponent_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_Drivetrain_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_Drivetrain = { "Drivetrain", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, Drivetrain), Z_Construct_UClass_UDrivetrain_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_Drivetrain_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_Drivetrain_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_ECU_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_ECU = { "ECU", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, ECU), Z_Construct_UClass_UECU_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_ECU_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_ECU_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_HeunNumIterations_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Car Propperties" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_HeunNumIterations = { "HeunNumIterations", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, HeunNumIterations), METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_HeunNumIterations_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_HeunNumIterations_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_COMOffset_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Car Propperties" },
		{ "Comment", "//Center of mass offset relative to the default position.\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
		{ "ToolTip", "Center of mass offset relative to the default position." },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_COMOffset = { "COMOffset", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, COMOffset), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_COMOffset_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_COMOffset_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_MaxSteerAngle_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Wheel" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_MaxSteerAngle = { "MaxSteerAngle", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, MaxSteerAngle), METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_MaxSteerAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_MaxSteerAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_WheelAnimZOffset_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Wheel" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_WheelAnimZOffset = { "WheelAnimZOffset", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, WheelAnimZOffset), METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_WheelAnimZOffset_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_WheelAnimZOffset_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_SteerCurve_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Car Propperties" },
		{ "Comment", "//Amount of steering applied versus speed, more speed less steering\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
		{ "ToolTip", "Amount of steering applied versus speed, more speed less steering" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_SteerCurve = { "SteerCurve", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, SteerCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_SteerCurve_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_SteerCurve_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_AckermanPercent_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Steering" },
		{ "Comment", "// 100 = full Ackerman, 0 = parallel, -100 = full anti-Ackerman\n" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
		{ "ToolTip", "100 = full Ackerman, 0 = parallel, -100 = full anti-Ackerman" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_AckermanPercent = { "AckermanPercent", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, AckermanPercent), METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_AckermanPercent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_AckermanPercent_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bVisualizeCOM_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bVisualizeCOM_SetBit(void* Obj)
	{
		((ACar*)Obj)->bVisualizeCOM = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bVisualizeCOM = { "bVisualizeCOM", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bVisualizeCOM_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bVisualizeCOM_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bVisualizeCOM_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bLogDeltaTime_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bLogDeltaTime_SetBit(void* Obj)
	{
		((ACar*)Obj)->bLogDeltaTime = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bLogDeltaTime = { "bLogDeltaTime", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bLogDeltaTime_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bLogDeltaTime_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bLogDeltaTime_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bLogVelocityKMH_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bLogVelocityKMH_SetBit(void* Obj)
	{
		((ACar*)Obj)->bLogVelocityKMH = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bLogVelocityKMH = { "bLogVelocityKMH", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bLogVelocityKMH_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bLogVelocityKMH_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bLogVelocityKMH_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bLogAngVelocity_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bLogAngVelocity_SetBit(void* Obj)
	{
		((ACar*)Obj)->bLogAngVelocity = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bLogAngVelocity = { "bLogAngVelocity", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bLogAngVelocity_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bLogAngVelocity_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bLogAngVelocity_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bLogAcceleration_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bLogAcceleration_SetBit(void* Obj)
	{
		((ACar*)Obj)->bLogAcceleration = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bLogAcceleration = { "bLogAcceleration", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bLogAcceleration_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bLogAcceleration_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bLogAcceleration_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelSteerAngle_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelSteerAngle_SetBit(void* Obj)
	{
		((ACar*)Obj)->bLogWheelSteerAngle = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelSteerAngle = { "bLogWheelSteerAngle", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelSteerAngle_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelSteerAngle_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelSteerAngle_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bLogCarData_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bLogCarData_SetBit(void* Obj)
	{
		((ACar*)Obj)->bLogCarData = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bLogCarData = { "bLogCarData", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bLogCarData_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bLogCarData_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bLogCarData_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelData_MetaData[] = {
		{ "AllowPrivateAccess", "TRUE" },
		{ "Category", "Debug" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	void Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelData_SetBit(void* Obj)
	{
		((ACar*)Obj)->bLogWheelData = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelData = { "bLogWheelData", nullptr, (EPropertyFlags)0x0020080000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ACar), &Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelData_SetBit, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelData_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelData_MetaData)) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_WheelColliders_Inner = { "WheelColliders", nullptr, (EPropertyFlags)0x0000000000080008, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UClass_UWheelCollider_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACar_Statics::NewProp_WheelColliders_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Vehicles/Physics/Car.h" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UClass_ACar_Statics::NewProp_WheelColliders = { "WheelColliders", nullptr, (EPropertyFlags)0x0020088000000008, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ACar, WheelColliders), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::NewProp_WheelColliders_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::NewProp_WheelColliders_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ACar_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_ClutchShaft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_TransferCaseShaft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_FrontRight,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_FrontLeft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_RearRight,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_RearLeft,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_CarMesh,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_SpringArm,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_Camera,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_Engine,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_TransmissionComponent,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_Drivetrain,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_ECU,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_HeunNumIterations,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_COMOffset,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_MaxSteerAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_WheelAnimZOffset,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_SteerCurve,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_AckermanPercent,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bVisualizeCOM,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bLogDeltaTime,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bLogVelocityKMH,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bLogAngVelocity,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bLogAcceleration,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelSteerAngle,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bLogCarData,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_bLogWheelData,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_WheelColliders_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACar_Statics::NewProp_WheelColliders,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ACar_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACar>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ACar_Statics::ClassParams = {
		&ACar::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_ACar_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ACar_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ACar_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ACar()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ACar_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ACar, 3685809865);
	template<> VEHICLEPHYSICS_API UClass* StaticClass<ACar>()
	{
		return ACar::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ACar(Z_Construct_UClass_ACar, &ACar::StaticClass, TEXT("/Script/VehiclePhysics"), TEXT("ACar"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ACar);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
