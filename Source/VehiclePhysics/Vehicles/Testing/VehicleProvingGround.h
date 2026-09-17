// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VehicleProvingGround.generated.h"

class UStaticMesh;
class UMaterialInterface;

/**
 * A drivable test facility, built from primitives at the press of a button.
 *
 * The measurement bench (AVehicleTestBench) tells you what the car did in
 * numbers. This is the other half: ground to do it ON, and to feel it on. Every
 * section here exists because some specific question is hard to answer without
 * it - you cannot judge damping without a repeating input, or an anti-roll bar
 * without lifting one corner and not the other.
 *
 * Lay it out with Build, which spawns real actors you can then select, nudge and
 * save into the level like anything else. Clear removes what it made. Nothing is
 * hidden or procedural at runtime unless you ask for it.
 *
 * Everything is placed RELATIVE to this actor, so drop it wherever there is room
 * and point its +X down the direction you want to drive.
 */
UCLASS()
class VEHICLEPHYSICS_API AVehicleProvingGround : public AActor
{
	GENERATED_BODY()

public:
	AVehicleProvingGround();

	/** Spawns the enabled sections. Clears anything a previous build left first. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Proving Ground")
	void Build();

	/** Removes everything Build made, leaving the rest of the level alone. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Proving Ground")
	void Clear();

	/** Build automatically at play. Off if you built once and saved the actors. */
	UPROPERTY(EditAnywhere, Category = "Proving Ground")
	bool bBuildOnBeginPlay = false;

	// ---------------- Rally: what a sealed surface cannot ask ----------------
	//
	// The road sections all keep four wheels down and the contact patch flat. These
	// deliberately do not, because three things only appear once that stops being
	// true: what the suspension does at full travel, what the tyre model does with
	// no load at all, and what happens on the way back down.

	/**
	 * Whoops - big, widely spaced humps across the full width.
	 *
	 * Not the washboard with larger numbers. The washboard is small and close, and
	 * asks about damping while the tyres stay on the ground; these are large and far
	 * apart, and ask what happens when they do not. Expect the car to go light over
	 * the crests and land between them, which is the only section that exercises the
	 * airborne path - relaxation state reset, re-acquiring load, and whether the
	 * suspension bottoms on the way back down.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally")
	bool bWhoops = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "2"))
	int32 WhoopCount = 6;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "200.0", Units = "cm"))
	float WhoopSpacingCm = 500.f;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "20.0", Units = "cm"))
	float WhoopDiameterCm = 70.f;

	/**
	 * A ramp that launches the car clean off the ground.
	 *
	 * The one section where every wheel leaves at once. Landing is the interesting
	 * half: four tyres re-acquiring load simultaneously is the hardest thing a
	 * contact model does, and a car that lands badly usually reveals it here first.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally")
	bool bJumpRamp = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "300.0", Units = "cm"))
	float JumpRampLengthCm = 900.f;

	/** Shallow on purpose. Past about 15 degrees this stops being a jump and becomes a wall. */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "2.0", ClampMax = "25.0"))
	float JumpRampAngleDeg = 10.f;

	/**
	 * A long side slope to drive ALONG rather than across.
	 *
	 * Gravity gets a permanent lateral component here, so the tyres carry a steady
	 * side load with no steering input at all. It separates two things a flat corner
	 * always confuses: grip that comes from slip angle, and grip that comes from
	 * load. A car that slides down this one is telling you about mu, not balance.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally")
	bool bOffCamber = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "500.0", Units = "cm"))
	float OffCamberLengthCm = 2500.f;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "2.0", ClampMax = "30.0"))
	float OffCamberAngleDeg = 12.f;

	/**
	 * Scattered rocks at mixed heights and angles.
	 *
	 * Every other section presents a surface whose normal is level or tilted one
	 * known way. This one does not, which is the point: it is the only place the
	 * contact normal changes under each wheel independently and unpredictably, and
	 * a car that wanders off line here is reporting something about how the normal
	 * is being found rather than about its suspension.
	 *
	 * Placed from a seeded stream, so a rebuild lays out the identical field and two
	 * runs over it are comparable.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally")
	bool bRockField = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "1"))
	int32 RockCount = 40;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "500.0", Units = "cm"))
	float RockFieldLengthCm = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally", meta = (ClampMin = "2.0", Units = "cm"))
	float RockMaxHeightCm = 18.f;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Rally")
	int32 RockFieldSeed = 1337;

	// ---------------- Ride: repeating input ----------------

	/**
	 * A washboard - evenly spaced cylinders across the full width.
	 *
	 * The one section that tests DAMPING rather than springs. A single bump only
	 * shows the first compression; a repeating one at driving speed sweeps the
	 * suspension through its natural frequency, and an under-damped car will visibly
	 * build up until it skips. Drive it at a range of speeds, not one.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Washboard")
	bool bWashboard = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Washboard", meta = (ClampMin = "2"))
	int32 WashboardCount = 12;

	/** Centre-to-centre, cm. With diameter, this sets the frequency at a given speed. */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Washboard", meta = (ClampMin = "50", Units = "cm"))
	float WashboardSpacingCm = 200.f;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Washboard", meta = (ClampMin = "2", Units = "cm"))
	float WashboardDiameterCm = 16.f;

	// ---------------- Ride: isolated inputs ----------------

	/**
	 * Three single bumps of increasing size, well separated.
	 *
	 * Separated deliberately: the car must return to rest between them, so each is
	 * a clean step input and you can watch one compression and one rebound without
	 * the next bump arriving mid-recovery.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Single Bumps")
	bool bSingleBumps = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Single Bumps", meta = (ClampMin = "2", Units = "cm"))
	float SingleBumpSmallestDiameterCm = 10.f;

	// ---------------- Articulation ----------------

	/**
	 * Bumps on the LEFT HALF only, so one side rides and the other does not.
	 *
	 * This is the anti-roll bar test. A bar does nothing when both wheels move
	 * together, so no amount of driving over symmetric bumps will show it; you have
	 * to break the symmetry. Run it with the bars fitted and again at zero.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Articulation")
	bool bSplitBumps = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Articulation", meta = (ClampMin = "2", Units = "cm"))
	float SplitBumpDiameterCm = 24.f;

	/**
	 * An RTI ramp - a long incline you drive ONE side up until a wheel lifts.
	 *
	 * The off-road industry's articulation measure. Distance travelled up the ramp
	 * before the diagonal wheel leaves the ground, divided by wheelbase, x1000.
	 * Stock 4x4s score 400-550; disconnecting the bars is worth hundreds of points,
	 * which is exactly the "dramatic difference" worth reproducing.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Articulation")
	bool bRtiRamp = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Articulation", meta = (ClampMin = "5", ClampMax = "35"))
	float RtiRampAngleDeg = 20.f;

	// ---------------- Handling ----------------

	/**
	 * A marked circle. Drive it at a steady speed and hold the line.
	 *
	 * Radius is the point: with a known radius, speed alone gives lateral
	 * acceleration (v^2/r), so the circle turns a speedometer into a g-meter. Two
	 * rings so you can see whether the car is running wide.
	 */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Skidpad")
	bool bSkidpad = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Skidpad", meta = (ClampMin = "500", Units = "cm"))
	float SkidpadRadiusCm = 2500.f;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Skidpad", meta = (ClampMin = "8"))
	int32 SkidpadMarkerCount = 48;

	/** Cones in a line. Transient response you can feel rather than plot. */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Slalom")
	bool bSlalom = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Slalom", meta = (ClampMin = "2"))
	int32 SlalomCount = 8;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Slalom", meta = (ClampMin = "500", Units = "cm"))
	float SlalomSpacingCm = 1800.f;

	// ---------------- Straight line ----------------

	/** Gates every 10 m down the straight, so braking distance is readable by eye. */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Straight")
	bool bDistanceMarkers = true;

	UPROPERTY(EditAnywhere, Category = "Proving Ground|Straight", meta = (ClampMin = "2"))
	int32 DistanceMarkerCount = 20;

	/** A kerb to put two wheels over - asymmetric, sharp, and unforgiving. */
	UPROPERTY(EditAnywhere, Category = "Proving Ground|Straight")
	bool bKerb = true;

protected:
	virtual void BeginPlay() override;

private:
	/** Everything Build spawned, so Clear can undo exactly that and no more. */
	UPROPERTY()
	TArray<AActor*> Spawned;

	/** SizeCm is the desired size in the MESH's own axes; rotation applies on top. */
	AActor* SpawnPiece(UStaticMesh* Mesh, const FVector& LocalLocation, const FRotator& LocalRotation,
		const FVector& SizeCm, const FLinearColor& Colour, bool bBlocking = true);

	/** A readable sign beside a section: what it is, and what to drive. */
	void SpawnSign(float ForwardCm, const FString& Title, const FString& Instruction);

	/** A cylinder lying across the path, i.e. a bump. Width in cm, or 0 for full width. */
	void SpawnBump(float ForwardCm, float LateralCm, float DiameterCm, float WidthCm, const FLinearColor& Colour);

	void BuildWashboard(float& ForwardCursorCm);
	void BuildSingleBumps(float& ForwardCursorCm);
	void BuildSplitBumps(float& ForwardCursorCm);
	void BuildKerb(float& ForwardCursorCm);
	void BuildRtiRamp(float& ForwardCursorCm);
	void BuildWhoops(float& ForwardCursorCm);
	void BuildJumpRamp(float& ForwardCursorCm);
	void BuildOffCamber(float& ForwardCursorCm);
	void BuildRockField(float& ForwardCursorCm);
	void BuildSkidpad(float ForwardCm);
	void BuildSlalom(float& ForwardCursorCm);
	void BuildDistanceMarkers();

	UStaticMesh* CubeMesh = nullptr;
	UStaticMesh* CylinderMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
	UMaterialInterface* BaseMaterial = nullptr;
	bool LoadAssets();
};
