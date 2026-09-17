// Fill out your copyright notice in the Description page of Project Settings.

#include "SaloonCar.h"
#include "WishboneSuspension.h"
#include "Drivetrain.h"
#include "VehicleEngine.h"
#include "PacejkaTyreComponent.h"

/**
 * Derivation, so these numbers can be re-derived rather than merely trusted.
 *
 * The ball joints come first, from three targets that fix them outright. Kingpin
 * inclination sets how far inboard the upper joint sits relative to the lower over
 * the 29 cm between them; caster does the same fore-aft; and the scrub radius then
 * pins the pair laterally, because scrub is just where that axis, extended, meets
 * the ground relative to the contact patch.
 *
 * The inner pivot HEIGHTS are what remain, and they were solved numerically for two
 * things at once: camber gain and roll centre height. Both come from the same
 * source - the angle each arm makes in the front view - which is why they cannot be
 * chosen independently and why guessing at hardpoints so rarely lands both.
 *
 * The key insight the placeholder geometry missed: camber gain needs the two arms
 * to be at DIFFERENT ANGLES, not merely different lengths. With both arms level,
 * each ball joint's lateral motion is second order in the arm's rotation - an even
 * function - so camber comes out as a parabola centred on rest and the corner gains
 * nothing either way. Tilt them relative to each other and a first-order term
 * appears, which is the entire mechanism.
 */

FWishboneHardpoints ASaloonCar::FrontHardpoints()
{
	// CAMBER GAIN WAS THE WRONG WAY ROUND HERE, and the inner pivot heights below are
	// the corrected solve. The mechanism described above is right; the numbers sat on
	// the wrong side of it.
	//
	// The sign of the camber gain is decided by ONE comparison, and it is worth writing
	// down so it cannot invert again:
	//
	//     gain per cm of bump = (tan(phi_upper) - tan(phi_lower)) / kingpin_length
	//
	// with phi positive for an arm sloping DOWN toward the outside. The arm LENGTHS
	// cancel out of it entirely once both ball joints are required to rise together, so
	// a short upper arm buys nothing on its own - THE LOWER ARM MUST BE THE MORE STEEPLY
	// INCLINED OF THE TWO. It was the other way round: upper 8.88 deg against the
	// lower's 6.10, which runs the top of the kingpin outboard faster than the bottom
	// and leans the wheel OUT in bump. The bench measured +0.127 deg/cm where the header
	// claimed -0.372.
	//
	// Both inner pivots move, not just the upper one. Arm inclination sets the roll
	// centre as well as the camber curve, and correcting camber with the upper arm alone
	// would have pushed this roll centre from 5.9 to over 13 cm - past the rear's 8.9 -
	// inverting the nose-down roll axis that is a deliberate choice here.
	//
	// The BALL JOINTS do not move, so KPI, caster, scrub radius and kingpin length are
	// all exactly as they were. DamperMount rides the lower arm - WishboneSuspension
	// orbits it about the LCA hinge - so it drops with the arm, keeping the same position
	// ALONG it and therefore the same motion ratio.

	// KPI 12 deg, caster 5 deg, scrub +8 mm, lower arm 38 cm, upper 24 cm.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(145.50000f, 34.90374f, 7.12400f);
	P.LCA_B       = FVector(119.50000f, 34.90374f, 7.12400f);
	P.LCA_U       = FVector(132.50000f, 72.90374f,  6.00000f);
	P.UCA_F       = FVector(140.36283f, 42.73960f, 33.84700f);
	P.UCA_B       = FVector(119.56283f, 42.73960f, 33.84700f);
	P.UCA_U       = FVector(129.96283f, 66.73960f, 35.00000f);
	P.DamperMount = FVector(132.50000f, 59.98374f,  6.38212f);
	P.CoiloverTop = FVector(130.50000f, 44.00000f, 56.00000f);
	P.WheelCentre = FVector(132.50000f, 77.50000f, 21.13677f);
	return P;
}

FWishboneHardpoints ASaloonCar::RearHardpoints()
{
	// Less KPI and no caster - nothing steers back here - and a higher roll centre,
	// which is the usual way round: a rear roll centre above the front tilts the roll
	// axis nose-down and moves load transfer forward under cornering.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(-119.50000f, 37.12284f, 8.95800f);
	P.LCA_B       = FVector(-145.50000f, 37.12284f, 8.95800f);
	P.LCA_U       = FVector(-132.50000f, 75.12284f,  6.00000f);
	P.UCA_F       = FVector(-122.10000f, 46.07482f, 35.43600f);
	P.UCA_B       = FVector(-142.90000f, 46.07482f, 35.43600f);
	P.UCA_U       = FVector(-132.50000f, 72.07482f, 35.00000f);
	P.DamperMount = FVector(-132.50000f, 61.44284f,  7.06499f);
	P.CoiloverTop = FVector(-134.50000f, 44.00000f, 56.00000f);
	P.WheelCentre = FVector(-132.50000f, 77.50000f, 21.13677f);
	return P;
}

FWishboneCornerSetup ASaloonCar::FrontSetup()
{
	// 60/40, NOT 50/50, and that one change is why these rates all moved.
	//
	// A front-wheel-drive car at 50/50 is not a configuration any manufacturer ships, and
	// the bench had been saying so for several runs - 'static weight on front 50.1%'
	// against a band of 55-62. It cost the car twice over. Only half the weight sat on
	// the driven axle and acceleration moved it AWAY, down to 40%, so the car was
	// traction limited at 0.52 g with 0.77 g of engine behind it and took 10.1 s to
	// 100 km/h. And with equal weight on similar axles the understeer gradient collapses
	// toward zero: it measured 0.691 deg/g against a road-car band of 2-4.
	//
	// At 60/40 the corner masses become 450 front and 300 rear, so the front spring gets
	// STIFFER than the rear rather than softer, which is the opposite of the old setup
	// and correct for where the weight now is.
	//
	// The confirmation is the roll split. At 60/40 the springs ALONE give 56.3% front,
	// which is the 56.2% this class has always documented as its target - previously it
	// took a 14000 N/m front bar to drag 46.2% up to it. The bars are trim now, not
	// structure.
	FWishboneCornerSetup S;
	// 1.25 Hz on a 375 kg corner is 23.1 kN/m at the wheel; through a motion ratio
	// of 0.610 that is 62.2 kN/m at the spring. Damper likewise: zeta 0.35 of
	// critical at the wheel, divided by MR squared to get back to the strut.
	// STIFFENED x1.30. Roll and pitch both go as (mass x height) / stiffness, so one
	// multiplier moves both: 7.108 deg/g of roll and 2.110 deg/g of pitch become about
	// 5.47 and 1.62, which is mid-band for a road car rather than at the top of it.
	//
	// Ride frequency goes as the SQUARE ROOT, so 1.235 Hz becomes about 1.41 - still
	// inside road 1.0-1.5, and the front/rear ratio is untouched because both ends are
	// scaled by the same number. That ratio is the flat-ride relationship, and it is the
	// one thing here that must not move.
	S.SpringRateNPerM = 95099.f;
	// x sqrt(1.30), not x1.30. The damping RATIO is c / (2 sqrt(k m)), so matching the
	// spring's square root leaves 0.346 exactly where it was - the car gets firmer
	// without getting differently damped.
	S.DamperRateNsPerM = 7434.f;
	// BARS x3.85, AND THE SPRINGS DELIBERATELY LEFT ALONE.
	//
	// The car leaned badly in corners and the springs were not the reason - at 1.41 Hz
	// they are mid-band for a road car, and stiffening them again would have bought less
	// roll at the cost of ride, which is the wrong trade.
	//
	// The real fault is above: "the bars are trim now, not structure". That was decided on
	// BALANCE - at 60/40 the springs alone already gave the 56.3% front split, so the bar
	// was shrunk because it was no longer needed to correct the distribution. True, and it
	// missed that a bar does a second job. Roll MAGNITUDE was then carried almost entirely
	// by the springs, and springs are capped by ride frequency, so nothing was holding it.
	//
	// The numbers say the same thing. The bars were supplying 14.1% of total roll
	// stiffness where a road car is nearer 30-50%, and this front bar was 6.1 kN/m against
	// the 15-30 kN/m that AntiRollRateNPerM documents as typical - less than half the
	// bottom of its own stated range.
	//
	// At x3.85 the front bar is 23.5 kN/m, mid-range, and roll goes 5.47 -> 3.80 deg/g.
	// Front and rear are scaled together so the roll split moves only 54.0 -> 54.6 per
	// cent, which is a slight shift toward understeer and otherwise leaves the balance
	// where it was.
	//
	// Nothing else moves: a bar acts only in the roll mode - one wheel up while the other
	// goes down - so ride frequency, damping ratio and the pitch gradient are all exactly
	// as they were. That is the whole reason to fix lean here rather than in the springs.
	// SOFTENED to land a ROAD-CAR roll gradient rather than a sports one.
	//
	// x3.85 above put this at 4.245 deg/g measured, which is inside the sports band of
	// 3-5 on a car whose band is 5-7. It was chosen against the static prediction of
	// the day, and that prediction was wrong by 55 per cent - it omitted the tyre's own
	// vertical rate, which sits in series with the suspension and is most of the
	// compliance. Corrected, the bench predicts and measures within a few per cent.
	//
	// 13411 front / 10431 rear lands about 5.0 deg/g: the bottom of the road band, visibly
	// softer, and short of the roughly 5.7 the original bars gave - which is the lean
	// that prompted the complaint in the first place.
	//
	// THIS COSTS NO INNER-WHEEL GRIP, which is the thing worth being clear about.
	// Total lateral load transfer is m*g*h/t - mass, centre of gravity height and
	// track. No spring or bar term appears in it. Stiffness sets how far the body
	// TIPS, not how much load leaves the inside. At the measured 1.05 g limit the
	// inner wheels still carry 64-69% of static either way, and lifting one would
	// take 2.9 g. Both bars are scaled by the same factor, so the front/rear split -
	// and with it the load transfer distribution and the balance - does not move.
	S.AntiRollRateNPerM = 13411.f;
	// PRELOAD, and it is not optional. The coil is squeezed by exactly the preload at the
	// design position, so the spring force there IS PreloadForceN. Leave it at zero and
	// the spring holds nothing at design height and the corner sinks until it makes its
	// own load - 15.5 cm here, against roughly 8 cm of bump. That is a car on its bump
	// stops with no room left.
	//
	// The sag is exactly the corner load over the WHEEL rate, confirmed against a settled
	// bench run to three decimals, so the preload that cancels it is the corner load over
	// the MOTION RATIO:
	//
	//     375 kg * 9.81 = 3679 N at the wheel,  / 0.616  =  5945 N at the spring
	//
	// That parks the car AT the design position, which is where every geometry figure
	// these classes quote is measured - the roll centre, the camber curve, the motion
	// ratio. A car resting anywhere else runs geometry nobody specified.
	//
	// MEASURE THIS AFTER THE CAR HAS SETTLED, not at bench start. Sampled early the
	// figure is the car mid-drop: the saloon front reads about 2 cm before settling and
	// 15 cm after, and comparing two unsettled samples once had me revert this value as
	// making things worse.
	//
	// It is NOT a constant push - WishboneSuspension folds preload into the spring's FREE
	// LENGTH, so it decays to zero once the coil leaves its perch, the same as a real one.
	// 7166 + 193, from the bench's own preload-correction row after the x1.30 stiffening.
	// A stiffer spring carries the same corner load on less compression, so the front rose
	// 3.3 mm off design height until this put it back.
	S.PreloadForceN = 7359.f;
	S.StaticCamberDeg = -1.0f;
	// A little toe-in for straight-line stability, the usual road-car compromise.
	S.StaticToeDeg = 0.08f;
	S.UnsprungMassKg = 40.f;
	S.WheelRadiusCm = 33.f;
	// +/-12 deg of arm sweep is about 15.5 cm of wheel travel. Kept inside the range
	// where the upper-arm circle-sphere solve stays on one branch.
	S.MinArticulationAngleDeg = -12.f;
	S.MaxArticulationAngleDeg = 12.f;
	return S;
}

FWishboneCornerSetup ASaloonCar::RearSetup()
{
	FWishboneCornerSetup S;
	// Rear runs a slightly higher frequency than the front - the classic "flat ride"
	// trick. A bump reaches the rear wheels later, and a stiffer rear catches up in
	// phase so the body heaves rather than pitching.
	// x1.30, matching the front - see the note there.
	S.SpringRateNPerM = 82274.f;
	S.DamperRateNsPerM = 5955.f;
	// Scaled with the front so the roll split holds - see the note there.
	S.AntiRollRateNPerM = 10431.f;
	// PRELOAD, and it is not optional. The coil is squeezed by exactly the preload at the
	// design position, so the spring force there IS PreloadForceN. Leave it at zero and
	// the spring holds nothing at design height and the corner sinks until it makes its
	// own load - 13.1 cm here, against roughly 8 cm of bump. That is a car on its bump
	// stops with no room left.
	//
	// The sag is exactly the corner load over the WHEEL rate, confirmed against a settled
	// bench run to three decimals, so the preload that cancels it is the corner load over
	// the MOTION RATIO:
	//
	//     375 kg * 9.81 = 3679 N at the wheel,  / 0.584  =  6328 N at the spring
	//
	// That parks the car AT the design position, which is where every geometry figure
	// these classes quote is measured - the roll centre, the camber curve, the motion
	// ratio. A car resting anywhere else runs geometry nobody specified.
	//
	// MEASURE THIS AFTER THE CAR HAS SETTLED, not at bench start. Sampled early the
	// figure is the car mid-drop: the saloon front reads about 2 cm before settling and
	// 15 cm after, and comparing two unsettled samples once had me revert this value as
	// making things worse.
	//
	// It is NOT a constant push - WishboneSuspension folds preload into the spring's FREE
	// LENGTH, so it decays to zero once the coil leaves its perch, the same as a real one.
	// 5039 - 33; the rear barely moved, which is what a 0.7 mm offset is worth.
	S.PreloadForceN = 5006.f;
	S.StaticCamberDeg = -0.8f;
	S.StaticToeDeg = 0.12f;
	S.UnsprungMassKg = 40.f;
	S.WheelRadiusCm = 33.f;
	S.MinArticulationAngleDeg = -12.f;
	S.MaxArticulationAngleDeg = 12.f;
	return S;
}

ASaloonCar::ASaloonCar()
{
	const FWishboneHardpoints Front = FrontHardpoints();
	const FWishboneHardpoints Rear = RearHardpoints();
	const FWishboneCornerSetup FrontTune = FrontSetup();
	const FWishboneCornerSetup RearTune = RearSetup();

	// Hardpoints are written as the RIGHT-hand corner throughout; the mirror flag is
	// already set per corner by ACar's constructor and InitialiseGeometry applies it,
	// so both sides come from one set of numbers and cannot drift apart.
	if (FrontRightSuspension) { FrontRightSuspension->Hardpoints = Front; FrontRightSuspension->ApplySetup(FrontTune); }
	if (FrontLeftSuspension)  { FrontLeftSuspension->Hardpoints  = Front; FrontLeftSuspension->ApplySetup(FrontTune); }
	if (RearRightSuspension)  { RearRightSuspension->Hardpoints  = Rear;  RearRightSuspension->ApplySetup(RearTune); }
	if (RearLeftSuspension)   { RearLeftSuspension->Hardpoints   = Rear;  RearLeftSuspension->ApplySetup(RearTune); }

	// ---------------------------------------------------------------------------
	// Rotating inertias
	// ---------------------------------------------------------------------------
	//
	// All of these were FShaft's untouched 1.0 kg*m^2 default, which is the inertia of
	// a road wheel - so the engine, the gearbox shaft and both differentials were each
	// carrying a wheel's worth of rotational mass.
	//
	// Set in the constructor, so a Blueprint created FROM this class picks them up. One
	// that already exists stored the old values when it was saved and will keep them.

	// 24 kg of rotating mass on a 0.33 m radius at roughly 0.6*m*r^2. Also the
	// denominator of the wheel-spin stability number G, so the 1.0 default had been
	// inflating G by 57% on every corner of this car since it was written.
	const float WheelInertia = 1.57f;
	if (FrontRightTire) { FrontRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (FrontLeftTire)  { FrontLeftTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearRightTire)  { RearRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearLeftTire)   { RearLeftTire->GetShaft()->SetInertia(WheelInertia); }

	// Crank and a standard dual-mass flywheel.
	if (Engine) { Engine->GetShaft()->SetInertia(0.22f); }

	FrontDiffShaft.SetInertia(0.05f);
	RearDiffShaft.SetInertia(0.05f);

	if (Drivetrain && Drivetrain->TransmissionShaft)
	{
		Drivetrain->TransmissionShaft->SetInertia(0.03f);
	}
}
