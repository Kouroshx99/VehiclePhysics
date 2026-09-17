// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheelContactFrame.generated.h"

/**
 * Everything a tyre model needs to know about one contact patch, and nothing
 * about what produced it.
 *
 * UNITS: positions are world cm and velocities world cm/s - the Unreal domain,
 * matching the suspension that fills this in. Every other field carries its unit
 * in its name. The tyre model is strictly SI, so the consumer converts; this
 * struct is the boundary where that conversion belongs.
 */

USTRUCT(BlueprintType)
struct VEHICLEPHYSICS_API FWheelContactFrame
{
	GENERATED_USTRUCT_BODY()

	bool bInContact = false;
	
	FVector ContactPointWS = FVector::ZeroVector;

	/**
	 * Orthonormal frame at the patch. Forward and Lateral lie IN the road plane,
	 * with Lateral = Normal x Forward, so Lateral points to the vehicle's right on
	 * both sides of the car and the slip signs need no per-corner correction.
	 */
	FVector NormalWS = FVector::UpVector;

	FVector ForwardWS = FVector::ForwardVector;
	FVector LateralWS = FVector::RightVector;
	
	/** Velocity of the vehicle's material point at the patch, world cm/s. */
	FVector ContactVelocityWS = FVector::ZeroVector;
	
	/**
	 * Camber against the ROAD, not the body - the two differ under body roll and on
	 * any slope, and the Magic Formula wants the road one.
	 *
	 * Positive leans the top of the wheel toward the wheel's own RIGHT (it turns
	 * with steer), and means the same thing on both sides of the car.
	 *
	 * NOT "outboard", and NOT the same convention as StaticCamberDeg or
	 * LiveCamberDeg, which are positive-OUTBOARD and therefore mirror between
	 * sides. This one deliberately does not mirror - the tyre model puts slip, Fy
	 * and Mz in a single frame, so its camber has to live in that frame too. Expect
	 * the two numbers to report opposite signs on the left corners; that is correct.
	 */
	float CamberRad = 0.f;
	
	/** Pacejka's rho_z, cm. The input a load model wants (MF 6.1.2 eqn 4.E68). */
	float DeflectionCm = 0.f;

	/** Vertical load the suspension is carrying here, N. */
	float LoadN = 0.f;

};
