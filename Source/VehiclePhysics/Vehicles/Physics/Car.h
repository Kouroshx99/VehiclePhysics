// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehiclePhysics/Vehicles/VehiclePhysicsCompat.h"
#include "Curves/CurveFloat.h"
#include "VehicleEngine.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/BodyInstance.h"
#include "VehiclePhysics/Vehicles/Types/Wheel.h"
#include "Car.generated.h"

class UPacejkaTyreComponent;
class UECU;
class UTorqueConverterComponent;
class UCameraComponent;
class USpringArmComponent;
class UVehicleEngine;
class UDrivetrain;
class UWishboneSuspension;
class STireGraphWidget;

/**
 * One corner: the linkage and the tyre riding on it, kept together.
 *
 * The two only work as a pair - the suspension owns the geometry and hands out a
 * contact frame, the tyre owns every force including the vertical one - so pairing
 * them by matching indices in two separate arrays was a mis-ordering waiting to
 * happen, and one nothing would have reported.
 *
 * Deliberately NOT a pointer from the suspension to the tyre. That would make the
 * suspension depend on a concrete tyre model type, which is the coupling
 * FWheelContactFrame exists to prevent, and it would force a shared tyre-component
 * base class the moment a second model appears. The vehicle owns the wiring; the
 * two components stay ignorant of each other.
 */
USTRUCT()
struct FCornerAssembly
{
	GENERATED_BODY()

	UPROPERTY()
	UWishboneSuspension* Suspension = nullptr;

	UPROPERTY()
	UPacejkaTyreComponent* Tyre = nullptr;
};

/**
 * Post-physics tick for ACar's own debug drawing.
 *
 * Exists for the same reason FWishboneDebugDrawTickFunction does. The actor's
 * normal tick runs at TG_PrePhysics, so anything drawn from it shows the body
 * where it was at the END of the PREVIOUS frame's physics - a whole frame behind
 * what is on screen, which at speed is metres of lag. Drawing from
 * TG_PostUpdateWork instead means physics has already synced the body, so the
 * transform being drawn against is the one being rendered.
 */
USTRUCT()
struct FCarDebugDrawTickFunction : public FTickFunction
{
	GENERATED_USTRUCT_BODY()

	class ACar* Target = nullptr;

	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
		const FGraphEventRef& MyCompletionGraphEvent) override;

	virtual FString DiagnosticMessage() override;
};

template<>
struct TStructOpsTypeTraits<FCarDebugDrawTickFunction> : public TStructOpsTypeTraitsBase2<FCarDebugDrawTickFunction>
{
	enum { WithCopy = false };
};

/**
 * The chassis body's state for ONE solve, read from the physics thread when there is one.
 *
 * Under Chaos every FBodyInstance accessor - GetUnrealWorldTransform_AssumesLocked,
 * GetUnrealWorldVelocity_AssumesLocked, GetCOMPosition - resolves to
 * GetGameThreadAPI(), which is refreshed once per RENDERED FRAME. Inside an async
 * physics tick that means every step of a frame reads the same body state while the
 * body is actually moving, and a suspension damper reading a frozen velocity applies
 * the same correction repeatedly: the system ends up effectively undamped and rings.
 *
 * The _AssumesLocked suffix does not help - both variants call the same function and
 * the suffix only skips taking the read lock. Under UE4/PhysX the substep callback ran
 * on the physics thread so the habit was correct there; under Chaos it is not.
 *
 * So the state is read ONCE per solve, from Chaos's internal particle when the solve is
 * running on the physics thread, and from the game-thread accessors otherwise. One
 * struct, one read, and bFromPhysicsThread says which it was.
 */
struct FCarBodyState
{
	FQuat   Rotation           = FQuat::Identity;
	FVector COMWS              = FVector::ZeroVector;
	FVector LinearVelocityWS   = FVector::ZeroVector;
	FVector AngularVelocityRad = FVector::ZeroVector;

	/** False means the values came from the game thread and may be a frame old. */
	bool bFromPhysicsThread = false;

	/**
	 * False means there was NOTHING SAFE TO READ and the state is untouched defaults.
	 *
	 * Distinct from bFromPhysicsThread on purpose. A game-thread read is merely stale; an
	 * unavailable read is unusable, and the two need different handling - the first can
	 * be worked with, the second must stop the solve.
	 */
	bool bValid = false;
};

UCLASS()
class VEHICLEPHYSICS_API ACar : public APawn
{
	GENERATED_BODY()

public:
	ACar();

	float GetWheelSpeed(EWheel Wheel);
	float GetWheelZPosition(EWheel Wheel);
	FRotator GetWheelRotation(EWheel Wheel);
	FVector GetWheelPosition(EWheel Wheel);
	float GetWheelAngle(EWheel Wheel);

	/** Double-wishbone articulation-angle physics for one corner. */
	UWishboneSuspension* GetCornerSuspension(EWheel Wheel) const;

	// ---------------- Automated test access ----------------
	//
	// A narrow surface for AVehicleTestBench. ApplyDriverInput goes through the
	// SAME Set* path a human uses, deliberately - a bench that bypassed the input
	// path would be measuring a car nobody drives. The rest are reads.

	/** Drives the car exactly as player input does. Steer is -1..1. */
	UFUNCTION(BlueprintCallable, Category = "Vehicle|Test")
	void ApplyDriverInput(float InThrottle, float InBrake, float InSteer);

	FORCEINLINE USkeletalMeshComponent* GetCarMesh() const { return CarMesh; }
	FORCEINLINE UDrivetrain* GetDrivetrain() const { return Drivetrain; }
	FORCEINLINE const TArray<FCornerAssembly>& GetCorners() const { return Corners; }
	FORCEINLINE float GetTrackWidthCm() const { return TrackWidth; }
	FORCEINLINE float GetWheelBaseCm() const { return WheelBase; }
	FORCEINLINE float GetMaxSteerAngleDeg() const { return MaxSteerAngle; }
	FORCEINLINE float GetForwardSpeedKph() const { return ForwardVelocityKMH; }

	FORCEINLINE UWishboneSuspension* GetFrontRightSuspension() const { return FrontRightSuspension; }

	UPROPERTY(EditAnywhere, Category="Car", meta=(AllowPrivateAccess=true))
	FShaft ClutchShaft;
	
	UPROPERTY(EditAnywhere, Category="Car", meta=(AllowPrivateAccess=true))
	FShaft FrontDiffShaft;
	
	UPROPERTY(EditAnywhere, Category="Car", meta=(AllowPrivateAccess=true))
	FShaft RearDiffShaft;

	TSharedPtr<STireGraphWidget> TireGraphWidgetFrontRight;
	TSharedPtr<STireGraphWidget> TireGraphWidgetFrontLeft;
	TSharedPtr<STireGraphWidget> TireGraphWidgetRearRight;
	TSharedPtr<STireGraphWidget> TireGraphWidgetRearLeft;

	float GetEngineRPM() const {return Engine->CurrentEngineRPM;}

	/**
	 * The rpm the limiter actually cuts at - MaxRPM scaled by RedLineRPMRatio.
	 *
	 * Anything choosing a shift point needs THIS rather than an absolute number: a shift
	 * at 6200 rpm is sensible on an engine that revs to 8000 and unreachable on one that
	 * redlines at 4940, which leaves that car stuck in first gear on its limiter.
	 */
	float GetRedlineRPM() const;

	/**
	 * SUBSTEP ACCOUNTING for the frame being simulated. Instrumentation only - nothing
	 * in the physics reads either of these.
	 *
	 * Here because two cars with identical settings were observed integrating at
	 * different rates. The steer slew runs once per substep at a fixed rate, so it
	 * doubles as a tachometer for the substep loop, and it read 10.21 /s on ASaloonCar
	 * against 3.02 /s on ASportsCar with SteeringResponsiveness measured at 10.00 on
	 * every corner of both. Working back from the per-frame decay, the saloon takes
	 * four substeps a frame - exactly a 60 Hz frame under a 200 Hz cap - and the sports
	 * car about one.
	 *
	 * That is inference from a side effect. These two count the thing directly: how
	 * many times AddCustomPhysics called back, and how much simulated time those calls
	 * were handed in total. The sum SHOULD equal the frame's own delta - if it does
	 * not, the body is being integrated over time the vehicle never got to apply
	 * forces across, and every rate this project measures is suspect on that car.
	 */
	FORCEINLINE int32 GetSubstepCountThisFrame() const { return SubstepCountThisFrame; }
	FORCEINLINE float GetSubstepTimeSumThisFrame() const { return SubstepTimeSumThisFrame; }

	/**
	 * TOTAL simulated time handed to the solve since BeginPlay, never reset.
	 *
	 * The per-frame count above was the right measurement for PhysX substepping, where
	 * every callback for a frame arrived inside that frame. An async tick does not align
	 * with frames at all, so a single frame's window catches 2 callbacks or 6 depending
	 * on where it lands, and the per-frame figure reads 50% or 150% while nothing is
	 * wrong. The total against elapsed time has no such window and is the figure that
	 * actually says whether the integrator is keeping up with the clock.
	 */
	FORCEINLINE double GetSubstepTimeTotalSeconds() const { return SubstepTimeTotalSeconds; }
	FORCEINLINE int32 GetGear() const;

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * FREEZE THE SPEED-DEPENDENT STEER LIMIT at whatever it is right now, and keep
	 * everything else about steering exactly as it was.
	 *
	 * SetSteering scales the input by a curve of speed, so a CONSTANT INPUT IS NOT A
	 * CONSTANT ANGLE - the wheels move whenever the car speeds up or slows down. For
	 * driving that is the point. For a measurement it is fatal: the bench's step steer
	 * and power-on tests both hold the input still and assume the wheels are still, and
	 * a car that accelerates through the manoeuvre quietly steers itself.
	 *
	 * The obvious fix - writing angles straight to the corners - is worse, because it
	 * skips the Ackermann split and the max-angle clamp, and then the bench is measuring
	 * a car nobody drives. This freezes ONLY the speed term, so the same setters, the
	 * same Ackermann and the same limits all still apply.
	 *
	 * Call it once the car is at test speed. UnlockSteerLimit puts the curve back.
	 */
	UFUNCTION(BlueprintCallable, Category = "Vehicle|Steering")
	void LockSteerLimitAtCurrentSpeed();

	/** Hand steering back to the speed curve. */
	UFUNCTION(BlueprintCallable, Category = "Vehicle|Steering")
	void UnlockSteerLimit();

	/** Negative means no override and the curve is in charge. */
	FORCEINLINE bool IsSteerLimitLocked() const { return SteerLimitOverride >= 0.f; }

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DrawCOM(FBodyInstance* BodyInstance);

	/**
	 * Draws where the body has BEEN, coloured by how hard it was accelerated
	 * vertically at each point.
	 *
	 * The question this exists to answer is "does the body stay smooth", and that is
	 * not answerable from a still frame - a car being launched and a car riding a
	 * crest look identical at any single instant. A trail makes it a shape you can
	 * look at: a smooth road draws a smooth green line, and anything that kicks the
	 * car leaves a red kink exactly where it happened, still visible after you have
	 * driven past and turned round to look.
	 *
	 * Vertical acceleration rather than height, because height alone cannot tell a
	 * hill from a hit.
	 */
	void DrawBodyMotion(float DeltaSeconds);

	/**
	 * Draws the chassis collision geometry, and a line from the centre of mass to
	 * each corner's mount.
	 *
	 * The suspension debug draws four corners floating in space with nothing between
	 * them - so what is NOT visible is the thing they are all bolted to. This draws
	 * the actual shapes out of the physics asset rather than an approximation of
	 * them, because the useful question is usually whether the collision matches
	 * what you think it is: a hull that is too long catches on kerbs the visual mesh
	 * clears, and nothing else on screen would show you that.
	 */
	void DrawBodyCollision();

	/** Everything drawn from the post-physics tick. */
	void DrawDebugPass(float DeltaSeconds);

	/** The scale UE actually built a bone's physics shapes at - see the .cpp note. */
	FVector ShapeScaleForBone(FName BoneName) const;

	/** One AggGeom at one transform. Shared by every route DrawBodyCollision tries. */
	int32 DrawAggGeom(const struct FKAggregateGeom& Geom, const FTransform& At, const FColor& Colour);

	/** Says once what the collision actually is, so a silent nothing is never the answer. */
	bool bLoggedCollisionSetup = false;

	/** Registers the post-physics debug-draw tick alongside the normal one. */
	virtual void RegisterActorTickFunctions(bool bRegister) override;

	UPROPERTY()
	FCarDebugDrawTickFunction DebugDrawTick;

	/** Trail sample: where the centre of mass was, and the vertical g there. */
	struct FBodyTrailPoint
	{
		FVector PosWS = FVector::ZeroVector;
		float VerticalG = 0.f;
	};
	TArray<FBodyTrailPoint> BodyTrail;
	FVector BodyTrailLastVelocity = FVector::ZeroVector;
	float BodyTrailPeakG = 0.f;
	int32 VelocityKMH;
	FORCEINLINE int32 GetSpeedKMH() const {return VelocityKMH;}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SceneComponents", meta=(AllowPrivateAccess=true))
	USkeletalMeshComponent* CarMesh;

	UPROPERTY(EditAnywhere, Category="SceneComponents")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SceneComponents", meta=(AllowPrivateAccess=true))
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SceneComponents", meta=(AllowPrivateAccess=true))
	UVehicleEngine* Engine;

	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess=true))
	UDrivetrain* Drivetrain;
	
	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess=true))
	UECU* ECU;

	// The suspension. Each corner integrates its own articulation angle from inside
	// this actor's physics callback and pushes its reaction into the body; the
	// animation layer reads the angle back out.
	//
	// Front and rear each ship with their own baked hardpoints (see Car.cpp,
	// BuildRearHardpoints), mirrored per side. The rear set is a designed
	// placeholder rather than a measured rig - retune it before relying on it.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UWishboneSuspension* FrontRightSuspension;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UWishboneSuspension* FrontLeftSuspension;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UWishboneSuspension* RearRightSuspension;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UWishboneSuspension* RearLeftSuspension;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UPacejkaTyreComponent* FrontRightTire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UPacejkaTyreComponent* FrontLeftTire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UPacejkaTyreComponent* RearRightTire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	UPacejkaTyreComponent* RearLeftTire;

	UPROPERTY(EditAnywhere, Category="Car Propperties", meta=(AllowPrivateAccess=true)) //Center of mass offset relative to the default position.
	FVector COMOffset; 

	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	float MaxSteerAngle = 25.f;

	/**
	 * Shapes the STICK, not the car. Thumb travel is raised to this power before it
	 * becomes a steer angle, so early travel is finer while the lock at the end of
	 * the throw is exactly what it was.
	 *
	 * This is an input-resolution problem, not a handling one. The steer angle that
	 * reaches the tyres' limit falls as the SQUARE of speed, so even with SteerCurve's
	 * taper already applied the usable band collapses:
	 *
	 *     140 km/h   lock 12.1 deg   limit 1.21 deg   10.0% of travel
	 *     180 km/h   lock  9.6 deg   limit 0.73 deg    7.6% of travel
	 *
	 * On a wheel 10% of travel is about 45 degrees of rim and easy to sit inside. On a
	 * thumbstick it is 1.5 mm - finer than a thumb holds - so any deliberate input at
	 * speed commands well past the limit, both axles saturate, and the end that lets go
	 * first is whichever has less margin. At 2.2 that same limit sits near 40% of travel.
	 *
	 * 1.0 restores the previous linear mapping exactly.
	 */
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true, ClampMin="1.0", ClampMax="4.0"))
	float SteerInputGamma = 2.2f;

	/**
	 * Speed band over which the gamma fades in, km/h.
	 *
	 * The curve is only wanted where the limit is a sliver of the travel. Parking and
	 * hairpins are the opposite case - the whole 25 degrees is usable there and a curve
	 * only makes the car feel vague - so the mapping stays linear below the onset and
	 * reaches full gamma at the second speed.
	 */
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true, Units="km/h"))
	float SteerGammaOnsetKMH = 40.f;

	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true, Units="km/h"))
	float SteerGammaFullKMH = 120.f;
	
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	float WheelAnimZOffset = 0.f;

	UPROPERTY(EditAnywhere, Category="Car Propperties", meta=(AllowPrivateAccess=true)) //Amount of steering applied versus speed, more speed less steering
	UCurveFloat* SteerCurve;

	/**
	 * Steering limit against speed, carried BY VALUE, used whenever SteerCurve is unset.
	 *
	 * SteerCurve is an asset reference and it was dereferenced without a guard, so a
	 * Blueprint that had not been given one crashed on the first steering input -
	 * EXCEPTION_ACCESS_VIOLATION in SetSteering. Every other pointer in that function
	 * was guarded; this one was the exception, and it only surfaced when a new vehicle
	 * class produced a Blueprint nobody had assigned a curve to.
	 *
	 * A guard alone would have stopped the crash and left the car steering at full lock
	 * at 250 km/h, which is a worse failure for being quiet. So this carries a usable
	 * default instead: full lock to 40 km/h, tapering to 0.30 by 260. An inline curve
	 * serialises by value, so it needs no asset and cannot be forgotten.
	 *
	 * SteerCurve still wins when both are set.
	 */
	UPROPERTY(EditAnywhere, Category="Car Propperties", meta=(AllowPrivateAccess=true))
	FRuntimeFloatCurve InlineSteerCurve; 

	UPROPERTY(EditAnywhere, Category = "Steering", meta=(AllowPrivateAccess=true)) // 100 = full Ackerman, 0 = parallel, -100 = full anti-Ackerman
	float AckermanPercent = 100.0f; 

	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bVisualizeCOM = false;

	/**
	 * Trail of the body's path, coloured green through red by vertical acceleration.
	 *
	 * Read it as a ribbon: straight and green is a body being carried, and every red
	 * kink is the road hitting the car rather than supporting it. It persists, so a
	 * bump can be driven over and then inspected at a standstill.
	 */
	UPROPERTY(EditAnywhere, Category = "Car|Debug")
	bool bDrawBodyMotion = false;

	/** How many seconds of trail to keep. Longer is more history and more lines. */
	UPROPERTY(EditAnywhere, Category = "Car|Debug", meta = (ClampMin = "0.5", ClampMax = "60.0"))
	float BodyTrailSeconds = 8.f;

	/**
	 * Vertical g the trail counts as fully red.
	 *
	 * 1 g is a sensible place to start: it is the point where a wheel can begin to
	 * leave the ground, since the suspension is being asked for as much force again
	 * as the car weighs.
	 */
	UPROPERTY(EditAnywhere, Category = "Car|Debug", meta = (ClampMin = "0.1"))
	// 2 g, not 1. At 1 g a car merely STANDING draws its whole trail fully red, which
	// is the most alarming colour the scale has being used for the least alarming
	// thing - and it puts a large red curve next to the red contact arrows, so the two
	// get confused for each other. 2 g means red is a corner or a landing.
	float BodyTrailRedAtG = 2.f;

	/**
	 * Chassis collision shapes from the physics asset, plus the spokes out to each
	 * corner's mount.
	 *
	 * Independent of bDrawBodyMotion - one shows where the body went, this shows
	 * what the body IS.
	 */
	UPROPERTY(EditAnywhere, Category = "Car|Debug")
	bool bDrawBodyCollision = false;


	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bLogDeltaTime = false;

	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bLogVelocityKMH = false;
	
	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bLogAngVelocity = false;

	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bLogAcceleration = false;

	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bLogWheelSteerAngle = false;

	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bLogCarData = false;
	
	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bLogWheelData = false;
	
	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bDrawGraphs;
	

	void SubstepPhysics(float DeltaTime, FBodyInstance* BodyInstance);

#if VP_HAS_ASYNC_PHYSICS
	/**
	 * The Chaos async physics tick - this is what drives the vehicle now.
	 *
	 * AddCustomPhysics used to. Under Chaos it is a stub: FPhysScene_Chaos's whole
	 * implementation is one line that calls the delegate inline, once, with the frame
	 * delta. Measured against 4.27 the integrator went from 4 callbacks of 4.17 ms to 1
	 * of 16.67 ms - no time lost, but the timestep became the FRAME time, so handling
	 * tracked frame rate. That is a determinism bug, not a tuning one.
	 *
	 * This fires at AsyncFixedTimeStepSize instead, which is fixed and independent of
	 * how long the frame took. Requires bTickPhysicsAsync in Project Settings; without
	 * it the callback still arrives but carries a variable frame delta, which is the
	 * old behaviour wearing a new name.
	 */
	virtual void AsyncPhysicsTickActor(float DeltaTime, float SimTime) override;
#endif

	/**
	 * Read the body once per solve - see FCarBodyState.
	 *
	 * bAllowGameThreadRead MUST be false when called from the async physics tick.
	 * FBodyInstance's accessors all ensure on IsInGameThreadContext(), so reaching for
	 * them off the game thread is not a stale read, it is a handled ensure with a stack
	 * walk attached - and the value it hands back is the spawn pose.
	 */
	FCarBodyState ReadBodyState(FBodyInstance* BodyInstance, bool bAllowGameThreadRead) const;

	/** Steps every wishbone corner and applies its reaction. One substep's worth. */
	void SolveWishboneCorners(float DeltaTime, FBodyInstance* BodyInstance,
		const FCarBodyState& State);

	/**
	 * Hands each corner the wheel travel of the one across its axle, which is what
	 * its anti-roll bar acts on. Call before stepping any of them.
	 */
	void ExchangeAntiRollTravel();
	void DrawDebugHUD();

	/** Pushes each tyre's transient slip state to its on-screen graph. Once per frame. */
	void UpdateTireGraphs();

	/**
	 * The mesh's component transform, rebuilt from the LIVE body state.
	 *
	 * Needed because the wishbone hardpoints are in CarMesh component space, and
	 * inside a substep the component transform is up to a frame stale - only the
	 * body instance has moved. Anchoring to the centre of mass keeps this exact
	 * wherever the physics asset's root bone happens to sit relative to the mesh
	 * origin.
	 */
	FTransform GetSubstepBodyToWorld(const FCarBodyState& State) const;

	FORCEINLINE void SetThrottle(float InputThrottle) {Throttle = InputThrottle;}
	FORCEINLINE void SetBrake(float InputBrake) {Brake = InputBrake;}
	
	FORCEINLINE void GearUp();
	
	FORCEINLINE void GearDown();
	float LastSteer = 0;

	/** Frozen speed-term for steering; negative means the curve is in charge. */
	float SteerLimitOverride = -1.f;

	/** Reset in Tick before AddCustomPhysics, accumulated in SubstepPhysics. */
	int32 SubstepCountThisFrame = 0;
	float SubstepTimeSumThisFrame = 0.f;
	double SubstepTimeTotalSeconds = 0.0;
	float CurrentSteer = 0;
	void SetSteering(float Steering);

	/**
	 * The axis binding's entry point, and the ONLY place SteerInputGamma is applied.
	 *
	 * SetSteering stays the raw interface deliberately. ApplyDriverInput - and so every
	 * bench test - calls straight into it, so this curve cannot move a measured number:
	 * a test that asks for 0.3 still gets 0.3.
	 */
	void SetSteeringFromPlayer(float Steering);
	FORCEINLINE void SetMouseX(float Horizontal)
	{
		MouseXSpeed = Horizontal;
	}
	FORCEINLINE void SetMouseY(float Vertical) {MouseYSpeed = Vertical;}

	FVector DefaultLocation = FVector::ZeroVector;
	FRotator DefaultRotation = FRotator::ZeroRotator;
	float LastVelocity = 0.f;
	float MouseXSpeed = 0.f;
	float MouseYSpeed = 0.f;
	float Throttle = 0.f;
	float Brake = 0.f;
	// cm, distance front to rear axle. Resolved in BeginPlay from the corners.
	float WheelBase = 0.f;
	// cm, distance left to right wheel. Resolved in BeginPlay from the corners.
	float TrackWidth = 0.f; 
	FRotator OGSpringArmRotation = FRotator(0.f, 0.f, 0.f);
	FCalculateCustomPhysics CustomPhysics = FCalculateCustomPhysics();
	float ForwardVelocityKMH = 0.f;
	float Acceleration = 0.f;
	/**
	 * The corners, FR, FL, RR, RL.
	 *
	 * One array of PAIRS rather than two parallel ones. A suspension and its tyre
	 * are meaningless apart: the corner has no vertical spring without the tyre, and
	 * the tyre has no contact frame without the corner. Held as two arrays indexed in
	 * lockstep, building them in different orders silently drives the rear-left
	 * corner with the front-right tyre - no compile error, no crash, just a car that
	 * handles wrong. Constructed together, they cannot be mis-paired.
	 */
	UPROPERTY()
	TArray<FCornerAssembly> Corners;

	/**
	 * Centre of mass in CarMesh component space, cm. Cached in BeginPlay AFTER the
	 * COM nudge, so it already accounts for it. Lets GetSubstepBodyToWorld rebuild
	 * the component transform from the live COM.
	 */
	FVector COMLocalToMesh = FVector::ZeroVector;

	UFUNCTION()
	void ResetCar();

	/**
	 * Steps to the next car in the roster, or the previous one with a negative step.
	 *
	 * The work is not done here. This forwards to the level's AVehicleRoster, because the
	 * swap destroys this actor - a car cannot be the thing that outlives its own
	 * replacement. A level with no roster gets no switching, which is deliberate. All that belongs on the pawn is the binding, which is here because
	 * the pawn is the one place input reliably arrives: the project sets no
	 * PlayerController class in C++, so anything hung off ACarController would depend on
	 * a Blueprint nobody has promised to use.
	 */
	void CycleCar(int32 Delta);

	void NextCar()     { CycleCar(1); }
	void PreviousCar() { CycleCar(-1); }
};