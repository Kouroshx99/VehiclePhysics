// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheelContactFrame.h"
#include "WishbonePose.h"
#include "Components/ActorComponent.h"
#include "WishboneGeometry.h"
#include "WishboneSuspension.generated.h"

class UWishboneSuspension;
struct FBodyInstance;

/**
 * Secondary tick that draws the corner's debug geometry, at TG_PostUpdateWork.
 *
 * Late on purpose: every line has to be placed after physics has synced the body,
 * or the whole rig renders one chassis step behind it - invisible parked, tens of
 * centimetres adrift at speed. The component's own tick runs at TG_PostPhysics,
 * which is also post-sync, so the ground samples it gathers line up with what is
 * drawn here.
 */
USTRUCT()
struct FWishboneDebugDrawTickFunction : public FTickFunction
{
	GENERATED_USTRUCT_BODY()

	UWishboneSuspension* Target = nullptr;

	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
		const FGraphEventRef& MyCompletionGraphEvent) override;

	virtual FString DiagnosticMessage() override;
};

template<>
struct TStructOpsTypeTraits<FWishboneDebugDrawTickFunction> : public TStructOpsTypeTraitsBase2<FWishboneDebugDrawTickFunction>
{
	enum { WithCopy = false };
};

/**
 * Rest positions of one corner's hardpoints, in the body's local space, cm.
 *
 * These are plain numbers - nothing is read from a mesh, a skeleton, or bones,
 * so the body can be a static mesh, a skeletal mesh, or no mesh at all.
 *
 * The defaults are a measured double-wishbone front corner, meant as a working
 * baseline to tune from rather than a description of any particular vehicle.
 * Note the baseline is deliberately degenerate in two ways, so that a fresh
 * setup behaves predictably before it is tuned:
 *  - The kingpin is exactly vertical (LCA_U and UCA_U share X and Y), so caster
 *    and kingpin inclination are both zero.
 *  - Both arms sweep the same radius, which makes the upper arm track the lower
 *    one exactly and produces zero camber gain. Give them different lengths and
 *    real camber, caster and KPI gain all appear.
 */
USTRUCT(BlueprintType)
struct VEHICLEPHYSICS_API FWishboneHardpoints
{
	GENERATED_BODY()

	/** Lower arm, front chassis bush. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector LCA_F = FVector(143.246658f, 57.374817f, 4.712109f);

	/** Lower arm, rear chassis bush. With LCA_F this defines the hinge axis. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector LCA_B = FVector(114.543495f, 57.408588f, 4.699930f);

	/** Lower arm, outer ball joint. Carries the knuckle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector LCA_U = FVector(132.274857f, 99.871216f, 4.239235f);

	/** Upper arm, front chassis bush. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector UCA_F = FVector(143.245117f, 57.373707f, 38.508877f);

	/** Upper arm, rear chassis bush. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector UCA_B = FVector(114.541969f, 57.407295f, 38.496670f);

	/** Upper arm, outer ball joint. With LCA_U this is the steering axis. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector UCA_U = FVector(132.274857f, 99.871216f, 38.034309f);

	/** Where the coilover picks up on the lower arm. Rigid to that arm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector DamperMount = FVector(128.070442f, 85.290191f, 8.898479f);

	/** Chassis end of the coilover. Fixed to the body. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector CoiloverTop = FVector(126.280006f, 62.784325f, 57.652046f);

	/**
	 * Wheel centre. Rigid to the KNUCKLE, so it translates with the lower ball
	 * joint and rotates with the kingpin - it does not orbit the lower arm.
	 *
	 * Its distance outboard of the kingpin line sets both track width and scrub
	 * radius, so it is usually the first number to set for a new vehicle.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hardpoints")
	FVector WheelCentre = FVector(132.274857f, 106.000000f, 21.136772f);

	/** Mirror across the vehicle centreline (negate Y). Use for left corners. */
	void MirrorAcrossCentreline();
};

/**
 * How the ground under one wheel is sampled before it is reduced to a plane.
 *
 * More points cost more traces but produce a steadier plane. See
 * UWishboneSuspension::UpdateGroundPlane for what is done with them.
 */
UENUM(BlueprintType)
enum class EWishboneGroundSamplePattern : uint8
{
	/** One trace at the wheel centre. The original behaviour; keep for A/B. */
	Single UMETA(DisplayName = "Single Point"),

	/**
	 * Centre plus arms fore/aft and left/right - no corners. Five traces at the
	 * default counts, and enough of a 2D spread to fit a plane.
	 */
	Cross UMETA(DisplayName = "Cross"),

	/**
	 * Full Longitudinal x Lateral grid, corners included. Nine traces at 3x3.
	 * The corners are what a cross cannot see: a ridge running diagonally
	 * across the patch registers on them and on nothing else.
	 */
	Grid UMETA(DisplayName = "Grid")
};

/**
 * One corner's tune - everything a setup sheet lists, in one struct.
 *
 * Exists so a vehicle can be defined in C++ rather than only in a Blueprint. The
 * rate properties are protected deliberately: they are read every substep and
 * nothing should poke at them casually. A preset vehicle still has to set them,
 * and one struct applied in one call is a far smaller hole in that wall than
 * making ten properties public.
 */
/**
 * HOW AN ACCUMULATED REACTION REACHES THE BODY.
 *
 * SEAM. The two engines genuinely differ here, and it is not a rename.
 *
 * 4.27 runs the vehicle inside a PhysX substep callback, where a force added is held for
 * that whole substep. The accumulator is built as J = sum(F * dt), so dividing by the
 * window hands back exactly the force that delivers J over it, and any corner time left
 * in the accumulator carries into the next substep rather than being lost. Doc (W.42).
 *
 * Chaos cannot be driven that way. Its external force accumulator is NOT cleared per
 * step, so N AddForce calls within one frame deliver roughly N times the intended
 * impulse - measured at six times over on a spring that should settle at 0.1334 m and
 * settled at 0.8165 m. bAllowSubstepping does not help: the engine does not divide the
 * force for you. An impulse carries no integration window to get wrong, so N calls
 * deliver N correct partial impulses.
 *
 * On 5.x there is a second reason to come through here. FBodyInstance::AddImpulse is
 * game-thread API; called from an async physics tick it trips Chaos's
 * IsInGameThreadContext() ensure, which dumps a stack every time and lands the impulse
 * on the game-thread copy of the particle rather than the one the solver is about to
 * integrate. FChaosEngineInterface's writers take a bIsInternal flag routing them to
 * GetPhysicsThreadAPI() instead.
 *
 * So callers state what they accumulated and over how long, and each engine delivers it
 * the only way that engine can. Units are the same either way: ImpulseUU is UU*s =
 * kg*cm/s, and AngularImpulseUU is kg*cm^2/s about the centre of mass.
 */
namespace VehiclePhysicsBody
{
	VEHICLEPHYSICS_API void ApplyLinearReaction(FBodyInstance* BodyInstance,
		const FVector& ImpulseUU, float WindowSeconds);
	VEHICLEPHYSICS_API void ApplyAngularReaction(FBodyInstance* BodyInstance,
		const FVector& AngularImpulseUU, float WindowSeconds);
}

USTRUCT(BlueprintType)
struct VEHICLEPHYSICS_API FWishboneCornerSetup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Setup")
	float SpringRateNPerM = 62000.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float DamperRateNsPerM = 5500.f;

	/** At the wheel, in the roll mode - see UWishboneSuspension::AntiRollRateNPerM. */
	UPROPERTY(EditAnywhere, Category = "Setup")
	float AntiRollRateNPerM = 0.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float PreloadForceN = 0.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float StaticCamberDeg = -1.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float StaticToeDeg = 0.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float UnsprungMassKg = 40.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float WheelRadiusCm = 33.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MinArticulationAngleDeg = -12.f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float MaxArticulationAngleDeg = 12.f;
};

/**
 * Physics for one double-wishbone corner, integrated in the suspension's own
 * generalized coordinate.
 *
 * Every equation this corner solves is written out and numbered in
 * Docs/WishboneSuspension.md, with a symbol table. The `// Doc (W.n)` markers
 * through this class and WishboneGeometry.h cite it, so a number in a comment
 * and a number on that page always mean the same thing. What follows here is
 * what you need to USE the component; the page has the derivations.
 *
 * THE MODEL
 *
 * The corner has exactly one degree of freedom: the lower arm's articulation
 * angle theta about its hinge axis (front bush -> rear bush). Every other point
 * follows from theta, so theta and its rate are the entire state.
 *
 * Forces are built as honest 3D vectors and then projected onto the tangent of
 * their own point of application - d(point)/d(theta) - to become generalized
 * forces, exactly like a pendulum. Two consequences worth knowing:
 *  - Gravity and tyre load act at the wheel centre; spring and damper act at the
 *    damper mount. Projecting the coilover forces onto the wheel's tangent
 *    instead would silently discard the motion ratio.
 *  - Effective inertia is Mass * |tangent|^2, which is just m*r^2 - the tangent's
 *    magnitude IS the moment arm, so no arm length is tracked separately.
 *
 * Deliberately NOT built on physics constraints: stiff spring-like constraints
 * are a well-known jitter source in iterative solvers, which is why most
 * commercial vehicle physics computes suspension force analytically instead.
 *
 * SPACES AND UNITS
 *
 * Hardpoints and all geometry are in the body's local space, cm. Forces and the
 * integrator work in SI (N, m, kg, rad). Lengths are compared in local space and
 * directions are taken in world space, which assumes the body is not scaled.
 *
 * THE REACTION ON THE CHASSIS
 *
 * Taking the unsprung assembly as a free body - a point mass m_u at the wheel
 * centre, the arms massless - linear and angular momentum balance pin the
 * reaction down completely, with nothing left to choose. Both the force AND the
 * moment come out reproduced exactly by two point forces:
 *
 *   - N, the tyre load, at the CONTACT PATCH
 *   - (G - m_u*a_wc) at the WHEEL CENTRE
 *
 * Derived in Docs/WishboneSuspension.md, (W.36)-(W.39). The integrator jumps
 * straight to that result, so grepping for W.36/W.37 finds no line computing
 * them. Verified against an exactly-solved determinate linkage to 1e-16.
 *
 * Two things that will bite if you do not know them:
 *
 *  - The coilover force is ABSENT from that wrench and must stay absent. It
 *    genuinely pushes up on the strut tower, but the arms pull down on their
 *    chassis bushes by exactly that reaction - same chassis - so it cancels.
 *    CoiloverForceOnChassisN publishes it as a diagnostic; adding it as well
 *    double-counts the entire spring.
 *  - The chassis body must carry the SPRUNG mass. At rest a_wc is zero, so
 *    equilibrium gives N = (M_body + m_u)*g. Give the body the whole vehicle
 *    mass and every corner over-loads by m_u*g - about 9.5% on a typical corner.
 *
 * Roll centres, jacking and anti-dive/anti-squat need no special-casing; they
 * fall out of applying N at a contact patch that moves with theta.
 *
 * WHAT IS SIMPLIFIED
 *
 *  - a_wc is measured RELATIVE to the body, so the transport terms (the body's
 *    own acceleration, alpha x r, centrifugal, Coriolis) are missing. They are
 *    identically zero at rest and grow with hard chassis motion; a_com cannot be
 *    included without a coupled solve, since it depends on the force being
 *    computed. UWheelCollider::IntegrateUnsprungMass makes the same trade.
 *  - The travel stops kill theta' outright, and the reaction is taken BEFORE
 *    that clamp on purpose: differencing across an instantaneous velocity kill
 *    manufactures an unbounded, timestep-dependent a_wc. The real bump-stop
 *    impulse is therefore dropped - a progressive bump rubber is the proper fix.
 *  - The ground is a plane fitted to a small grid of traces, refreshed once per
 *    frame and reused across the substeps. A plane, however well fitted, still
 *    cannot represent a kerb edge under one side of the tyre.
 *  - The tyre is not modelled here AT ALL, in either direction. This corner
 *    reports how far the carcass is squashed and applies whatever ground force it
 *    is handed; a tyre component owns the rest. So a corner with nothing feeding
 *    SetTyreContactForceN carries no load and settles to full droop.
 */
UCLASS(ClassGroup = (VehiclePhysics), meta = (BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UWishboneSuspension : public UActorComponent
{
	GENERATED_BODY()

public:
	UWishboneSuspension();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** The corner's live articulation angle, degrees - its single DOF. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetArticulationAngleDeg() const { return ArticulationAngleDeg; }

	/** Rate of change of the articulation angle, degrees/sec. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetArticulationRateDegPerSec() const { return FMath::RadiansToDegrees(ThetaDotRad); }

	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE bool IsInContact() const { return bInContact; }

	/**
	 * Tyre radial deflection at the contact patch, cm. This is Pacejka's rho_z -
	 * the input a real tyre model needs to produce Fz (MF 6.1.2 eqn 4.E68), and
	 * the quantity a brush model needs for contact patch length.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetTyreDeflectionCm() const { return TyreDeflectionCm; }

	/**
	 * Unloaded wheel radius, cm - what the contact geometry is solved against.
	 *
	 * The SAME physical quantity as the tyre asset's R0, held here as well because
	 * the corner has to place the contact patch with no tyre attached at all, in the
	 * editor. Whoever pairs the two is responsible for checking they agree: this one
	 * measures the deflection and R0 prices it, so a disagreement is a systematic
	 * error in both Fz and slip ratio, and a silent one. ACar checks at BeginPlay.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetWheelRadiusCm() const { return WheelRadiusCm; }

	/**
	 * The sphere the swept path actually probes with, cm.
	 *
	 * Extracted so the sampler and the debug draw cannot disagree about it, which
	 * they did: the draw put a 1.2 cm dot where the sampler had swung a 33 cm
	 * sphere. That picture says "lots of small probes across the contact patch",
	 * which is the exact opposite of what a full-radius sweep does, and it is a
	 * misreading the drawing itself caused.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetSweepRadiusCm() const
	{
		return FMath::Max(WheelRadiusCm * FMath::Clamp(SweptContactRadiusScale, 0.05f, 1.f), 1.f);
	}

	/** Live wheel-centre position in world space, cm. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FVector GetWheelCentreWorld() const;

	/** Ground normal from this frame's trace, world space. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE FVector GetGroundNormalWorld() const { return GroundNormalWS; }

	/**
	 * Re-trace the ground every SUBSTEP instead of once per frame.
	 *
	 * The ground pass used to run once per frame and be reused by every substep, to
	 * spend arithmetic instead of scene queries. That trade is wrong at speed and it
	 * is worth being precise about how wrong: at 200 km/h a 60 Hz frame is 0.93 m of
	 * travel, so the last substep of the frame was solving the wheel against ground
	 * sampled the better part of a metre behind it. On a ring, where the surface is
	 * curving away underneath, that is not a small error - it is the difference
	 * between a plane that is under the wheel and one that is not.
	 *
	 * The saving was never large either. A Single pattern at 200 Hz is four traces
	 * per corner per second per wheel - 800 for the car; even a nine-sample Grid is
	 * 7200, which is nothing next to what a physics scene does anyway.
	 *
	 * Off restores the old per-frame behaviour for comparison.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground")
	bool bSampleGroundPerSubstep = true;

	/**
	 * Cast the ground probes down the WHEEL's plane instead of the body's.
	 *
	 * A cambered wheel does not touch the ground under its hub - it touches at the
	 * lowest point of a tilted disc, which the contact solver has always known and
	 * the sampler did not. Left on the body axis the whole sampling window sits off
	 * to one side of the patch it is measuring, by R*sin(camber): 1.7 cm at 3
	 * degrees, 2.9 at 5, 5.7 at 10 under roll. Against a pattern 10 cm wide that
	 * displaces the window by half its own span.
	 *
	 * Identical to the body axis at zero camber, so the only setups this changes are
	 * the ones it was wrong for. Off restores the previous behaviour for A/B, which
	 * is worth doing once before trusting a bench number against an older one.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground")
	bool bSampleInWheelPlane = true;

	/**
	 * Sweep a sphere for the ground instead of casting a line.
	 *
	 * Two things a line cannot do. It ENVELOPS: the sphere meets a kerb or a stone
	 * with its lower surface some way before the hub is over it and comes to rest on
	 * it, so the ground rises smoothly under the wheel instead of jumping the instant
	 * the hub crosses the edge. And it RECOVERS: a wheel that has got inside the
	 * geometry is handed a minimum translation out, where a downward line from in
	 * there finds nothing above it and the corner is stranded with no load at all.
	 *
	 * The sphere is swept with its BOTTOM starting at the hub rather than its centre,
	 * and that distinction is the entire feature. Centred on the hub, a wheel-radius
	 * sphere is already touching the road before the query begins, so it never
	 * travels and returns exactly what a line would have. That was the behaviour
	 * until the raised start was added, and it is why this appeared to do nothing.
	 *
	 * Where the sphere's answer is not floor-like - it has caught a wall or a kerb
	 * face with its side - the sample falls back to the line rather than being thrown
	 * away. Discarding it was a real defect: one lateral hit left the corner with no
	 * ground at all, and the wheel fell through debris it was standing beside. With
	 * the fallback this can only add information, never remove it.
	 *
	 * Separate from GroundSamplePattern, which envelops the other way - several
	 * samples, with GroundEnvelopingBias resting the plane on the highest. The two
	 * compose: the sphere rounds each sample, the pattern spans between them.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground")
	bool bUseSweptContact = false;

	/**
	 * Sweep radius as a fraction of the wheel radius.
	 *
	 * 1.0 is the honest answer - the sphere is then the tyre, and Hit.Location is
	 * exactly where the wheel centre would rest, so the effective ground height
	 * falls straight out with no correction.
	 *
	 * At 1.0 a Grid or Cross pattern is largely wasted: the sphere already spans the
	 * whole contact patch, so the samples overlap and report near-identical points.
	 * Use Single with a full-radius sweep, or reduce this if you want the pattern to
	 * keep saying something.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground",
		meta = (EditCondition = "bUseSweptContact", ClampMin = "0.05", ClampMax = "1.0"))
	float SweptContactRadiusScale = 1.f;

	/**
	 * The whole ground force on the tyre for the NEXT substep, world N - normal
	 * load and in-plane together.
	 *
	 * THE CORNER CANNOT HOLD THE CAR UP WITHOUT THIS. There is no vertical spring
	 * left in here to fall back on, so a corner nobody feeds settles to full droop.
	 * Pair every UWishboneSuspension that carries load with a tyre component; ACar
	 * checks the pairing at BeginPlay and complains if it is missing.
	 *
	 * One vector rather than a load plus a separate in-plane part, because the
	 * chassis wrench never needed the split: it comes from momentum balance on the
	 * unsprung mass, which only sees the total ground force. It reaches the chassis
	 * through that one wrench and drives theta through one Q_Contact - which is
	 * what produces anti-dive, anti-squat and lateral jacking, none of it
	 * special-cased. It is just a contact force that no longer points straight up.
	 *
	 * Lags by one substep. The loop is genuinely circular - the tyre needs the
	 * deflection, which needs theta, which needs the force - so an explicit handoff
	 * is the cheap resolution, consistent with the rest of the corner. At 1 ms it
	 * is far below any tyre relaxation length.
	 */
	UFUNCTION(BlueprintCallable, Category = "Suspension")
	void SetTyreContactForceN(const FVector& ForceN) { ExternalContactForceN = ForceN; }

	/**
	 * Wheel spin angle for the debug draw, radians.
	 *
	 * Pushed in rather than read out for the same reason the contact force is: the
	 * shaft belongs to UPacejkaTyreComponent, the drawing belongs here, and ACar is
	 * the only thing that holds both. Debug-only - nothing in the solve reads it.
	 */
	void SetWheelSpinAngleRad(float AngleRad) { DebugWheelSpinRad = AngleRad; }

	/**
	 * Hands this corner the wheel travel of the one on the other end of its
	 * anti-roll bar. Push it every substep, before SolveSubstep, from whoever owns
	 * the axle pairing. Leave it alone and the corner behaves as if the bar were
	 * disconnected on this side.
	 *
	 * The corner cannot find its own partner: pairing is left-right on one AXLE,
	 * which is knowledge the vehicle has and a single corner does not. Same
	 * division of labour as SetTyreContactForceN.
	 *
	 * Like every cross-component handoff here this is one substep old, and for the
	 * same reason - the two corners are mutually coupled, so somebody has to go
	 * first. Harmless at 1 ms against a bar whose whole job is a low-frequency
	 * load transfer.
	 */
	UFUNCTION(BlueprintCallable, Category = "Suspension")
	void SetAntiRollPartnerTravelCm(float PartnerTravelCm) { AntiRollPartnerTravelCm = PartnerTravelCm; }

	/**
	 * Wheel-centre rise above its rest position in BODY space, cm. Positive is
	 * bump. This is the anti-roll bar's input - read it off both corners of an
	 * axle and hand each the other's.
	 *
	 * Body-space height rather than theta, because theta is not comparable across
	 * an axle: the two corners are mirrored, so the same physical bump turns theta
	 * opposite ways. Both wheels rise in bump on both sides, so height needs no
	 * mirror handling at all.
	 *
	 * Measured from the DESIGN position, not static ride height, and it does not
	 * matter which: the bar only ever sees the difference between two corners, and
	 * any offset the two share cancels out of it.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetWheelTravelCm() const { return WheelTravelCm; }

	/** Anti-roll rate, N/m at the wheel. Zero means no bar. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetAntiRollRateNPerM() const { return AntiRollRateNPerM; }

	/**
	 * Retunes the bar while running. Exists for AVehicleTestBench, which measures
	 * the same manoeuvre with the bars connected and disconnected - the only
	 * honest way to attribute a handling change to the bar rather than to two
	 * runs that differed in some other way too.
	 */
	UFUNCTION(BlueprintCallable, Category = "Suspension")
	void SetAntiRollRateNPerM(float RateNPerM) { AntiRollRateNPerM = FMath::Max(RateNPerM, 0.f); }

	/**
	 * Applies a whole corner tune at once, then re-resolves the rest geometry.
	 *
	 * The re-resolve is not optional: static camber and toe are baked into the rest
	 * spindle axis and the radius sets where the contact patch sits, so writing them
	 * without rebuilding leaves the readouts describing the previous setup.
	 */
	UFUNCTION(BlueprintCallable, Category = "Suspension")
	void ApplySetup(const FWishboneCornerSetup& Setup);

	// ---------------- Measurement surface ----------------
	//
	// Readouts the test bench needs. All of them already exist as Live properties;
	// these just make them reachable from C++ as well as Blueprint.

	/** Vertical tyre load carried last substep, N. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetContactForceN() const { return ContactForceN; }

	/** Live camber, degrees. Positive = top leans outboard. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetCamberDeg() const { return LiveCamberDeg; }

	/** Strut travel per unit wheel travel at the current angle. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetMotionRatio() const { return MotionRatio; }

	/**
	 * Live ROAD-WHEEL steer angle, degrees - what the tyre actually points at,
	 * after the speed-dependent limit and the Ackermann split.
	 *
	 * The bench needs this rather than the driver's input: ACar scales steer by
	 * SteerCurve(speed), so holding one input through a speed ramp does not hold
	 * one wheel angle, and an understeer gradient computed from the input would
	 * be measuring the steer curve instead of the vehicle.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetSteerAngleDeg() const { return SteerAngleDeg; }

	/** Anti-roll force at the wheel centre along body up, N. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetAntiRollForceN() const { return AntiRollForceN; }

	FORCEINLINE float GetSpringRateNPerM() const { return SpringRateNPerM; }
	FORCEINLINE float GetDamperRateNsPerM() const { return DamperRateNsPerM; }
	FORCEINLINE float GetUnsprungMassKg() const { return UnsprungMassKg; }
	FORCEINLINE float GetStaticToeDeg() const { return StaticToeDeg; }
	FORCEINLINE bool IsMirrored() const { return bMirrorAcrossCentreline; }

	/**
	 * Wheel-centre rise above rest at an ARBITRARY angle, cm, steer taken as zero.
	 * Lets the bench sweep the corner's travel without moving it. Doc (W.28a).
	 */
	FORCEINLINE float GetWheelTravelAtAngleCm(float AngleDeg) const { return WheelTravelAtAngle(AngleDeg); }

	/**
	 * Records whether this corner is mirrored. Call from a constructor -
	 * Hardpoints is not trustworthy yet at that point (rear corners still hold
	 * the front defaults, and Blueprint overrides land after the constructor
	 * returns), so the actual placement is deferred to InitialiseGeometry at
	 * BeginPlay.
	 */
	void SetMirrorAcrossCentreline(bool bInMirrorAcrossCentreline);

	// ---------------- Driving this from the owner's physics substep ----------------
	//
	// THE ONLY WAY THIS CORNER INTEGRATES. Its own tick does the once-per-frame work
	// - one ground-trace pass and the steer slew - and then returns; whoever owns
	// the body must call SolveSubstep from inside that body's physics callback.
	//
	// Not a preference. Stepping from the tick would run at frame rate against a
	// body transform up to a frame stale, and the reaction would land a frame after
	// the state that produced it. Driven from the substep, the transform is live and
	// force and body motion advance together.
	//
	// The practical consequence: a UWishboneSuspension dropped on an actor with no
	// owner calling SolveSubstep does nothing at all. It has neither an integrator of
	// its own nor - since the vertical spring moved to the tyre - any load to carry.
	// ACar is the reference implementation of what an owner has to do.

	/**
	 * Advances this corner across one of the owner's physics substeps and
	 * accumulates the wrench it applies back to the chassis.
	 *
	 * The corner keeps its own fixed step internally, so this subdivides
	 * SubstepTime rather than integrating it in one go. Pair with
	 * ApplyReactionToBody.
	 *
	 * BodyToWorld is one snapshot for the whole call, not resampled per internal
	 * step - this call IS one AddCustomPhysics invocation for one engine substep,
	 * and PhysX does not advance the body until every callback for that substep
	 * has returned and its queued forces are applied, so the body cannot have
	 * moved between one internal step and the next regardless of how often it is
	 * re-queried. What keeps this fresh is the CALLER re-sampling once per call,
	 * which is once per actual engine substep - as fresh as the engine's own
	 * MaxSubstepDeltaTime allows; a coarser project setting there, not a stale
	 * snapshot here, is what widens the gap between the corner's world placement
	 * and the car's true position at speed.
	 *
	 * @param SubstepTime  the owner's substep, seconds.
	 * @param BodyToWorld  body transform for THIS substep - not the component's.
	 * @param BodyCOMWS    centre of mass, world cm. The torque reference.
	 */
	void SolveSubstep(float SubstepTime, const FTransform& BodyToWorld, const FVector& BodyCOMWS);

	/** Applies, then clears, the wrench SolveSubstep accumulated. Doc (W.42). */
	void ApplyReactionToBody(FBodyInstance* BodyInstance);
	
	/**
	 * The contact patch as a tyre model sees it. Only meaningful after this
	 * frame's UpdateGroundPlane and the substep that followed it.
	 *
	 * Takes the body's motion as parameters rather than reading it back off the
	 * component, so it can be called from inside a physics substep where the
	 * component transform is a frame stale.
	 */
	FWheelContactFrame GetContactFrame(const FTransform& BodyToWorld, const FVector& BodyVelocityWS,
		const FVector& BodyAngularVelocityRad, const FVector& BodyCOMWS) const;

	/** Net force this corner puts into the chassis, N. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE FVector GetChassisReactionForceN() const { return ChassisReactionForceN; }

	/** Coilover load at the strut tower, N. Diagnostic - see CoiloverForceOnChassisN. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetCoiloverForceOnChassisN() const { return CoiloverForceOnChassisMagN; }

	/** Live contact patch, world cm. Where the tyre load is applied. */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE FVector GetContactPointWorld() const { return ContactPointWS; }

	/** Draws the debug geometry. Called from the post-physics tick. */
	void DrawDebugPass();

	/**
	 * The whole linkage resolved at an ARBITRARY angle and steer, world space.
	 *
	 * Does not touch state, so it is safe to ask for a pose the corner is not at -
	 * a sweep preview, or the rest pose to compare against. The editor visualizer
	 * is built entirely on this, which is what stops the viewport and the physics
	 * from ever disagreeing about where the arms are.
	 *
	 * @param AngleDeg  articulation angle, degrees. NOT clamped to the travel
	 *                  limits: a rig-check wants to see past them.
	 * @param SteerDeg  steer about the live kingpin, degrees. Positive = right.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FWishbonePose GetPose(float AngleDeg, float SteerDeg) const;

	// Signed travel limits, degrees. Which sign is bump is derived from the
	// geometry, not configured, so these are not symmetric about zero and Min is
	// not necessarily droop - see TravelFractionToAngleDeg.
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetMinArticulationAngleDeg() const { return MinArticulationAngleDeg; }

	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE float GetMaxArticulationAngleDeg() const { return MaxArticulationAngleDeg; }

	/**
	 * Maps -1 (full droop) .. 0 (rest) .. +1 (full bump) onto a signed angle.
	 *
	 * Normalized rather than raw degrees because which SIGN of theta lifts the
	 * wheel mirrors between sides of the car. A raw-degrees preview slider would
	 * compress the right corner and extend the left, which is exactly the
	 * mirror-image confusion bPositiveThetaIsBump exists to prevent.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	float TravelFractionToAngleDeg(float Fraction) const;

	// ---------------- Geometry ----------------
	//
	// Public (rather than behind a setter) so a vehicle's constructor can assign
	// distinct per-corner values directly - e.g. ACar gives its rear corners their
	// own Hardpoints rather than reusing the front corner's.

	UPROPERTY(EditAnywhere, Category = "Suspension|Geometry")
	FWishboneHardpoints Hardpoints;
	
	
	/**
	 * Target steer angle at this corner, degrees. Positive turns the wheel RIGHT -
	 * the same sense UWheelCollider::SetTargetSteerAngleDeg uses, so one value
	 * feeds both.
	 *
	 * A TARGET, not the live angle: SteerAngleDeg slews toward it in TickComponent.
	 * Applied raw it would jump the wheel plane, and with it Frame.ForwardWS, which
	 * spikes the slip angle the tyre model derives from it.
	 */
	void SetTargetSteerAngleDeg(float InTargetSteerAngleDeg) { TargetSteerAngleDeg = InTargetSteerAngleDeg; }
	
	/**
	 * Wheel centre at rest in body space, cm, AFTER the mirror has been applied.
	 *
	 * Not interchangeable with Hardpoints.WheelCentre: that is the raw authored
	 * value, and mirroring is done to a local copy in InitialiseGeometry, so the
	 * raw hardpoints read IDENTICAL on both sides of the car. Measuring track
	 * width off them yields zero.
	 */
	UFUNCTION(BlueprintPure, Category = "Suspension")
	FORCEINLINE FVector GetWheelCentreRest() const { return WheelCentre_Rest; }

protected:
	virtual void BeginPlay() override;

	/**
	 * Resolves the rest geometry outside Play too.
	 *
	 * InitialiseGeometry runs from BeginPlay, which never fires in the editor - so
	 * without this every *_Rest member, every hinge axis and every alignment
	 * readout would sit at zero in the details panel and the viewport preview would
	 * draw a corner collapsed on the origin.
	 */
	virtual void OnRegister() override;

#if WITH_EDITOR
	/**
	 * Re-resolves after a hardpoint or alignment edit, so the readouts and the
	 * viewport track the change immediately rather than at the next Play.
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/** Registers the post-physics debug-draw tick alongside the normal one. */
	virtual void RegisterComponentTickFunctions(bool bRegister) override;

	UPROPERTY()
	FWishboneDebugDrawTickFunction DebugDrawTick;

	/**
	 * Mirror the hardpoints across the vehicle centreline, for left-hand corners,
	 * so one baked set serves both sides.
	 *
	 * Mirroring is isometric, so every rigid length survives it, and the
	 * bump/droop sign is re-derived from the mirrored geometry - nothing needs
	 * flipping by hand.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Geometry")
	bool bMirrorAcrossCentreline = false;

	// ---------------- Mass, spring, damper ----------------

	/**
	 * Unsprung mass: wheel, tyre, hub, brake and roughly half the arms. Modelled
	 * as a point mass at the wheel centre.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Mass", meta = (ClampMin = "1.0", Units = "kg"))
	float UnsprungMassKg = 40.f;

	/** Spring rate along the coilover axis. 45 N/mm is a firm road car. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Spring", meta = (ClampMin = "0.0"))
	float SpringRateNPerM = 45000.f;

	/**
	 * The coil's length on the car at rest, already squeezed by the preload - what
	 * a tape measure across the coil would read, cm.
	 *
	 * The coil is shorter than the strut: perches, damper eyes and collars make up
	 * the difference, and that difference becomes SpringMountingOffsetCm. Leave
	 * this at zero and the coil is taken to span the whole strut.
	 *
	 * Together with SpringRateNPerM and PreloadForceN this fully specifies the
	 * spring - SpringFreeLengthCm is then derived, not typed.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Spring", meta = (ClampMin = "0.0", Units = "cm"))
	float SpringInstalledLengthCm = 0.f;

	/**
	 * Preload - how hard the perch is wound up against the coil at rest, N.
	 *
	 * Sets how much the coil is already squeezed before the car does anything:
	 * PreloadForceN / SpringRateNPerM. That deflection is what separates the
	 * coil's installed length from its free length, so preload lengthens the
	 * spring's reach rather than adding a constant push.
	 *
	 * The distinction matters. Added as a bare force it would survive the coil
	 * going slack and keep shoving at full droop and in mid-air forever - set it
	 * near the corner weight and the corner could never unload. As part of the
	 * free length it decays honestly to zero once the coil reaches that length and
	 * leaves its perch, exactly like the real thing.
	 *
	 * It costs droop travel: the coil goes slack that much sooner. Wind in more
	 * than the corner has in droop and the wheel gets a dead zone at full
	 * extension - real behaviour, and why too much preload makes an inside wheel
	 * go light.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Spring", meta = (ClampMin = "0.0"))
	float PreloadForceN = 0.f;

	/** Damper rate along the coilover axis. Single rate - no bump/rebound split. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Damper", meta = (ClampMin = "0.0"))
	float DamperRateNsPerM = 3500.f;

	// ---------------- Anti-roll bar ----------------

	/**
	 * Anti-roll rate, N/m AT THE WHEEL, in the roll mode - the extra vertical force
	 * this corner takes per metre it moves while its partner moves the same amount
	 * the other way. Zero disconnects the bar. 15-30 kN/m is a typical road-car
	 * front bar.
	 *
	 * At the wheel rather than at the bar, because a bar's own torsional rate in
	 * N*m/rad only becomes a wheel rate once you have applied the droplink ratio
	 * and the motion ratio squared, and that is arithmetic the setup sheet has
	 * already done.
	 *
	 * Defined so it means exactly what a spring's wheel rate means, and so the two
	 * add the way you would expect:
	 *
	 *     K_roll = k * track^2 / 2      for a bar AND for the coilover
	 *
	 * with the coilover's k being SpringRateNPerM * MotionRatio^2. So a 20 kN/m bar
	 * against a 45 kN/m spring at MR 0.63 (17.9 kN/m at the wheel) adds 112% of the
	 * springs' roll stiffness - and that ratio, per axle, is the roll-stiffness
	 * distribution that sets understeer balance.
	 *
	 * Set both corners of an axle to the same value. They are two ends of ONE bar,
	 * and unequal rates would have it push harder on one side than the other -
	 * Newton's third law violated by a typo. ACar checks this at BeginPlay.
	 *
	 * Deliberately not gated on ground contact. The bar does not know or care: it
	 * keeps twisting when a wheel lifts, and that push is exactly what drops the
	 * inside wheel and unloads it further.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Anti-roll bar", meta = (ClampMin = "0.0"))
	float AntiRollRateNPerM = 0.f;

	/**
	 * Bump-rubber rate at the wheel, N/m. Zero restores the bare clamp.
	 *
	 * The travel stops were purely kinematic - clamp theta at the limit, zero the
	 * inbound rate - which ABSORBS the motion rather than resisting it. A corner
	 * driven past full bump then hands the chassis nothing, so a car pulling several
	 * g through a loop bottoms out and simply keeps going until the body hits the
	 * track, with the suspension geometry doing none of the stopping. Doc (W.33)
	 * called progressive rubbers a later refinement; this is it.
	 *
	 * Stiff is the point. Once a rubber is crushed the load path is nearly rigid and
	 * the tyre load goes almost straight into the chassis, which is exactly what has
	 * to happen for the car to be held off the ground. An order of magnitude above
	 * the main spring is the usual starting point.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Springs", meta = (ClampMin = "0.0", Units = "N"))
	float BumpStopRateNPerM = 250000.f;

	/**
	 * Damping in the rubber, N*s/m, and one-way: it resists being crushed and does
	 * nothing on the way back out.
	 *
	 * Rubber dissipates as much as it stores. Without this a stiff stop turns a
	 * heavy landing into a bounce straight back off the rubber, and a two-way damper
	 * here would hold the suspension down into its own stop.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Springs", meta = (ClampMin = "0.0"))
	float BumpStopDampingNsPerM = 4000.f;

	/**
	 * How far before the hard limit the rubber first touches, degrees of travel.
	 *
	 * What makes it progressive rather than a wall. At zero the stop engages exactly
	 * at the limit, which is stable and feels like concrete, because it is: the force
	 * goes from nothing to everything inside one substep.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Springs", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float BumpStopEngageDeg = 2.f;

	/** Travel past the point where the rubber doubles its rate, m. Sets how progressive it is. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Springs", meta = (ClampMin = "0.005", Units = "m"))
	float BumpStopProgressiveM = 0.03f;

	/** Live bump-stop force, N. Zero whenever the rubber is not touching. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float BumpStopForceN = 0.f;

	// ---------------- Tyre and alignment ----------------

	/** Loaded tyre radius, cm. Sets where the contact patch sits below the hub. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Tyre", meta = (ClampMin = "1.0", Units = "cm"))
	float WheelRadiusCm = 33.f;

	/**
	 * Static camber, degrees. Positive = top of the wheel leans OUTBOARD.
	 *
	 * A property of the knuckle casting, not something the ball joints dictate. A
	 * real upright is L-shaped: its upper arm reaches inboard to meet a tilted
	 * steering axis while the spindle stays level, which is how a car runs 8 deg
	 * of KPI at zero camber. Deriving the spindle perpendicular to the kingpin
	 * would force camber to track KPI one-for-one, so this is an independent
	 * input and IncludedAngleDeg (= KPI + camber) reports the knuckle's own angle.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Alignment")
	float StaticCamberDeg = 0.f;

	/** Static toe, degrees. Positive = toe-IN (wheel's front points inboard). */
	UPROPERTY(EditAnywhere, Category = "Suspension|Alignment")
	float StaticToeDeg = 0.f;

	/**
	 * Steer angle at this corner, degrees. Positive follows Unreal's yaw sense -
	 * about the upward kingpin, so positive turns the wheel to the RIGHT.
	 *
	 * Applied as a real rotation about the live, tilted steering axis rather than
	 * about vertical, so steer/camber coupling is exact rather than a correction
	 * curve: caster contributes an antisymmetric camber change, KPI a symmetric
	 * one, plus the vertical jacking KPI produces. None of it is special-cased.
	 *
	 * Nothing drives this yet - it is a manual input, separate from ACar's
	 * Ackermann steering.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension|Alignment")
	float SteerAngleDeg = 0.f;

	// ---------------- Ground contact ----------------
	//
	// This corner owns NO vertical tyre spring. It reports how far the carcass is
	// squashed - TyreDeflectionCm, which is pure geometry - and a tyre model turns
	// that into newtons and hands the whole ground force back through
	// SetTyreContactForceN.
	//
	// The linear rate and damping that used to live here were a placeholder for
	// exactly that, and UPacejkaTyreComponent now does it properly via 4.E68, which
	// a linear rate cannot match: load there carries camber, inflation pressure and
	// rolling-speed sensitivity. Keeping a second vertical spring would have meant
	// two models disagreeing about the same tyre.

	/** Trace channel for drivable ground. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Tyre Vertical")
	TEnumAsByte<ECollisionChannel> GroundTraceChannel = ECC_Visibility;

	// ---------------- Ground sampling ----------------
	//
	// A tyre reads the road over its contact patch, not at a point - Pacejka's
	// EFFECTIVE ROAD PLANE (3rd ed. ch. 10). Several traces are reduced to one
	// plane here, and the rest of the corner consumes only that plane.
	//
	// Not optional polish: a line trace returns the flat FACE normal of the
	// triangle it hit, so one trace steps discontinuously at every triangle edge
	// even on terrain that looks smooth, and the tyre load is applied along that
	// normal. UpdateGroundPlane has the mechanism and why it fits positions rather
	// than normals.

	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling")
	EWishboneGroundSamplePattern GroundSamplePattern = EWishboneGroundSamplePattern::Cross;

	/**
	 * Samples along the rolling direction, counting the centre. Odd values keep the
	 * pattern symmetric about the wheel; even values still work, the fit just
	 * carries a small offset. Ignored by the Single pattern.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling", meta = (ClampMin = "1", ClampMax = "9"))
	int32 GroundSamplesLongitudinal = 3;

	/** Samples across the wheel, counting the centre. See above. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling", meta = (ClampMin = "1", ClampMax = "9"))
	int32 GroundSamplesLateral = 3;

	/**
	 * Half the sampled length, cm - how far fore and aft the outermost traces sit.
	 * Default is this wheel's contact patch half-length, sqrt(2*R*rho).
	 *
	 * Compare it against the COLLISION TRIANGLE size, not the tyre: a filter
	 * narrower than one triangle has nothing to average, since every sample lands
	 * on the same face and returns the same plane. Against UE's default 100 cm
	 * landscape quad a patch-sized filter removes only about a third of the
	 * frame-to-frame normal jump. Finer collision is the real fix; widening this is
	 * the workaround and it smooths away detail that is genuinely there.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling", meta = (ClampMin = "0.0", Units = "cm"))
	float GroundSampleHalfLengthCm = 10.f;
	// How far ahead this reaches IS the enveloping reach, and it is geometric rather
	// than a matter of taste. A tyre of radius R first touches a step of height h
	// when its centre is sqrt(R^2 - (R-h)^2) horizontally short of the edge - for
	// R = 33 cm that is 17.5 cm at a 5 cm step and 23.7 cm at 10 cm. At the default
	// 10 cm the plane only starts to rise 10 cm from the edge, so the wheel climbs
	// later and harder than a real one. Raising it toward 20 cm makes the corner
	// behave much more like a tyre over kerbs and rocks.
	//
	// It is not free: the fore and aft samples sit further apart, so on a crest the
	// highest-sample rule lifts the wheel sooner. That IS what a tyre does, but it
	// means the number wants to be near the tyre's real reach rather than as large
	// as possible.

	/**
	 * Half the sampled width, cm - which is to say HALF THE TYRE'S SECTION WIDTH.
	 *
	 * Worth stating plainly because this is the only place in the whole model where
	 * the tyre has a width at all. The Magic Formula absorbs it into fitted
	 * coefficients, the corner geometry never asks, and nothing cross-checks this
	 * number against anything. Set it from the tyre being modelled - 10 cm is a 200
	 * section, 11.25 is a 225 - because no other part of the code will notice if it
	 * is wrong.
	 *
	 * It reads narrow on screen against the drawn wheel, which is 2R across - 66 cm
	 * on this car against 20 cm of sampled width. That comparison is the misleading
	 * one: the contact patch is as wide as the TYRE, not as wide as the wheel is
	 * tall, so a fifth of the disc's diameter is about right and looking wrong is not
	 * evidence of being wrong.
	 *
	 * Note this and GroundSampleHalfLengthCm are not doing the same job, despite
	 * sitting together. Lateral is patch width, and there is no reason to exceed it.
	 * Longitudinal doubles as the enveloping reach - see its own note - so it is
	 * routinely set well past the patch on purpose. An asymmetric pattern is normal
	 * here and is not a sign either number is off.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling", meta = (ClampMin = "0.0", Units = "cm"))
	float GroundSampleHalfWidthCm = 10.f;

	/**
	 * Where the plane sits between the mean of the samples (0) and the highest of
	 * them (1).
	 *
	 * 1 is the physical choice: a tyre rests on the peaks and bridges the dips
	 * between them, so the plane that SUPPORTS the samples is the one it rides on.
	 * Averaging instead lets the wheel settle to the mean height - into the terrain.
	 * It costs a little ride height on rough ground, which is real; a car does sit
	 * higher on gravel than on glass.
	 *
	 * Free on smooth ground at any slope, since samples are measured against the
	 * FITTED normal and a planar patch puts mean and maximum together. The lift is
	 * the patch's departure from planar, which grows with the SQUARE of the sampled
	 * span - so lower this if you widen the pattern much past the real contact
	 * patch, or it will jack the car up over dips it should ride through.
	 *
	 * OFFROAD, this is the control that decides whether a wheel drops into a trench
	 * or bridges across it. Toward 0 it descends, which is what a rally car does in
	 * a rut; at 1 a single sample catching the rim holds the whole corner up.
	 *
	 * But it can only choose among the heights it is HANDED. It cannot see a feature
	 * no probe resolved, and that is the part worth remembering: a full-radius sweep
	 * sphere - 33 cm on this car - straddling a 30 cm trench has every probe resting
	 * on the rim, so every sample reports the rim, the mean equals the maximum, and
	 * this knob does nothing whatsoever. Small probes are what give it something to
	 * average: a line trace, or SweptContactRadiusScale down around 0.2, so a sample
	 * can actually reach the floor of the rut. Probe size first, then this.
	 *
	 * BlueprintReadWrite so it can be moved at runtime - per surface, or per car,
	 * without duplicating a corner setup for a rally variant.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension|Ground Sampling",
		meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GroundEnvelopingBias = 1.f;

	/**
	 * Drop samples the tyre is BRIDGING rather than standing on, before the fit.
	 *
	 * A tyre is a circle: at in-plane offset d from its contact, its own underside is
	 * already R - sqrt(R^2 - d^2) above that contact, so ground deeper than that is
	 * spanned, not touched. On a 33 cm wheel that is 1.55 cm at 10 cm out and 6.75 cm
	 * at 20 - a big circle is very nearly flat near its bottom, which is why the
	 * limit is so much tighter than it looks.
	 *
	 * What it buys depends entirely on the bias, and not symmetrically:
	 *
	 *   bias 1 - the peak survives by construction so the HEIGHT was never at risk.
	 *            This only cleans up the NORMAL, which nothing protected: one probe
	 *            finding floor 60 cm below a ledge fits as a near-vertical plane.
	 *
	 *   bias 0 - load-bearing. The mean is dragged down by exactly the samples the
	 *            tyre cannot reach, so a 60 cm outlier moves the plane 30 cm below
	 *            the road the wheel is on. This is what makes a low bias usable, and
	 *            a low bias is what makes a wheel drop into a rut instead of
	 *            bridging it.
	 *
	 * Not deflection-aware, on purpose - see the note at the implementation. Off by
	 * default because it changes plane geometry and every bench number with it.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling")
	bool bRejectBridgedSamples = false;

	/**
	 * Slack on the bridging test, cm. Zero is the bare geometry.
	 *
	 * Wanted because the criterion is an unloaded rigid circle and a real carcass
	 * conforms somewhat further into a dip than that. A centimetre keeps the test
	 * aimed at gross outliers - the ledge, the pothole edge - rather than at the
	 * millimetre of surface texture it has no business adjudicating.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling",
		meta = (ClampMin = "0.0", Units = "cm", EditCondition = "bRejectBridgedSamples"))
	float BridgedSampleToleranceCm = 1.f;

	/**
	 * Crown radius of the tread ACROSS the tyre's width, cm. Used only by the
	 * bridging test.
	 *
	 * A tyre is a torus, not a sphere. Around its circumference it curves at the
	 * rolling radius - 33 cm here - but across its width it is very nearly flat, and
	 * treating both with the wheel radius says the tread falls away sideways as
	 * sharply as it does fore and aft. It does not.
	 *
	 * 40 cm is a reasonable passenger-tyre crown and is chosen to be honest about
	 * what it is: an approximation of a tread that is flat through the middle and
	 * rounds off at the shoulder, not a measured number. Raising it toward flat makes
	 * the test STRICTER laterally, since a flatter tread bridges more; lowering it is
	 * more permissive. Anything much below the section half-width is not describing a
	 * tyre any more, so it is floored against the wheel radius.
	 *
	 * At the default pattern the two come out comparable - 1.55 cm of fore/aft reach
	 * at 10 cm out against 1.27 cm laterally - which is the right shape for a contact
	 * patch that is itself roughly square at those offsets.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling",
		meta = (ClampMin = "1.0", Units = "cm", EditCondition = "bRejectBridgedSamples"))
	float TyreCrownRadiusCm = 40.f;

	/**
	 * Steepest surface treated as drivable, degrees from world up. Samples beyond
	 * it are discarded before the fit, so a kerb face or wall alongside the wheel
	 * cannot tip the plane over and fire a large lateral load into the chassis.
	 *
	 * Tested against the reported face normal, which is the one jumpy signal here -
	 * acceptable only because the threshold is far from any surface a car drives
	 * on, so it takes genuinely non-drivable geometry to trip it.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Ground Sampling", meta = (ClampMin = "0.0", ClampMax = "89.0"))
	float MaxDrivableSlopeDeg = 55.f;

	// ---------------- Travel ----------------

	/**
	 * Travel limits as POSITIVE magnitudes from rest - bump is upward wheel
	 * motion, droop downward, on every corner.
	 *
	 * Signed theta limits are deliberately not exposed, because which sign means
	 * bump flips between corners: the wheel travels along HingeAxis x (Wheel - F),
	 * so it depends on the hinge axis relative to the arm's outboard direction.
	 * Hand-setting signed limits per corner is a guaranteed mirror-image bug, so
	 * the sign is derived from the geometry - see bPositiveThetaIsBump.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Travel", meta = (ClampMin = "0.0"))
	float BumpTravelDeg = 12.f;

	UPROPERTY(EditAnywhere, Category = "Suspension|Travel", meta = (ClampMin = "0.0"))
	float DroopTravelDeg = 12.f;

	// ---------------- Solver ----------------

	/**
	 * Fixed integration step, decoupled from frame delta on purpose: a stiff
	 * spring integrated at a variable rate is stable at 120 fps and explodes at
	 * 30. 1 ms is ~13x oversampled against the default 13 Hz hop mode; offline it
	 * stays stable out to 8 ms, so there is headroom to make this cheaper.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Solver", meta = (ClampMin = "0.0001", Units = "s"))
	float SubstepSeconds = 0.001f;

	/**
	 * Backstop against a frame hitch becoming a multi-second catch-up. At the
	 * default substep this covers frames down to ~15 fps before time is dropped.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Solver", meta = (ClampMin = "1"))
	int32 MaxSubstepsPerFrame = 64;

	/**
	 * Push this corner's reaction into the chassis body. Turn it off to run the
	 * corner as a pure observer - it still integrates and still publishes every
	 * readout, the body just does not feel it.
	 *
	 * Only ever reaches the body when the owner is also driving the substeps and
	 * calling ApplyReactionToBody; a standalone corner has no body to push.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Solver")
	bool bApplyReactionToChassis = true;

	// ---------------- Debug ----------------

	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawDebug = false;

	/** Hardpoint markers, arm triangles, hinge axes, kingpin, coilover. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawHardpoints = true;

	/**
	 * The arc each moving hardpoint sweeps between the travel limits - the actual
	 * paths, so an arc cutting through the mesh means bad geometry, not bad physics.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawTravelArcs = true;

	/** Dim copy of the corner at theta = 0, to show displacement from rest. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawRestPose = true;

	/**
	 * Tyre silhouette, spindle axis and the travel tangent.
	 *
	 * Split from the contact forces because the two answer different questions and
	 * get in each other's way. The disc is a reference outline for reading camber -
	 * it is 32 segments of line at the wheel radius, and at four corners that is a
	 * lot of geometry sitting exactly where the force arrows need to be seen. Turn
	 * it off and leave bDrawContactForces on when the question is about load rather
	 * than about geometry.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawWheelDisc = true;

	/** The contact patch, the tyre load arrow and the coilover diagnostic arrow. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawContactForces = true;

	/**
	 * Where each ground trace landed, plus the plane fitted through them.
	 *
	 * The point of turning this on is to compare the two: samples scattered over a
	 * bumpy patch with a steady plane through them is the mechanism working. A
	 * plane that snaps about while the samples barely move means the fit is being
	 * driven by too few surviving samples - widen the pattern or check the slope
	 * guard is not discarding them.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawGroundSamples = false;

	/**
	 * Keep drawing the ground under a wheel that is OFF it.
	 *
	 * Off by default, because everything drawn under a wheel reads as contact at a
	 * glance and no amount of dimming fixes that - hiding the plane and leaving the
	 * sample dots was still read as contact, correctly, because dots under a wheel
	 * look like a wheel on the ground.
	 *
	 * The information is real and occasionally wanted: the trace still finds ground
	 * up to a wheel radius plus the remaining travel below the hub, and seeing where
	 * a landing is about to happen is useful. It is just not what you want on screen
	 * while judging whether a corner is loaded.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug", meta = (EditCondition = "bDrawGroundSamples"))
	bool bDrawGroundWhenAirborne = false;

	/**
	 * Split the strut into its coil and its rigid mounts, and mark where the coil
	 * reaches free length - the point the damper mount has to travel out to before
	 * the spring goes slack, so the gap to it is the droop left before the corner
	 * stops carrying load.
	 *
	 * Which END the mounts sit at is a drawing convention, not modelled: only the
	 * total offset affects anything, so they are shown at the chassis end.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawSpring = true;

	/**
	 * How far outboard of the strut to draw that breakdown, cm.
	 *
	 * Drawn on the strut itself everything is collinear and unreadable - the coil
	 * sits exactly under the strut line, and the free-length mark under both. As a
	 * parallel bar to one side, with tie lines back to the strut's ends, each
	 * length can be read on its own. Zero puts it back on the strut.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug", meta = (EditCondition = "bDrawSpring", Units = "cm"))
	float SpringDrawOffsetCm = 9.f;

	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawReadout = true;

	/**
	 * Short symbol at each hardpoint - F_L, T, L, U, D, W, C and so on - matching
	 * the reference doc one for one, so a point on screen and a point on the page
	 * are never in doubt.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDrawLabels = true;

	/**
	 * Bypass the integrator and sweep theta across the full travel range. A
	 * kinematics rig-check rather than a physics test: every arm, the knuckle, the
	 * coilover and the wheel should articulate together with nothing detaching or
	 * leaving its arc. Physics does not run while this is on.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Debug")
	bool bDebugSweepTravel = false;

	UPROPERTY(EditAnywhere, Category = "Suspension|Debug", meta = (ClampMin = "0.1", Units = "s"))
	float DebugSweepPeriodSeconds = 4.f;

public:
#if WITH_EDITORONLY_DATA
	// ---------------- Editor viewport preview ----------------
	//
	// Read by FWishboneSuspensionVisualizer in the editor module. The corner is
	// drawn at whatever these say, entirely through GetPose, with no physics
	// running - so a hardpoint edit shows its effect on camber, caster, KPI and
	// travel immediately, before anything is simulated.

	/** Draw the linkage in the editor viewport while this component is selected. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Editor Preview")
	bool bEditorPreview = true;

	/**
	 * Where in the travel to draw: -1 full droop, 0 rest, +1 full bump. Means the
	 * same thing on both sides of the car - see TravelFractionToAngleDeg.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Editor Preview",
		meta = (EditCondition = "bEditorPreview", UIMin = "-1.0", UIMax = "1.0", ClampMin = "-1.0", ClampMax = "1.0"))
	float EditorPreviewTravel = 0.f;

	/**
	 * Steer to draw at, degrees. Positive turns the wheel RIGHT.
	 *
	 * Drag this with the rest pose on and watch the wheel circle tilt: that is
	 * caster and KPI producing camber through steer, and the wheel centre lifting
	 * is the KPI jacking. Neither is special-cased anywhere - both fall out of
	 * rotating the knuckle about its real, tilted steering axis.
	 */
	UPROPERTY(EditAnywhere, Category = "Suspension|Editor Preview",
		meta = (EditCondition = "bEditorPreview", UIMin = "-45.0", UIMax = "45.0"))
	float EditorPreviewSteerDeg = 0.f;

	/** Dim copy at rest (theta = 0, no steer), to measure the preview against. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Editor Preview", meta = (EditCondition = "bEditorPreview"))
	bool bEditorPreviewRestPose = true;

	/** The arc each moving hardpoint sweeps between the travel limits. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Editor Preview", meta = (EditCondition = "bEditorPreview"))
	bool bEditorPreviewTravelArcs = true;

	/** Tyre circle and spindle axis - what camber and toe are visible on. */
	UPROPERTY(EditAnywhere, Category = "Suspension|Editor Preview", meta = (EditCondition = "bEditorPreview"))
	bool bEditorPreviewWheel = true;

	/**
	 * Republishes theta and the alignment readouts at the previewed travel and
	 * steer, so the Live and Steering Geometry categories follow the sliders rather
	 * than sitting at the rest pose while only the drawing moves.
	 *
	 * Editor only. Nothing else writes ArticulationAngleDeg outside Play, so there
	 * is no integrator state to contend with.
	 */
	void RefreshEditorPreview();
#endif

protected:

	// ---------------- Live state ----------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float ArticulationAngleDeg = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float SpringForceN = 0.f;

	/**
	 * Where the coil actually is right now, cm - the live strut length less the
	 * rigid mounting offset. Compare against SpringOwnFreeLengthCm: shorter means
	 * squeezed and pushing, equal or longer means it has reached free length and
	 * carries nothing.
	 *
	 * Tracks the strut one-for-one, never as a fraction of it, since the offset it
	 * subtracts is constant. Reads as the raw strut length while
	 * SpringInstalledLengthCm is left unspecified.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float SpringLengthCm = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float DamperForceN = 0.f;

	/** Wheel-centre rise above rest in body space, cm. Positive is bump. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float WheelTravelCm = 0.f;

	/**
	 * Anti-roll force at the wheel centre, N, along body up. Negative pushes the
	 * wheel back down - which is what the compressed side of an axle should feel.
	 * Zero whenever this corner and its partner sit at the same height.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float AntiRollForceN = 0.f;

	/** Vertical tyre load actually applied this substep, N. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float ContactForceN = 0.f;

	/** Tyre radial deflection, cm. Pacejka's rho_z. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float TyreDeflectionCm = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	bool bInContact = false;

	/**
	 * Strut travel per unit wheel travel at the current angle. Falls out of the
	 * two tangents, and it is what sets wheel rate (k_wheel = k_spring * MR^2).
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float MotionRatio = 0.f;

	/**
	 * Live contact patch, world space cm - the live wheel centre projected onto
	 * the cached ground plane, which is where the tyre load is applied.
	 *
	 * Not the traced hit point: that came from a vertical trace at frame start and
	 * drifts away from the wheel centre as theta moves through the substeps.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	FVector ContactPointWS = FVector::ZeroVector;

	/**
	 * Net force this corner applies to the chassis this substep, N - the sum of
	 * the tyre load at the patch and (unsprung weight - unsprung inertia) at the
	 * wheel centre. See the class comment for where it comes from.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	FVector ChassisReactionForceN = FVector::ZeroVector;

	/**
	 * Coilover reaction on the chassis at CoiloverTop, N - what a strut-tower load
	 * cell would read.
	 *
	 * DIAGNOSTIC ONLY. Do NOT add it to the body. It is real, but it is already
	 * accounted for: the arms pull down on their bushes by exactly the reaction
	 * the strut pushes up with, so it cancels out of the net wrench. Applying it
	 * on top of ChassisReactionForceN double-counts the whole spring.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	FVector CoiloverForceOnChassisN = FVector::ZeroVector;

	/** Magnitude of the above, N. Convenient for graphing. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float CoiloverForceOnChassisMagN = 0.f;

	/** Wheel-centre acceleration relative to the body, m/s^2. Sets the inertial term. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	FVector WheelAccelRelativeMPerSec2 = FVector::ZeroVector;

	// ---------------- Resolved rest geometry ----------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector LCA_F_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector LCA_B_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector LCA_U_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector LCA_HingeAxis = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector UCA_F_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector UCA_B_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector UCA_U_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector UCA_HingeAxis = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector DamperMount_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector CoiloverTop_Rest = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector WheelCentre_Rest = FVector::ZeroVector;

	/** Spindle (axle) direction at rest, built from static camber and toe. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector SpindleAxis_Rest = FVector::ZeroVector;

	/** Installed coilover length at rest, cm. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	float CoiloverRestLengthCm = 0.f;

	/**
	 * The rigid, non-coil part of the strut, cm - perches, damper eyes, collars.
	 * CoiloverRestLengthCm - SpringInstalledLengthCm, so zero when the installed
	 * length is left unspecified.
	 *
	 * Constant by definition: it is the part that cannot compress, which is
	 * exactly why the coil takes the strut's full travel rather than a share of it.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	float SpringMountingOffsetCm = 0.f;

	/**
	 * The coil's free length, cm - what it relaxes to off the car, and the length
	 * at which it stops pushing.
	 *
	 * DERIVED, never typed: SpringInstalledLengthCm + PreloadForceN/SpringRateNPerM.
	 * The coil is squeezed by exactly the preload at rest, so backing that
	 * deflection off its installed length is where it would relax to.
	 *
	 * Compared against SpringLengthCm every substep - shorter coil means squeezed
	 * and pushing, equal or longer means slack and carrying nothing.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	float SpringFreeLengthCm = 0.f;

	/**
	 * Distance between the two ball joints. The knuckle is rigid, so this is a
	 * constraint the upper arm's angle is solved against - never a result.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	float KingpinLengthCm = 0.f;

	/**
	 * False when the two upper chassis bushes are coincident, leaving no axis for
	 * the upper arm to orbit. The knuckle then translates with the lower ball
	 * joint without rotating - degenerate, but coherent, and resolved once here so
	 * the kinematics and the debug draw cannot disagree about which case they are
	 * in.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	bool bHasUpperArm = false;

	/**
	 * Perpendicular distance from each hinge axis to its ball joint - the radius
	 * that actually sets travel per degree. Unequal radii create camber gain.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	float LowerArmSweepRadiusCm = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	float UpperArmSweepRadiusCm = 0.f;

	/** Which sign of theta lifts the wheel. Derived from geometry, not configured. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Travel")
	bool bPositiveThetaIsBump = false;

	/** Resolved signed limits, for checking against the drawn travel arcs. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Travel")
	float MinArticulationAngleDeg = -12.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Travel")
	float MaxArticulationAngleDeg = 12.f;

	// ---------------- Steering geometry ----------------
	//
	// Standard design metrics, evaluated live at the current angle and steer so
	// their gain through travel is visible. They cost only a few dot products and
	// none of them feed the forces yet - they need tyre in-plane forces to act.

	/** Live camber at the current angle and steer, deg. Positive = top outboard. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Steering Geometry")
	float LiveCamberDeg = 0.f;

	/** Camber at rest. Baseline the camber-gain readout measures against. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Steering Geometry")
	float RestCamberDeg = 0.f;

	/** Steering-axis lean from the front, deg. Positive = top leans inboard. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Steering Geometry")
	float KingpinInclinationDeg = 0.f;

	/** Steering-axis lean from the side, deg. Positive = top leans rearward. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Steering Geometry")
	float CasterAngleDeg = 0.f;

	/**
	 * KPI + camber, the standard alignment-rack figure: it separates a bent PART
	 * from bent MOUNTING. Camber out of spec with the included angle correct means
	 * an arm or the subframe has moved; the included angle itself wrong, or unequal
	 * left to right, means the knuckle or strut is bent.
	 *
	 * READ IT AT ZERO STEER ONLY, exactly as an alignment rack does. Both terms are
	 * front-view measurements, and steering rotates the knuckle ABOUT the kingpin -
	 * so KPI does not move at all while camber moves a great deal, and the sum just
	 * tracks camber. Measured on a corner with 8.4 deg of caster and KPI: through
	 * full travel it holds to 0.03 deg, but steering it -25 to +36 deg swings it
	 * over 10 deg with nothing whatsoever wrong.
	 *
	 * So it is NOT the rigid-knuckle invariant, despite reading like one.
	 * SpindleToKingpinDeg is.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Steering Geometry")
	float IncludedAngleDeg = 0.f;

	/**
	 * The angle between the spindle and the kingpin in 3D - what the knuckle
	 * casting physically fixes, and the quantity IncludedAngleDeg only approximates
	 * in the front view.
	 *
	 * THIS is the rigid-knuckle invariant, and it is exact rather than approximate:
	 * the swing term carries the spindle with the kingpin and the steer term rotates
	 * about the kingpin, and neither operation can change an angle measured TO that
	 * axis. Measured constant to 0.00e+00 across every travel and steer combination
	 * tested. Mirror-invariant by construction too, since a reflection applied to
	 * both operands leaves their dot product unchanged.
	 *
	 * Drift here means the kinematics are deforming the knuckle - which is what the
	 * kingpin-length check catches for DISTANCE between the ball joints. This pins
	 * the spindle's ORIENTATION to that line; together they pin the knuckle as a
	 * rigid body. Reported as the acute angle so both sides of the car agree.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Steering Geometry")
	float SpindleToKingpinDeg = 0.f;

	/**
	 * Lateral distance at ground level from where the steering axis meets the
	 * ground to the contact patch centre, mm. Positive = axis inboard of the
	 * patch. Mirror-aware, so both sides report the same value for symmetric
	 * geometry.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Steering Geometry")
	float ScrubRadiusMm = 0.f;
	
	/** Wheel forward at rest, in the wheel plane. Body +X made perpendicular to
	 *  the spindle, so it points forward on BOTH sides - unlike a cross product
	 *  against the spindle, whose sign mirrors. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Rest")
	FVector ForwardAxis_Rest = FVector::ForwardVector;
	
	/**
	 * Whole ground force the tyre handed over last substep, world N. Zero means
	 * airborne - or that nothing is feeding this corner, which looks identical
	 * from here and is why ACar checks the pairing rather than leaving it to be
	 * inferred from a sagging car.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	FVector ExternalContactForceN = FVector::ZeroVector;

	/** See SetWheelSpinAngleRad. Drawing only. */
	float DebugWheelSpinRad = 0.f;

	/**
	 * Partner corner's wheel travel last substep, cm. Zero is also what an unwired
	 * corner reads, and the two are indistinguishable from here - but harmlessly
	 * so: with AntiRollRateNPerM left at its default there is no bar to be wrong
	 * about, and ACar reports a rate set on only one end of an axle.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float AntiRollPartnerTravelCm = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Suspension|Alignment", meta = (ClampMin = "0.1"))
	float SteeringResponsiveness = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Suspension|Live")
	float TargetSteerAngleDeg = 0.f;

private:
	/**
	 * Resolves the baked hardpoints (applying mirror and offset) and derives
	 * everything constant that follows from them.
	 *
	 * @param bLogSummary  log the resolved alignment once. Wanted at BeginPlay,
	 *                     suppressed on the editor paths, which re-resolve on every
	 *                     keystroke in a hardpoint field.
	 */
	void InitialiseGeometry(bool bLogSummary = true);

	/** One fixed-size integration step of theta / theta'. */
	void IntegrateSubstep(float SubstepTime, const FTransform& BodyToWorld);

	/**
	 * Adds this step's share of the chassis wrench to the accumulators.
	 *
	 * Called from IntegrateSubstep after theta has been advanced but BEFORE the
	 * travel stops clamp it, so a stop cannot manufacture an unbounded a_wc.
	 *
	 * @param WheelTangentOld  wheel tangent BEFORE the step, world m/rad.
	 * @param ThetaDotBefore   theta' BEFORE the step, rad/s.
	 * @param CoiloverForceOnArm  spring + damper, i.e. the force on the ARM.
	 */
	void AccumulateChassisReaction(float SubstepTime, const FTransform& BodyToWorld,
		const FVector& WheelCentreWS, const FVector& WheelTangentOld, float ThetaDotBefore,
		const FVector& GravityForce, const FVector& ContactForce,
		const FVector& CoiloverForceOnArm, const FVector& CoiloverTopWS);

	/** Traces for ground once per frame and caches the plane for the substeps. */
	void UpdateGroundPlane(const FTransform& BodyToWorld);

	/**
	 * The sample pattern as (longitudinal, lateral) offsets in cm, centred on the
	 * wheel. Ordering is irrelevant - the fit is a sum. Doc (W.47).
	 */
	void BuildGroundSampleOffsets(TArray<FVector2D>& OutOffsets) const;

	/** Full geometry visualisation. Draw-only; never touches simulation state. */
	void DrawDebugGeometry(const FTransform& BodyToWorld);

	/** The body this corner is mounted on. Any scene component; no mesh required. */
	USceneComponent* GetBodyComponent() const;

	// --- Kinematics at an arbitrary angle. All body-local space, cm. ---

	FVector LowerBallJointAtAngle(float AngleDeg) const;  // L(theta), Doc (W.9)
	FVector UpperBallJointAtAngle(float AngleDeg) const;  // U(theta), Doc (W.15)

	/** The upper arm's solved angle. Equals the lower arm's only for equal arms. Doc (W.14). */
	float UpperArmSolvedAngleAtAngle(float AngleDeg) const;

	/** Knuckle orientation: suspension swing then steer about the live kingpin. Doc (W.17). */
	FQuat KnuckleRotationAtAngle(float AngleDeg) const;

	/** Wheel centre - rigid to the knuckle, not to the lower arm. Doc (W.18). */
	FVector WheelCentreAtAngle(float AngleDeg) const;

	/**
	 * Wheel-centre rise above rest, body space cm, with steer taken as zero.
	 * Positive is bump. The anti-roll bar's input - Doc (W.28a).
	 *
	 * Steer-free deliberately. Steering rotates the KNUCKLE about the kingpin,
	 * which jacks the wheel centre up or down while the lower arm has not moved at
	 * all - and the droplink is bolted to the arm, so the bar cannot see any of it.
	 *
	 * SMALL, and worth knowing how small before reading much into it. Two mirrored
	 * corners at equal and opposite steer jack by exactly the same amount, so the
	 * difference the bar acts on cancels to zero identically, whatever the KPI and
	 * caster. Only Ackermann's left-right angle difference survives, and on a
	 * conventional corner (KPI 10 deg, caster 5 deg, 6 cm hub offset, 265/150 cm)
	 * that is 0.10 cm at 30 deg of lock - about 20 N at a 20 kN/m bar, and under
	 * 8 N below 20 deg. Done this way because it is right and costs one knuckle
	 * solve, not because the alternative was ruining anything.
	 */
	float WheelTravelAtAngle(float AngleDeg) const;

	/**
	 * Live spindle (axle) direction, body space, pointing outboard.
	 *
	 * Static camber and toe are already baked into SpindleAxis_Rest; this adds the
	 * suspension's live swing and the steer rotation. The wheel plane is
	 * perpendicular to this, so it is what decides where a tilted tyre actually
	 * touches the ground - the s in Doc (W.25)/(W.26), see SolveDiscContact.
	 */
	FVector SpindleAxisAtAngle(float AngleDeg) const;

	/**
	 * d(WheelCentre)/d(theta) in cm per radian, by central difference. Not
	 * OrbitTangent: the wheel does not orbit the lower arm's hinge, and the
	 * knuckle rotation term has no pleasant closed form. t_W, Doc (W.19).
	 */
	FVector WheelTangentAtAngle(float AngleDeg) const;

	/**
	 * Steering-geometry metrics at an arbitrary angle and steer. Doc (W.43)-(W.46).
	 *
	 * Steer is a parameter rather than read off SteerAngleDeg so the editor preview
	 * can evaluate the readouts at a steer nothing is actually driving.
	 */
	void UpdateSteeringGeometry(float AngleDeg, float SteerDeg);

	/**
	 * The linkage in BODY space at a given angle and steer.
	 *
	 * The half GetPose and UpdateSteeringGeometry share. They cannot share the pose
	 * itself: every alignment metric is an angle read off body-space components
	 * (the kingpin's X and Y against its Z, the spindle's Z), and a world-space
	 * point cannot answer those once the chassis is rolled or pitched.
	 */
	struct FLocalLinkage
	{
		FVector LowerBallJoint = FVector::ZeroVector;
		FVector UpperBallJoint = FVector::ZeroVector;
		FVector DamperMount = FVector::ZeroVector;
		FVector WheelCentre = FVector::ZeroVector;
		FVector SpindleAxis = FVector::ZeroVector;
		bool bUpperArmSolveFailed = false;
	};

	FLocalLinkage ResolveLinkage(float AngleDeg, float SteerDeg) const;

	// Generalized coordinate, in radians so no substep converts; degrees are
	// published once per frame at the end.
	float ThetaRad = 0.f;
	float ThetaDotRad = 0.f;

	float SubstepAccumulator = 0.f;
	float DebugSweepTime = 0.f;

	// Rest baselines for the gain readouts. Internal - the live values above are
	// what is worth inspecting.
	float RestKingpinInclinationDeg = 0.f;
	float RestCasterAngleDeg = 0.f;

	// The rigid-knuckle baseline SpindleToKingpinDeg is checked against. Internal:
	// unlike the camber/caster/KPI baselines this is not a tuning reference, it is
	// only ever there to be compared against, and any difference at all is a fault.
	float RestSpindleToKingpinDeg = 0.f;

	// Ground plane fitted from this frame's traces, world space.
	bool bGroundHit = false;
	FVector GroundPointWS = FVector::ZeroVector;

	/**
	 * The body pose the ground samples above were taken in.
	 *
	 * They are stored as ABSOLUTE world points, so they are only in the right place
	 * relative to the wheel while the body is still where it was when they were traced.
	 * Under async physics it is not: the solver runs ahead of the pose the debug overlay
	 * is drawn in - measured at a constant 12.5 ms, which is three of the 4.167 ms steps
	 * and 28 cm of travel at 80 km/h. The samples therefore appeared ahead of the wheel,
	 * and outboard of it in a corner, because 12.5 ms along an arc is both.
	 *
	 * Keeping the pose lets the overlay undo exactly that motion - see
	 * DrawDebugGeometry. Nothing in the solve reads this; the solve was always
	 * self-consistent, since the samples and the contact frame come from one read of
	 * the body in one instant. It was only the picture that mixed two.
	 */
	FTransform GroundSamplePoseWS = FTransform::Identity;
	FVector GroundNormalWS = FVector::UpVector;

	// Surviving sample hits behind that plane, world space, kept for the debug
	// draw. Bounded by the pattern (81 at the clamped maximum), refilled each
	// frame, and never read by the simulation.
	TArray<FVector> GroundSampleHitsWS;

	/**
	 * Where each surviving sample's probe STARTED, world space - parallel to
	 * GroundSampleHitsWS, same index, appended together so they cannot desynchronise.
	 *
	 * Kept purely so the debug draw can show the probe axis. Whether a sample sits on
	 * its own axis is the question that decides between "displaced sample" and
	 * "sample a long way down, seen in perspective", and it is not answerable by
	 * looking at the dots: at a ledge the trace reaches most of a metre below the hub,
	 * and a metre of depth reads as a large sideways offset in a perspective view.
	 * Two rounds of arguing about that from screenshots is what bought this array.
	 */
	TArray<FVector> GroundSampleStartsWS;

	/**
	 * Samples the bridging test THREW OUT, kept only so the debug can draw them.
	 *
	 * Removing a sample from the fit and from the picture at the same time makes the
	 * test unfalsifiable from the outside: a deep sample still on screen could mean
	 * the test passed it, or that the test never ran, and there is no way to tell
	 * those apart by looking. Keeping the rejects and striking them through
	 * distinguishes the two immediately, which is the entire reason this exists.
	 */
	TArray<FVector> GroundSampleBridgedWS;

	// True when the plane came from a least-squares fit rather than the
	// single-sample fallback. Published only through the readout.
	bool bGroundPlaneFitted = false;

	bool bGeometryReady = false;

	// Wrench accumulated across the owner's substep, as IMPULSES rather than
	// forces: the corner takes several internal steps per owner substep, and the
	// force is not constant across them, so the time-weighted sum is what is
	// meaningful. ApplyReactionToBody divides back out.
	//
	// Unreal force units throughout (1 N = 100 UU = 100 kg*cm/s^2). Crossing an
	// arm in cm with a force in UU lands directly in kg*cm^2/s^2, which is what
	// AddTorqueInRadians wants - so no metre conversion appears anywhere.
	FVector ReactionImpulseUU = FVector::ZeroVector;
	FVector ReactionAngularImpulseUU = FVector::ZeroVector;

	// Torque reference (the body's centre of mass) and the owner's substep length,
	// both captured at the top of SolveSubstep.
	FVector ReactionRefWS = FVector::ZeroVector;
	float ReactionWindowSeconds = 0.f;

	// Where the coilover diagnostic acts. Derived, so not published.
	FVector CoiloverApplicationWS = FVector::ZeroVector;
};
