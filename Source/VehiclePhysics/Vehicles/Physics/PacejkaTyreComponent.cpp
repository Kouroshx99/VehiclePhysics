// Fill out your copyright notice in the Description page of Project Settings.


#include "PacejkaTyreComponent.h"

#include "TireParamsDataAsset.h"
#include "TransientState.h"
#include "WheelContactFrame.h"
#include "VehiclePhysicsUnits.h"

// CmToM and friends are spelled out with their namespace below, deliberately.
//
// This file used them bare, which only ever compiled because unity builds paste
// several .cpp files into one translation unit and Car.cpp declares a file-scope
// CmToM of its own that was leaking in. Compiled alone the name did not exist.
//
// A "using namespace VehicleUnits" would fix that and break the other case: inside
// the blob, Car.cpp's CmToM and VehicleUnits::CmToM are then BOTH in scope and every
// use is ambiguous. Qualifying each one is the only spelling correct in both.


float UPacejkaTyreComponent::GetUnloadedRadiusCm() const
{
    return TireParams != nullptr ? TireParams->GetUnloadedRadiusCm() : 0.f;
}

void UPacejkaTyreComponent::SolveSubstep(const FWheelContactFrame& Frame, float SubstepTime)
{
    if (TireParams == nullptr || !Frame.bInContact || SubstepTime <= SMALL_NUMBER)
    {
        ContactForceN = FVector::ZeroVector;
        NormalLoadN = 0.f;
        AligningMomentNm = 0.f;
        // Airborne: drop the relaxation history so stale slip cannot carry into
        // the next landing. Same reason UWheelCollider resets on its air path.
        TransientState.Reset();
        // A wheel in the air has no carcass load to lend it inertia.
        WheelShaft.SetAddedInertia(0.f);
        return;
    }

    // The frame is in cm/s; the tyre model is strictly SI.
    const float Vcx = FVector::DotProduct(Frame.ContactVelocityWS, Frame.ForwardWS) * VehicleUnits::CmToM;
    const float Vcy = FVector::DotProduct(Frame.ContactVelocityWS, Frame.LateralWS) * VehicleUnits::CmToM;
    const float Vc  = Frame.ContactVelocityWS.Size() * VehicleUnits::CmToM;
    const float Omega = WheelShaft.GetAngularVelocity();

    // Kept for the bench: these two and PacejkaResult.Kappa are the three terms of
    // 4.E5, and only here are they simultaneous.
    OmegaUsed = Omega;
    VcxUsed = Vcx;

    // ---------- Normal load: the tyre's own vertical spring, 4.E68 ----------
    //
    // Computed HERE rather than handed in, because this is a tyre property, not a
    // suspension one - the suspension's job is to say how far the carcass is
    // squashed (rho_z, pure geometry), and the tyre's is to say what that costs in
    // newtons. Fz then carries camber, pressure and rolling-speed sensitivity for
    // free, none of which a linear rate could express.
    //
    // 4.E68's Fx/Fy terms make it implicit, so it takes the PREVIOUS substep's
    // in-plane forces - the same explicit coupling the rest of this loop already
    // uses, per the function's own note.
    const float PrevFxN = FVector::DotProduct(ContactForceN, Frame.ForwardWS);
    const float PrevFyN = FVector::DotProduct(ContactForceN, Frame.LateralWS);

    const float StaticLoadN = TireParams->ComputeNormalLoad(
        Frame.DeflectionCm * VehicleUnits::CmToM, Frame.CamberRad, Omega, PrevFxN, PrevFyN);

    // Radial damping, which 4.E68 has no term for - see RadialDampingNsPerM.
    // Positive approach speed is the patch closing on the ground, so it ADDS load.
    const float ApproachSpeedMPerSec =
        -FVector::DotProduct(Frame.ContactVelocityWS, Frame.NormalWS) * VehicleUnits::CmToM;

    // Clamped at zero as one sum, not per term: the ground pushes and never pulls,
    // and damping alone must not be able to drag a rebounding tyre back down.
    NormalLoadN = FMath::Max(StaticLoadN + (RadialDampingNsPerM * ApproachSpeedMPerSec), 0.f);

    // ---------- In-plane, at the load just computed ----------
    PacejkaResult = TireParams->ComputeTireForces(
        NormalLoadN, Frame.CamberRad, Vc, Vcy, Vcx, Omega);

    if (bUseTransients)
    {
        // Fx_SS/Fy_SS come back in UU (the model multiplies by 100 on the way
        // out); UpdateTransientState wants newtons, hence the CmToM here. This
        // asymmetry is real - Fx/Fy are pre-converted, Mz is not.
        TransientState = TireParams->UpdateTransientState(
            Vc, Vcx, Vcy, Frame.CamberRad, Omega,
            PacejkaResult.Fx * VehicleUnits::CmToM, PacejkaResult.Fy * VehicleUnits::CmToM,
            NormalLoadN, SubstepTime, TransientState);

        PacejkaResult = TireParams->ComputeTireForcesOverriden(
            NormalLoadN, TransientState.kappa_prime_damped, TransientState.alpha_prime_damped,
            TransientState.gamma_prime, Vc, Vcy, Vcx, Omega);
    }

    KappaSpanMin = FMath::Min(KappaSpanMin, PacejkaResult.Kappa);
    KappaSpanMax = FMath::Max(KappaSpanMax, PacejkaResult.Kappa);

    // Fx and Fy arrive in UU, Mz in N*m. Convert the forces back to SI so the
    // suspension - which is SI throughout - can take them unchanged.
    const float FxN = PacejkaResult.Fx * VehicleUnits::CmToM;
    const float FyN = PacejkaResult.Fy * VehicleUnits::CmToM;

    // The normal load rides the ROAD NORMAL, which is what ISO defines Fz against and
    // what this tyre model's own axis system assumes: Fz normal to the road, Fx and Fy
    // in it. Every Magic Formula coefficient in the asset was fitted in that frame.
    //
    // There was briefly a toggle here to apply it along the wheel plane's radial axis
    // instead, and it is gone rather than left off. The two differ by the camber angle,
    // so the alternative moves sin(camber) of the load out of the road normal - and a
    // fitted Fy already contains the camber contribution, so it double-counts. A knob
    // whose only settings are "correct" and "double-counted" is not an experiment, it
    // is a way to get a wrong answer later.
    ContactForceN = (Frame.NormalWS * NormalLoadN)
        + (Frame.ForwardWS * FxN) + (Frame.LateralWS * FyN);
    AligningMomentNm = PacejkaResult.Mz;

    // Reaction on the wheel's spin: traction at radius opposes the slip.
    //const float RadiusM = TireParams->GetUnloadedRadiusCm() * CmToM;
    //WheelShaft.ApplyImpulse(-FxN * RadiusM * SubstepTime);
    
     // Reaction on the wheel's spin: traction at radius opposes the slip.
    // The shaft's ANGLE, which nothing had been integrating - FShaft carried the
    // member and an UpdateAngle that was commented out, so it sat at zero forever.
    // Integrated here because this is the one place that runs per substep with the
    // spin and the timestep both in hand. Wrapped, or a few minutes of driving puts
    // it past the point where a float has degrees of resolution left.
    WheelShaft.Angle = FMath::Fmod(
        WheelShaft.Angle + (WheelShaft.GetAngularVelocity() * SubstepTime), 2.f * PI);

    const float RadiusM = TireParams->GetUnloadedRadiusCm() * VehicleUnits::CmToM;

    // Applied implicitly. The explicit form evaluates Fx at the spin the wheel had
    // at the TOP of the step and then uses it to change that spin, so a stiff slip
    // curve overshoots, flips sign and overshoots harder. Backward Euler wants Fx at
    // the spin the wheel ENDS with; linearising once about the current point, with
    // C = dFx/dkappa,
    //
    //   dOmega * (1 + G) = -Fx*R0*dt / I,    G = C * (dkappa/dOmega) * R0 * dt / I
    //
    // so the correction is a factor of (1 + G). The error multiplier becomes
    // 1/(1+G) rather than the explicit (1-G): G is positive, so it can never exceed
    // 1, and the wheel approaches monotonically instead of ringing past and
    // diverging above G = 2. The two agree to first order for small G, and G -> 0 as
    // dt -> 0, so this converges to the same physics - it just cannot blow up on the
    // way.
    //
    // APPLIED AS INERTIA, not as a divided impulse. I*(1+G) is an effective inertia:
    // the tyre's resistance to having its slip changed acts as inertia on the wheel
    // for the duration of the step. Writing it into the shaft means the engine
    // torque and the drivetrain constraint solve - both of which run after this,
    // within the same substep - divide by it too.
    //
    // Dividing only the tyre's impulse was wrong, and not subtly. With transients
    // off, dkappa/dOmega goes as 1/|Vx| and the tyre model floors |Vx| at
    // epsilon_v = 0.1 m/s, so G reaches ~726 at a standstill: the tyre was removed
    // from the wheel equation while engine torque arrived in full, the wheels span
    // up freely, kappa ran past the force peak, and the car would not pull away.
    //
    // dkappa/dOmega differs between the two paths, and that difference IS the margin
    // the transient model buys:
    //
    //   transients on   kappa' is relaxed    d/dOmega = R0*dt / (sigma + |Vx|*dt)
    //   transients off  kappa is algebraic   d/dOmega = R0 / |Vx|
    //
    // The ratio is |Vx|dt/(sigma + |Vx|dt), about 1/5 at 50 km/h - which is why the
    // steady-state path ran at G = 5.2 while the transient one sat at 0.98.
    const float GammaUsed = bUseTransients ? TransientState.gamma_prime : Frame.CamberRad;

    // Clamped positive: past the force peak dFx/dkappa goes NEGATIVE, which would put
    // 1+G below one and, at G = -1, divide by zero. Zeroing it there falls back to the
    // plain explicit impulse, safe precisely because the curve is flat there and G is
    // small anyway. The same positive-definite trick implicit cloth solvers use.
    const float SlipStiffnessN = FMath::Max(0.f, TireParams->GetSlipStiffnessN(
        Vc, Vcy, Vcx, Omega, GammaUsed, NormalLoadN, PacejkaResult.Kappa));

    const float dKappa_dOmega = bUseTransients
        ? (RadiusM * SubstepTime) / FMath::Max(
            TransientState.prev_sigma_kappa + (FMath::Abs(Vcx) * SubstepTime), KINDA_SMALL_NUMBER)
        : RadiusM / FMath::Max(FMath::Abs(Vcx), TireParams->epsilon_v);

    // Against the shaft's OWN inertia. GetInvInertia() now carries whatever was
    // lent last substep, and feeding that back in here would compound it.
    const float G = (SlipStiffnessN * dKappa_dOmega * RadiusM * SubstepTime)
        / FMath::Max(WheelShaft.GetInertia(), KINDA_SMALL_NUMBER);

    // Before the impulse, so the tyre's own reaction divides by it as well.
    WheelShaft.SetAddedInertia(G * WheelShaft.GetInertia());

    WheelShaft.ApplyImpulse(-FxN * RadiusM * SubstepTime);

    // Rolling resistance, book (4.E70). A torque on the SHAFT, opposing rotation -
    // not a force at the patch. It reaches the car by slowing the wheel, which
    // shifts slip, which moves Fx; adding a patch drag as well would charge the
    // same resistance twice.
    //
    // FMath::Sign gives exactly zero at a standstill, so a parked wheel is left
    // alone. Near zero it will alternate sign step to step, which the book flags by
    // pointing 4.E70 at Chapter 9 (9.230, 9.231, 9.236) for the standstill-capable
    // form. Not implemented here - if it chatters at walking pace, that is where the
    // answer is, rather than in a fade invented locally.
    WheelShaft.ApplyImpulse(-FMath::Sign(Omega) * FMath::Abs(PacejkaResult.My) * SubstepTime);
}
