// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Car.h"
// By value, not forward-declared: the setup and hardpoint structs are returned by
// value below, and Car.h only forward-declares the component that owns them.
#include "WishboneSuspension.h"
#include "OffroadCar.generated.h"

/**
 * A heavy off-road 4x4 - the fourth setup, and the one that stops being a car.
 *
 * The rally car already left tarmac. This one leaves the assumptions: it is nearly twice
 * the saloon's mass, it rolls twice as far, its wheels move 37 cm, and it is the first
 * vehicle here whose TYRE IS A DIFFERENT SIZE. Every corner in this plugin has run a
 * 33 cm wheel; this one runs 42, and that single change moves scrub radius and roll
 * centre without anyone touching a hardpoint.
 *
 *   2200 kg sprung, 52/48 front, 1.75 m track, 3.00 m wheelbase
 *   ride         1.05 Hz front / 1.15 Hz rear, damping ratio 0.45
 *   roll         8.35 deg/g, 54% front share
 *   travel       +/-28 deg of arm sweep, 37.3 cm at the wheel
 *   tyre         UTireParamsOffroad, R0 0.42 m, peak 0.70
 *
 * WHAT IT IS FOR. Four cars now span 1300 to 2200 kg, 1.05 to 1.80 Hz, 2.16 to 8.35 deg/g
 * of roll, 16 to 37 cm of travel, two tyre radii and four drivetrain layouts. A tyre
 * model that stays plausible across all of that is being tested; one that only ever sees
 * a saloon is not. This end of the range is where the Magic Formula is least comfortable
 * and most worth watching - large slip angles, low friction, and a body that transfers
 * enough weight to unload a wheel.
 *
 * ROLL GRADIENT 8.35 DEG/G is not a mistake and not a soft setup badly chosen. It is what
 * 2200 kg with its mass 62 cm above the roll axis does to springs soft enough to keep
 * 37 cm of travel useful. This vehicle leans, and correcting it with a bar stiff enough
 * to matter would tie the axles together over exactly the ground they exist to follow.
 *
 * WHY THE GEOMETRY IS STILL THE SALOON'S. Scaled laterally for the track, shifted in x
 * for the wheelbase, and dropped 12 cm for ride height - which with the bigger tyre puts
 * the body 21 cm higher off the ground than the saloon's. Those three preserve the
 * linkage's angles, so camber gain and the nose-down roll axis survive; the tyre radius
 * does not, and the scrub radius goes NEGATIVE as a result. That is honest for a vehicle
 * on wheels this size and it is called out rather than hidden.
 *
 * Everything is applied in the constructor, so Blueprint overrides still win.
 */
UCLASS()
class VEHICLEPHYSICS_API AOffroadCar : public ACar
{
	GENERATED_BODY()

public:
	AOffroadCar();

	/** Right-hand corner; ACar mirrors it for the left. */
	static FWishboneHardpoints FrontHardpoints();
	static FWishboneHardpoints RearHardpoints();

	static FWishboneCornerSetup FrontSetup();
	static FWishboneCornerSetup RearSetup();
};
