// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacejkaResult.h"
#include "Shaft.h"
#include "TransientState.h"
#include "Components/ActorComponent.h"
#include "PacejkaTyreComponent.generated.h"


class UTireParamsDataAsset;
struct FWheelContactFrame;

UCLASS(ClassGroup=(VehiclePhysics), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UPacejkaTyreComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/** The drivetrain borrows this, exactly as it borrows UWheelCollider's. */
	FORCEINLINE FShaft* GetShaft() { return &WheelShaft; }

	/**
	 * Omega and Vcx AS THE SLIP SAW THEM, i.e. sampled at the top of SolveSubstep
	 * rather than read off the shaft afterwards.
	 *
	 * Reading the shaft from Tick gives the POST-solve spin: the drivetrain runs
	 * after the tyres inside the same substep, so by the time anything outside can
	 * look, omega is no longer the one that produced kappa. These three must satisfy
	 * 4.E5 exactly - kappa = (R0*Omega - Vcx)/|Vcx| - so any residual is a real
	 * inconsistency, and a clean match with a different post-solve omega measures
	 * how far the drivetrain moved the wheel within the step.
	 */
	FORCEINLINE float GetOmegaUsed() const { return OmegaUsed; }
	FORCEINLINE float GetVcxUsed() const { return VcxUsed; }

	/** Substep-rate kappa span since the previous call; resets the window. */
	FORCEINLINE float ReadAndResetKappaSpan()
	{
		const float Span = KappaSpanMax - KappaSpanMin;
		KappaSpanMin = KappaSpanMax = PacejkaResult.Kappa;
		return Span;
	}
	
	FORCEINLINE const FPacejkaResult& GetResult() const { return PacejkaResult; }
	FORCEINLINE const FTransientTireState& GetTransientState() const { return TransientState; }
	
	/**
	 * The WHOLE contact force this tyre puts on the ground, world N - normal load
	 * and in-plane together. Hand it to the suspension, which applies it as one
	 * vector at the patch.
	 *
	 * One force rather than a load and a separate in-plane part, because the
	 * chassis wrench never cared about the split: it comes from momentum balance on
	 * the unsprung mass, which only ever sees the total ground force. Splitting it
	 * only created two things to keep in step.
	 */
	FORCEINLINE FVector GetContactForceN() const { return ContactForceN; }

	/** Normal load alone, N. Diagnostic - GetContactForceN already contains it. */
	FORCEINLINE float GetNormalLoadN() const { return NormalLoadN; }
	
	FORCEINLINE UTireParamsDataAsset* GetTireParams() const { return TireParams; }

	/**
	 * Unloaded radius from the tyre asset, cm. Zero when no asset is assigned.
	 *
	 * The suspension carries this number too, because it has to place a contact
	 * patch with no tyre attached at all in the editor. Whoever pairs the two is
	 * responsible for checking they agree - see ACar::BeginPlay.
	 */
	float GetUnloadedRadiusCm() const;

	/** Self-aligning moment about the road normal, N*m. Applied separately - a
	 *  pure couple has no application point, so it cannot ride the two-point wrench. */
	FORCEINLINE float GetAligningMomentNm() const { return AligningMomentNm; }

	void SolveSubstep(const FWheelContactFrame& Frame, float SubstepTime);

protected:
	UPROPERTY(EditAnywhere, Category = "Tyre")
	UTireParamsDataAsset* TireParams = nullptr;

	UPROPERTY(EditAnywhere, Category = "Tyre")
	bool bUseTransients = true;

	/**
	 * Tyre radial damping - the carcass's own hysteresis, N*s/m.
	 *
	 * Not part of the Magic Formula: 4.E68 is a static load curve with no velocity
	 * term, so nothing in Pacejka damps the wheel-hop mode. That mode sits near
	 * 13 Hz on this corner, and the coilover only reaches it through the motion
	 * ratio SQUARED - roughly 0.4 of its damping - so without this the corner
	 * buzzes on its own tyre spring.
	 */
	UPROPERTY(EditAnywhere, Category = "Tyre", meta = (ClampMin = "0.0"))
	float RadialDampingNsPerM = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Tyre")
	FShaft WheelShaft;

	FTransientTireState TransientState;
	FPacejkaResult PacejkaResult;

	/**
	 * Widest kappa seen since the last ReadAndResetKappaSpan, i.e. across every
	 * SUBSTEP rather than the one the frame happens to end on.
	 *
	 * The bench samples per frame at ~60 Hz while physics runs at 200. Anything
	 * oscillating at the substep rate aliases down into what looks like a steady
	 * offset, which is exactly how a diverging wheel reads in the CSV. The span
	 * cannot alias: if it is wide, the wheel is ringing between samples.
	 */
	float KappaSpanMin = 0.f;
	float KappaSpanMax = 0.f;

	/** See GetOmegaUsed - the inputs kappa was actually formed from. */
	float OmegaUsed = 0.f;
	float VcxUsed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tyre|Live")
	FVector ContactForceN = FVector::ZeroVector;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tyre|Live")
	float NormalLoadN = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tyre|Live")
	float AligningMomentNm = 0.f;
};
