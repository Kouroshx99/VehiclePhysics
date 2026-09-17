// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TireParamsDataAsset.h"
#include "TireParamsSportsCar.generated.h"

/**
 * A performance summer tyre for ASportsCar - 255/40R18, near enough.
 *
 * WHY THIS IS A CLASS AND NOT A .uasset. The tuning is 179 parameters and the useful
 * part is not the values, it is the REASONING for the dozen that were changed and the
 * hundred and sixty that were not. That reasoning survives in a header and does not
 * survive in a binary a curve editor owns, so the numbers live here and the asset is
 * a three-click instance of them:
 *
 *   Content Browser, in VehiclePhysics/Vehicles - right-click, Miscellaneous, Data
 *   Asset, pick "Tire Params Sports Car", name it TP_SportsCar.
 *
 * That lands the asset exactly where TP_GenericSedan already lives, with these values
 * as its defaults, and anything overridden on the asset from then on shows up in the
 * editor as an override rather than being indistinguishable from the baseline. I
 * cannot author the .uasset directly - it is a binary package and the Python plugin
 * is not enabled in this project - and enabling one to write a file that this does
 * better is not a trade worth making.
 *
 * WHAT WAS CHANGED, and why each one. The saloon's set is Pacejka's own book example -
 * a 205-section passenger tyre - and twelve parameters carry the difference between
 * that and a performance tyre. Everything else is left alone deliberately: the shape
 * of a tyre curve is mostly conserved across compounds, and changing coefficients
 * whose effect you cannot predict is how a parameter set stops describing a tyre.
 *
 *   PEAK FRICTION is the headline. PDY1 0.879 -> 1.15 lateral, PDX1 1.042 -> 1.30
 *   longitudinal. A performance summer tyre is worth roughly 30% over a touring tyre
 *   and longitudinal peak sits above lateral on essentially every tyre, which the
 *   saloon's set already reflects.
 *
 *   LOAD SENSITIVITY goes slightly the wrong way, which is not a mistake. PDY2
 *   -0.0645 -> -0.075, PDX2 -0.0829 -> -0.095. Stickier compounds lose MORE with load,
 *   not less: the mechanism that makes them grip is the one that saturates. This is
 *   what makes a sports car more sensitive to load transfer than a saloon despite
 *   having more grip, and leaving it flat would have produced a tyre that does not
 *   exist.
 *
 *   STIFFNESS, from a lower profile and a stiffer carcass. PKY1 -15.32 -> -19.5,
 *   PKX1 21.69 -> 27.0, PKX2 13.73 -> 15.0. That is 25-27%, which is about the
 *   difference between a 60-series and a 40-series sidewall.
 *
 *   PEAK SHARPNESS. PCY1 1.337 -> 1.45 and PEY1 -0.806 -> -0.65 together sharpen the
 *   peak and steepen the falloff past it; PCX1 and PEX1 likewise, less so. This is
 *   the honest cost of the grip: a performance tyre gives more and then gives up
 *   faster, and a set with high peak friction and a touring tyre's plateau would
 *   flatter the car in a way no tyre does.
 *
 *   AND TWO THAT ARE FIXES RATHER THAN TUNING. FZ0 2500 -> 3300 N, because the
 *   load-sensitivity terms above are all relative to it and this car's corners carry
 *   3112 N front and 3510 rear - a nominal load 25% below the operating point puts
 *   every friction term on the wrong part of its own curve. And R0 0.32 -> 0.33 m to
 *   match WheelRadiusCm, which the base header itself warns about: a radius mismatch
 *   is a silent error in both Fz and slip ratio, and 1 cm is 3%.
 *
 * ONE CONSEQUENCE WORTH KNOWING before running the stability check. Raising PKX1 by
 * 25% raises the longitudinal slip stiffness by 25%, and the wheel-spin stability
 * number G is directly proportional to it - G = C*(dkappa/dOmega)*R0*dt/I. The
 * implicit treatment added in 40326 handles it, since the effective inertia carries
 * the 1+G, but the margin is 25% smaller than the saloon's. If a launch on this tyre
 * oscillates where the saloon's did not, that is where to look first.
 */
UCLASS()
class VEHICLEPHYSICS_API UTireParamsSportsCar : public UTireParamsDataAsset
{
	GENERATED_BODY()

public:
	UTireParamsSportsCar();
};
