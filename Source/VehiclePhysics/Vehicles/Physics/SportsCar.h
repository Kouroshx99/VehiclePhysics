// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Car.h"
// By value, not forward-declared: the setup and hardpoint structs are returned by
// value below, and Car.h only forward-declares the component that owns them.
#include "WishboneSuspension.h"
#include "SportsCar.generated.h"

/**
 * A worked, coherent sports-car setup - the saloon's sibling, tuned as a whole.
 *
 * Point of comparison rather than a second car for its own sake. A tyre model that
 * only ever sees one vehicle is not being tested: every coefficient can be quietly
 * wrong in a way that one setup happens to hide. Two cars that differ in mass,
 * track, wheelbase, ride frequency, roll gradient and roll-stiffness split, both
 * behaving plausibly, is evidence about the MODEL. One car behaving plausibly is
 * evidence about the tune.
 *
 * Derived to targets exactly the way ASaloonCar was, and the derivation is in
 * the .cpp so it can be re-run rather than merely believed:
 *
 *   1.60 m track, 2.50 m wheelbase, 1350 kg sprung, 47/53 front/rear
 *   ride         1.60 Hz front / 1.80 Hz rear, damping ratio 0.42
 *   roll         2.49 deg/g, 52.0% front roll-stiffness share
 *   camber       -2.2 deg front / -1.6 rear static
 *   toe          +0.05 deg front / +0.15 rear
 *   travel       +/-9 deg of arm sweep
 *   engine       430 N*m, 7000 rpm limiter, its own torque curve
 *   driveline    REAR wheel drive, 6 speeds, 3.13 final, spread 4.48
 *   performance  297 hp at 6000 rpm, 220 hp/tonne, 250 km/h in 6th
 *
 * AGAINST THE SALOON, which is the only reason those numbers mean anything:
 *
 *                 saloon              sports
 *   mass          1500 kg             1350 kg
 *   track         1.55 m              1.60 m
 *   ride          1.25 / 1.35 Hz      1.60 / 1.80 Hz
 *   roll          5.05 deg/g          2.49 deg/g
 *   front share   56.2%               52.0%
 *   torque        250 N*m             400 N*m
 *
 * Half the roll and a quarter more grip-limited entry speed, with less of the
 * understeer built in - 52% against 56.2% - so it rotates on throttle where the
 * saloon pushes. That difference is deliberate and it is the interesting part: if
 * the bench does not report the sports car as the looser of the two, something in
 * the roll-stiffness path is not doing what the arithmetic says.
 *
 * WHAT THIS DOES NOT SET. The hardpoint GEOMETRY is the saloon's, scaled: lateral
 * coordinates by 80.0/77.5 for the wider track, X shifted for the shorter
 * wheelbase, heights untouched. Scaling laterally preserves the arm ANGLES, which
 * is what sets camber gain and roll centre height, so both come out within a few
 * percent of the saloon's rather than being re-solved for sports-car targets.
 * More caster and more KPI would suit it and both need the ball joints moving,
 * which is a numerical re-solve rather than a scale - a follow-up, not a detail
 * that can be typed.
 *
 * HOW TO USE IT: duplicate your vehicle Blueprint and reparent the copy to this
 * class. Everything is applied in the constructor, so Blueprint overrides still
 * win - a starting point, not a cage.
 *
 * Verify with AVehicleTestBench. Kinematics reports the geometry, the skidpad and
 * step-steer runs report the roll gradient and the balance.
 */
UCLASS()
class VEHICLEPHYSICS_API ASportsCar : public ACar
{
	GENERATED_BODY()

public:
	ASportsCar();

	/** Front corner hardpoints, right-hand side. Mirrored for the left. */
	static FWishboneHardpoints FrontHardpoints();

	/** Rear corner hardpoints, right-hand side. */
	static FWishboneHardpoints RearHardpoints();

	static FWishboneCornerSetup FrontSetup();
	static FWishboneCornerSetup RearSetup();
};
