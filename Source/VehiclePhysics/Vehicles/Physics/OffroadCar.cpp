// Fill out your copyright notice in the Description page of Project Settings.

#include "OffroadCar.h"
#include "WishboneSuspension.h"
#include "VehicleEngine.h"
#include "Drivetrain.h"
#include "PacejkaTyreComponent.h"
#include "Curves/CurveFloat.h"

/**
 * Derivation. Every number below is arithmetic on the targets - nothing is typed.
 *
 *   TARGETS   2200 kg sprung, 52/48 front/rear, 1.75 m track, 3.00 m wheelbase,
 *             CoG 0.62 m above the roll axis, ride 1.05/1.15 Hz, damping ratio 0.45,
 *             roll-stiffness share 54% front, +/-28 deg of arm sweep, 42 cm tyre.
 *
 * 1. CORNER MASSES.  2200 kg at 52/48 is 572.0 kg per front corner, 528.0 rear.
 *
 * 2. WHEEL RATES.  k = m(2*pi*f)^2
 *      front  572.0 * (2*pi*1.05)^2 = 24896 N/m
 *      rear   528.0 * (2*pi*1.15)^2 = 27567 N/m
 *
 *    NEARLY THE RALLY CAR'S RATES ON ALMOST TWICE THE MASS, which is what a 1.05 Hz
 *    ride frequency means and is the whole character of the vehicle. The spring is not
 *    holding the body up so much as keeping it roughly level while the wheels get on
 *    with following ground that is not flat.
 *
 * 3. DAMPING.  c = 2*zeta*sqrt(k*m), zeta 0.45.
 *      front 3396 N*s/m   rear 3434 N*s/m
 *
 * 4. ROLL STIFFNESS FROM THE SPRINGS.  K = 0.5*k*t^2, on a 1.75 m track.
 *      front 38122, rear 42212, total 80334 N*m/rad - the front is 47.5%.
 *
 * 5. THE FRONT BAR, sized to the 54% target: K_fb = 11434 N*m/rad, which is 7465 N/m at
 *    the wheel. No rear bar. That is a bigger bar than any other car here carries and it
 *    still only takes the split from 47.5% to 54%, because the springs are so soft that
 *    everything is a large fraction of them.
 *
 * 6. THE ROLL GRADIENT FALLS OUT.  M*g*h / K_total
 *      = 2200*9.81*0.62 / 91768 = 0.1458 rad = 8.35 deg/g.
 *
 *    Nearly four times the sports car's. THIS IS THE ANSWER, not a problem with it:
 *    2200 kg with its mass 62 cm above the roll axis, on springs soft enough to keep
 *    37 cm of travel useful, leans. Correcting it would need a bar stiff enough to tie
 *    the axles together over exactly the ground they exist to follow independently.
 *
 * 7. BACK THROUGH THE MOTION RATIO - 0.616 front, 0.584 rear, from the inherited linkage.
 *      front  spring 65610 N/m   damper  8950 N*s/m
 *      rear   spring 80828 N/m   damper 10068 N*s/m
 *
 * 8. PRELOAD = corner load / MR.  front 9109 N, rear 8869 N. Proportioned: 13.88 cm of
 *    front preload deflection against 11.49 cm of droop leaves 2.40 cm in hand, and the
 *    rear sits 0.08 cm from going slack at full droop.
 *
 * THE TYRE IS THE ONE THING THE OTHER THREE SHARE AND THIS DOES NOT. 42 cm against 33,
 * and WheelRadiusCm must match UTireParamsOffroad's R0 or Fz and slip ratio are both
 * quietly wrong. It also drops the contact patch 9 cm further from the wheel centre,
 * which is why scrub goes negative and both roll centres rise - measured, not assumed:
 *
 *      front  camber -0.132 deg/cm   roll centre  7.71 cm   KPI 13.49   scrub -12.6 mm
 *      rear   camber -0.106 deg/cm   roll centre 10.37 cm   KPI  6.77   scrub  -5.0 mm
 *
 * Front roll centre still sits BELOW the rear, so the roll axis keeps its nose-down tilt.
 */

FWishboneHardpoints AOffroadCar::FrontHardpoints()
{
	// The saloon's front geometry: lateral x 87.5/77.5 for the 1.75 m track, x shifted
	// 17.5 cm forward for the 3.00 m wheelbase, z shifted DOWN 12 cm for ride height.
	// With the 42 cm tyre that puts the body 21 cm higher off the ground than the
	// saloon's - 12 from the linkage, 9 from the wheel.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(   163.00000f,     39.40745f,     -4.87600f);
	P.LCA_B       = FVector(   137.00000f,     39.40745f,     -4.87600f);
	P.LCA_U       = FVector(   150.00000f,     82.31067f,     -6.00000f);
	P.UCA_F       = FVector(   157.86283f,     48.25439f,     21.84700f);
	P.UCA_B       = FVector(   137.06283f,     48.25439f,     21.84700f);
	P.UCA_U       = FVector(   147.46283f,     75.35116f,     23.00000f);
	P.DamperMount = FVector(   150.00000f,     67.72358f,     -5.61788f);
	P.CoiloverTop = FVector(   148.00000f,     49.67742f,     44.00000f);
	P.WheelCentre = FVector(   150.00000f,     87.50000f,      9.13677f);
	return P;
}

FWishboneHardpoints AOffroadCar::RearHardpoints()
{
	// Same three transformations, same reasoning as every rear here: less KPI, no caster,
	// and a roll centre above the front so the roll axis tilts nose-down.
	FWishboneHardpoints P;
	P.LCA_F       = FVector(  -137.00000f,     41.91288f,     -3.04200f);
	P.LCA_B       = FVector(  -163.00000f,     41.91288f,     -3.04200f);
	P.LCA_U       = FVector(  -150.00000f,     84.81611f,     -6.00000f);
	P.UCA_F       = FVector(  -139.60000f,     52.01996f,     23.43600f);
	P.UCA_B       = FVector(  -160.40000f,     52.01996f,     23.43600f);
	P.UCA_U       = FVector(  -150.00000f,     81.37480f,     23.00000f);
	P.DamperMount = FVector(  -150.00000f,     69.37095f,     -4.93501f);
	P.CoiloverTop = FVector(  -152.00000f,     49.67742f,     44.00000f);
	P.WheelCentre = FVector(  -150.00000f,     87.50000f,      9.13677f);
	return P;
}

FWishboneCornerSetup AOffroadCar::FrontSetup()
{
	FWishboneCornerSetup S;
	// 1.05 Hz on a 572 kg corner - nearly the rally car's wheel rate on almost twice the
	// mass. The softest setup in the project by a wide margin, and deliberately.
	S.SpringRateNPerM = 65610.f;
	S.DamperRateNsPerM = 8950.f;

	// The biggest bar here, and it still only moves the split from 47.5% to 54%, because
	// springs this soft make everything a large fraction of them.
	S.AntiRollRateNPerM = 7465.f;

	// 572 kg * 9.81 / 0.616.
	S.PreloadForceN = 9109.f;

	// Almost none. A tyre this tall and this soft carries its own camber compliance, and
	// static camber on a vehicle that spends its time on uneven ground mostly succeeds in
	// putting the tyre on one shoulder at the moment the ground tilts the other way.
	S.StaticCamberDeg = -0.5f;

	// A little toe-in at both ends, against the rally car's front toe-out. Nothing about
	// this vehicle wants to be eager to change direction.
	S.StaticToeDeg = 0.15f;

	// The heaviest unsprung mass here by some way: a 42 cm wheel, a mud-terrain tyre and
	// the brakes to stop 2200 kg.
	S.UnsprungMassKg = 62.f;

	// 42 cm, and it MUST match UTireParamsOffroad's R0. A radius mismatch is silently
	// wrong in both Fz and slip ratio, and 9 cm on 33 is not a rounding error.
	S.WheelRadiusCm = 42.f;

	// +/-28 DEGREES. 37.3 cm of travel at the wheel, against the tarmac cars' 16 and the
	// rally car's 26.6. This is the widest envelope the linkage has ever been asked for.
	S.MinArticulationAngleDeg = -28.f;
	S.MaxArticulationAngleDeg = 28.f;
	return S;
}

FWishboneCornerSetup AOffroadCar::RearSetup()
{
	FWishboneCornerSetup S;
	// 1.15 Hz on a 528 kg corner. Higher than the front for the same flat-ride reason as
	// every other car here.
	S.SpringRateNPerM = 80828.f;
	S.DamperRateNsPerM = 10068.f;

	// NO REAR BAR, and here it matters more than anywhere else. Articulation IS the point
	// of this vehicle; a rear bar would tie the two rear wheels together over exactly the
	// ground they exist to follow one at a time.
	S.AntiRollRateNPerM = 0.f;

	S.PreloadForceN = 8869.f;
	S.StaticCamberDeg = -0.3f;
	S.StaticToeDeg = 0.20f;
	S.UnsprungMassKg = 62.f;
	S.WheelRadiusCm = 42.f;
	S.MinArticulationAngleDeg = -28.f;
	S.MaxArticulationAngleDeg = 28.f;
	return S;
}

AOffroadCar::AOffroadCar()
{
	const FWishboneHardpoints Front = FrontHardpoints();
	const FWishboneHardpoints Rear = RearHardpoints();
	const FWishboneCornerSetup FrontTune = FrontSetup();
	const FWishboneCornerSetup RearTune = RearSetup();

	// Hardpoints are written as the RIGHT-hand corner throughout; the mirror flag is
	// already set per corner by ACar's constructor and InitialiseGeometry applies it.
	if (FrontRightSuspension) { FrontRightSuspension->Hardpoints = Front; FrontRightSuspension->ApplySetup(FrontTune); }
	if (FrontLeftSuspension)  { FrontLeftSuspension->Hardpoints  = Front; FrontLeftSuspension->ApplySetup(FrontTune); }
	if (RearRightSuspension)  { RearRightSuspension->Hardpoints  = Rear;  RearRightSuspension->ApplySetup(RearTune); }
	if (RearLeftSuspension)   { RearLeftSuspension->Hardpoints   = Rear;  RearLeftSuspension->ApplySetup(RearTune); }

	// ---------------------------------------------------------------------------
	// Engine - large-displacement, torque low down
	// ---------------------------------------------------------------------------
	if (Engine)
	{
		// The OPPOSITE shape to the rally car's turbo four. That one is 0.32 at idle and
		// steps to full boost; this is 0.72 at 1000 rpm and never really climbs - 96% of
		// peak by 2000 and finished by 5000. It is a curve with no powerband because the
		// whole range is the powerband, which is what a vehicle needs when it is asked
		// for torque at walking pace against an obstacle rather than for power on a
		// straight.
		Engine->InlineTorqueCurve.GetRichCurve()->Reset();
		if (FRichCurve* Rich = Engine->InlineTorqueCurve.GetRichCurve())
		{
			const float RPM[] = { 800.f, 1200.f, 1600.f, 2000.f, 2400.f, 2800.f, 3200.f,
								  3600.f, 4000.f, 4400.f, 4800.f, 5200.f, 5600.f };
			const float Norm[] = { 0.72f, 0.85f, 0.93f, 0.98f, 1.00f, 1.00f, 0.98f,
								   0.95f, 0.90f, 0.83f, 0.74f, 0.63f, 0.50f };
			for (int32 i = 0; i < UE_ARRAY_COUNT(RPM); ++i)
			{
				Rich->AddKey(RPM[i], Norm[i]);
			}
		}

		// 560 N*m, the most here, and it still gives the worst power-to-weight: 2200 kg
		// and a 5200 rpm limit. Torque is the point and revs are not.
		Engine->MaxEngineTorque = 560.f;
		Engine->MaxRPM = 5200.f;
		Engine->IdleRPM = 700.f;
		Engine->RedLineRPMRatio = 0.95f;
	}

	// ---------------------------------------------------------------------------
	// Driveline
	// ---------------------------------------------------------------------------
	if (Drivetrain)
	{
		Drivetrain->DrivetrainType = EDrivetrainType::EDT_AllWheelDrive;

		// 4.10, and it is doing MORE work than the number suggests. Final drive is
		// geared against the rolling radius, and this vehicle's is 42 cm rather than 33 -
		// so 4.10 here pulls like about 3.22 would on the other cars. A big tyre is a
		// gearing change whether or not anyone treats it as one.
		Drivetrain->FinalDriveRatio = 4.10f;

		// Index 0 is REVERSE and must be negative. First is deliberately very low - 4.20,
		// against the rally car's 2.79 - because the useful bottom end here is a walking
		// pace with the engine somewhere it makes torque, not a standing start. The
		// spread is 5.7, the widest here, and the opposite choice to the rally car's
		// close 2.76: that one keeps an engine inside a narrow plateau, this one has a
		// plateau everywhere and needs the RANGE instead.
		Drivetrain->GearRatios = { -4.50f, 4.200f, 2.560f, 1.720f, 1.240f, 0.950f, 0.740f };

		// The tightest here. On loose ground a differential that lets a wheel spin has
		// given the drive away entirely, and this vehicle will routinely have a wheel in
		// the air where a locked centre is the only reason the others still pull.
		Drivetrain->PreloadTorque = 200.f;
		Drivetrain->PowerLockCoeff = 0.75f;
		Drivetrain->CoastLockCoeff = 0.50f;
		Drivetrain->ClutchMaxTorque = 700.f;
		Drivetrain->SolverIterations = 10;
	}

	// ---------------------------------------------------------------------------
	// Rotating inertias
	// ---------------------------------------------------------------------------
	//
	// Every one is FShaft's untouched 1.0 kg*m^2 default until something sets it.
	//
	// The wheel figure is the one that matters: it is the denominator of the wheel-spin
	// stability number G = C * (dkappa/dOmega) * R0 * dt / I. This vehicle has the
	// largest wheel inertia here AND the lowest cornering stiffness, which push G in
	// opposite directions - worth watching in the bench's wheel-spin block rather than
	// assuming it lands where the others do.
	const float WheelInertia = 3.30f;   // 42 cm mud-terrain, ~= 0.6*m*r^2 at ~31 kg
	if (FrontRightTire) { FrontRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (FrontLeftTire)  { FrontLeftTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearRightTire)  { RearRightTire->GetShaft()->SetInertia(WheelInertia); }
	if (RearLeftTire)   { RearLeftTire->GetShaft()->SetInertia(WheelInertia); }

	if (Engine) { Engine->GetShaft()->SetInertia(0.32f); }   // big crank and flywheel

	FrontDiffShaft.SetInertia(0.09f);
	RearDiffShaft.SetInertia(0.09f);

	if (Drivetrain && Drivetrain->TransmissionShaft)
	{
		Drivetrain->TransmissionShaft->SetInertia(0.06f);
	}

	// ---------------------------------------------------------------------------
	// Mass
	// ---------------------------------------------------------------------------
	//
	// The physics asset carries mass, centre of mass AND rotational inertia, and none of
	// the three is set anywhere else in this plugin. Until this vehicle has its own hulls
	// it inherits whatever asset its Blueprint points at - and a saloon's hulls under a
	// 2200 kg 4x4 will read a CoG far too low and inertias barely half what they should
	// be. Check the bench's sprung figure says 2200 before trusting a frequency here.
	if (CarMesh)
	{
		CarMesh->SetMassOverrideInKg(NAME_None, 2200.f, true);
	}
}
