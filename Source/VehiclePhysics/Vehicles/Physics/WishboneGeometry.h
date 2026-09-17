// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Closed-form double-wishbone kinematics, shared by the physics integrator
 * and by anything that needs to draw or follow the same linkage.
 *
 * Both layers MUST route every "where is this hardpoint at angle theta"
 * question through here. That is the whole point of this file: the physics
 * layer integrates theta and the animation layer draws theta, so if they ever
 * disagreed about the geometry the wheel would render somewhere other than
 * where it was simulated, and nothing would tell you.
 *
 * Space and units: everything here is skeletal-mesh COMPONENT space, cm.
 * Angles are DEGREES where they name an articulation angle (matching
 * FVector::RotateAngleAxis and the animation properties), and RADIANS where
 * they appear as a derivative (the tangents), because that is what the
 * generalized-coordinate physics wants.
 */
namespace WishboneGeometry
{
	/** Hinge direction of a wishbone, front bush -> rear bush, normalized. */
	FORCEINLINE FVector HingeAxis(const FVector& F, const FVector& B)
	{
		return (B - F).GetSafeNormal(); // Doc (W.1)
	}

	/**
	 * Where a point rigidly attached to a wishbone ends up once the arm has
	 * articulated by AngleDeg about its hinge.
	 *
	 * The arm's only real degree of freedom is this rotation, so every point on
	 * it - the outer ball joint, the damper mount, the wheel centre - travels a
	 * circular arc, not a vertical line.
	 *
	 * Axis must already be normalized (RotateAngleAxis does not normalize).
	 */
	FORCEINLINE FVector OrbitPoint(const FVector& F, const FVector& RestPoint, const FVector& Axis, float AngleDeg)
	{
		return F + (RestPoint - F).RotateAngleAxis(AngleDeg, Axis); // Doc (W.7)
	}

	/**
	 * d(OrbitPoint)/d(theta) with theta in RADIANS, i.e. cm per radian.
	 *
	 * Two things fall out of this one cross product, and the physics layer uses
	 * both:
	 *   - direction: the instantaneous direction the point travels, which is
	 *     what force vectors get projected onto to become generalized forces.
	 *   - magnitude: the perpendicular distance from the hinge axis, which is
	 *     the moment arm. So the effective inertia is just Mass * |Tangent|^2,
	 *     with no separate arm-length bookkeeping to keep in sync.
	 *
	 * Pass the LIVE point (from OrbitPoint), not the rest point.
	 */
	FORCEINLINE FVector OrbitTangent(const FVector& F, const FVector& LivePoint, const FVector& Axis)
	{
		return FVector::CrossProduct(Axis, LivePoint - F); // Doc (W.8)
	}

	/**
	 * Solves the UPPER arm's articulation angle from the lower arm's.
	 *
	 * The upper arm's angle is not a free choice and is NOT equal to the lower
	 * arm's: the knuckle is a rigid link between the two ball joints, so the
	 * upper joint must land exactly KingpinLengthCm from the lower one. A shorter
	 * upper arm therefore has to swing through a larger angle to keep up - which
	 * is what produces camber gain. Assuming the two angles are equal is only
	 * exact for equal-length parallel arms, and otherwise stretches the knuckle.
	 *
	 * Geometrically this is a circle-sphere intersection: the upper ball joint is
	 * confined to a circle (its arc about the upper hinge) and to a sphere
	 * (KingpinLengthCm about the live lower ball joint). Reduces to
	 * A*cos(phi) + B*sin(phi) = K, solvable in closed form.
	 *
	 * Two roots exist - one per side of the arc. GuessAngleDeg selects between
	 * them; pass the lower arm's angle, which is the correct branch for any sane
	 * rig and degrades exactly to the parallelogram case when the arms match.
	 *
	 * Returns false when the constraint is unreachable (the arms cannot span the
	 * kingpin at this angle), leaving OutAngleDeg untouched.
	 */
	FORCEINLINE bool SolveUpperArmAngle(const FVector& UCA_F, const FVector& UCA_U_Rest, const FVector& Axis,
	                                    const FVector& LCA_U_Live, float KingpinLengthCm,
	                                    float GuessAngleDeg, float& OutAngleDeg)
	{
		// Decompose the rest arm into along-axis and perpendicular parts. Only the
		// perpendicular part sweeps; the along-axis part fixes the circle's plane.
		const FVector RestArm = UCA_U_Rest - UCA_F;
		const float AlongAxis = FVector::DotProduct(Axis, RestArm);
		const FVector Perpendicular = RestArm - Axis * AlongAxis;
		const float SweepRadius = Perpendicular.Size(); // Doc (W.10): a, p, r_U

		if (SweepRadius < KINDA_SMALL_NUMBER)
		{
			return false;
		}

		// E1/E2 are chosen so that phi matches OrbitPoint's angle convention
		// exactly: at phi = 0 we are at the rest point, and d/dphi = Axis x arm.
		const FVector E1 = Perpendicular / SweepRadius;
		const FVector E2 = FVector::CrossProduct(Axis, E1);
		const FVector CircleCentre = UCA_F + Axis * AlongAxis;

		const FVector ToCentre = CircleCentre - LCA_U_Live; // Doc (W.11): e1, e2, C, d
		// A*cos(phi) + B*sin(phi) = K - Doc (W.12), the constraint |U-L| = l_kp.
		const float A = 2.f * SweepRadius * FVector::DotProduct(ToCentre, E1);
		const float B = 2.f * SweepRadius * FVector::DotProduct(ToCentre, E2);
		const float K = (KingpinLengthCm * KingpinLengthCm)
			- ToCentre.SizeSquared() - (SweepRadius * SweepRadius); // Doc (W.13)

		const float Magnitude = FMath::Sqrt(A * A + B * B);
		if (Magnitude < KINDA_SMALL_NUMBER || FMath::Abs(K) > Magnitude)
		{
			return false;
		}

		// Doc (W.14): phi = atan2(B,A) +/- acos(K / |A,B|), unwound below.
		const float Phase = FMath::Atan2(B, A);
		const float Offset = FMath::Acos(FMath::Clamp(K / Magnitude, -1.f, 1.f));
		const float Guess = FMath::DegreesToRadians(GuessAngleDeg);

		// Unwind each root onto the revolution nearest the guess before comparing,
		// so the branch choice cannot jump by a full turn mid-travel.
		const float RootA = Guess + FMath::UnwindRadians((Phase + Offset) - Guess);
		const float RootB = Guess + FMath::UnwindRadians((Phase - Offset) - Guess);

		const float Chosen = FMath::Abs(RootA - Guess) < FMath::Abs(RootB - Guess) ? RootA : RootB;
		OutAngleDeg = FMath::RadiansToDegrees(Chosen);
		return true;
	}

	/**
	 * How much the knuckle (upright) has rotated, as the rotation that carries
	 * the rest kingpin direction onto the live one.
	 *
	 * The knuckle is NOT rigid to either arm - it hangs between the two ball
	 * joints, and both joints rotate. Its orientation therefore follows the
	 * kingpin line, not the lower arm's hinge angle.
	 *
	 * This is the same swing rotation the animation layer applies to the knuckle
	 * master bone, deliberately: the physics has to agree with what is drawn.
	 * The animation layer additionally applies a tie-rod twist correction for
	 * steer, which is not modelled here yet - once steering is live, that twist
	 * belongs in this function too.
	 */
	FORCEINLINE FQuat KnuckleRotation(const FVector& LCA_U_Rest, const FVector& UCA_U_Rest,
	                                  const FVector& LCA_U_Live, const FVector& UCA_U_Live)
	{
		const FVector RestKingpin = (UCA_U_Rest - LCA_U_Rest).GetSafeNormal();
		const FVector LiveKingpin = (UCA_U_Live - LCA_U_Live).GetSafeNormal(); // Doc (W.16): k(theta)

		if (RestKingpin.IsNearlyZero() || LiveKingpin.IsNearlyZero())
		{
			return FQuat::Identity;
		}

		return FQuat::FindBetweenNormals(RestKingpin, LiveKingpin); // Doc (W.17), swing term
	}

	/**
	 * Full knuckle orientation: the suspension's swing (bump/droop) followed by
	 * the steer rotation about the LIVE steering axis.
	 *
	 * Steering about the real, tilted kingpin rather than about vertical is what
	 * produces the coupling between steer and camber. With caster, turning the
	 * wheel tips it - the outside wheel gains negative camber and the inside wheel
	 * gains positive - and with KPI it also jacks the corner vertically. Both fall
	 * out of this one rotation; neither needs a special-case formula.
	 *
	 * Order matters: the suspension places the knuckle first, then the knuckle
	 * turns about the steering axis it now has.
	 *
	 * SteerDeg is positive counter-clockwise seen from above (steering left), and
	 * the axis points upward on both sides, so the sign convention is the same for
	 * left and right corners.
	 */
	FORCEINLINE FQuat KnuckleOrientation(const FVector& LCA_U_Rest, const FVector& UCA_U_Rest,
	                                     const FVector& LCA_U_Live, const FVector& UCA_U_Live,
	                                     float SteerDeg)
	{
		const FQuat Swing = KnuckleRotation(LCA_U_Rest, UCA_U_Rest, LCA_U_Live, UCA_U_Live);

		if (FMath::IsNearlyZero(SteerDeg))
		{
			return Swing;
		}

		const FVector KingpinLive = (UCA_U_Live - LCA_U_Live).GetSafeNormal();
		if (KingpinLive.IsNearlyZero())
		{
			return Swing;
		}

		return FQuat(KingpinLive, FMath::DegreesToRadians(SteerDeg)) * Swing; // Doc (W.17), full q(theta)
	}

	/**
	 * Where a point rigidly attached to the KNUCKLE ends up - the wheel centre,
	 * the hub, the spindle.
	 *
	 * Note the difference from OrbitPoint: a point on the lower arm orbits the
	 * arm's hinge, but a point on the knuckle translates with the lower ball
	 * joint and rotates only by the knuckle's own rotation. Treating the wheel as
	 * welded to the lower arm swings it about the arm's hinge instead, which
	 * drags it inboard under bump and outboard under droop - the wheel's position
	 * relative to the kingpin must not change at all.
	 */
	FORCEINLINE FVector KnucklePoint(const FVector& RestPoint,
	                                 const FVector& LCA_U_Rest, const FVector& UCA_U_Rest,
	                                 const FVector& LCA_U_Live, const FVector& UCA_U_Live,
	                                 float SteerDeg = 0.f)
	{
		// The steering axis passes through the lower ball joint, so rotating the
		// offset about it is the whole steer motion - which is also why the wheel
		// centre sweeps sideways as it turns, giving scrub and KPI jacking.
		const FQuat Rotation = KnuckleOrientation(LCA_U_Rest, UCA_U_Rest, LCA_U_Live, UCA_U_Live, SteerDeg);
		return LCA_U_Live + Rotation.RotateVector(RestPoint - LCA_U_Rest); // Doc (W.18)
	}

	/** Where a disc meets a plane, and how far past it the disc reaches. */
	struct FDiscContact
	{
		/** Penetration of the lowest rim point past the plane, cm. Negative = clear of it. */
		float DeflectionCm = 0.f;

		/** That rim point brought back onto the plane - the contact patch. */
		FVector PointWS = FVector::ZeroVector;
	};

	/**
	 * Contact between the tyre - a DISC of radius R about the spindle, not a
	 * sphere - and the ground plane.
	 *
	 * WORLD space, unlike everything above it in this file, because the ground is
	 * not in the body's frame.
	 *
	 * A sphere's lowest point is always R below its centre, straight down the
	 * ground normal. A disc's is only R below when its axle is perpendicular to
	 * that normal; tilt either one - dial in camber, or park a level wheel on a
	 * slope - and the lowest rim point swings sideways WITHIN the wheel's own
	 * plane while reaching less far down.
	 *
	 * Minimising (Centre + R*u - PlanePoint).Normal over unit u perpendicular to
	 * Spindle gives u = -normalize(Normal perpendicular to Spindle), so the reach
	 * down the normal is R*sqrt(1 - (Normal.Spindle)^2). Verified against
	 * brute-force rim sampling: agrees to float precision at every orientation,
	 * and reduces exactly to the sphere formula when Normal.Spindle is zero, so
	 * flat ground at zero camber is bit-for-bit unchanged.
	 */
	FORCEINLINE FDiscContact SolveDiscContact(const FVector& CentreWS, const FVector& SpindleWS, float RadiusCm,
	                                          const FVector& PlanePointWS, const FVector& PlaneNormalWS)
	{
		const float SpindleAlongNormal = FVector::DotProduct(SpindleWS, PlaneNormalWS);

		// How much of the radius actually reaches down the normal: 1 with the wheel
		// upright against the surface, 0 with it lying flat (rim level with the hub).
		const float ReachFraction = FMath::Sqrt(
			FMath::Max(1.f - (SpindleAlongNormal * SpindleAlongNormal), 0.f)); // eta, Doc (W.25)

		const float CentreHeightCm = FVector::DotProduct(CentreWS - PlanePointWS, PlaneNormalWS); // h_g, Doc (W.25)

		FDiscContact Result;
		Result.DeflectionCm = (RadiusCm * ReachFraction) - CentreHeightCm; // rho_z, Doc (W.25)

		// The normal's component inside the wheel's own plane - "up" as the wheel
		// sees it. A full radius the other way lands on the lowest point of the rim.
		// Degenerate only when the wheel lies flat, where GetSafeNormal returns zero
		// and the patch collapses onto the hub - the right answer for a
		// zero-thickness disc, and it cannot produce a NaN.
		const FVector NormalInWheelPlane = PlaneNormalWS - (SpindleWS * SpindleAlongNormal);
		const FVector LowestRimWS = CentreWS - (NormalInWheelPlane.GetSafeNormal() * RadiusCm); // Doc (W.26)

		// That rim point sits DeflectionCm below the plane by construction, so this
		// lifts it exactly onto it.
		Result.PointWS = LowestRimWS + (PlaneNormalWS * Result.DeflectionCm); // C, Doc (W.26)
		return Result;
	}
}
