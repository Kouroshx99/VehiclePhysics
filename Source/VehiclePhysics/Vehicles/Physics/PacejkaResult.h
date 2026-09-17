#pragma once

#include "PacejkaResult.generated.h"

USTRUCT(BlueprintType)
struct FPacejkaResult
{
	GENERATED_USTRUCT_BODY()

	FPacejkaResult() {};

	float Kappa = 0.f;
	float Alpha = 0.f;

	float Ux0 = 0.f;
	float Ux = 0.f;
	float US_Vx = 0.f;
	float Fx0 = 0.f;
	float Fx = 0.f;

	float Uy0 = 0.f;
	float Uy = 0.f;
	float US_Vy = 0.f;
	float Fy0 = 0.f;
	float Fy = 0.f;

	/**
	 * Rolling resistance moment about the wheel's spin axis, N*m. Book (4.E70).
	 *
	 * A MOMENT, not a force. It reaches the vehicle by slowing the wheel, which
	 * changes slip, which changes Fx - applying it as a drag at the contact patch
	 * as well would count the same resistance twice.
	 */
	float My = 0.f;

	/**
	 * Aligning stiffness about the slip angle, N*m/rad. Book (4.E48), C_Malpha.
	 *
	 * Characterisation output, not an input to anything. Its value is that C_Malpha
	 * is one of the few tyre quantities routinely PUBLISHED, so it is directly
	 * comparable against a real tyre's data sheet - which makes it a way to check
	 * this coefficient set describes a plausible tyre rather than merely a
	 * self-consistent one.
	 */
	float Kzao = 0.f;

	/** Aligning stiffness about camber, N*m/rad. Book (4.E49), C_Mgamma. */
	float Kzgo = 0.f;

	float pneumaticTrail = 0.f;
	float Mz = 0.f;
	float Mz_normalized = 0.f;
	
};
