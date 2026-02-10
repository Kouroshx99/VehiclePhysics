// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VEHICLEPHYSICS_CarAnimInstance_generated_h
#error "CarAnimInstance.generated.h already included, missing '#pragma once' in CarAnimInstance.h"
#endif
#define VEHICLEPHYSICS_CarAnimInstance_generated_h

#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_SPARSE_DATA
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_RPC_WRAPPERS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_RPC_WRAPPERS_NO_PURE_DECLS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUCarAnimInstance(); \
	friend struct Z_Construct_UClass_UCarAnimInstance_Statics; \
public: \
	DECLARE_CLASS(UCarAnimInstance, UAnimInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UCarAnimInstance)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_INCLASS \
private: \
	static void StaticRegisterNativesUCarAnimInstance(); \
	friend struct Z_Construct_UClass_UCarAnimInstance_Statics; \
public: \
	DECLARE_CLASS(UCarAnimInstance, UAnimInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UCarAnimInstance)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UCarAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCarAnimInstance) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCarAnimInstance); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCarAnimInstance); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UCarAnimInstance(UCarAnimInstance&&); \
	NO_API UCarAnimInstance(const UCarAnimInstance&); \
public:


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UCarAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UCarAnimInstance(UCarAnimInstance&&); \
	NO_API UCarAnimInstance(const UCarAnimInstance&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCarAnimInstance); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCarAnimInstance); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCarAnimInstance)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__Car() { return STRUCT_OFFSET(UCarAnimInstance, Car); } \
	FORCEINLINE static uint32 __PPO__FrontRightRotation() { return STRUCT_OFFSET(UCarAnimInstance, FrontRightRotation); } \
	FORCEINLINE static uint32 __PPO__FrontLeftRotation() { return STRUCT_OFFSET(UCarAnimInstance, FrontLeftRotation); } \
	FORCEINLINE static uint32 __PPO__RearRightRotation() { return STRUCT_OFFSET(UCarAnimInstance, RearRightRotation); } \
	FORCEINLINE static uint32 __PPO__RearLeftRotation() { return STRUCT_OFFSET(UCarAnimInstance, RearLeftRotation); } \
	FORCEINLINE static uint32 __PPO__FrontRightAngle() { return STRUCT_OFFSET(UCarAnimInstance, FrontRightAngle); } \
	FORCEINLINE static uint32 __PPO__FrontLeftAngle() { return STRUCT_OFFSET(UCarAnimInstance, FrontLeftAngle); } \
	FORCEINLINE static uint32 __PPO__FrontRightCenterZ() { return STRUCT_OFFSET(UCarAnimInstance, FrontRightCenterZ); } \
	FORCEINLINE static uint32 __PPO__FrontLeftCenterZ() { return STRUCT_OFFSET(UCarAnimInstance, FrontLeftCenterZ); } \
	FORCEINLINE static uint32 __PPO__RearRightCenterZ() { return STRUCT_OFFSET(UCarAnimInstance, RearRightCenterZ); } \
	FORCEINLINE static uint32 __PPO__RearLeftCenterZ() { return STRUCT_OFFSET(UCarAnimInstance, RearLeftCenterZ); } \
	FORCEINLINE static uint32 __PPO__FrontRightRoll() { return STRUCT_OFFSET(UCarAnimInstance, FrontRightRoll); } \
	FORCEINLINE static uint32 __PPO__FrontLeftRoll() { return STRUCT_OFFSET(UCarAnimInstance, FrontLeftRoll); } \
	FORCEINLINE static uint32 __PPO__RearRightRoll() { return STRUCT_OFFSET(UCarAnimInstance, RearRightRoll); } \
	FORCEINLINE static uint32 __PPO__RearLeftRoll() { return STRUCT_OFFSET(UCarAnimInstance, RearLeftRoll); } \
	FORCEINLINE static uint32 __PPO__FrontRight() { return STRUCT_OFFSET(UCarAnimInstance, FrontRight); } \
	FORCEINLINE static uint32 __PPO__FrontLeft() { return STRUCT_OFFSET(UCarAnimInstance, FrontLeft); } \
	FORCEINLINE static uint32 __PPO__RearRight() { return STRUCT_OFFSET(UCarAnimInstance, RearRight); } \
	FORCEINLINE static uint32 __PPO__RearLeft() { return STRUCT_OFFSET(UCarAnimInstance, RearLeft); } \
	FORCEINLINE static uint32 __PPO__PosFR() { return STRUCT_OFFSET(UCarAnimInstance, PosFR); } \
	FORCEINLINE static uint32 __PPO__PosFL() { return STRUCT_OFFSET(UCarAnimInstance, PosFL); } \
	FORCEINLINE static uint32 __PPO__PosRR() { return STRUCT_OFFSET(UCarAnimInstance, PosRR); } \
	FORCEINLINE static uint32 __PPO__PosRL() { return STRUCT_OFFSET(UCarAnimInstance, PosRL); } \
	FORCEINLINE static uint32 __PPO__RestFR() { return STRUCT_OFFSET(UCarAnimInstance, RestFR); } \
	FORCEINLINE static uint32 __PPO__RestFL() { return STRUCT_OFFSET(UCarAnimInstance, RestFL); } \
	FORCEINLINE static uint32 __PPO__RestRR() { return STRUCT_OFFSET(UCarAnimInstance, RestRR); } \
	FORCEINLINE static uint32 __PPO__RestRL() { return STRUCT_OFFSET(UCarAnimInstance, RestRL); }


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_13_PROLOG
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_RPC_WRAPPERS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_INCLASS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> VEHICLEPHYSICS_API UClass* StaticClass<class UCarAnimInstance>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Animations_CarAnimInstance_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
