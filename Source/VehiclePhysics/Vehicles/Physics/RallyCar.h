// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Car.h"
// By value, not forward-declared: the setup and hardpoint structs are returned by
// value below, and Car.h only forward-declares the component that owns them.
#include "WishboneSuspension.h"
#include "RallyCar.generated.h"

/**
 * A gravel rally car - the third worked setup, and the one that leaves tarmac.
 *
 * The saloon and the sports car differ in mass, track and stiffness but are the same
 * KIND of car: short travel, hard tyre, flat ground. Both could be plausible while the
 * tyre model was wrong in ways neither exercises. This one is chosen to disagree with
 * them on the axes that matter:
 *
 *   TRAVEL       26.6 cm swept against their 16. Long travel is not a detail of a rally
 *                car, it is the definition of one, and it is the thing this plugin has
 *                never been asked to do. Every corner has spent its life inside +/-12
 *                degrees of arm sweep; this one runs +/-20.
 *   RIDE HEIGHT  the whole linkage sits 8 cm lower in body space, which lifts the body
 *                8 cm off the ground without altering a single angle in it.
 *   SURFACE      UTireParamsGravel, where peak friction is 0.60 against the saloon's
 *                0.88 and cornering stiffness is nearly HALVED. That last number is the
 *                interesting one - on gravel the surface shears before the tyre does, so
 *                slip angle grows a long way before force does.
 *   DRIVE        all four wheels. Neither existing car is AWD, so the differential
 *                constraint graph has never been solved with a centre path at all.
 *
 *   1300 kg sprung, 56/44 front, 1.60 m track, 2.55 m wheelbase
 *   ride         1.30 Hz front / 1.45 Hz rear, damping ratio 0.48
 *   roll         5.15 deg/g, 55% front share
 *   travel       +/-20 deg of arm sweep, 26.6 cm at the wheel
 *
 * SOFT AND HEAVILY DAMPED is the combination that reads oddly beside the other two and
 * is right here. Ride frequencies BELOW the saloon's, because on gravel the springs have
 * to let the wheels follow the ground rather than hold the body up; damping ratio 0.48
 * against the saloon's 0.35, because that much travel with that little spring would
 * otherwise pogo. A rally car is not a stiff car - it is a soft car that is not allowed
 * to oscillate.
 *
 * WHY THE GEOMETRY IS THE SALOON'S. Scaled laterally for the wider track, shifted in x
 * for the shorter wheelbase, and moved DOWN 8 cm for the ride height. Scaling laterally
 * preserves almost all of the camber gain and exactly the roll centre height, and a
 * uniform vertical shift changes neither - so this car inherits geometry that has been
 * measured rather than a fresh guess. The rates are its own; the linkage is not.
 *
 * Everything is applied in the constructor, so Blueprint overrides still win. A
 * Blueprint made from this class before a value changed keeps the old one - make a new
 * Blueprint, or right-click the property and Reset to Default.
 */
UCLASS()
class VEHICLEPHYSICS_API ARallyCar : public ACar
{
	GENERATED_BODY()

public:
	ARallyCar();

	/** Right-hand corner; ACar mirrors it for the left. */
	static FWishboneHardpoints FrontHardpoints();
	static FWishboneHardpoints RearHardpoints();

	static FWishboneCornerSetup FrontSetup();
	static FWishboneCornerSetup RearSetup();
};
