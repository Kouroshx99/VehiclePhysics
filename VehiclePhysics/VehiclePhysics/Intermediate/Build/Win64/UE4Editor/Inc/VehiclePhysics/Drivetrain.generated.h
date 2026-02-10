// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VEHICLEPHYSICS_Drivetrain_generated_h
#error "Drivetrain.generated.h already included, missing '#pragma once' in Drivetrain.h"
#endif
#define VEHICLEPHYSICS_Drivetrain_generated_h

#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_SPARSE_DATA
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_RPC_WRAPPERS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_RPC_WRAPPERS_NO_PURE_DECLS
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUDrivetrain(); \
	friend struct Z_Construct_UClass_UDrivetrain_Statics; \
public: \
	DECLARE_CLASS(UDrivetrain, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UDrivetrain)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_INCLASS \
private: \
	static void StaticRegisterNativesUDrivetrain(); \
	friend struct Z_Construct_UClass_UDrivetrain_Statics; \
public: \
	DECLARE_CLASS(UDrivetrain, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UDrivetrain)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UDrivetrain(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UDrivetrain) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDrivetrain); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDrivetrain); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UDrivetrain(UDrivetrain&&); \
	NO_API UDrivetrain(const UDrivetrain&); \
public:


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UDrivetrain(UDrivetrain&&); \
	NO_API UDrivetrain(const UDrivetrain&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UDrivetrain); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UDrivetrain); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UDrivetrain)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_PRIVATE_PROPERTY_OFFSET
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_22_PROLOG
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_RPC_WRAPPERS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_INCLASS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h_25_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> VEHICLEPHYSICS_API UClass* StaticClass<class UDrivetrain>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_Drivetrain_h


#define FOREACH_ENUM_EDRIVETRAINTYPE(op) \
	op(EDrivetrainType::EDT_FrontWheelDrive) \
	op(EDrivetrainType::EDT_RearWheelDrive) \
	op(EDrivetrainType::EDT_AllWheelDrive) 

enum class EDrivetrainType : uint8;
template<> VEHICLEPHYSICS_API UEnum* StaticEnum<EDrivetrainType>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
