// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FPacejkaResult;
#ifdef VEHICLEPHYSICS_TireParamsDataAsset_generated_h
#error "TireParamsDataAsset.generated.h already included, missing '#pragma once' in TireParamsDataAsset.h"
#endif
#define VEHICLEPHYSICS_TireParamsDataAsset_generated_h

#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_SPARSE_DATA
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execComputeTireForcesOverriden); \
	DECLARE_FUNCTION(execComputeTireForces);


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execComputeTireForcesOverriden); \
	DECLARE_FUNCTION(execComputeTireForces);


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUTireParamsDataAsset(); \
	friend struct Z_Construct_UClass_UTireParamsDataAsset_Statics; \
public: \
	DECLARE_CLASS(UTireParamsDataAsset, UDataAsset, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UTireParamsDataAsset)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_INCLASS \
private: \
	static void StaticRegisterNativesUTireParamsDataAsset(); \
	friend struct Z_Construct_UClass_UTireParamsDataAsset_Statics; \
public: \
	DECLARE_CLASS(UTireParamsDataAsset, UDataAsset, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/VehiclePhysics"), NO_API) \
	DECLARE_SERIALIZER(UTireParamsDataAsset)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UTireParamsDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTireParamsDataAsset) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTireParamsDataAsset); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTireParamsDataAsset); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTireParamsDataAsset(UTireParamsDataAsset&&); \
	NO_API UTireParamsDataAsset(const UTireParamsDataAsset&); \
public:


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UTireParamsDataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UTireParamsDataAsset(UTireParamsDataAsset&&); \
	NO_API UTireParamsDataAsset(const UTireParamsDataAsset&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UTireParamsDataAsset); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTireParamsDataAsset); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTireParamsDataAsset)


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__LFZ0() { return STRUCT_OFFSET(UTireParamsDataAsset, LFZ0); } \
	FORCEINLINE static uint32 __PPO__LCX() { return STRUCT_OFFSET(UTireParamsDataAsset, LCX); } \
	FORCEINLINE static uint32 __PPO__LMUX() { return STRUCT_OFFSET(UTireParamsDataAsset, LMUX); } \
	FORCEINLINE static uint32 __PPO__LEX() { return STRUCT_OFFSET(UTireParamsDataAsset, LEX); } \
	FORCEINLINE static uint32 __PPO__LHX() { return STRUCT_OFFSET(UTireParamsDataAsset, LHX); } \
	FORCEINLINE static uint32 __PPO__LVX() { return STRUCT_OFFSET(UTireParamsDataAsset, LVX); } \
	FORCEINLINE static uint32 __PPO__LXAL() { return STRUCT_OFFSET(UTireParamsDataAsset, LXAL); } \
	FORCEINLINE static uint32 __PPO__LCY() { return STRUCT_OFFSET(UTireParamsDataAsset, LCY); } \
	FORCEINLINE static uint32 __PPO__LMUY() { return STRUCT_OFFSET(UTireParamsDataAsset, LMUY); } \
	FORCEINLINE static uint32 __PPO__LEY() { return STRUCT_OFFSET(UTireParamsDataAsset, LEY); } \
	FORCEINLINE static uint32 __PPO__LKYAL() { return STRUCT_OFFSET(UTireParamsDataAsset, LKYAL); } \
	FORCEINLINE static uint32 __PPO__LKZC() { return STRUCT_OFFSET(UTireParamsDataAsset, LKZC); } \
	FORCEINLINE static uint32 __PPO__LHY() { return STRUCT_OFFSET(UTireParamsDataAsset, LHY); } \
	FORCEINLINE static uint32 __PPO__LVY() { return STRUCT_OFFSET(UTireParamsDataAsset, LVY); } \
	FORCEINLINE static uint32 __PPO__LT() { return STRUCT_OFFSET(UTireParamsDataAsset, LT); } \
	FORCEINLINE static uint32 __PPO__LYK() { return STRUCT_OFFSET(UTireParamsDataAsset, LYK); } \
	FORCEINLINE static uint32 __PPO__LVYK() { return STRUCT_OFFSET(UTireParamsDataAsset, LVYK); } \
	FORCEINLINE static uint32 __PPO__LMUV() { return STRUCT_OFFSET(UTireParamsDataAsset, LMUV); } \
	FORCEINLINE static uint32 __PPO__LS() { return STRUCT_OFFSET(UTireParamsDataAsset, LS); } \
	FORCEINLINE static uint32 __PPO__LMR() { return STRUCT_OFFSET(UTireParamsDataAsset, LMR); } \
	FORCEINLINE static uint32 __PPO__zeta_0() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_0); } \
	FORCEINLINE static uint32 __PPO__zeta_1() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_1); } \
	FORCEINLINE static uint32 __PPO__zeta_2() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_2); } \
	FORCEINLINE static uint32 __PPO__zeta_3() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_3); } \
	FORCEINLINE static uint32 __PPO__zeta_4() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_4); } \
	FORCEINLINE static uint32 __PPO__zeta_5() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_5); } \
	FORCEINLINE static uint32 __PPO__zeta_6() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_6); } \
	FORCEINLINE static uint32 __PPO__zeta_7() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_7); } \
	FORCEINLINE static uint32 __PPO__zeta_8() { return STRUCT_OFFSET(UTireParamsDataAsset, zeta_8); } \
	FORCEINLINE static uint32 __PPO__PCX1() { return STRUCT_OFFSET(UTireParamsDataAsset, PCX1); } \
	FORCEINLINE static uint32 __PPO__PDX1() { return STRUCT_OFFSET(UTireParamsDataAsset, PDX1); } \
	FORCEINLINE static uint32 __PPO__PDX2() { return STRUCT_OFFSET(UTireParamsDataAsset, PDX2); } \
	FORCEINLINE static uint32 __PPO__PDX3() { return STRUCT_OFFSET(UTireParamsDataAsset, PDX3); } \
	FORCEINLINE static uint32 __PPO__PEX1() { return STRUCT_OFFSET(UTireParamsDataAsset, PEX1); } \
	FORCEINLINE static uint32 __PPO__PEX2() { return STRUCT_OFFSET(UTireParamsDataAsset, PEX2); } \
	FORCEINLINE static uint32 __PPO__PEX3() { return STRUCT_OFFSET(UTireParamsDataAsset, PEX3); } \
	FORCEINLINE static uint32 __PPO__PEX4() { return STRUCT_OFFSET(UTireParamsDataAsset, PEX4); } \
	FORCEINLINE static uint32 __PPO__PKX1() { return STRUCT_OFFSET(UTireParamsDataAsset, PKX1); } \
	FORCEINLINE static uint32 __PPO__PKX2() { return STRUCT_OFFSET(UTireParamsDataAsset, PKX2); } \
	FORCEINLINE static uint32 __PPO__PKX3() { return STRUCT_OFFSET(UTireParamsDataAsset, PKX3); } \
	FORCEINLINE static uint32 __PPO__PHX1() { return STRUCT_OFFSET(UTireParamsDataAsset, PHX1); } \
	FORCEINLINE static uint32 __PPO__PHX2() { return STRUCT_OFFSET(UTireParamsDataAsset, PHX2); } \
	FORCEINLINE static uint32 __PPO__PVX1() { return STRUCT_OFFSET(UTireParamsDataAsset, PVX1); } \
	FORCEINLINE static uint32 __PPO__PVX2() { return STRUCT_OFFSET(UTireParamsDataAsset, PVX2); } \
	FORCEINLINE static uint32 __PPO__RBX1() { return STRUCT_OFFSET(UTireParamsDataAsset, RBX1); } \
	FORCEINLINE static uint32 __PPO__RBX2() { return STRUCT_OFFSET(UTireParamsDataAsset, RBX2); } \
	FORCEINLINE static uint32 __PPO__RBX3() { return STRUCT_OFFSET(UTireParamsDataAsset, RBX3); } \
	FORCEINLINE static uint32 __PPO__RCX1() { return STRUCT_OFFSET(UTireParamsDataAsset, RCX1); } \
	FORCEINLINE static uint32 __PPO__REX1() { return STRUCT_OFFSET(UTireParamsDataAsset, REX1); } \
	FORCEINLINE static uint32 __PPO__REX2() { return STRUCT_OFFSET(UTireParamsDataAsset, REX2); } \
	FORCEINLINE static uint32 __PPO__RHX1() { return STRUCT_OFFSET(UTireParamsDataAsset, RHX1); } \
	FORCEINLINE static uint32 __PPO__PPX1() { return STRUCT_OFFSET(UTireParamsDataAsset, PPX1); } \
	FORCEINLINE static uint32 __PPO__PPX2() { return STRUCT_OFFSET(UTireParamsDataAsset, PPX2); } \
	FORCEINLINE static uint32 __PPO__PPX3() { return STRUCT_OFFSET(UTireParamsDataAsset, PPX3); } \
	FORCEINLINE static uint32 __PPO__PPX4() { return STRUCT_OFFSET(UTireParamsDataAsset, PPX4); } \
	FORCEINLINE static uint32 __PPO__PCY1() { return STRUCT_OFFSET(UTireParamsDataAsset, PCY1); } \
	FORCEINLINE static uint32 __PPO__PDY1() { return STRUCT_OFFSET(UTireParamsDataAsset, PDY1); } \
	FORCEINLINE static uint32 __PPO__PDY2() { return STRUCT_OFFSET(UTireParamsDataAsset, PDY2); } \
	FORCEINLINE static uint32 __PPO__PDY3() { return STRUCT_OFFSET(UTireParamsDataAsset, PDY3); } \
	FORCEINLINE static uint32 __PPO__PEY1() { return STRUCT_OFFSET(UTireParamsDataAsset, PEY1); } \
	FORCEINLINE static uint32 __PPO__PEY2() { return STRUCT_OFFSET(UTireParamsDataAsset, PEY2); } \
	FORCEINLINE static uint32 __PPO__PEY3() { return STRUCT_OFFSET(UTireParamsDataAsset, PEY3); } \
	FORCEINLINE static uint32 __PPO__PEY4() { return STRUCT_OFFSET(UTireParamsDataAsset, PEY4); } \
	FORCEINLINE static uint32 __PPO__PEY5() { return STRUCT_OFFSET(UTireParamsDataAsset, PEY5); } \
	FORCEINLINE static uint32 __PPO__PKY1() { return STRUCT_OFFSET(UTireParamsDataAsset, PKY1); } \
	FORCEINLINE static uint32 __PPO__PKY2() { return STRUCT_OFFSET(UTireParamsDataAsset, PKY2); } \
	FORCEINLINE static uint32 __PPO__PKY3() { return STRUCT_OFFSET(UTireParamsDataAsset, PKY3); } \
	FORCEINLINE static uint32 __PPO__PKY4() { return STRUCT_OFFSET(UTireParamsDataAsset, PKY4); } \
	FORCEINLINE static uint32 __PPO__PKY5() { return STRUCT_OFFSET(UTireParamsDataAsset, PKY5); } \
	FORCEINLINE static uint32 __PPO__PKY6() { return STRUCT_OFFSET(UTireParamsDataAsset, PKY6); } \
	FORCEINLINE static uint32 __PPO__PKY7() { return STRUCT_OFFSET(UTireParamsDataAsset, PKY7); } \
	FORCEINLINE static uint32 __PPO__PHY1() { return STRUCT_OFFSET(UTireParamsDataAsset, PHY1); } \
	FORCEINLINE static uint32 __PPO__PHY2() { return STRUCT_OFFSET(UTireParamsDataAsset, PHY2); } \
	FORCEINLINE static uint32 __PPO__PVY1() { return STRUCT_OFFSET(UTireParamsDataAsset, PVY1); } \
	FORCEINLINE static uint32 __PPO__PVY2() { return STRUCT_OFFSET(UTireParamsDataAsset, PVY2); } \
	FORCEINLINE static uint32 __PPO__PVY3() { return STRUCT_OFFSET(UTireParamsDataAsset, PVY3); } \
	FORCEINLINE static uint32 __PPO__PVY4() { return STRUCT_OFFSET(UTireParamsDataAsset, PVY4); } \
	FORCEINLINE static uint32 __PPO__RBY1() { return STRUCT_OFFSET(UTireParamsDataAsset, RBY1); } \
	FORCEINLINE static uint32 __PPO__RBY2() { return STRUCT_OFFSET(UTireParamsDataAsset, RBY2); } \
	FORCEINLINE static uint32 __PPO__RBY3() { return STRUCT_OFFSET(UTireParamsDataAsset, RBY3); } \
	FORCEINLINE static uint32 __PPO__RBY4() { return STRUCT_OFFSET(UTireParamsDataAsset, RBY4); } \
	FORCEINLINE static uint32 __PPO__RCY1() { return STRUCT_OFFSET(UTireParamsDataAsset, RCY1); } \
	FORCEINLINE static uint32 __PPO__REY1() { return STRUCT_OFFSET(UTireParamsDataAsset, REY1); } \
	FORCEINLINE static uint32 __PPO__REY2() { return STRUCT_OFFSET(UTireParamsDataAsset, REY2); } \
	FORCEINLINE static uint32 __PPO__RHY1() { return STRUCT_OFFSET(UTireParamsDataAsset, RHY1); } \
	FORCEINLINE static uint32 __PPO__RHY2() { return STRUCT_OFFSET(UTireParamsDataAsset, RHY2); } \
	FORCEINLINE static uint32 __PPO__RVY1() { return STRUCT_OFFSET(UTireParamsDataAsset, RVY1); } \
	FORCEINLINE static uint32 __PPO__RVY2() { return STRUCT_OFFSET(UTireParamsDataAsset, RVY2); } \
	FORCEINLINE static uint32 __PPO__RVY3() { return STRUCT_OFFSET(UTireParamsDataAsset, RVY3); } \
	FORCEINLINE static uint32 __PPO__RVY4() { return STRUCT_OFFSET(UTireParamsDataAsset, RVY4); } \
	FORCEINLINE static uint32 __PPO__RBY5() { return STRUCT_OFFSET(UTireParamsDataAsset, RBY5); } \
	FORCEINLINE static uint32 __PPO__RVY6() { return STRUCT_OFFSET(UTireParamsDataAsset, RVY6); } \
	FORCEINLINE static uint32 __PPO__PPY1() { return STRUCT_OFFSET(UTireParamsDataAsset, PPY1); } \
	FORCEINLINE static uint32 __PPO__PPY2() { return STRUCT_OFFSET(UTireParamsDataAsset, PPY2); } \
	FORCEINLINE static uint32 __PPO__PPY3() { return STRUCT_OFFSET(UTireParamsDataAsset, PPY3); } \
	FORCEINLINE static uint32 __PPO__PPY4() { return STRUCT_OFFSET(UTireParamsDataAsset, PPY4); } \
	FORCEINLINE static uint32 __PPO__PPY5() { return STRUCT_OFFSET(UTireParamsDataAsset, PPY5); } \
	FORCEINLINE static uint32 __PPO__QBZ1() { return STRUCT_OFFSET(UTireParamsDataAsset, QBZ1); } \
	FORCEINLINE static uint32 __PPO__QBZ2() { return STRUCT_OFFSET(UTireParamsDataAsset, QBZ2); } \
	FORCEINLINE static uint32 __PPO__QBZ3() { return STRUCT_OFFSET(UTireParamsDataAsset, QBZ3); } \
	FORCEINLINE static uint32 __PPO__QBZ5() { return STRUCT_OFFSET(UTireParamsDataAsset, QBZ5); } \
	FORCEINLINE static uint32 __PPO__QBZ6() { return STRUCT_OFFSET(UTireParamsDataAsset, QBZ6); } \
	FORCEINLINE static uint32 __PPO__QBZ9() { return STRUCT_OFFSET(UTireParamsDataAsset, QBZ9); } \
	FORCEINLINE static uint32 __PPO__QBZ10() { return STRUCT_OFFSET(UTireParamsDataAsset, QBZ10); } \
	FORCEINLINE static uint32 __PPO__QCZ1() { return STRUCT_OFFSET(UTireParamsDataAsset, QCZ1); } \
	FORCEINLINE static uint32 __PPO__QDZ1() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ1); } \
	FORCEINLINE static uint32 __PPO__QDZ2() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ2); } \
	FORCEINLINE static uint32 __PPO__QDZ3() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ3); } \
	FORCEINLINE static uint32 __PPO__QD4() { return STRUCT_OFFSET(UTireParamsDataAsset, QD4); } \
	FORCEINLINE static uint32 __PPO__QDZ6() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ6); } \
	FORCEINLINE static uint32 __PPO__QDZ7() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ7); } \
	FORCEINLINE static uint32 __PPO__QDZ8() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ8); } \
	FORCEINLINE static uint32 __PPO__QDZ9() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ9); } \
	FORCEINLINE static uint32 __PPO__QDZ10() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ10); } \
	FORCEINLINE static uint32 __PPO__QDZ11() { return STRUCT_OFFSET(UTireParamsDataAsset, QDZ11); } \
	FORCEINLINE static uint32 __PPO__QEZ1() { return STRUCT_OFFSET(UTireParamsDataAsset, QEZ1); } \
	FORCEINLINE static uint32 __PPO__QEZ2() { return STRUCT_OFFSET(UTireParamsDataAsset, QEZ2); } \
	FORCEINLINE static uint32 __PPO__QEZ3() { return STRUCT_OFFSET(UTireParamsDataAsset, QEZ3); } \
	FORCEINLINE static uint32 __PPO__QEZ4() { return STRUCT_OFFSET(UTireParamsDataAsset, QEZ4); } \
	FORCEINLINE static uint32 __PPO__QEZ5() { return STRUCT_OFFSET(UTireParamsDataAsset, QEZ5); } \
	FORCEINLINE static uint32 __PPO__QHZ1() { return STRUCT_OFFSET(UTireParamsDataAsset, QHZ1); } \
	FORCEINLINE static uint32 __PPO__QHZ2() { return STRUCT_OFFSET(UTireParamsDataAsset, QHZ2); } \
	FORCEINLINE static uint32 __PPO__QHZ3() { return STRUCT_OFFSET(UTireParamsDataAsset, QHZ3); } \
	FORCEINLINE static uint32 __PPO__QHZ4() { return STRUCT_OFFSET(UTireParamsDataAsset, QHZ4); } \
	FORCEINLINE static uint32 __PPO__SSZ1() { return STRUCT_OFFSET(UTireParamsDataAsset, SSZ1); } \
	FORCEINLINE static uint32 __PPO__SSZ2() { return STRUCT_OFFSET(UTireParamsDataAsset, SSZ2); } \
	FORCEINLINE static uint32 __PPO__SSZ3() { return STRUCT_OFFSET(UTireParamsDataAsset, SSZ3); } \
	FORCEINLINE static uint32 __PPO__SSZ4() { return STRUCT_OFFSET(UTireParamsDataAsset, SSZ4); } \
	FORCEINLINE static uint32 __PPO__PPZ1() { return STRUCT_OFFSET(UTireParamsDataAsset, PPZ1); } \
	FORCEINLINE static uint32 __PPO__PPZ2() { return STRUCT_OFFSET(UTireParamsDataAsset, PPZ2); } \
	FORCEINLINE static uint32 __PPO__epsilon() { return STRUCT_OFFSET(UTireParamsDataAsset, epsilon); }


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_10_PROLOG
#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_RPC_WRAPPERS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_INCLASS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_PRIVATE_PROPERTY_OFFSET \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_SPARSE_DATA \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_INCLASS_NO_PURE_DECLS \
	HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h_13_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> VEHICLEPHYSICS_API UClass* StaticClass<class UTireParamsDataAsset>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Physics_TireParamsDataAsset_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
