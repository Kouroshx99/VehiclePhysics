// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef VEHICLEPHYSICS_Wheel_generated_h
#error "Wheel.generated.h already included, missing '#pragma once' in Wheel.h"
#endif
#define VEHICLEPHYSICS_Wheel_generated_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID HostProject_Plugins_VehiclePhysics_Source_VehiclePhysics_Vehicles_Types_Wheel_h


#define FOREACH_ENUM_EWHEEL(op) \
	op(EWheel::EW_FrontRight) \
	op(EWheel::EW_FrontLeft) \
	op(EWheel::EW_RearRight) \
	op(EWheel::EW_RearLeft) 

enum class EWheel : uint8;
template<> VEHICLEPHYSICS_API UEnum* StaticEnum<EWheel>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
