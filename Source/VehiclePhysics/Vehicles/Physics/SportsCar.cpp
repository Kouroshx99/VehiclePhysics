// Fill out your copyright notice in the Description page of Project Settings.

#include "SportsCar.h"
#include "WishboneSuspension.h"
#include "VehicleEngine.h"
#include "Drivetrain.h"
#include "PacejkaTyreComponent.h"
#include "Curves/CurveFloat.h"

/**
 * Derivation, so these numbers can be re-derived rather than merely trusted. Every
 * step below is arithmetic on the four targets at the top - nothing is typed.
 *
 *   TARGETS   1350 kg sprung, 47/53 front/rear, 1.60 m track, CoG 0.40 m above the
 *             roll axis, ride 1.60/1.80 Hz, damping ratio 0.42, roll-stiffness
 *             share 52% front.
 *
 * 1. CORNER MASSES.  1350 kg at 47/53 is 317.2 kg per front corner, 357.8 rear.
 *
 * 2. WHEEL RATES, from ride frequency.  k = m(2*pi*f)^2, which is just the
 *    single-degree-of-freedom result read backwards: pick the frequency you want the
 *    body to heave at, and the rate follows from the mass on it.
 *      front  317.2 * (2*pi*1.60)^2 = 32063 N/m
 *      rear   357.8 * (2*pi*1.80)^2 = 45760 N/m
 *
 * 3. ROLL STIFFNESS FROM THE SPRINGS.  K = 0.5*k*t^2 per axle - a spring pair at
 *    track t resisting a roll angle. Front 41040, rear 58573, total 99613 N*m/rad,
 *    of which the front is 41.2%.
 *
 *    Note what that means before any bar is fitted: the springs alone put the roll
 *    stiffness REARWARD, because the rear carries more mass and a higher frequency,
 *    and both raise its rate. A rear-biased roll split is a car that rotates. So the
 *    front bar is not an afterthought here, it is what makes the balance a choice.
 *
 * 4. THE FRONT BAR, sized to the 52% target and nothing else:
 *      K_fb = (share*(K_f + K_r) - K_f) / (1 - share) = 22413 N*m/rad
 *    and back to a wheel rate through the same 0.5*k*t^2, giving 17510 N/m. No rear
 *    bar: the rear already over-contributes and a bar there would push the split
 *    further the wrong way.
 *
 * 5. THE ROLL GRADIENT falls out, it is not chosen.  M*g*h / K_total
 *    = 1350*9.81*0.40 / 122026 = 0.0434 rad = 2.49 deg/g.
 *
 *    The saloon is 5.05. Half the roll for a car that is 150 kg lighter on a 5 cm
 *    wider track, which is most of what separates the two to drive.
 *
 * 6. BACK THROUGH THE MOTION RATIO.  Everything above is at the WHEEL; the spring
 *    and damper live on the arm, so both divide by MR^2 - 0.610 front, 0.570 rear,
 *    measured on the saloon geometry this inherits.
 *      front  spring  86167 N/m   damper  7200 N*s/m
 *      rear   spring 140843 N/m   damper 10461 N*s/m
 *    Damping is zeta 0.42 of critical AT THE WHEEL: c = 2*zeta*sqrt(k*m), then the
 *    same MR^2 division. Higher than the saloon's 0.35 because a stiffer car with
 *    less travel has less room to let the body oscillate.
 *
 * THE SAME CAVEAT AS THE SALOON: change the geometry and these rates stop being
 * right, because the motion ratio moves and the frequencies move with it.
 */

FWishboneHardpoints ASportsCar::FrontHardpoints()
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

	// The saloon's front geometry, scaled: lateral by 80.0/77.5 for the 1.60 m track,
	// X shifted 7.5 cm rearward for the 2.50 m wheelbase, heights untouched.
	//
	// Scaling LATERALLY preserves the arm angles, and the angles are what set camber
	// gain and roll centre height - so both land within a few percent of the saloon's
	// rather than drifting. Scaling the heights as well would have changed them.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(   138.00000f,    36.02967f,   7.06600f);
	P.LCA_B       = FVector(   112.00000f,    36.02967f,   7.06600f);
	P.LCA_U       = FVector(   125.00000f,    75.25547f,    6.00000f);
	P.UCA_F       = FVector(   132.86283f,    44.11830f,   33.74800f);
	P.UCA_B       = FVector(   112.06283f,    44.11830f,   33.74800f);
	P.UCA_U       = FVector(   122.46283f,    68.89249f,   35.00000f);
	P.DamperMount = FVector(   125.00000f,    61.91870f,    6.36245f);
	P.CoiloverTop = FVector(   123.00000f,    45.41935f,   56.00000f);
	P.WheelCentre = FVector(   125.00000f,    80.00000f,   21.13677f);
	return P;
}

FWishboneHardpoints ASportsCar::RearHardpoints()
{
	// Same treatment, and the same reasoning as the saloon's rear: less KPI, no
	// caster, and a higher roll centre than the front so the roll axis tilts
	// nose-down and load transfer moves forward under cornering.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(  -112.00000f,    38.32035f,   8.91300f);
	P.LCA_B       = FVector(  -138.00000f,    38.32035f,   8.91300f);
	P.LCA_U       = FVector(  -125.00000f,    77.54616f,    6.00000f);
	P.UCA_F       = FVector(  -114.60000f,    47.56110f,   35.35300f);
	P.UCA_B       = FVector(  -135.40000f,    47.56110f,   35.35300f);
	P.UCA_U       = FVector(  -125.00000f,    74.39981f,   35.00000f);
	P.DamperMount = FVector(  -125.00000f,    63.42487f,    7.04860f);
	P.CoiloverTop = FVector(  -127.00000f,    45.41935f,   56.00000f);
	P.WheelCentre = FVector(  -125.00000f,    80.00000f,   21.13677f);
	return P;
}

FWishboneCornerSetup ASportsCar::FrontSetup()
{
	FWishboneCornerSetup S;
	// 1.60 Hz on a 317.2 kg corner is 32.1 kN/m at the wheel; through a motion ratio
	// of 0.610 that is 86.2 kN/m at the spring. Damper likewise: zeta 0.42 of
	// critical at the wheel, divided by MR squared to get back to the strut.
	S.SpringRateNPerM = 86167.f;
	S.DamperRateNsPerM = 7200.f;

	// The whole balance is in this one number - see step 4 of the derivation. The
	// springs alone put 41.2% of the roll stiffness on the front axle, which is a car
	// that rotates; this brings it to 52%, which is a car that rotates on throttle
	// and not on entry. There is no rear bar, deliberately.
	S.AntiRollRateNPerM = 17510.f;

	// PRELOAD, and it is not optional. The coil is squeezed by exactly the preload at the
	// design position, so the spring force there IS PreloadForceN. Leave it at zero and
	// the spring holds nothing at design height and the corner sinks until it makes its
	// own load - 9.6 cm here, against roughly 8 cm of bump. That is a car on its bump
	// stops with no room left.
	//
	// The sag is exactly the corner load over the WHEEL rate, confirmed against a settled
	// bench run to three decimals, so the preload that cancels it is the corner load over
	// the MOTION RATIO:
	//
	//     318 kg * 9.81 = 3121 N at the wheel,  / 0.614  =  5083 N at the spring
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
	S.PreloadForceN = 5083.f;

	// More static camber than the saloon's -1.0. A stiffer car rolls less, so it
	// leans on camber gain less, and has to start closer to where it wants to be.
	S.StaticCamberDeg = -2.2f;

	// Less toe-in than the saloon's 0.08. Toe-in is straight-line stability bought
	// with initial response, and this car is being tuned the other way.
	S.StaticToeDeg = 0.05f;

	// Lighter wheels and brakes than the saloon's 40 kg.
	S.UnsprungMassKg = 38.f;
	S.WheelRadiusCm = 33.f;

	// +/-9 deg against the saloon's 12. A stiffer car needs less travel to absorb
	// the same bump, and less travel is what lets it sit lower.
	S.MinArticulationAngleDeg = -9.f;
	S.MaxArticulationAngleDeg = 9.f;
	return S;
}

FWishboneCornerSetup ASportsCar::RearSetup()
{
	FWishboneCornerSetup S;
	// 1.80 Hz on a 357.8 kg corner. The rear runs the higher frequency for the same
	// flat-ride reason as the saloon - a bump reaches the rear wheels later and a
	// stiffer rear catches up in phase, so the body heaves rather than pitching.
	S.SpringRateNPerM = 140843.f;
	S.DamperRateNsPerM = 10461.f;

	// NO REAR BAR. The rear already carries 58.8% of the spring roll stiffness
	// because it carries more mass at a higher frequency; a bar here would push the
	// split further rearward, which is the opposite of what the target asks for.
	S.AntiRollRateNPerM = 0.f;

	// PRELOAD, and it is not optional. The coil is squeezed by exactly the preload at the
	// design position, so the spring force there IS PreloadForceN. Leave it at zero and
	// the spring holds nothing at design height and the corner sinks until it makes its
	// own load - 7.3 cm here, against roughly 8 cm of bump. That is a car on its bump
	// stops with no room left.
	//
	// The sag is exactly the corner load over the WHEEL rate, confirmed against a settled
	// bench run to three decimals, so the preload that cancels it is the corner load over
	// the MOTION RATIO:
	//
	//     357 kg * 9.81 = 3502 N at the wheel,  / 0.582  =  6015 N at the spring
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
	S.PreloadForceN = 6015.f;
	S.StaticCamberDeg = -1.6f;

	// MORE rear toe-in than the front, and more than the saloon runs. Rear toe-in is
	// what keeps a rear-drive car pointing straight while it is being asked for
	// traction, and this one has 400 N*m to put down.
	S.StaticToeDeg = 0.15f;

	S.UnsprungMassKg = 38.f;
	S.WheelRadiusCm = 33.f;
	S.MinArticulationAngleDeg = -9.f;
	S.MaxArticulationAngleDeg = 9.f;
	return S;
}

ASportsCar::ASportsCar()
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

	// 400 N*m at 7800 rpm. At a 5500 rpm torque peak that is 230 kW - 309 hp, or
	// 229 per tonne against the saloon's 129, which is the difference between a car
	// that has to be driven around a corner and one that can be driven out of it.
	//
	// The engine is where the two cars differ most and where the tyre model is
	// stressed hardest: 400 N*m through a 33 cm wheel is 1212 N of tractive demand
	// per driven wheel before any gearing, which is a third of a corner's static
	// weight and enough to reach the combined-slip limit rather than approach it.
	// ---------------------------------------------------------------------------
	// Driveline
	// ---------------------------------------------------------------------------
	//
	// A80 Supra proportions throughout: 2.50 m wheelbase against the real car's 2.55,
	// rear drive, a straight-six torque curve and the V160's own 3.13 final drive.
	// The gearbox is derived from two ends and a rule for the middle, not copied.

	if (Engine)
	{
		// ITS OWN TORQUE CURVE, built here rather than pointed at the project's single
		// existing asset. That asset belongs to the saloon and this engine is not that
		// engine; sharing it would mean either driving a Supra on a saloon's powerband
		// or editing the saloon's to suit.
		//
		// Built in code rather than authored as a .uasset on purpose. These thirteen
		// numbers ARE the engine's character, and a physics project is better off with
		// them visible and diffable next to the derivation that depends on them than
		// hidden in a binary a curve editor owns.
		//
		// Shape is a big-turbo 2JZ: torque arriving early, flat from 3600 to 4200, and
		// tapering rather than falling off a cliff. Normalised, so MaxEngineTorque
		// scales the whole thing - Torque = curve(rpm) * MaxEngineTorque * throttle.
		// BY VALUE, not as a UObject. The first attempt created a UCurveFloat in this
		// constructor, which cannot work: it lives in this class's package, so a
		// Blueprint deriving from ASportsCar referenced a private object in an external
		// package, refused to save, and crashed the editor. An inline curve serialises
		// with whatever owns it and has no such problem.
		if (FRichCurve* Rich = Engine->InlineTorqueCurve.GetRichCurve())
		{
			Rich->Reset();
			Rich->AddKey(  1000.f, 0.500f);
			Rich->AddKey(  1500.f, 0.660f);
			Rich->AddKey(  2000.f, 0.790f);
			Rich->AddKey(  2500.f, 0.900f);
			Rich->AddKey(  3000.f, 0.970f);
			Rich->AddKey(  3600.f, 1.000f);
			Rich->AddKey(  4200.f, 1.000f);
			Rich->AddKey(  4800.f, 0.960f);
			Rich->AddKey(  5400.f, 0.900f);
			Rich->AddKey(  6000.f, 0.820f);
			Rich->AddKey(  6600.f, 0.710f);
			Rich->AddKey(  7000.f, 0.620f);
			Rich->AddKey(  7400.f, 0.500f);
		}

			// ENGINE INERTIA. ACar sets the clutch shaft to 0.2 kg*m^2 for the saloon; a
		// lighter flywheel on a sports car is the cheapest response a driver notices,
		// and it is the one number that decides how fast the engine picks up off idle.
		ClutchShaft.Inertia = 0.18f;
	}

	// ---------------------------------------------------------------------------
	// Rotating inertias
	// ---------------------------------------------------------------------------
	//
	// Every one of these is FShaft's untouched 1.0 kg*m^2 default until something sets
	// it, and 1.0 is the inertia of a road wheel - so an engine, a gearbox shaft and a
	// differential have all been carrying a wheel's worth of rotational mass each.
	//
	// Set in the constructor, which means a Blueprint created FROM this class picks
	// them up. A Blueprint that already exists will not: it stored these when it was
	// saved and the stored value wins. Make a new one, or reset the properties on the
	// old one.

	// Wheel and tyre: mass concentrated toward the rim, so I is about 0.6*m*r^2. 22 kg
	// of rotating mass on a 0.33 m radius. Lighter than the saloon's 24 kg because the
	// wheels are.
	//
	// This is also the denominator of the wheel-spin stability number,
	// G = C*(dkappa/dOmega)*R0*dt/I, so the 1.0 default was inflating G by 44%. The
	// tyre's PKX1 is 25% higher than the saloon's, which pushes G the other way, so the
	// two nearly cancel - coincidence, not design.
	const float WheelInertia = 1.44f;
	if (FrontRightTire) { FrontRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (FrontLeftTire)  { FrontLeftTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearRightTire)  { RearRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearLeftTire)   { RearLeftTire->GetShaft()->SetInertia(WheelInertia); }

	// Crank, flywheel and pulleys about the crank axis. A straight six with a lightened
	// flywheel; the saloon runs 0.20. This is the number that decides how fast the
	// engine picks up, and at 1.0 it was picking up like a marine diesel.
	if (Engine) { Engine->GetShaft()->SetInertia(0.18f); }

	// Crown wheel and carrier. Small, and 1.0 was twenty times it.
	FrontDiffShaft.SetInertia(0.05f);
	RearDiffShaft.SetInertia(0.05f);

	// Gearbox shafts and gears.
	if (Drivetrain && Drivetrain->TransmissionShaft)
	{
		Drivetrain->TransmissionShaft->SetInertia(0.03f);
	}

	// ClutchShaft is left at the 0.2 ACar sets. It is high for a clutch disc and input
	// shaft - 0.02 would be nearer - but it was chosen rather than defaulted, and the
	// anti-stall and engagement timing were tuned against it. Changing it is a separate
	// question from filling in the ones nobody set.


	if (Engine)
	{
		// 430 N*m, which is the 2JZ-GTE's figure. With the curve above that peaks at
		// 222 kW - 297 hp - at 6000 rpm.
		Engine->MaxEngineTorque = 430.f;

		// AND NOW MAXRPM IS SAFE TO SET, which it was not in 40496. The curve is looked
		// up by absolute rpm, so moving the limiter under a curve someone else authored
		// changes the powerband blind - that is why 40503 reverted it. Owning the curve
		// removes the objection: it is authored past the limiter deliberately, so the
		// lookup at the cut is defined rather than clamped off the end.
		Engine->MaxRPM = 7000.f;
	}

	if (Drivetrain)
	{
		// REAR WHEEL DRIVE, and this is the line that was missing. EDrivetrainType
		// defaults to EDT_FrontWheelDrive, so without it the sports car was a
		// front-drive car wearing rear-drive geometry and a limited-slip differential
		// it could not use.
		Drivetrain->DrivetrainType = EDrivetrainType::EDT_RearWheelDrive;

		// GEAR RATIOS, derived from the two ends and a rule for the middle.
		//
		// SIXTH from top speed: 250 km/h wants the engine at 5000 rpm through a
		// 0.33 m wheel, which is a total ratio of 2.488.
		//
		// FIRST from traction: the rear axle carries 7019 N static, so 6317 N of grip
		// at mu 0.90. A first gear is sized to BEAT that - a 430 N*m rear-drive car
		// that cannot spin its wheels off the line is geared like a van - so this is
		// set to 2.3x, giving a total ratio of 11.15.
		//
		// That is a spread of 4.48. The real V160 is 4.83, which is a closer
		// agreement than the method deserves and worth not reading too much into.
		//
		// THE MIDDLE is a tapered geometric progression - steps of 1.45, 1.41, 1.37,
		// 1.33, 1.28 - rather than a constant ratio. A constant step is the textbook
		// answer and no gearbox uses it: the low gears want to be far apart because
		// there is torque to spare, and the high ones close together because there is
		// not, and a flat progression makes fifth to sixth feel like nothing happened.
		//
		// Index 0 is REVERSE, which is how GearRatios is read - GearRatios[CurrentGear]
		// with CurrentGear clamped from zero.
		Drivetrain->FinalDriveRatio = 3.13f;
		// REVERSE IS NEGATIVE, at index 0. The ratio goes straight into a
		// FGearConstraint between the transmission and the diff, so its SIGN is the
		// direction of travel - a positive reverse gear is a seventh forward gear with
		// the wrong label. -3.90 rather than -3.562 because reverse is conventionally
		// slightly lower than first.
		Drivetrain->GearRatios = { -3.90f, 3.562f, 2.457f, 1.742f, 1.272f, 0.956f, 0.747f };
	}


	// THE MASS, which is the number the whole derivation above is against and which
	// nothing was setting. ACar takes its mass from the skeletal mesh's physics asset -
	// 1500 kg on the reference body - so without this the sports car would have had
	// sports springs sized for 317 kg corners carrying 352 kg ones, and every frequency
	// and the roll gradient with them would have been out by the square root of the
	// ratio: 1.60 Hz becomes 1.52, and the stated 2.49 deg/g becomes 2.77.
	//
	// Not a small error and not a visible one, which is the bad combination. Note that
	// ASaloonCar has the same gap and gets away with it because the mesh happens to
	// weigh what it claims.
	if (CarMesh)
	{
		CarMesh->SetMassOverrideInKg(NAME_None, 1350.f, true);
	}

	// A limited-slip differential, which is the other thing that makes it a sports
	// car. The saloon's open diff sends torque to whichever wheel has least grip, so
	// a lifted inside wheel ends the corner; locking on power keeps the outside one
	// working. Coast lock is deliberately less than power lock - a diff that locks
	// hard off-throttle drags the car straight on entry.
	//
	// Inert if the drivetrain is configured with an open carrier rather than an LSD;
	// these are the coefficients, not the choice.
	if (Drivetrain)
	{
		Drivetrain->PreloadTorque = 80.f;
		Drivetrain->PowerLockCoeff = 0.50f;
		Drivetrain->CoastLockCoeff = 0.25f;
	}

}
