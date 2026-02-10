// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VEHICLEPHYSICS_Car_generated_h
#error "Car.generated.h already included, missing '#pragma once' in Car.h"
#endif
#define VEHICLEPHYSICS_Car_generated_h

#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_SPARSE_DATA
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execResetCar);


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execResetCar);


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesACar(); \
	friend struct Z_Construct_UClass_ACar_Statics; \
public: \
	DECLARE_CLASS(ACar, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(ACar)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_INCLASS \
private: \
	static void StaticRegisterNativesACar(); \
	friend struct Z_Construct_UClass_ACar_Statics; \
public: \
	DECLARE_CLASS(ACar, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(ACar)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ACar(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ACar) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACar); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACar); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ACar(ACar&&); \
	NO_API ACar(const ACar&); \
public:


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ACar(ACar&&); \
	NO_API ACar(const ACar&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACar); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACar); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ACar)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__FrontRight() { return STRUCT_OFFSET(ACar, FrontRight); } \
	FORCEINLINE static uint32 __PPO__FrontLeft() { return STRUCT_OFFSET(ACar, FrontLeft); } \
	FORCEINLINE static uint32 __PPO__RearRight() { return STRUCT_OFFSET(ACar, RearRight); } \
	FORCEINLINE static uint32 __PPO__RearLeft() { return STRUCT_OFFSET(ACar, RearLeft); } \
	FORCEINLINE static uint32 __PPO__CarMesh() { return STRUCT_OFFSET(ACar, CarMesh); } \
	FORCEINLINE static uint32 __PPO__SpringArm() { return STRUCT_OFFSET(ACar, SpringArm); } \
	FORCEINLINE static uint32 __PPO__Camera() { return STRUCT_OFFSET(ACar, Camera); } \
	FORCEINLINE static uint32 __PPO__Engine() { return STRUCT_OFFSET(ACar, Engine); } \
	FORCEINLINE static uint32 __PPO__TransmissionComponent() { return STRUCT_OFFSET(ACar, TransmissionComponent); } \
	FORCEINLINE static uint32 __PPO__Drivetrain() { return STRUCT_OFFSET(ACar, Drivetrain); } \
	FORCEINLINE static uint32 __PPO__ECU() { return STRUCT_OFFSET(ACar, ECU); } \
	FORCEINLINE static uint32 __PPO__HeunNumIterations() { return STRUCT_OFFSET(ACar, HeunNumIterations); } \
	FORCEINLINE static uint32 __PPO__COMOffset() { return STRUCT_OFFSET(ACar, COMOffset); } \
	FORCEINLINE static uint32 __PPO__MaxSteerAngle() { return STRUCT_OFFSET(ACar, MaxSteerAngle); } \
	FORCEINLINE static uint32 __PPO__WheelAnimZOffset() { return STRUCT_OFFSET(ACar, WheelAnimZOffset); } \
	FORCEINLINE static uint32 __PPO__SteerCurve() { return STRUCT_OFFSET(ACar, SteerCurve); } \
	FORCEINLINE static uint32 __PPO__AckermanPercent() { return STRUCT_OFFSET(ACar, AckermanPercent); } \
	FORCEINLINE static uint32 __PPO__bVisualizeCOM() { return STRUCT_OFFSET(ACar, bVisualizeCOM); } \
	FORCEINLINE static uint32 __PPO__bLogDeltaTime() { return STRUCT_OFFSET(ACar, bLogDeltaTime); } \
	FORCEINLINE static uint32 __PPO__bLogVelocityKMH() { return STRUCT_OFFSET(ACar, bLogVelocityKMH); } \
	FORCEINLINE static uint32 __PPO__bLogAngVelocity() { return STRUCT_OFFSET(ACar, bLogAngVelocity); } \
	FORCEINLINE static uint32 __PPO__bLogAcceleration() { return STRUCT_OFFSET(ACar, bLogAcceleration); } \
	FORCEINLINE static uint32 __PPO__bLogWheelSteerAngle() { return STRUCT_OFFSET(ACar, bLogWheelSteerAngle); } \
	FORCEINLINE static uint32 __PPO__bLogCarData() { return STRUCT_OFFSET(ACar, bLogCarData); } \
	FORCEINLINE static uint32 __PPO__bLogWheelData() { return STRUCT_OFFSET(ACar, bLogWheelData); } \
	FORCEINLINE static uint32 __PPO__WheelColliders() { return STRUCT_OFFSET(ACar, WheelColliders); }


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_25_PROLOG
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_RPC_WRAPPERS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_INCLASS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h_28_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> VEHICLEPHYSICS_API UClass* StaticClass<class ACar>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Car_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
