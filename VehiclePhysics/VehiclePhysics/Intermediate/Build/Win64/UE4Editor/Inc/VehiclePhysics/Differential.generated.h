// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VEHICLEPHYSICS_Differential_generated_h
#error "Differential.generated.h already included, missing '#pragma once' in Differential.h"
#endif
#define VEHICLEPHYSICS_Differential_generated_h

#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_SPARSE_DATA
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_RPC_WRAPPERS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_RPC_WRAPPERS_NO_PURE_DECLS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUDifferential(); \
	friend struct Z_Construct_UClass_UDifferential_Statics; \
public: \
	DECLARE_CLASS(UDifferential, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UDifferential)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_INCLASS \
private: \
	static void StaticRegisterNativesUDifferential(); \
	friend struct Z_Construct_UClass_UDifferential_Statics; \
public: \
	DECLARE_CLASS(UDifferential, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UDifferential)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UDifferential(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UDifferential) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDifferential); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDifferential); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UDifferential(UDifferential&&); \
	NO_API UDifferential(const UDifferential&); \
public:


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UDifferential(UDifferential&&); \
	NO_API UDifferential(const UDifferential&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDifferential); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDifferential); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UDifferential)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__Shaft() { return STRUCT_OFFSET(UDifferential, Shaft); } \
	FORCEINLINE static uint32 __PPO__bIsLocked() { return STRUCT_OFFSET(UDifferential, bIsLocked); } \
	FORCEINLINE static uint32 __PPO__bIsTorsen() { return STRUCT_OFFSET(UDifferential, bIsTorsen); } \
	FORCEINLINE static uint32 __PPO__FinalDriveRatio() { return STRUCT_OFFSET(UDifferential, FinalDriveRatio); } \
	FORCEINLINE static uint32 __PPO__BiasRatio() { return STRUCT_OFFSET(UDifferential, BiasRatio); } \
	FORCEINLINE static uint32 __PPO__TorsenSlipLimit() { return STRUCT_OFFSET(UDifferential, TorsenSlipLimit); } \
	FORCEINLINE static uint32 __PPO__TorsenPreLoad() { return STRUCT_OFFSET(UDifferential, TorsenPreLoad); } \
	FORCEINLINE static uint32 __PPO__bIsLimitedSlip() { return STRUCT_OFFSET(UDifferential, bIsLimitedSlip); }


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_14_PROLOG
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_RPC_WRAPPERS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_INCLASS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> VEHICLEPHYSICS_API UClass* StaticClass<class UDifferential>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Differential_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
