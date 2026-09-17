// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TireParamsDataAsset.h"
#include "TireParamsPresets.generated.h"

/**
 * Tyre presets, so a surface or a compound is one asset rather than a tuning session.
 *
 * Each is a subclass of UTireParamsDataAsset, so making one is right-click,
 * Miscellaneous, Data Asset, pick the class. They derive from the base defaults, which
 * as of change 40524 are TP_GenericSedan's driven values - a touring tyre - so each
 * preset only states what makes it different and inherits a set that has been used.
 *
 * WHERE THE PEAKS COME FROM. Published ranges rather than taste:
 *
 *   street / touring, dry   0.7 - 0.9    the baseline is 0.88
 *   performance summer      1.0 - 1.2    UTireParamsSportsCar is 1.15
 *   semi-slick / track      1.2 - 1.4    Track is 1.35
 *   racing slick            1.4 - 1.8    GT cars measure 1.30-1.35, F1 1.55-1.60
 *   wet                     0.4 - 0.8    Wet is 0.65
 *   gravel                  around 0.6   Gravel is 0.60
 *
 * WHAT THESE ARE NOT. Fitted parameter sets. A real one comes from a flat-track or
 * drum rig and 179 coefficients that agree with each other; these are the baseline set
 * with a dozen coefficients moved to land the right peak, stiffness and peak shape.
 * They will behave plausibly and they are not measurements of any particular tyre, and
 * anywhere a number matters more than plausibility a measured .tir file should replace
 * them.
 *
 * IMPORTING ONE MEANS FLIPPING PKY1 - published sets carry it negative and this
 * implementation needs it positive. See the note at the parameter and in
 * TireParams_Baseline.md.
 *
 * The four levers each preset moves, and what each does:
 *
 *   PDX1 / PDY1   peak friction. The headline, and the only one most people change.
 *   PDX2 / PDY2   how the peak falls with load. MORE negative for stickier compounds,
 *                 which is counterintuitive and correct: the mechanism that makes a
 *                 soft compound grip is the one that saturates. It is why a track car
 *                 is more sensitive to load transfer than a road car despite gripping
 *                 harder, and why gravel - which grips by shearing loose material
 *                 rather than by adhesion - is the LEAST load-sensitive here.
 *   PKX1 / PKY1   slip stiffness. Construction rather than compound: sidewall height
 *                 and carcass stiffness, and on a loose surface, how much the surface
 *                 itself shears before the tyre does.
 *   PCX1 / PCY1   peak shape, with PEX1 / PEY1. Higher C is a sharper peak and a
 *                 steeper falloff past it. This is what separates a tyre that warns
 *                 you from one that lets go, and it is the honest cost of grip - a set
 *                 with slick friction and a touring plateau would corner harder AND be
 *                 more forgiving, which no tyre does.
 */

/**
 * Semi-slick track tyre - Cup 2, A052, R888 class.
 *
 * Peak 1.35 lateral, 1.50 longitudinal, and a sharp one. Sits between a road tyre and
 * a slick: more grip than anything road-legal has a right to, and much less warning
 * before it goes. The stiffness is a 30-series sidewall on a wide rim.
 *
 * Pair with ASportsCar and expect it to be faster and less forgiving in the same lap.
 */
UCLASS()
class VEHICLEPHYSICS_API UTireParamsTrackDay : public UTireParamsDataAsset
{
	GENERATED_BODY()
public:
	UTireParamsTrackDay();
};

/**
 * Wet road tyre, or a road tyre in standing water - the two are the same model here.
 *
 * Peak 0.65 lateral against the baseline's 0.88, which is the published wet range for
 * a treaded tyre and about three quarters of its own dry figure. Slip stiffness drops
 * with it: a water film lets the contact patch shear before the rubber does.
 *
 * NOT a hydroplaning model. Peak friction falls, and that is all - there is no speed
 * at which the tyre stops touching the road, because nothing here models a water
 * wedge. Below that speed it is reasonable; above it, it is optimistic.
 */
UCLASS()
class VEHICLEPHYSICS_API UTireParamsWet : public UTireParamsDataAsset
{
	GENERATED_BODY()
public:
	UTireParamsWet();
};

/**
 * Gravel and dirt - a road tyre on a loose surface.
 *
 * Peak 0.60, which is the published figure, and the shape matters more than the peak.
 * Cornering stiffness drops by nearly half, because on gravel the SURFACE shears
 * before the tyre does and there is no stiff coupling to build force against. The peak
 * is broader and the falloff much gentler: a car on gravel can sit well past peak slip
 * and keep most of its grip, which is why a rally car can be driven sideways and a
 * road car on tarmac cannot.
 *
 * Load sensitivity is the LEAST negative of any preset here, and that is not an
 * oversight. Adhesion saturates with load; shearing loose material does not, or much
 * less so.
 *
 * A REAL LIMIT WORTH KNOWING. This is still a rigid-surface model with the numbers
 * moved. Gravel deforms, the tyre sinks, and a large share of the thrust comes from
 * shearing soil rather than rubber friction - that is terramechanics, Bekker and Wong,
 * and it is outside the Magic Formula entirely. This preset makes a road tyre behave
 * like one on a loose surface; it does not make it a rally tyre in a rally model.
 */
UCLASS()
class VEHICLEPHYSICS_API UTireParamsGravel : public UTireParamsDataAsset
{
	GENERATED_BODY()
public:
	UTireParamsGravel();
};

/**
 * Mud-terrain on a 33-inch tyre - the offroad car's, and the only preset that changes R0.
 *
 * Peak 0.70, ABOVE the gravel preset's 0.60 rather than below it, which is the one number
 * here that surprises people. An aggressive block tread on loose ground does not slide
 * the way a road tyre does: it digs, and part of what resists it is the shear strength of
 * the material piling up in front of each block. What it gives away instead is stiffness -
 * PKY1 8.0 against a road tyre's 15.3, barely half - and warning, because there is no
 * carcass stiff enough to tell you what a tread block is doing.
 *
 * R0 IS 0.42, NOT 0.33, and it must match the corner's WheelRadiusCm or both Fz and slip
 * ratio are silently wrong. A bigger tyre is most of what makes a vehicle capable off
 * road - it clears obstacles, it lowers effective gearing, and it drops the contact patch
 * nine centimetres further from the wheel centre, which moves scrub radius and roll
 * centre whether or not anyone intended it.
 *
 * FZ0 5500 N, because AOffroadCar carries 5611 N on a front corner. Every load-sensitivity
 * term is relative to it, and the 3000 N default would sit 45% under the operating point.
 */
UCLASS()
class VEHICLEPHYSICS_API UTireParamsOffroad : public UTireParamsDataAsset
{
	GENERATED_BODY()
public:
	UTireParamsOffroad();
};
