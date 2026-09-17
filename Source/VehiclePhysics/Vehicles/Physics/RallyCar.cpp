// Fill out your copyright notice in the Description page of Project Settings.

#include "RallyCar.h"
#include "WishboneSuspension.h"
#include "VehicleEngine.h"
#include "Drivetrain.h"
#include "PacejkaTyreComponent.h"
#include "Curves/CurveFloat.h"

/**
 * Derivation, so these can be re-derived rather than merely trusted. Every number below
 * is arithmetic on the targets at the top - nothing is typed.
 *
 *   TARGETS   1300 kg sprung, 56/44 front/rear, 1.60 m track, 2.55 m wheelbase,
 *             CoG 0.476 m above the roll axis, ride 1.30/1.45 Hz, damping ratio 0.48,
 *             roll-stiffness share 55% front, +/-20 deg of arm sweep.
 *
 * 1. CORNER MASSES.  1300 kg at 56/44 is 364.0 kg per front corner, 286.0 rear.
 *
 * 2. WHEEL RATES.  k = m(2*pi*f)^2
 *      front  364.0 * (2*pi*1.30)^2 = 24286 N/m
 *      rear   286.0 * (2*pi*1.45)^2 = 23739 N/m
 *
 *    THESE ARE SOFTER THAN THE SALOON'S, which is the point and reads wrong until it
 *    does not. A rally car is not a stiff car. On gravel the springs' job is letting the
 *    wheels follow ground that moves, not holding the body up over ground that does not,
 *    and a stiff spring over a long travel simply throws the car off the surface.
 *
 * 3. DAMPING.  c = 2*zeta*sqrt(k*m) at the wheel, zeta 0.48 against the saloon's 0.35.
 *      front  2854 N*s/m    rear  2501 N*s/m
 *    That much travel on that little spring would pogo without it. Soft AND heavily
 *    damped is the combination - a soft car that is not allowed to oscillate.
 *
 * 4. ROLL STIFFNESS FROM THE SPRINGS.  K = 0.5*k*t^2 per axle.
 *      front 31085, rear 30386, total 61471 N*m/rad, of which the front is 50.6%.
 *
 * 5. THE FRONT BAR, sized to the 55% target and nothing else:
 *      K_fb = (share*(K_f + K_r) - K_f) / (1 - share) = 6053 N*m/rad
 *    and back through the same 0.5*k*t^2 to 4729 N/m at the wheel. No rear bar.
 *
 * 6. THE ROLL GRADIENT FALLS OUT.  M*g*h / K_total
 *      = 1300*9.81*0.476 / 67524 = 0.0899 rad = 5.15 deg/g.
 *    Between the saloon's 4.44 and a soft SUV. A rally car leans, and is meant to.
 *
 * 7. BACK THROUGH THE MOTION RATIO.  Everything above is at the WHEEL; the spring and
 *    damper live on the arm, so both divide by MR^2 - 0.616 front, 0.584 rear, measured
 *    on the geometry this inherits.
 *      front  spring 64001 N/m   damper 7522 N*s/m
 *      rear   spring 69604 N/m   damper 7334 N*s/m
 *
 * 8. PRELOAD = corner load / MR, which parks the car at its design height.
 *      front 5797 N   rear 4804 N
 *    And unlike the saloon this one is PROPORTIONED: 9.06 cm of preload deflection
 *    against 8.20 cm of droop at the front strut leaves the coil 0.85 cm from going
 *    slack at full droop, and the rear goes slack just before its stop. The wheels can
 *    actually unload, which on this surface is most of what the suspension is for.
 */

FWishboneHardpoints ARallyCar::FrontHardpoints()
{
	// The saloon's front geometry, three transformations deep and no fresh guesses:
	//
	//   lateral  x 80.0/77.5 for the 1.60 m track
	//   x        shifted 5 cm rearward for the 2.55 m wheelbase
	//   z        shifted DOWN 8 cm, every point including the wheel centre
	//
	// That last one is the ride height and it is worth being clear about the direction.
	// Moving the whole linkage down in BODY space puts the body further above the
	// ground - the contact patch sits at WheelCentre.Z - 33, so dropping the wheel
	// centre to 13.137 puts the ground at -19.863 instead of -11.863. Eight centimetres
	// of lift, and because every point moved by the same amount, not one angle in the
	// linkage changed: camber gain, roll centre height, KPI, caster and scrub are all
	// exactly the saloon's measured values.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(   140.50000f,    36.02967f,   -0.87600f);
	P.LCA_B       = FVector(   114.50000f,    36.02967f,   -0.87600f);
	P.LCA_U       = FVector(   127.50000f,    75.25547f,   -2.00000f);
	P.UCA_F       = FVector(   135.36283f,    44.11830f,   25.84700f);
	P.UCA_B       = FVector(   114.56283f,    44.11830f,   25.84700f);
	P.UCA_U       = FVector(   124.96283f,    68.89249f,   27.00000f);
	P.DamperMount = FVector(   127.50000f,    61.91870f,   -1.61788f);
	P.CoiloverTop = FVector(   125.50000f,    45.41935f,   48.00000f);
	P.WheelCentre = FVector(   127.50000f,    80.00000f,   13.13677f);
	return P;
}

FWishboneHardpoints ARallyCar::RearHardpoints()
{
	// Same three transformations, and the same reasoning as the saloon's rear: less KPI,
	// no caster, and a roll centre above the front so the roll axis tilts nose-down.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(  -114.50000f,    38.32035f,    0.95800f);
	P.LCA_B       = FVector(  -140.50000f,    38.32035f,    0.95800f);
	P.LCA_U       = FVector(  -127.50000f,    77.54616f,   -2.00000f);
	P.UCA_F       = FVector(  -117.10000f,    47.56110f,   27.43600f);
	P.UCA_B       = FVector(  -137.90000f,    47.56110f,   27.43600f);
	P.UCA_U       = FVector(  -127.50000f,    74.39981f,   27.00000f);
	P.DamperMount = FVector(  -127.50000f,    63.42487f,   -0.93501f);
	P.CoiloverTop = FVector(  -129.50000f,    45.41935f,   48.00000f);
	P.WheelCentre = FVector(  -127.50000f,    80.00000f,   13.13677f);
	return P;
}

FWishboneCornerSetup ARallyCar::FrontSetup()
{
	FWishboneCornerSetup S;
	// 1.30 Hz on a 364 kg corner is 24.3 kN/m at the wheel; through a motion ratio of
	// 0.616 that is 64.0 kN/m at the spring. SOFTER than the saloon's 1.253 Hz car,
	// deliberately - see the derivation.
	S.SpringRateNPerM = 64001.f;

	// zeta 0.48 against the saloon's 0.35. The spring is soft and the travel is long;
	// without this the car would oscillate on its own springs for several seconds after
	// every crest, which on a stage is the difference between a car and a boat.
	S.DamperRateNsPerM = 7522.f;

	// Sized to the 55% front share and nothing else. The springs alone put 50.6% on the
	// front, so this bar is doing far less work than the sports car's - a rally car
	// wants its axles free to move independently, and a big bar is the opposite of that.
	S.AntiRollRateNPerM = 4729.f;

	// 364 kg * 9.81 / 0.616. Parks the car at design height rather than letting it sink
	// most of its travel before it has hit anything.
	S.PreloadForceN = 5797.f;

	// Less than the sports car's -2.2. Gravel rewards keeping the whole contact patch on
	// a surface that is already moving underneath it, rather than leaning the tyre onto
	// its outer shoulder for a grip level this surface will not supply.
	S.StaticCamberDeg = -1.8f;

	// TOE-OUT at the front, which neither tarmac car runs. Negative is out. It trades
	// straight-line calm for turn-in, and on a stage the corner is always arriving.
	S.StaticToeDeg = -0.10f;

	// Heavier than either tarmac car: gravel wheels, tall sidewalls, and brakes that
	// have to survive being buried in stones.
	S.UnsprungMassKg = 45.f;
	S.WheelRadiusCm = 33.f;

	// +/-20 DEGREES, against +/-12 on both tarmac cars. 26.6 cm of travel at the wheel
	// rather than 16. This is the number that makes it a rally car, and it is the one
	// piece of this plugin's envelope that no existing setup has ever exercised.
	S.MinArticulationAngleDeg = -20.f;
	S.MaxArticulationAngleDeg = 20.f;
	return S;
}

FWishboneCornerSetup ARallyCar::RearSetup()
{
	FWishboneCornerSetup S;
	// 1.45 Hz on a 286 kg corner. The rear runs the higher frequency for the same
	// flat-ride reason as the other two - a bump reaches the rear wheels later and a
	// stiffer rear catches up in phase, so the body heaves rather than pitching.
	S.SpringRateNPerM = 69604.f;
	S.DamperRateNsPerM = 7334.f;

	// NO REAR BAR. The rear already carries 49.4% of the spring roll stiffness and this
	// car needs its rear axle able to articulate - a bar here would tie the two rear
	// wheels together over exactly the ruts they are meant to follow independently.
	S.AntiRollRateNPerM = 0.f;

	S.PreloadForceN = 4804.f;
	S.StaticCamberDeg = -1.3f;

	// Rear toe-IN against the front's toe-out. The front is set up to turn and the rear
	// to stay behind it; on a loose surface with all four wheels driven, a rear axle
	// that steers itself is how a car ends up facing the wrong way.
	S.StaticToeDeg = 0.20f;

	S.UnsprungMassKg = 45.f;
	S.WheelRadiusCm = 33.f;
	S.MinArticulationAngleDeg = -20.f;
	S.MaxArticulationAngleDeg = 20.f;
	return S;
}

ARallyCar::ARallyCar()
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
	// Engine - 2.0 turbo
	// ---------------------------------------------------------------------------
	if (Engine)
	{
		// ITS OWN CURVE, and the shape is the whole story. A turbocharged four is not a
		// small version of the Supra's six: it is nearly absent below 2500 rpm, arrives
		// all at once, holds a flat plateau while the turbo is on song, and gives up
		// early. The saloon's naturally aspirated curve rises smoothly and the Supra's
		// big-turbo six holds its peak for 1200 rpm; this one is 0.32 at idle and 1.00
		// by 3600, which is a step, not a ramp.
		//
		// That step is why a rally car is driven on the throttle rather than with it,
		// and it is a genuinely different demand on the tyre model - torque arriving
		// faster than the driver can meter it, onto a surface with 0.60 of friction.
		Engine->InlineTorqueCurve.GetRichCurve()->Reset();
		if (FRichCurve* Rich = Engine->InlineTorqueCurve.GetRichCurve())
		{
			const float RPM[] = { 1000.f, 1500.f, 2000.f, 2500.f, 3000.f, 3600.f, 4200.f,
								  4800.f, 5400.f, 6000.f, 6600.f, 7000.f, 7400.f };
			const float Norm[] = { 0.32f, 0.45f, 0.62f, 0.82f, 0.95f, 1.00f, 1.00f,
								   1.00f, 0.97f, 0.90f, 0.80f, 0.70f, 0.58f };
			for (int32 i = 0; i < UE_ARRAY_COUNT(RPM); ++i)
			{
				Rich->AddKey(RPM[i], Norm[i]);
			}
		}

		// 420 N*m at 1300 kg. Modest against the Supra's 430 in a lighter car, and it
		// does not matter: on 0.60 of friction the limit is the surface, not the engine.
		Engine->MaxEngineTorque = 420.f;
		Engine->MaxRPM = 7000.f;
		Engine->IdleRPM = 900.f;
		Engine->RedLineRPMRatio = 0.95f;
	}

	// ---------------------------------------------------------------------------
	// Driveline - all four wheels
	// ---------------------------------------------------------------------------
	if (Drivetrain)
	{
		// ALL WHEEL DRIVE, and this is the first car in the project to ask for it. The
		// constraint graph has solved front-drive and rear-drive; a centre path is a
		// different shape of problem and it has never been exercised.
		Drivetrain->DrivetrainType = EDrivetrainType::EDT_AllWheelDrive;

		// SHORT. 4.30 against the saloon's 3.15 and the Supra's 3.13, because a stage is
		// not a motorway: the useful speed range is 40 to 160 km/h and the gearing
		// should spend all six ratios inside it rather than saving three for a speed the
		// car will never see.
		Drivetrain->FinalDriveRatio = 4.30f;

		// Index 0 is REVERSE and must be negative - it goes straight into a gear
		// constraint, so its sign is the direction of travel.
		//
		// Top from a 200 km/h maximum: total = omega_engine / (v / r) = 4.35, so sixth
		// is 1.01 on a 4.30 final. First from traction, with all four wheels driven on
		// 0.60 of gravel friction: 2.79. That is a spread of 2.76 against the Supra's
		// 4.48 - the ratios are CLOSE, because with this little friction the engine has
		// to be kept inside the plateau rather than being allowed to fall out of it.
		Drivetrain->GearRatios = { -3.20f, 2.790f, 2.180f, 1.760f, 1.450f, 1.210f, 1.010f };

		// Tighter than either tarmac car. On a loose surface a differential that lets a
		// wheel spin has given away the drive entirely, and the usual argument against
		// locking - that it drags the car straight on entry - matters less when the car
		// is being turned with the throttle anyway.
		Drivetrain->PreloadTorque = 120.f;
		Drivetrain->PowerLockCoeff = 0.60f;
		Drivetrain->CoastLockCoeff = 0.40f;
		Drivetrain->ClutchMaxTorque = 500.f;
		Drivetrain->SolverIterations = 10;
	}

	// ---------------------------------------------------------------------------
	// Rotating inertias
	// ---------------------------------------------------------------------------
	//
	// Every one of these is FShaft's untouched 1.0 kg*m^2 default until something sets
	// it, which is a road wheel's worth of rotating mass on an engine, a gearbox shaft
	// and each differential.
	//
	// Wheel inertia is the denominator of the wheel-spin stability number
	// G = C * (dkappa/dOmega) * R0 * dt / I, so running 1.0 where the physics wants 1.65
	// inflates G by 65% on every corner.
	const float WheelInertia = 1.65f;   // gravel wheel and tyre, ~= 0.6*m*r^2
	if (FrontRightTire) { FrontRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (FrontLeftTire)  { FrontLeftTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearRightTire)  { RearRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearLeftTire)   { RearLeftTire->GetShaft()->SetInertia(WheelInertia); }

	if (Engine) { Engine->GetShaft()->SetInertia(0.20f); }

	// Heavier than the tarmac cars' 0.05: an all-wheel-drive car carries two output
	// differentials AND a centre one, and none of them is a light assembly.
	FrontDiffShaft.SetInertia(0.06f);
	RearDiffShaft.SetInertia(0.06f);

	if (Drivetrain && Drivetrain->TransmissionShaft)
	{
		Drivetrain->TransmissionShaft->SetInertia(0.04f);
	}

	// ---------------------------------------------------------------------------
	// Mass
	// ---------------------------------------------------------------------------
	//
	// The physics asset carries mass, centre of mass AND rotational inertia, and none of
	// the three is set anywhere else in this plugin. Until this car has its own hulls it
	// will inherit whatever asset its Blueprint points at - check the bench's sprung
	// figure reads 1300 before trusting a single frequency in this file.
	if (CarMesh)
	{
		CarMesh->SetMassOverrideInKg(NAME_None, 1300.f, true);
	}
}
