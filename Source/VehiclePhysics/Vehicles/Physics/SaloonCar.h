// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Car.h"
// By value, not forward-declared: the setup and hardpoint structs are returned by
// value below, and Car.h only forward-declares the component that owns them.
#include "WishboneSuspension.h"
#include "SaloonCar.generated.h"

/**
 * A worked, coherent vehicle setup - a mid-size saloon, tuned as a whole.
 *
 * ACar's built-in hardpoints are a placeholder and were never a car: both arms
 * level and parallel, hinges at the same lateral position, zero kingpin
 * inclination, zero caster and a 61 mm scrub radius. That geometry has no camber
 * gain (0.001 deg across full travel, against 1-3 for a real double wishbone)
 * and its roll centre lands 17 cm UNDERGROUND. Nothing tuned on top of it can
 * behave like a car, which makes it a poor thing to test a tyre model against.
 *
 * This class replaces the lot with a setup derived to targets rather than typed:
 *
 *   1.55 m track, 2.65 m wheelbase, 1500 kg sprung
 *   camber       -1.30 deg front / -1.08 rear at 5 cm bump, monotonic
 *   roll centre  +5.9 cm front / +8.9 cm rear, both above ground
 *   KPI          12 deg front / 6 rear      caster 5 deg front
 *   scrub        +8 mm front / +5 mm rear
 *   motion ratio 0.62 front / 0.58 rear
 *   ride         1.26 Hz front / 1.38 Hz rear, damping ratio 0.35
 *
 * These are BENCH-MEASURED, not intended. The camber line in particular used to claim
 * -1.86 / -1.46 monotonic while the geometry delivered +0.13 deg/cm - gaining POSITIVE
 * camber in bump - and nothing compared the two until the bench sweep did. Anything
 * quoted here that has not been measured is a target wearing a result's clothes.
 *   roll         5.05 deg/g, 56.2% front roll-stiffness share (mild understeer)
 *
 * The rates are not guesses either: ride frequency and damping ratio were chosen
 * first, then spring and damper rates worked backwards through each corner's
 * measured motion ratio, and the bar rates set to land the roll gradient and the
 * front/rear split. Change the geometry and the rates stop being right - the
 * motion ratio moves and the frequencies move with it.
 *
 * HOW TO USE IT: duplicate your vehicle Blueprint and reparent the copy to this
 * class. Everything below is applied in the constructor, so Blueprint-level
 * overrides still win - this is a starting point, not a cage.
 *
 * Verify with AVehicleTestBench on Kinematics; every figure above is one it
 * reports. The WHEEL CENTRES MOVE (half-track 106 -> 77.5 cm), so a mesh built
 * around the old track will need its wheels moving to match.
 */
UCLASS()
class VEHICLEPHYSICS_API ASaloonCar : public ACar
{
	GENERATED_BODY()

public:
	ASaloonCar();

	/** Front corner hardpoints, right-hand side. Mirrored for the left. */
	static FWishboneHardpoints FrontHardpoints();

	/** Rear corner hardpoints, right-hand side. */
	static FWishboneHardpoints RearHardpoints();

	static FWishboneCornerSetup FrontSetup();
	static FWishboneCornerSetup RearSetup();
};
