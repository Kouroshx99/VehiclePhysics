// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VEHICLEPHYSICS_CarController_generated_h
#error "CarController.generated.h already included, missing '#pragma once' in CarController.h"
#endif
#define VEHICLEPHYSICS_CarController_generated_h

#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_SPARSE_DATA
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetGear); \
	DECLARE_FUNCTION(execGetEngineRPM); \
	DECLARE_FUNCTION(execGetSpeedKMH);


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetGear); \
	DECLARE_FUNCTION(execGetEngineRPM); \
	DECLARE_FUNCTION(execGetSpeedKMH);


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesACarController(); \
	friend struct Z_Construct_UClass_ACarController_Statics; \
public: \
	DECLARE_CLASS(ACarController, APlayerController, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(ACarController)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_INCLASS \
private: \
	static void StaticRegisterNativesACarController(); \
	friend struct Z_Construct_UClass_ACarController_Statics; \
public: \
	DECLARE_CLASS(ACarController, APlayerController, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(ACarController)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ACarController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ACarController) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACarController); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACarController); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ACarController(ACarController&&); \
	NO_API ACarController(const ACarController&); \
public:


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ACarController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ACarController(ACarController&&); \
	NO_API ACarController(const ACarController&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ACarController); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ACarController); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ACarController)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_PRIVATE_PROPERTY_OFFSET
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_10_PROLOG
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_RPC_WRAPPERS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_INCLASS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> VEHICLEPHYSICS_API UClass* StaticClass<class ACarController>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_CarController_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
