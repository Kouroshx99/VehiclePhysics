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

	float pneumaticTrail = 0.f;
	float Mz = 0.f;
	float Mz_normalized = 0.f;
	
};
