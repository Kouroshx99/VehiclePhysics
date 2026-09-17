// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WishbonePose.generated.h"

/**
 * One double-wishbone corner's linkage, fully resolved at a given articulation
 * angle and steer, in WORLD space, cm.
 *
 * Exists so anything that needs to see the linkage - the editor visualizer, an
 * animation layer, a rig-check tool - can get it without reaching into
 * UWishboneSuspension's private kinematics, and without being able to disagree
 * with the physics about where the arms are. Same reason FWheelContactFrame sits
 * between the suspension and the tyre model.
 *
 * A pose is a QUERY, not state: it is computed on demand for whatever angle you
 * ask for and never stored on the component, so asking for a pose the corner is
 * not currently at (a sweep preview, the rest pose for comparison) costs nothing
 * and disturbs nothing.
 */
USTRUCT(BlueprintType)
struct VEHICLEPHYSICS_API FWishbonePose
{
	GENERATED_BODY()

	/** False when the corner has no resolved geometry yet, or has no body to hang off. */
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	bool bValid = false;

	/**
	 * True when the rigid-knuckle constraint could not be satisfied at this angle -
	 * the two arcs cannot span the kingpin, so the linkage would have to come
	 * apart. The upper arm falls back to the parallelogram angle to stay
	 * continuous, which means the drawn kingpin will be the WRONG LENGTH. Worth
	 * surfacing rather than drawing a lie: it means the travel limits are wider
	 * than the geometry allows.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	bool bUpperArmSolveFailed = false;

	// ---------- Chassis-fixed: these never move with theta ----------
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector LcaFront = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector LcaRear = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector UcaFront = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector UcaRear = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector CoiloverTop = FVector::ZeroVector;

	// ---------- Moving with theta ----------
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector LowerBallJoint = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector UpperBallJoint = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector DamperMount = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector WheelCentre = FVector::ZeroVector;

	// ---------- Directions, unit ----------
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector LowerHingeAxis = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector UpperHingeAxis = FVector::ZeroVector;

	/** Wheel spin axis, pointing outboard. Carries camber, toe and steer. */
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector SpindleAxis = FVector::ZeroVector;

	/** Orthonormal pair spanning the wheel plane, for drawing the tyre circle. */
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector WheelPlaneU = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	FVector WheelPlaneV = FVector::ZeroVector;

	/** Loaded tyre radius, cm - carried so a consumer needs nothing else to draw the wheel. */
	UPROPERTY(BlueprintReadOnly, Category = "Wishbone Pose")
	float WheelRadiusCm = 0.f;
};

/**
 * The debug palette, one hue per SUBSYSTEM.
 *
 * Declared here rather than at either draw site because there are now two of
 * them - the runtime line-batcher draw and the editor component visualizer - and
 * a corner drawn one colour in the viewport and another in play would be worse
 * than no colour coding at all. These are also the exact RGB values
 * Docs/WishboneSuspension.md uses, so the page and the screen agree too.
 */
namespace WishbonePalette
{
	const FColor Chassis(66, 133, 224);   // fixed bushes and hinge axes
	const FColor Arm(230, 129, 38);       // both arms and their ball joints
	const FColor Kingpin(196, 64, 172);   // the knuckle line, L to U
	const FColor Coilover(222, 181, 40);  // T, D and the strut
	const FColor Wheel(43, 179, 108);     // W, the hub arm, the spindle
	const FColor Contact(219, 62, 52);    // the live contact patch only

	/**
	 * Same hue, darker - "this is a reference, not the live thing". UE's debug
	 * line batcher ignores Color.A entirely, so brightness and line weight have to
	 * carry what transparency normally would.
	 */
	FORCEINLINE FColor Dim(const FColor& C, float Factor)
	{
		return FColor(uint8(C.R * Factor), uint8(C.G * Factor), uint8(C.B * Factor));
	}
}
