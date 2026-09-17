// Fill out your copyright notice in the Description page of Project Settings.



#include "VehicleEngine.h"
#include "Curves/CurveFloat.h"


UVehicleEngine::UVehicleEngine()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVehicleEngine::BeginPlay()
{
	Super::BeginPlay();

}

void UVehicleEngine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Torque is NOT computed here any more - see UpdateTorque, which the owner calls
	// once per physics substep. An engine whose owner never calls it makes no torque
	// at all, the same contract UWishboneSuspension::SolveSubstep has.
}

void UVehicleEngine::UpdateTorque()
{
	// SPEED FIRST. This used to be read at the very end of the tick, so the torque
	// applied through the following frame was the torque for the speed the engine
	// had before it.
	CurrentEngineRPM = Shaft.GetAngularVelocity() * 60.f / (PI * 2.f);
	if (CurrentEngineRPM < 0.f)
	{
		Shaft.SetAngularVelocity(0.f);
		CurrentEngineRPM = 0.f;
	}
	if (CurrentEngineRPM > MaxRPM)
	{
		Shaft.SetAngularVelocity(MaxRPM * (2.f * PI) / 60.f);
		CurrentEngineRPM = MaxRPM;
	}

	const float NormalizedRPM = FMath::Clamp(CurrentEngineRPM / MaxRPM, 0.f, 1.f);
	// The asset wins if there is one; otherwise the inline curve, which is how a class
	// like ASportsCar defines its own engine without needing a .uasset. Zero only when
	// neither exists, which is a genuinely unconfigured engine rather than a fallback.
	const FRichCurve* Inline = InlineTorqueCurve.GetRichCurveConst();
	const float NormalizedTorque = TorqueCurve
		? TorqueCurve->GetFloatValue(CurrentEngineRPM)
		: ((Inline != nullptr && Inline->GetNumKeys() > 0) ? Inline->Eval(CurrentEngineRPM) : 0.f);

	// Rev limiter, with hysteresis: cut at MaxRPM and do not light again until the
	// engine has fallen back to RedLineRPMRatio. The hysteresis is exactly what stops
	// a limiter chattering on and off at the limit, and is left as it was.
	float ThrottleChoker = 0.f;
	if (NormalizedRPM >= 1.0f)
	{
		bIsBurning = true;
		ThrottleChoker = 0.f;
	}
	if (!bIsBurning)
	{
		ThrottleChoker = 1.f;
	}
	if (NormalizedRPM <= RedLineRPMRatio)
	{
		bIsBurning = false;
		ThrottleChoker = 1.f;
	}

	const float EffectiveThrottle = FMath::Clamp(Throttle * ThrottleChoker, 0.f, 1.f);
	Torque = NormalizedTorque * MaxEngineTorque * EffectiveThrottle;

	const float EngineBrakingTorque = -EngineBrakingCoefficient * NormalizedRPM;

	// Pumping and friction losses, scaled by how far the throttle is SHUT rather than
	// switched on at exactly zero.
	//
	// The switch was a discontinuity and also wrong: a hair of throttle turned engine
	// braking completely off. Real losses do not vanish. They are largest with the
	// throttle closed - highest manifold vacuum, the engine working hardest to
	// breathe - and fall away smoothly as it opens. Identical to the old behaviour at
	// both extremes; only part throttle differs, where the engine now brakes a little
	// instead of freewheeling.
	float BrakingCoef = 1.f - EffectiveThrottle;

	const float LimiterBand = FMath::Max(1.f - RedLineRPMRatio, KINDA_SMALL_NUMBER);
	const float OverRev = FMath::Clamp((NormalizedRPM - RedLineRPMRatio) / LimiterBand, 0.f, 1.f);
	BrakingCoef = FMath::Lerp(BrakingCoef, RedlineBrakingMultiplier, OverRev);

	NetTorque = Torque + (EngineBrakingTorque * BrakingCoef);

	// --- Idle governor -------------------------------------------------------
	//
	// Props the engine up as it falls under IdleRPM, in proportion to how far under
	// it has got, fading to nothing exactly at idle.
	//
	// Deliberately a TORQUE and not a clamp on the shaft. Clamping the angular
	// velocity would hold the number steady while conjuring the energy to do it from
	// nowhere - and with the clutch engaged that invented energy would drive the car
	// along. Torque has to fight the load like anything else, so a hard enough load
	// can still drag the engine under and stall it. That is correct: the clutch is
	// what should give way first, and UDrivetrain::UpdateShift eases it off before
	// this has to lose.
	if (IdleRPM > KINDA_SMALL_NUMBER && CurrentEngineRPM < IdleRPM)
	{
		const float Shortfall = FMath::Clamp((IdleRPM - CurrentEngineRPM) / IdleRPM, 0.f, 1.f);
		NetTorque += IdleGovernorTorque * Shortfall;

		// Engine braking is what drags it under in the first place, and an engine
		// sitting at idle is in equilibrium rather than braking itself to a halt.
		NetTorque -= EngineBrakingTorque * BrakingCoef * Shortfall;
	}
}
