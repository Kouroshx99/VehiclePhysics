// Fill out your copyright notice in the Description page of Project Settings.

#include "TireParamsPresets.h"

/**
 * All three presets against the baseline they derive from, in one place so they can be
 * compared rather than read one at a time:
 *
 *                     PDY1   PDX1     PDY2     PDX2    PKY1    PKX1   PCY1   PCX1    PEY1
 *   Touring (base)    0.878  1.042  -0.0645  -0.0829   15.32   21.69   1.34   1.58   -0.81
 *   Performance       1.150  1.300  -0.0750  -0.0950   19.50   27.00   1.45   1.62   -0.65
 *   Track semi-slick  1.350  1.500  -0.0850  -0.1050   23.00   31.00   1.52   1.68   -0.55
 *   Wet               0.650  0.720  -0.0700  -0.0850   14.00   19.50   1.30   1.55   -0.85
 *   Gravel            0.600  0.650  -0.0450  -0.0550    9.50   13.00   1.20   1.45   -1.10
 *
 * Read down the columns rather than across the rows and the pattern is the point:
 * friction and stiffness rise together for dry compounds, C rises with them so grip
 * costs warning, and gravel breaks the pattern on every count because it grips by a
 * different mechanism.
 *
 * FZ0 is left at the base 3000 N in all three. It is the load these are quoted AT, and
 * moving it silently rescales every load-sensitivity term - so it belongs to the car
 * the tyre is fitted to, not to the compound. ASportsCar's own tyre sets 3300 because
 * its corners carry that; a preset cannot know.
 */

// ---------------------------------------------------------------------------

UTireParamsTrackDay::UTireParamsTrackDay()
{
	// Peak. Semi-slicks measure 1.2 to 1.4 lateral; this is the top of that, and the
	// longitudinal figure sits above it as it does on every tyre.
	PDY1 = 1.35f;
	// 1.35, matched to PDY1. At 1.50 the tyre had 11 per cent more longitudinal
	// grip than lateral, which is what made it hook up harder than it turned.
	PDX1 = 1.35f;

	// More load-sensitive than a road tyre, because it is stickier. See the header.
	PDY2 = -0.085f;
	PDX2 = -0.105f;

	// A 30-series sidewall on a wide rim - stiffer than the sports tyre's 40-series by
	// about as much again.
	PKY1 = 23.0f;
	PKX1 = 31.0f;
	PKX2 = 16.5f;

	// And the sharpest peak of the set. This is what a semi-slick actually feels like:
	// enormous grip with very little between the limit and the other side of it.
	PCY1 = 1.52f;
	PCX1 = 1.68f;
	PEY1 = -0.55f;
	PEX1 = 0.13f;
}

// ---------------------------------------------------------------------------

UTireParamsWet::UTireParamsWet()
{
	// 0.65 against the baseline's 0.88 - about three quarters, and inside the published
	// 0.4 to 0.8 for a treaded tyre in the wet.
	PDY1 = 0.65f;
	PDX1 = 0.72f;

	// Slightly more load-sensitive than dry: a water film is squeezed out by load, so
	// the relationship is less linear rather than more.
	PDY2 = -0.070f;
	PDX2 = -0.085f;

	// Lower than dry, and this is the part that matters for how it drives. A film
	// between rubber and road shears before the rubber does, so force builds more
	// slowly with slip and the car feels vague rather than merely slippery.
	PKY1 = 14.0f;
	PKX1 = 19.5f;
	PKX2 = 12.5f;

	// A softer, rounder peak than dry. Wet grip goes away gradually, which is the one
	// respect in which it is more forgiving than dry grip.
	PCY1 = 1.30f;
	PCX1 = 1.55f;
	PEY1 = -0.85f;
	PEX1 = 0.11f;
}

// ---------------------------------------------------------------------------

UTireParamsGravel::UTireParamsGravel()
{
	// The published figure for gravel, and the least interesting number here.
	PDY1 = 0.60f;
	PDX1 = 0.65f;

	// THE LEAST load-sensitive of the set, and deliberately. Every other preset gets
	// more negative as it gets stickier because adhesion saturates with load. Gravel
	// does not grip by adhesion - it grips by shearing loose material - and shear
	// strength rises with normal load far more nearly linearly. A gravel tyre made
	// more load-sensitive because it has less grip would be wrong twice.
	PDY2 = -0.045f;
	PDX2 = -0.055f;

	// Nearly half the baseline. On gravel the SURFACE shears before the tyre does, so
	// there is no stiff coupling to build force against and slip angle grows a long way
	// before force does. This one number is most of why a rally car is driven with the
	// wheel further from centre than a circuit car.
	PKY1 = 9.5f;
	PKX1 = 13.0f;
	PKX2 = 9.0f;

	// A BROAD peak with a gentle falloff - the opposite end of the set from the
	// semi-slick, and the reason a car can be held sideways on gravel. Well past peak
	// slip most of the force is still there, where a track tyre at the same slip has
	// let go entirely.
	PCY1 = 1.20f;
	PCX1 = 1.45f;
	PEY1 = -1.10f;
	PEX1 = 0.05f;
}

// ---------------------------------------------------------------------------

UTireParamsOffroad::UTireParamsOffroad()
{
	// A 33-inch tyre. This has to match the corner's WheelRadiusCm - a radius mismatch is
	// silently wrong in both Fz and slip ratio, and 9 cm on 33 is not a rounding error.
	R0 = 0.42f;

	// AOffroadCar carries 5611 N on a front corner and 5180 on a rear.
	FZ0 = 5500.f;

	// ABOVE gravel's 0.60, which reads backwards until the mechanism does. A block tread
	// on loose ground digs rather than slides, and part of what resists it is the shear
	// strength of material piling in front of each block. It is not grip in the road-tyre
	// sense and it does not behave like it.
	PDY1 = 0.70f;
	PDX1 = 0.78f;

	// The LEAST load-sensitive set here, for the same reason gravel is: adhesion
	// saturates with load and shearing loose material does not, or much less so.
	PDY2 = -0.040f;
	PDX2 = -0.050f;

	// Barely half a road tyre's, and this is what the vehicle feels. A tall soft sidewall
	// on a surface that shears before the tyre does means slip angle grows a long way
	// before force does - the wheel is turned well past where a road car would be
	// cornering before anything happens.
	PKY1 = 8.0f;
	PKX1 = 11.0f;
	PKX2 = 8.0f;

	// The broadest, gentlest peak of the whole set. Well past peak slip most of the force
	// is still there, which is why an offroad vehicle can be held at an angle that would
	// have long since spun a track car - and why it gives almost no warning first.
	PCY1 = 1.15f;
	PCX1 = 1.40f;
	PEY1 = -1.20f;
	PEX1 = 0.04f;

	// SAME LIMIT AS THE GRAVEL PRESET, and worth restating because this one invites the
	// mistake more. It is a rigid-surface model with the numbers moved. Real soft ground
	// deforms, the tyre sinks, and a large share of the thrust comes from shearing soil
	// rather than rubber friction - terramechanics, Bekker and Wong, outside the Magic
	// Formula entirely. This makes a tyre behave like one on loose ground; it does not
	// make it a tyre in a soil model.
}
