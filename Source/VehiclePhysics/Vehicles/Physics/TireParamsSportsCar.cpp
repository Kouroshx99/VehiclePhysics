// Fill out your copyright notice in the Description page of Project Settings.

#include "TireParamsSportsCar.h"

UTireParamsSportsCar::UTireParamsSportsCar()
{
	// ---- geometry and reference load, which are fixes rather than tuning ----------

	// 0.33 m to match the corner setups' WheelRadiusCm. The base default is 0.32 and
	// the base header says outright what that costs: a radius mismatch is a silent
	// error in both Fz and slip ratio. 1 cm on 33 is 3%.
	R0 = 0.33f;     // already the base value; restated because a wrong radius is silent

	// 3300 N. Every load-sensitivity term below is relative to FZ0, and this car
	// carries 3112 N on a front corner and 3510 on a rear. The base 2500 N sat 25%
	// under the operating point, which puts every friction term on the wrong part of
	// its own curve before any of them are changed.
	FZ0 = 3300.f;   // base is 3000 now, matched to the saloon; this car carries more

	// ---- peak friction -----------------------------------------------------------

	// A performance summer tyre is worth roughly 30% over a touring tyre, and the
	// longitudinal peak sits above the lateral one on essentially every tyre - which
	// the saloon's set already reflects and this keeps.
	PDX1 = 1.30f;     // 1.0422
	PDY1 = 1.15f;     // 0.8785

	// ---- load sensitivity --------------------------------------------------------

	// MORE negative, which is not a slip. Stickier compounds lose more peak with load,
	// because the mechanism that makes them grip is the one that saturates. This is
	// what makes a sports car more sensitive to load transfer than a saloon in spite
	// of having more grip, and flattening it would describe a tyre that does not exist.
	PDX2 = -0.095f;   // -0.08285
	PDY2 = -0.075f;   // -0.06452

	// ---- stiffness, from a lower profile and a stiffer carcass --------------------

	// 25 to 27%, which is about the difference between a 60-series sidewall and a
	// 40-series one.
	PKX1 = 27.0f;     // 21.687
	PKX2 = 15.0f;     // 13.728
	// Stiffer carcass, same sign as the base - which is positive, and deliberately not
	// what a published set carries. See the note on PKY1 in TireParamsDataAsset.h: ISO
	// has it negative and this implementation needs it positive, so a value imported
	// from a .tir file has to be flipped before it lands here.
	PKY1 = 19.5f;     // base 15.324

	// ---- peak sharpness ----------------------------------------------------------

	// The honest cost of the grip above. Raising C and pulling E toward zero sharpens
	// the peak and steepens the falloff past it. A set with performance-tyre friction
	// and a touring tyre's plateau would flatter the car in a way no real tyre does -
	// it would corner harder AND be more forgiving at the limit, and tyres do not
	// offer both.
	PCX1 = 1.62f;     // 1.579
	PEX1 = 0.12f;     // 0.11113
	PCY1 = 1.45f;     // 1.337
	PEY1 = -0.65f;    // -0.8057

	// EVERYTHING ELSE IS LEFT ALONE, deliberately and not for want of time. The shape
	// of a tyre curve is largely conserved across compounds and constructions: the
	// combined-slip weighting functions, the aligning-moment set, the camber terms and
	// the relaxation lengths describe how a tyre behaves rather than how much it
	// grips. Changing coefficients whose effect cannot be predicted is how a parameter
	// set stops describing a tyre and starts being a collection of numbers that
	// happened to feel right once.
	//
	// THE BASELINE IS NOW SOUND, which it was not when this class was written. It
	// derives from UTireParamsDataAsset's defaults, and those defaults were the
	// header's unverified set rather than TP_GenericSedan's driven one - eleven
	// parameters differed, including PKY1's sign, which is why this shipped with an
	// inverted lateral curve.
	//
	// The header now carries the driven values, so deriving from it is safe and a new
	// data asset made from the menu is correct out of the box. The eleven are recorded
	// in TireParams_Baseline.md along with the old values, so nothing was lost.
	//
	// sigma_min in particular is untouched. It is the relaxation-length floor and it
	// sets the wheel-spin stability margin - see the note in the header about G rising
	// 25% with PKX1 - and shortening it to suit a stiffer carcass would compound that
	// rather than balance it.
}
