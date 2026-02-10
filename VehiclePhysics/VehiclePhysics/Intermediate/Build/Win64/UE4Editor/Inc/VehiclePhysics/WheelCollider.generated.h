// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VEHICLEPHYSICS_WheelCollider_generated_h
#error "WheelCollider.generated.h already included, missing '#pragma once' in WheelCollider.h"
#endif
#define VEHICLEPHYSICS_WheelCollider_generated_h

#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_SPARSE_DATA
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_RPC_WRAPPERS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_RPC_WRAPPERS_NO_PURE_DECLS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUWheelCollider(); \
	friend struct Z_Construct_UClass_UWheelCollider_Statics; \
public: \
	DECLARE_CLASS(UWheelCollider, USceneComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UWheelCollider)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_INCLASS \
private: \
	static void StaticRegisterNativesUWheelCollider(); \
	friend struct Z_Construct_UClass_UWheelCollider_Statics; \
public: \
	DECLARE_CLASS(UWheelCollider, USceneComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UWheelCollider)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UWheelCollider(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UWheelCollider) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UWheelCollider); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UWheelCollider); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UWheelCollider(UWheelCollider&&); \
	NO_API UWheelCollider(const UWheelCollider&); \
public:


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UWheelCollider(UWheelCollider&&); \
	NO_API UWheelCollider(const UWheelCollider&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UWheelCollider); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UWheelCollider); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UWheelCollider)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__TireParams() { return STRUCT_OFFSET(UWheelCollider, TireParams); } \
	FORCEINLINE static uint32 __PPO__WheelRadius() { return STRUCT_OFFSET(UWheelCollider, WheelRadius); } \
	FORCEINLINE static uint32 __PPO__SteeringResponsiveness() { return STRUCT_OFFSET(UWheelCollider, SteeringResponsiveness); } \
	FORCEINLINE static uint32 __PPO__AngVelItersSlip() { return STRUCT_OFFSET(UWheelCollider, AngVelItersSlip); } \
	FORCEINLINE static uint32 __PPO__AngVelItersNoSlip() { return STRUCT_OFFSET(UWheelCollider, AngVelItersNoSlip); } \
	FORCEINLINE static uint32 __PPO__SpringStiffness_NM() { return STRUCT_OFFSET(UWheelCollider, SpringStiffness_NM); } \
	FORCEINLINE static uint32 __PPO__DamperCoefficient() { return STRUCT_OFFSET(UWheelCollider, DamperCoefficient); } \
	FORCEINLINE static uint32 __PPO__AntiRollStiffness() { return STRUCT_OFFSET(UWheelCollider, AntiRollStiffness); } \
	FORCEINLINE static uint32 __PPO__RestLenght() { return STRUCT_OFFSET(UWheelCollider, RestLenght); } \
	FORCEINLINE static uint32 __PPO__MinSpringTravel() { return STRUCT_OFFSET(UWheelCollider, MinSpringTravel); } \
	FORCEINLINE static uint32 __PPO__MaxSpringTravel() { return STRUCT_OFFSET(UWheelCollider, MaxSpringTravel); } \
	FORCEINLINE static uint32 __PPO__CasterAngle() { return STRUCT_OFFSET(UWheelCollider, CasterAngle); } \
	FORCEINLINE static uint32 __PPO__StaticCamberAngle() { return STRUCT_OFFSET(UWheelCollider, StaticCamberAngle); } \
	FORCEINLINE static uint32 __PPO__ToeAngle() { return STRUCT_OFFSET(UWheelCollider, ToeAngle); } \
	FORCEINLINE static uint32 __PPO__KingPinAngle() { return STRUCT_OFFSET(UWheelCollider, KingPinAngle); } \
	FORCEINLINE static uint32 __PPO__HubOffset() { return STRUCT_OFFSET(UWheelCollider, HubOffset); } \
	FORCEINLINE static uint32 __PPO__SATC() { return STRUCT_OFFSET(UWheelCollider, SATC); } \
	FORCEINLINE static uint32 __PPO__bDebug() { return STRUCT_OFFSET(UWheelCollider, bDebug); } \
	FORCEINLINE static uint32 __PPO__WheelShaft() { return STRUCT_OFFSET(UWheelCollider, WheelShaft); }


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_20_PROLOG
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_RPC_WRAPPERS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_INCLASS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h_23_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> VEHICLEPHYSICS_API UClass* StaticClass<class UWheelCollider>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_WheelCollider_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
