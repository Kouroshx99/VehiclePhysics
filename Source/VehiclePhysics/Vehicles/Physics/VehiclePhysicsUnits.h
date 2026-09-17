// VehiclePhysicsUnits.h
#pragma once

/**
 * Unit conversions shared across the plugin's physics code. Unreal's domain is
 * centimetres and Unreal force units (UU = kg*cm/s^2); every tire and
 * suspension model here works in SI (m, N, kg, rad). These conversions cross
 * that boundary and are used identically everywhere, so they are declared
 * exactly once rather than re-declared, under a different name, per file.
 */
namespace VehicleUnits
{
	constexpr float CmToM = 0.01f;
	constexpr float MToCm = 100.f;
	constexpr float NToUU = 100.f;              // 1 N = 100 UU
	constexpr float MPerSecToKMPerHour = 3.6f;
}