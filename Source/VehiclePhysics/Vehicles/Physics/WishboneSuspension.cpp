// Fill out your copyright notice in the Description page of Project Settings.

#include "WishboneSuspension.h"
#include "VehiclePhysics/Vehicles/VehiclePhysicsCompat.h"

#include "Engine/World.h"
#include "PhysicsEngine/BodyInstance.h"

// Chaos only. 4.27 reaches the body through FBodyInstance - see VehiclePhysicsBody.
#if VP_HAS_ASYNC_PHYSICS
#include "Chaos/ChaosEngineInterface.h"
#include "Chaos/ParticleHandle.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#endif

namespace VehiclePhysicsBody
{
#if VP_HAS_ASYNC_PHYSICS

	// bIsInternal is decided by whether a physics-thread particle exists, which is only
	// true while the solver is actually stepping - so this is the same test the state
	// read uses, and the two can never disagree about which thread they are on.
	static bool ShouldTargetInternal(const FPhysicsActorHandle& Handle)
	{
		return Handle != nullptr && Handle->GetPhysicsThreadAPI() != nullptr;
	}

	// The window is not used on Chaos: an impulse is already the integrated quantity.
	void ApplyLinearReaction(FBodyInstance* BodyInstance, const FVector& ImpulseUU, float)
	{
		if (BodyInstance == nullptr) { return; }
		FPhysicsActorHandle Handle = BodyInstance->GetPhysicsActor();
		if (Handle == nullptr) { return; }
		FChaosEngineInterface::AddImpulse_AssumesLocked(Handle, ImpulseUU,
			ShouldTargetInternal(Handle));
	}

	void ApplyAngularReaction(FBodyInstance* BodyInstance, const FVector& AngularImpulseUU, float)
	{
		if (BodyInstance == nullptr) { return; }
		FPhysicsActorHandle Handle = BodyInstance->GetPhysicsActor();
		if (Handle == nullptr) { return; }
		FChaosEngineInterface::AddAngularImpulseInRadians_AssumesLocked(Handle, AngularImpulseUU,
			ShouldTargetInternal(Handle));
	}

#else

	// Back to a force over the window it was accumulated over. bAllowSubstepping is false
	// because we are already inside a substep callback: letting Unreal re-substep this
	// would apply it a second time.
	//
	// The zero-window guard is new. The old code divided unconditionally, so a call with
	// nothing accumulated would have computed 0/0 and handed the body a NaN force. It
	// appears never to have happened - the window is a sum of substep deltas and the call
	// follows accumulation - but dividing without checking was luck rather than design.
	void ApplyLinearReaction(FBodyInstance* BodyInstance, const FVector& ImpulseUU, float WindowSeconds)
	{
		if (BodyInstance == nullptr || WindowSeconds <= SMALL_NUMBER) { return; }
		BodyInstance->AddForce(ImpulseUU / WindowSeconds,
			/*bAllowSubstepping*/ false, /*bAccelChange*/ false);
	}

	void ApplyAngularReaction(FBodyInstance* BodyInstance, const FVector& AngularImpulseUU, float WindowSeconds)
	{
		if (BodyInstance == nullptr || WindowSeconds <= SMALL_NUMBER) { return; }
		BodyInstance->AddTorqueInRadians(AngularImpulseUU / WindowSeconds,
			/*bAllowSubstepping*/ false, /*bAccelChange*/ false);
	}

#endif
}

namespace
{
	constexpr float WishboneCmToM = 0.01f;
	constexpr float WishboneMToCm = 100.f;

	// Unreal force units are kg*cm/s^2, so 1 N = 100 UU. Everything the integrator
	// computes is SI; this is the only place forces cross into Unreal's units.
	constexpr float WishboneNToUU = 100.f;

	// Below this the wheel centre is effectively on the hinge axis, where the
	// generalized inertia vanishes and theta'' would blow up. Real rigs are
	// nowhere near it; this is a guard, not a modelling decision.
	constexpr float MinEffectiveInertia = 1.e-6f;

	// Central-difference step for tangents. Small enough to be exact to well past
	// float precision for the smooth functions involved.
	constexpr float TangentStepDeg = 0.05f;
}

// ---------------------------------------------------------------------------
// Hardpoints
// ---------------------------------------------------------------------------

void FWishboneHardpoints::MirrorAcrossCentreline()
{
	for (FVector* Point : { &LCA_F, &LCA_B, &LCA_U, &UCA_F, &UCA_B, &UCA_U, &DamperMount, &CoiloverTop, &WheelCentre })
	{
		Point->Y = -Point->Y;
	}
}

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------

UWishboneSuspension::UWishboneSuspension()
{
	PrimaryComponentTick.bCanEverTick = true;

	// TG_PostPhysics, which is AFTER TG_EndPhysics has synced the body - so the
	// ground pass traces from where the wheel actually ended up this frame.
	//
	// It looks like it should be pre-physics, but the substeps do not consume this
	// frame's plane, they consume the one sampled at the end of the previous frame -
	// which is the same body position they start from. Identical input to the
	// integrator either way; the difference is that the samples now line up with the
	// wheel the debug draw renders, instead of trailing it by a whole chassis step.
	PrimaryComponentTick.TickGroup = TG_PostPhysics;

	DebugDrawTick.bCanEverTick = true;
	DebugDrawTick.TickGroup = TG_PostUpdateWork;
	DebugDrawTick.bStartWithTickEnabled = true;
	DebugDrawTick.bTickEvenWhenPaused = false;
}

void UWishboneSuspension::ApplySetup(const FWishboneCornerSetup& Setup)
{
	SpringRateNPerM = FMath::Max(Setup.SpringRateNPerM, 0.f);
	DamperRateNsPerM = FMath::Max(Setup.DamperRateNsPerM, 0.f);
	AntiRollRateNPerM = FMath::Max(Setup.AntiRollRateNPerM, 0.f);
	PreloadForceN = FMath::Max(Setup.PreloadForceN, 0.f);
	StaticCamberDeg = Setup.StaticCamberDeg;
	StaticToeDeg = Setup.StaticToeDeg;
	UnsprungMassKg = FMath::Max(Setup.UnsprungMassKg, 1.f);
	WheelRadiusCm = FMath::Max(Setup.WheelRadiusCm, 1.f);
	MinArticulationAngleDeg = FMath::Min(Setup.MinArticulationAngleDeg, 0.f);
	MaxArticulationAngleDeg = FMath::Max(Setup.MaxArticulationAngleDeg, 0.f);

	// Camber, toe and radius all feed the resolved rest geometry, so it has to be
	// rebuilt or the corner keeps describing the setup it had a moment ago.
	InitialiseGeometry(/*bLogSummary*/ false);
}

void UWishboneSuspension::SetMirrorAcrossCentreline(bool bInMirrorAcrossCentreline)
{
	bMirrorAcrossCentreline = bInMirrorAcrossCentreline;
}

void UWishboneSuspension::RegisterComponentTickFunctions(bool bRegister)
{
	Super::RegisterComponentTickFunctions(bRegister);

	if (bRegister)
	{
		if (SetupActorComponentTickFunction(&DebugDrawTick))
		{
			DebugDrawTick.Target = this;
		}
	}
	else if (DebugDrawTick.IsTickFunctionRegistered())
	{
		DebugDrawTick.UnRegisterTickFunction();
	}
}

void UWishboneSuspension::BeginPlay()
{
	Super::BeginPlay();

	InitialiseGeometry();

	// No tick prerequisite on the body any more. It used to exist so the body ticked
	// after this component and an animation blueprint could read this frame's theta -
	// but theta has not come from this tick since SolveSubstep took over publishing
	// it, and SolveSubstep runs inside the physics step, ahead of any tick group the
	// mesh could be in. The dependency bought nothing.
	//
	// It would now actively hurt: this component ticks at TG_PostPhysics, so making
	// the mesh wait on it would drag the skeletal mesh's tick to the end of the
	// frame. An animation layer reads theta from the previous physics step either
	// way, which is what a pose one physics step old means and is fine.
}

USceneComponent* UWishboneSuspension::GetBodyComponent() const
{
	const AActor* Owner = GetOwner();
	return Owner != nullptr ? Owner->GetRootComponent() : nullptr;
}

void UWishboneSuspension::OnRegister()
{
	Super::OnRegister();

	// BeginPlay never fires in the editor, so without this the whole resolved rest
	// geometry - hinge axes, kingpin length, spindle, alignment readouts - would
	// read zero in the details panel and the viewport preview would draw a corner
	// collapsed onto the origin. Safe this early because InitialiseGeometry works
	// purely in body-local space off Hardpoints; it never touches the body
	// transform, which is not trustworthy yet.
	InitialiseGeometry(/*bLogSummary*/ false);

#if WITH_EDITORONLY_DATA
	RefreshEditorPreview();
#endif
}

#if WITH_EDITOR
void UWishboneSuspension::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Every derived value hangs off the hardpoints and alignment, so rather than
	// enumerate which properties matter - and get it wrong the next time one is
	// added - just re-resolve. It is a few dozen dot products.
	//
	// Note InitialiseGeometry leaves the readouts at the UNSTEERED REST pose, since
	// that is the baseline the gain readouts are measured against. The preview
	// refresh then moves them to whatever is being previewed, which is what makes
	// the travel and steer sliders drive the Live and Steering Geometry values
	// rather than only the drawing.
	InitialiseGeometry(/*bLogSummary*/ false);
	RefreshEditorPreview();
}
#endif

#if WITH_EDITORONLY_DATA
void UWishboneSuspension::RefreshEditorPreview()
{
	// Blocked in a game world, and ONLY there. OnRegister runs on PIE start too, and
	// publishing the previewed angle would leave ArticulationAngleDeg disagreeing
	// with ThetaRad until the first physics tick caught up - long enough for an
	// animation blueprint reading it on frame zero to pose the wheel at the preview
	// instead of at rest.
	//
	// A NULL world has to pass. In the Blueprint editor the details panel edits the
	// component TEMPLATE, which belongs to the SCS rather than to any world, so
	// GetWorld() is null there - and that template is the object whose readouts the
	// panel displays. Rejecting null skipped exactly the case this exists for. None
	// of the work below needs a world: it is all body-space math.
	const UWorld* World = GetWorld();
	if (!bGeometryReady || (World != nullptr && World->IsGameWorld()))
	{
		return;
	}

	// Publish theta itself too, so the Live category agrees with the drawing and
	// with the Steering Geometry values below it. Outside Play nothing else writes
	// it, so there is no integrator state to fight over.
	ArticulationAngleDeg = TravelFractionToAngleDeg(EditorPreviewTravel);
	UpdateSteeringGeometry(ArticulationAngleDeg, EditorPreviewSteerDeg);
}
#endif

void UWishboneSuspension::InitialiseGeometry(bool bLogSummary)
{
	// ---------- Resolve the baked hardpoints for this corner ----------
	FWishboneHardpoints Resolved = Hardpoints;

	if (bMirrorAcrossCentreline)
	{
		Resolved.MirrorAcrossCentreline();
	}

	LCA_F_Rest = Resolved.LCA_F;
	LCA_B_Rest = Resolved.LCA_B;
	LCA_U_Rest = Resolved.LCA_U;
	UCA_F_Rest = Resolved.UCA_F;
	UCA_B_Rest = Resolved.UCA_B;
	UCA_U_Rest = Resolved.UCA_U;
	DamperMount_Rest = Resolved.DamperMount;
	CoiloverTop_Rest = Resolved.CoiloverTop;
	WheelCentre_Rest = Resolved.WheelCentre;

	// ---------- Derive everything constant ----------
	LCA_HingeAxis = WishboneGeometry::HingeAxis(LCA_F_Rest, LCA_B_Rest); // n_L, n_U - Doc (W.1)
	UCA_HingeAxis = WishboneGeometry::HingeAxis(UCA_F_Rest, UCA_B_Rest);

	if (LCA_HingeAxis.IsNearlyZero())
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s: disabled - the lower arm's two chassis bushes are coincident, so there is no "
				 "hinge axis to rotate about. Check Hardpoints LCA_F and LCA_B."), *GetName());
		return;
	}

	// The rigid-knuckle constraint the upper arm's angle is solved against.
	KingpinLengthCm = FVector::Dist(UCA_U_Rest, LCA_U_Rest); // l_kp, Doc (W.2)

	// Whether there is an upper arm at all, decided once here rather than
	// re-tested wherever the upper geometry is touched. Coincident upper bushes
	// leave no axis to orbit about, so the knuckle can only translate with the
	// lower ball joint - a degenerate rig, but a coherent one, and every consumer
	// needs to agree on which case it is in.
	bHasUpperArm = !UCA_HingeAxis.IsNearlyZero();

	// Perpendicular distance from each hinge axis, not raw arm length - this is
	// what sets travel per degree, and unequal radii create camber gain.
	LowerArmSweepRadiusCm = FVector::CrossProduct(LCA_HingeAxis, LCA_U_Rest - LCA_F_Rest).Size();
	UpperArmSweepRadiusCm = bHasUpperArm
		? FVector::CrossProduct(UCA_HingeAxis, UCA_U_Rest - UCA_F_Rest).Size()
		: 0.f; // r_L, r_U - Doc (W.3)

	CoiloverRestLengthCm = FVector::Dist(DamperMount_Rest, CoiloverTop_Rest); // l_c0, Doc (W.4)

	// ---------- Resolve the spring ----------
	//
	// Three inputs describe it completely: the coil's installed length at rest,
	// its rate, and the preload wound into it. Everything below is derived.

	// The coil is shorter than the strut; the rest is perches and collars. Zero
	// means it was not measured, so take the coil as spanning the whole strut.
	float InstalledCm = SpringInstalledLengthCm > KINDA_SMALL_NUMBER
		? SpringInstalledLengthCm
		: CoiloverRestLengthCm;

	if (InstalledCm > CoiloverRestLengthCm)
	{
		// A coil longer than the strut it sits in would need negative hardware.
		UE_LOG(LogTemp, Warning,
			TEXT("%s: SpringInstalledLengthCm (%.2f cm) is longer than the strut at rest (%.2f cm), "
				 "which would need negative mounting hardware. Treating the coil as spanning the "
				 "whole strut."),
			*GetName(), SpringInstalledLengthCm, CoiloverRestLengthCm);
		InstalledCm = CoiloverRestLengthCm;
	}

	// Rigid, so constant - which is exactly why the coil takes the strut's FULL
	// travel and not a share of it. A proportional split would need this steel to
	// compress, and would deliver SpringRateNPerM * ratio at the strut.
	SpringMountingOffsetCm = CoiloverRestLengthCm - InstalledCm; // s, Doc (W.22a)

	// The coil sits squeezed by exactly the preload at rest, so backing that
	// deflection off its installed length is where it would relax to. Guarded
	// because a zero rate makes the deflection meaningless (and the force zero
	// anyway, so nothing is lost).
	const float PreloadDeflectionCm = SpringRateNPerM > KINDA_SMALL_NUMBER
		? (PreloadForceN / SpringRateNPerM) * WishboneMToCm
		: 0.f;
	SpringFreeLengthCm = InstalledCm + PreloadDeflectionCm; // l_0, Doc (W.22)

	// Spindle built from the knuckle's own alignment, NOT from the kingpin. A real
	// upright reaches inboard to meet a tilted steering axis while the spindle
	// stays level, which is how a car runs KPI at zero camber; deriving the
	// spindle perpendicular to the kingpin would force camber to track KPI.
	{
		const float OutboardSign = WheelCentre_Rest.Y >= 0.f ? 1.f : -1.f;
		FVector Spindle(0.f, OutboardSign, 0.f);

		// Toe about vertical. Toe-in swings the spindle forward on both sides.
		Spindle = Spindle.RotateAngleAxis(-OutboardSign * StaticToeDeg, FVector::UpVector);

		// Camber about the wheel's forward direction. Positive camber tips the
		// outboard end down on either side, so no second sign is needed.
		const FVector ForwardAxis = FVector::CrossProduct(FVector::UpVector, Spindle).GetSafeNormal();
		if (!ForwardAxis.IsNearlyZero())
		{
			Spindle = Spindle.RotateAngleAxis(StaticCamberDeg, ForwardAxis);
		}

		SpindleAxis_Rest = Spindle.GetSafeNormal(); // Doc (W.5)
	}
	
	// Body +X projected perpendicular to the spindle. Deliberately not
	// cross(Up, Spindle) as the camber block above uses: that flips sign between
	// sides, which is correct there (it is a rotation axis) and wrong here.
	ForwardAxis_Rest = (FVector::ForwardVector
		- SpindleAxis_Rest * FVector::DotProduct(FVector::ForwardVector, SpindleAxis_Rest)).GetSafeNormal();
	if (ForwardAxis_Rest.IsNearlyZero())
	{
		ForwardAxis_Rest = FVector::ForwardVector;   // spindle points dead ahead: nonsense rig
	}

	// Which sign of theta lifts the wheel is decided by the geometry, never by
	// configuration - the answer mirrors between sides, and hand-setting it per
	// corner is how mirror-image bugs get in.
	const FVector RestTangent = WishboneGeometry::OrbitTangent(LCA_F_Rest, WheelCentre_Rest, LCA_HingeAxis);
	bPositiveThetaIsBump = RestTangent.Z > 0.f; // Doc (W.6)

	const float BumpLimit = bPositiveThetaIsBump ? BumpTravelDeg : -BumpTravelDeg;
	const float DroopLimit = bPositiveThetaIsBump ? -DroopTravelDeg : DroopTravelDeg;
	MinArticulationAngleDeg = FMath::Min(BumpLimit, DroopLimit);
	MaxArticulationAngleDeg = FMath::Max(BumpLimit, DroopLimit);

	bGeometryReady = true;

	// Baselines for the gain readouts, then a first evaluation at rest.
	UpdateSteeringGeometry(0.f, 0.f);
	RestCamberDeg = LiveCamberDeg;
	RestKingpinInclinationDeg = KingpinInclinationDeg;
	RestCasterAngleDeg = CasterAngleDeg;
	RestSpindleToKingpinDeg = SpindleToKingpinDeg;

	if (bLogSummary)
	{
		UE_LOG(LogTemp, Log,
			TEXT("%s ready: arm sweep L %.2f / U %.2f cm, kingpin %.2f cm, coilover %.2f cm, "
				 "camber %+.2f, KPI %+.2f, caster %+.2f, scrub %+.1f mm, travel [%.1f .. %.1f] deg, "
				 // The spring, which was invisible here and decides where the car actually
				 // rests: the coil is squeezed by exactly the preload at the design
				 // position, so preload zero means it holds nothing there and the corner
				 // sinks until it makes its own load.
				 "spring %.0f N/m, preload %.0f N, free length %.2f cm"),
			*GetName(), LowerArmSweepRadiusCm, UpperArmSweepRadiusCm, KingpinLengthCm, CoiloverRestLengthCm,
			RestCamberDeg, KingpinInclinationDeg, CasterAngleDeg, ScrubRadiusMm,
			MinArticulationAngleDeg, MaxArticulationAngleDeg,
			SpringRateNPerM, PreloadForceN, SpringFreeLengthCm);
	}
}

float UWishboneSuspension::TravelFractionToAngleDeg(float Fraction) const
{
	// Bump is whichever signed limit lifts the wheel, which mirrors between sides -
	// so resolve it from bPositiveThetaIsBump rather than assuming Max is bump.
	const float BumpDeg = bPositiveThetaIsBump ? MaxArticulationAngleDeg : MinArticulationAngleDeg;
	const float DroopDeg = bPositiveThetaIsBump ? MinArticulationAngleDeg : MaxArticulationAngleDeg;

	const float Clamped = FMath::Clamp(Fraction, -1.f, 1.f);
	return Clamped >= 0.f ? Clamped * BumpDeg : -Clamped * DroopDeg;
}

FWishbonePose UWishboneSuspension::GetPose(float AngleDeg, float SteerDeg) const
{
	FWishbonePose Pose;

	const USceneComponent* Body = GetBodyComponent();
	if (!bGeometryReady || Body == nullptr)
	{
		return Pose;
	}

	// The world-space face of ResolveLinkage, which UpdateSteeringGeometry shares -
	// so a pose and the alignment readouts can never describe different linkages.
	const FLocalLinkage Linkage = ResolveLinkage(AngleDeg, SteerDeg);
	Pose.bUpperArmSolveFailed = Linkage.bUpperArmSolveFailed;

	const FTransform ToWorld = Body->GetComponentTransform();
	auto Point = [&ToWorld](const FVector& P) { return ToWorld.TransformPosition(P); };
	auto Dir = [&ToWorld](const FVector& D) { return ToWorld.TransformVector(D).GetSafeNormal(); };

	Pose.LcaFront = Point(LCA_F_Rest);
	Pose.LcaRear = Point(LCA_B_Rest);
	Pose.UcaFront = Point(UCA_F_Rest);
	Pose.UcaRear = Point(UCA_B_Rest);
	Pose.CoiloverTop = Point(CoiloverTop_Rest);

	Pose.LowerBallJoint = Point(Linkage.LowerBallJoint);
	Pose.UpperBallJoint = Point(Linkage.UpperBallJoint);
	Pose.DamperMount = Point(Linkage.DamperMount);
	Pose.WheelCentre = Point(Linkage.WheelCentre);

	Pose.LowerHingeAxis = Dir(LCA_HingeAxis);
	Pose.UpperHingeAxis = Dir(UCA_HingeAxis);
	Pose.SpindleAxis = Dir(Linkage.SpindleAxis);

	// Any orthonormal pair perpendicular to the spindle spans the wheel plane; the
	// circle drawn in it is what makes camber and toe visible.
	Pose.SpindleAxis.FindBestAxisVectors(Pose.WheelPlaneU, Pose.WheelPlaneV);
	Pose.WheelRadiusCm = WheelRadiusCm;

	Pose.bValid = true;
	return Pose;
}

// ---------------------------------------------------------------------------
// Kinematics
// ---------------------------------------------------------------------------

FVector UWishboneSuspension::LowerBallJointAtAngle(float AngleDeg) const
{
	return WishboneGeometry::OrbitPoint(LCA_F_Rest, LCA_U_Rest, LCA_HingeAxis, AngleDeg); // L(theta), Doc (W.9)
}

float UWishboneSuspension::UpperArmSolvedAngleAtAngle(float AngleDeg) const
{
	// phi(theta), Doc (W.10)-(W.14).
	// The upper arm's angle is solved, not assumed equal to the lower arm's: the
	// knuckle is rigid, so the upper ball joint must stay exactly KingpinLengthCm
	// from the lower one. A shorter upper arm swings further to manage it, and
	// that difference is what generates camber gain.
	float UpperAngleDeg = AngleDeg;

	if (!WishboneGeometry::SolveUpperArmAngle(UCA_F_Rest, UCA_U_Rest, UCA_HingeAxis,
		LowerBallJointAtAngle(AngleDeg), KingpinLengthCm, /*Guess*/ AngleDeg, UpperAngleDeg))
	{
		// Out of reach: the arcs cannot span the kingpin here, so the linkage would
		// have to come apart. The travel limits are wider than the geometry allows.
		// Fall back to the parallelogram angle to stay continuous - the kingpin
		// readout turns red so it is not silent.
		UpperAngleDeg = AngleDeg;
	}

	return UpperAngleDeg;
}

FVector UWishboneSuspension::UpperBallJointAtAngle(float AngleDeg) const
{
	if (!bHasUpperArm)
	{
		// The knuckle translates with the lower ball joint without rotating.
		return UCA_U_Rest + (LowerBallJointAtAngle(AngleDeg) - LCA_U_Rest);
	}

	return WishboneGeometry::OrbitPoint(
		UCA_F_Rest, UCA_U_Rest, UCA_HingeAxis, UpperArmSolvedAngleAtAngle(AngleDeg)); // U(theta), Doc (W.15)
}

FQuat UWishboneSuspension::KnuckleRotationAtAngle(float AngleDeg) const
{
	// q(theta), Doc (W.16)-(W.17).
	// Suspension swing AND steer - the wheel leans with caster when turned, so
	// camber cannot be evaluated from the suspension angle alone.
	return WishboneGeometry::KnuckleOrientation(LCA_U_Rest, UCA_U_Rest,
		LowerBallJointAtAngle(AngleDeg), UpperBallJointAtAngle(AngleDeg), SteerAngleDeg);
}

FVector UWishboneSuspension::WheelCentreAtAngle(float AngleDeg) const
{
	return WishboneGeometry::KnucklePoint(WheelCentre_Rest, LCA_U_Rest, UCA_U_Rest,
		LowerBallJointAtAngle(AngleDeg), UpperBallJointAtAngle(AngleDeg), SteerAngleDeg); // W(theta), Doc (W.18)
}

float UWishboneSuspension::WheelTravelAtAngle(float AngleDeg) const
{
	// Same knuckle map as above with the steer term dropped - see the header for
	// why the bar must not see it. Measured from the DESIGN position rather than
	// static ride height, which cannot matter: only the difference between two
	// corners is ever used, and any offset the pair shares cancels out of it.
	const FVector WheelCentreUnsteered = WishboneGeometry::KnucklePoint(
		WheelCentre_Rest, LCA_U_Rest, UCA_U_Rest,
		LowerBallJointAtAngle(AngleDeg), UpperBallJointAtAngle(AngleDeg), /*SteerDeg*/ 0.f);

	return WheelCentreUnsteered.Z - WheelCentre_Rest.Z; // z_w, Doc (W.28a)
}

FVector UWishboneSuspension::SpindleAxisAtAngle(float AngleDeg) const
{
	return KnuckleRotationAtAngle(AngleDeg).RotateVector(SpindleAxis_Rest).GetSafeNormal(); // s, Doc (W.25)-(W.26)
}

FVector UWishboneSuspension::WheelTangentAtAngle(float AngleDeg) const
{
	// t_W(theta), Doc (W.19).
	return (WheelCentreAtAngle(AngleDeg + TangentStepDeg) - WheelCentreAtAngle(AngleDeg - TangentStepDeg))
		/ (2.f * FMath::DegreesToRadians(TangentStepDeg));
}

FVector UWishboneSuspension::GetWheelCentreWorld() const
{
	const USceneComponent* Body = GetBodyComponent();
	if (Body == nullptr || !bGeometryReady)
	{
		return FVector::ZeroVector;
	}

	return Body->GetComponentTransform().TransformPosition(WheelCentreAtAngle(ArticulationAngleDeg));
}

UWishboneSuspension::FLocalLinkage UWishboneSuspension::ResolveLinkage(float AngleDeg, float SteerDeg) const
{
	// Built from WishboneGeometry's free functions rather than this class's own
	// *AtAngle wrappers, because those read the LIVE SteerAngleDeg member and this
	// has to be answerable at an arbitrary steer without disturbing state.
	FLocalLinkage Linkage;

	Linkage.LowerBallJoint = WishboneGeometry::OrbitPoint(
		LCA_F_Rest, LCA_U_Rest, LCA_HingeAxis, AngleDeg); // L(theta), Doc (W.9)

	float UpperAngleDeg = AngleDeg;
	if (bHasUpperArm)
	{
		if (!WishboneGeometry::SolveUpperArmAngle(UCA_F_Rest, UCA_U_Rest, UCA_HingeAxis,
			Linkage.LowerBallJoint, KingpinLengthCm, /*Guess*/ AngleDeg, UpperAngleDeg)) // phi, Doc (W.10)-(W.14)
		{
			// Out of reach: the arcs cannot span the kingpin here. Fall back to the
			// parallelogram angle to stay continuous, and say so - the kingpin is then
			// the wrong length, which a consumer should be able to flag.
			Linkage.bUpperArmSolveFailed = true;
			UpperAngleDeg = AngleDeg;
		}
		Linkage.UpperBallJoint = WishboneGeometry::OrbitPoint(
			UCA_F_Rest, UCA_U_Rest, UCA_HingeAxis, UpperAngleDeg); // U(theta), Doc (W.15)
	}
	else
	{
		Linkage.UpperBallJoint = UCA_U_Rest + (Linkage.LowerBallJoint - LCA_U_Rest);
	}

	Linkage.DamperMount = WishboneGeometry::OrbitPoint(
		LCA_F_Rest, DamperMount_Rest, LCA_HingeAxis, AngleDeg); // D(theta), Doc (W.9)

	const FQuat Knuckle = WishboneGeometry::KnuckleOrientation(LCA_U_Rest, UCA_U_Rest,
		Linkage.LowerBallJoint, Linkage.UpperBallJoint, SteerDeg); // q(theta), Doc (W.16)-(W.17)

	Linkage.WheelCentre = Linkage.LowerBallJoint
		+ Knuckle.RotateVector(WheelCentre_Rest - LCA_U_Rest); // W, Doc (W.18)
	Linkage.SpindleAxis = Knuckle.RotateVector(SpindleAxis_Rest).GetSafeNormal(); // s, Doc (W.5)

	return Linkage;
}

void UWishboneSuspension::UpdateSteeringGeometry(float AngleDeg, float SteerDeg)
{
	const FLocalLinkage Linkage = ResolveLinkage(AngleDeg, SteerDeg);

	const FVector LowerBJ = Linkage.LowerBallJoint;
	const FVector UpperBJ = Linkage.UpperBallJoint;
	const FVector WheelCentre = Linkage.WheelCentre;
	const FVector SpindleLive = Linkage.SpindleAxis;

	const float OutboardSign = WheelCentre_Rest.Y >= 0.f ? 1.f : -1.f;

	// Positive camber = top leans outboard, which tips the outboard end of the
	// spindle DOWN, hence the negation. Mirror-safe: the spindle always points
	// outboard.
	LiveCamberDeg = FMath::RadiansToDegrees(-FMath::Asin(FMath::Clamp(SpindleLive.Z, -1.f, 1.f))); // gamma, Doc (W.43)

	// Read the raw offset, not the normalized direction - atan2 is scale
	// invariant, so this is exact rather than an approximation.
	const FVector KingpinOffset = UpperBJ - LowerBJ; // kappa, Doc (W.44)
	CasterAngleDeg = FMath::RadiansToDegrees(FMath::Atan2(-KingpinOffset.X, KingpinOffset.Z)); // Doc (W.44)
	KingpinInclinationDeg = FMath::RadiansToDegrees(
		FMath::Atan2(-OutboardSign * KingpinOffset.Y, KingpinOffset.Z)); // Doc (W.44)

	// The alignment-rack figure, for comparing against a spec sheet. Only meaningful
	// at zero steer - see the header; it is not the rigid-knuckle invariant it looks
	// like, because steering moves camber without moving KPI at all.
	IncludedAngleDeg = KingpinInclinationDeg + LiveCamberDeg; // Doc (W.45)

	// The actual invariant: the angle the knuckle casting holds between its spindle
	// and its steering axis. Exact under both travel and steer, so ANY drift here is
	// the kinematics deforming the knuckle rather than a projection artefact.
	//
	// Acute angle via the absolute dot, so the two sides of the car report the same
	// number - the spindle points outboard, which is opposite in Y between them.
	const FVector KingpinDir = KingpinOffset.GetSafeNormal();
	SpindleToKingpinDeg = FMath::RadiansToDegrees(FMath::Acos(
		FMath::Clamp(FMath::Abs(FVector::DotProduct(SpindleLive, KingpinDir)), 0.f, 1.f)));

	// Where the steering axis pierces the ground, versus the contact patch under
	// the wheel centre. Uses the nominal ground plane (wheel centre minus loaded
	// radius) rather than the traced one, so the metric stays comparable when the
	// corner is airborne. Doc (W.46).
	if (!FMath::IsNearlyZero(KingpinDir.Z))
	{
		const float GroundZ = WheelCentre.Z - WheelRadiusCm;
		const FVector AxisAtGround = LowerBJ + KingpinDir * ((GroundZ - LowerBJ.Z) / KingpinDir.Z);
		ScrubRadiusMm = OutboardSign * (WheelCentre.Y - AxisAtGround.Y) * 10.f; // scrub, Doc (W.46)
	}
}

// ---------------------------------------------------------------------------
// Tick
// ---------------------------------------------------------------------------

void UWishboneSuspension::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const USceneComponent* Body = GetBodyComponent();
	if (!bGeometryReady || Body == nullptr)
	{
		return;
	}

	// Sampled once and held across this frame's substeps. Stale by up to a frame
	// inside the physics step - which is why SolveSubstep rebuilds it per substep
	// instead - but perfectly adequate for placing the ground trace.
	const FTransform BodyToWorld = Body->GetComponentTransform();

	// Only when the substeps are NOT doing it themselves. With per-substep sampling
	// on, this pass would be immediately superseded by the first substep's, so it is
	// pure cost - but it still has to run when per-substep is off, and it is what
	// keeps the debug draw fed in sweep mode where the integrator never runs.
	if (!bSampleGroundPerSubstep || bDebugSweepTravel)
	{
		UpdateGroundPlane(BodyToWorld);
	}

	// THE INTEGRATOR DOES NOT RUN HERE. The owner steps this corner from inside its
	// physics callback, once per engine substep, where the body transform is live
	// and the reaction lands in the same step it was computed - see SolveSubstep.
	// Integrating here as well would advance theta twice per frame, half of it
	// against a frame-stale pose, and would leave the impulse accumulators filled
	// by a caller that never drains them.
	//
	// The sweep below is the one exception, and it is not integration: it drives
	// theta by hand for a kinematics rig-check and passes a zero timestep.
	if (!bDebugSweepTravel)
	{
		return;
	}

	{
		// Kinematics rig-check: drive theta directly across the travel range so the
		// arms can be watched articulating without waiting on the physics.
		DebugSweepTime += DeltaTime;
		const float Phase = FMath::Sin(2.f * PI * DebugSweepTime / FMath::Max(DebugSweepPeriodSeconds, 0.1f));
		const float Midpoint = 0.5f * (MinArticulationAngleDeg + MaxArticulationAngleDeg);
		const float HalfRange = 0.5f * (MaxArticulationAngleDeg - MinArticulationAngleDeg);
		const float SweptRad = FMath::DegreesToRadians(Midpoint + (Phase * HalfRange));

		// Evaluate forces at the swept angle so the readout and contact markers
		// stay meaningful, then restore it - a zero timestep cannot move theta,
		// but the stops clamp and the velocity terms would still fire.
		ThetaRad = SweptRad;
		ThetaDotRad = 0.f;
		IntegrateSubstep(0.f, BodyToWorld);
		ThetaRad = SweptRad;
		ThetaDotRad = 0.f;
		SubstepAccumulator = 0.f;
	}

	// Sweep mode only - SolveSubstep publishes these itself on the physics path.
	ArticulationAngleDeg = FMath::RadiansToDegrees(ThetaRad);
	UpdateSteeringGeometry(ArticulationAngleDeg, SteerAngleDeg);

	// Debug drawing runs in DrawDebugPass from the post-physics tick - this
	// transform is still last frame's.
}

void UWishboneSuspension::BuildGroundSampleOffsets(TArray<FVector2D>& OutOffsets) const
{
	// (longitudinal, lateral) in cm about the wheel centre. Doc (W.47).
	OutOffsets.Reset();

	if (GroundSamplePattern == EWishboneGroundSamplePattern::Single)
	{
		OutOffsets.Add(FVector2D::ZeroVector);
		return;
	}

	const int32 NumLong = FMath::Max(GroundSamplesLongitudinal, 1);
	const int32 NumLat = FMath::Max(GroundSamplesLateral, 1);

	// Spread evenly across [-Half, +Half]. A count of one sits at the CENTRE rather
	// than at an end, so turning an axis down to a single sample collapses the
	// pattern onto the wheel instead of shunting it off to one side.
	auto AxisOffset = [](int32 Index, int32 Count, float HalfExtentCm)
	{
		return Count > 1
			? FMath::Lerp(-HalfExtentCm, HalfExtentCm, static_cast<float>(Index) / static_cast<float>(Count - 1))
			: 0.f;
	};

	if (GroundSamplePattern == EWishboneGroundSamplePattern::Cross)
	{
		// Centre once, then each arm around it - the zero test is what stops the
		// centre being traced a second and third time.
		OutOffsets.Add(FVector2D::ZeroVector);

		for (int32 i = 0; i < NumLong; ++i)
		{
			const float U = AxisOffset(i, NumLong, GroundSampleHalfLengthCm);
			if (!FMath::IsNearlyZero(U))
			{
				OutOffsets.Add(FVector2D(U, 0.f));
			}
		}

		for (int32 j = 0; j < NumLat; ++j)
		{
			const float V = AxisOffset(j, NumLat, GroundSampleHalfWidthCm);
			if (!FMath::IsNearlyZero(V))
			{
				OutOffsets.Add(FVector2D(0.f, V));
			}
		}

		return;
	}

	for (int32 i = 0; i < NumLong; ++i)
	{
		const float U = AxisOffset(i, NumLong, GroundSampleHalfLengthCm);
		for (int32 j = 0; j < NumLat; ++j)
		{
			OutOffsets.Add(FVector2D(U, AxisOffset(j, NumLat, GroundSampleHalfWidthCm)));
		}
	}
}

void UWishboneSuspension::UpdateGroundPlane(const FTransform& BodyToWorld)
{
	// One pass of traces per frame, reduced to a single plane that the whole
	// substep loop then reuses - so substeps still cost arithmetic rather than
	// scene queries, exactly as before. Only the plane's PROVENANCE changes here:
	// nothing downstream of GroundPointWS/GroundNormalWS knows the difference.
	const float ThetaDeg = FMath::RadiansToDegrees(ThetaRad);
	const FVector WheelCentreWS = BodyToWorld.TransformPosition(WheelCentreAtAngle(ThetaDeg));

	// ---------- Sampling frame ----------
	//
	// Laid out in the plane perpendicular to the body's up, because the pattern has
	// to tile the GROUND it is measuring. Only the heading is taken from the
	// wheel: the spindle flattened against the body's up is the direction the tyre
	// points, and the rolling direction follows from it.
	//
	// (Forward, Lateral, Up) is right-handed by construction, which the plane fit
	// below depends on. The lateral SIGN is mirror-dependent and deliberately left
	// uncorrected - every pattern is symmetric about the wheel and the fitted
	// normal is forced upward afterwards, so both sides land on the same plane.
	// The BODY's up, not the world's.
	//
	// This was FVector::UpVector, and that is only defensible while the car is level.
	// A suspension ray has to follow the SUSPENSION: the wheel can move along its own
	// travel path and nowhere else, so the question the trace asks is "how far along
	// that path is the ground", and a world-vertical ray does not ask it.
	//
	// On a slope the two differ by the tilt, and the hit lands displaced downhill from
	// the wheel - visible immediately with bDrawGroundSamples, which is how this was
	// found. On a wall or a loop it is not a displacement but a failure: world-down
	// from a wheel on a vertical wall points at open air, so the trace finds nothing,
	// the corner reports no contact, and the car falls off a surface it is standing on.
	// Neither the sample pattern nor the swept path could mask it, because both
	// inherit this direction.
	//
	// Everything downstream still works: the plane fit is solved in this frame, the
	// drivable-slope guard measures against it, and both mean the same thing relative
	// to the car that they used to mean relative to the world. On level ground the two
	// are the same vector and nothing changes.
	const FVector BodyUpWS = BodyToWorld.GetUnitAxis(EAxis::Z);
	const FVector SpindleWS = BodyToWorld.TransformVector(SpindleAxisAtAngle(ThetaDeg));

	// DOWN THE WHEEL'S OWN PLANE, not the body's. Camber is the whole reason.
	//
	// A cambered wheel does not touch the ground under its hub. It touches at the
	// lowest point of a TILTED disc, which SolveDiscContact has always found
	// correctly - it projects the ground normal into the wheel plane and steps a
	// radius along it. The probes did not follow: they ran down the body axis, so on
	// a cambered corner the whole sampling window sat off to one side of the patch
	// it was supposed to be measuring. R*sin(camber) of error - 1.7 cm at 3 degrees,
	// 2.9 at 5, 5.7 at 10 under roll - against a pattern only 10 cm wide, so at large
	// camber the window is displaced by half its own span and the contact patch is
	// nearer its edge than its middle.
	//
	// Projecting the body's up into the wheel plane is the non-circular way to get
	// this. The exact in-plane down direction depends on the ground normal, which is
	// what the trace is trying to find; the projection of body-up needs nothing but
	// the spindle, is identical to body-up at zero camber, and tilts with the wheel
	// thereafter.
	//
	// Note this is also what makes LateralDir below collapse to the spindle itself,
	// since the spindle is perpendicular to anything in the wheel plane by
	// definition. The frame becomes the wheel's own.
	FVector Up = BodyUpWS;
	if (bSampleInWheelPlane)
	{
		const FVector SpindleDir = SpindleWS.GetSafeNormal();
		const FVector InWheelPlane = BodyUpWS - (SpindleDir * FVector::DotProduct(BodyUpWS, SpindleDir));
		if (!InWheelPlane.IsNearlyZero())
		{
			Up = InWheelPlane.GetSafeNormal();
		}
	}

	FVector LateralDir = (SpindleWS - Up * FVector::DotProduct(SpindleWS, Up)).GetSafeNormal();
	if (LateralDir.IsNearlyZero())
	{
		// Spindle vertical - 90 degrees of camber, so the wheel is not driving on
		// anything and any frame will do. Take the body's.
		LateralDir = BodyToWorld.GetUnitAxis(EAxis::Y);
	}
	const FVector ForwardDir = FVector::CrossProduct(LateralDir, Up);

	TArray<FVector2D> Offsets;
	BuildGroundSampleOffsets(Offsets);

	// ---------- Trace extents ----------
	//
	// Reach past the wheel radius by the full remaining travel, so the plane is
	// still known while the corner extends toward the ground.
	const float ArmSpanCm = (WheelCentre_Rest - LCA_F_Rest).Size();
	const float TravelReachCm = ArmSpanCm * FMath::Max(
		FMath::Abs(FMath::Sin(FMath::DegreesToRadians(MinArticulationAngleDeg))),
		FMath::Abs(FMath::Sin(FMath::DegreesToRadians(MaxArticulationAngleDeg))));

	const float SlopeDeg = FMath::Clamp(MaxDrivableSlopeDeg, 0.f, 89.f);

	// An offset sample stands over ground up to HalfExtent*tan(slope) above or below
	// the one under the wheel centre. Only the DOWNHILL half needs paying for, by
	// making the ray longer - the uphill half is already covered, because the ray
	// starts at the hub and the hub sits a whole wheel radius above the ground. At
	// the default +/-10 cm even a 55 degree slope only lifts a sample 14 cm, against
	// 32 cm of headroom the geometry gives for free.
	//
	// DO NOT LIFT THE RAY'S START. Nothing else stops a sample landing above the hub,
	// and a sample above the hub is what launches the car: SolveDiscContact reads it
	// as CentreHeight < 0 and reports rho_z = R + |CentreHeight|, which 4.E68 then
	// squares. Not a large load - an absurd one.
	//
	// The drivable-slope guard below cannot cover it either, which is what makes this
	// worth spelling out: a flat ceiling's top face is exactly as horizontal as a
	// road and passes that test as excellent ground.
	//
	// A lift is exactly how this went wrong before. It coupled the start height to
	// the sample extent, so widening the pattern to fight coarse collision - which
	// the extent's own comment recommends - raised the ray into the air with it: a
	// metre above the contact patch at +/-50 cm, nearly two at +/-100. From up there
	// the ray begins above a low overhang and hits its top face coming down. Starting
	// at the hub, a downward ray cannot reach above the hub at all, and that geometric
	// fact is the whole defence.
	//
	// Still capped: no drivable slope justifies reaching further down than the wheel
	// could ever travel.
	const float SlopeReachCm = FMath::Min(
		FMath::Max(GroundSampleHalfLengthCm, GroundSampleHalfWidthCm)
			* FMath::Tan(FMath::DegreesToRadians(SlopeDeg)),
		WheelRadiusCm);

	// Down from the hub: past the tyre, past whatever travel is left, plus the
	// downhill allowance above. Only once, not twice - the other SlopeReach was
	// paying for a lift that no longer happens.
	const float TraceLengthCm = WheelRadiusCm + TravelReachCm + SlopeReachCm;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(WishboneSuspensionGroundTrace), /*bTraceComplex*/ false);
	Params.AddIgnoredActor(GetOwner());

	// ---------- Sample ----------
	const float MinNormalZ = FMath::Cos(FMath::DegreesToRadians(SlopeDeg));

	GroundSampleHitsWS.Reset(Offsets.Num());
	GroundSampleStartsWS.Reset(Offsets.Num());
	GroundSampleBridgedWS.Reset();   // every pass, or last pass's rejects linger on screen

	// The pose these are about to be traced in, so the overlay can put them back under
	// the wheel when it draws them in a different one. See GroundSamplePoseWS.
	GroundSamplePoseWS = BodyToWorld;

	// Held per sample rather than summed as we go, because the bridging reject below
	// has to be able to drop a sample's normal along with its position.
	TArray<FVector, TInlineAllocator<16>> SampleFaceNormals;

	for (const FVector2D& Offset : Offsets)
	{
		// At hub height, never above it - see SlopeReachCm.
		const FVector Start = WheelCentreWS
			+ (ForwardDir * Offset.X) + (LateralDir * Offset.Y);

		const FVector End = Start - (Up * TraceLengthCm);

		FHitResult Hit;
		bool bHit = false;

		// The point the sample contributes to the plane fit. For a line trace it is
		// where the line met the geometry; for a sweep it is where the WHEEL CENTRE
		// would come to rest, brought back down by the sweep radius - and that is the
		// whole difference. Rolling at a step, the line's answer jumps and the
		// sphere's rises smoothly, because a sphere touches the corner first.
		FVector SamplePointWS = FVector::ZeroVector;

		if (bUseSweptContact)
		{
			const float SweepRadiusCm = GetSweepRadiusCm();

			// BOTH ENDS RAISED BY THE RADIUS, so the sphere's LOWEST POINT starts at
			// the hub and travels exactly the segment the line trace covers. That one
			// offset is the difference between a sphere that envelops and one that
			// does nothing whatsoever.
			//
			// Centred on the hub instead - which is what this did - a sphere of the
			// wheel's own radius is ALREADY touching the road before the query begins,
			// because the hub sits a radius above the road and so the sphere's lowest
			// point is already on it. It then never sweeps: the query returns an
			// initial overlap, the way out of flat ground is straight up by the tyre
			// deflection, and resolving along it hands back precisely the point a line
			// trace would have hit. Identical, arithmetically, not approximately -
			// which is exactly how it behaved.
			//
			// Raised, the sphere descends and meets a kerb or a stone with its lower
			// surface some way before the hub is over it, then comes to rest on it.
			// Which is what a tyre does, and what this was always supposed to do.
			const FVector SweepStart = Start + (Up * SweepRadiusCm);
			const FVector SweepEnd = End + (Up * SweepRadiusCm);

			bHit = GetWorld()->SweepSingleByChannel(Hit, SweepStart, SweepEnd, FQuat::Identity,
				GroundTraceChannel, FCollisionShape::MakeSphere(SweepRadiusCm), Params);

			// Whether the sphere's answer is usable AS GROUND, which it is not always.
			bool bSweptUsable = false;

			if (bHit && Hit.bStartPenetrating)
			{
				// Inside the geometry, and the case the swept path genuinely exists
				// for: a downward line from in there finds nothing above it, contact is
				// lost, and with no tyre load the corner sinks further and never comes
				// back. A sweep reports the minimum translation out, so the surface is
				// known even though nothing was swept through - resolve along it and
				// the usual centre-minus-radius gives a point well below the wheel,
				// which reads as a large deflection and pushes the corner back out.
				//
				// Only when the way out points UPWARD. A wheel beside a kerb face
				// overlaps that face and its way out is horizontal; resolving along
				// that would put the "ground" off to one side and a radius below it.
				const float NormalAlongUp = FVector::DotProduct(Hit.Normal, Up);
				if (NormalAlongUp >= MinNormalZ)
				{
					// LIFTED ALONG UP, not along the way out.
					//
					// The minimum translation is the SHORTEST way out, and the guard
					// above accepts it at up to MaxDrivableSlopeDeg off vertical. So
					// resolving along it slid the sample sideways by depth*sin(theta) -
					// 0.82 of the depth at the 55 degree default. That is how samples
					// came to sit well away from the probes they belong to next to a
					// ledge, which is visible now that the footprints are drawn at size.
					//
					// Wrong twice over. A sample is meant to be the surface height UNDER
					// its probe, and every other path here puts it exactly on that
					// probe's vertical line. And the plane fit reads each sample's (u, v)
					// from its actual position, so a displaced sample reports a height at
					// coordinates it was never measured at - it corrupts the gradient, not
					// just the picture.
					//
					// Moving t along Up reduces the penetration by t*dot(N, Up), so
					// clearing it vertically costs depth/dot(N, Up). Bounded by the same
					// guard: 1/cos(55) = 1.74x the depth at the default. Note that a very
					// high MaxDrivableSlopeDeg loosens this - at 89 degrees it admits
					// near-horizontal normals and the factor reaches 57 - which is one
					// more reason not to set that absurdly high.
					const FVector Resolved = SweepStart + (Up * (Hit.PenetrationDepth / NormalAlongUp));
					SamplePointWS = Resolved - (Up * SweepRadiusCm);
					bSweptUsable = true;
				}
			}
			else if (bHit && FVector::DotProduct(Hit.ImpactNormal, Up) >= MinNormalZ)
			{
				// Came to rest on something floor-like. Hit.Location is the sphere's
				// centre there, so the tyre's contact is a radius below it.
				SamplePointWS = Hit.Location - (Up * SweepRadiusCm);
				bSweptUsable = true;
			}

			if (!bSweptUsable)
			{
				// FALL BACK TO THE LINE - do not discard the sample. A sphere carries a
				// radius of lateral reach the line does not, so it meets vertical faces
				// the line would have passed cleanly by: a kerb side, a wall, a cube or
				// a piece of debris standing NEXT TO the wheel rather than under it.
				//
				// Rejecting those as ground is right. Rejecting the whole SAMPLE
				// because of them was the bug, and a bad one: with the Single pattern a
				// single lateral hit left the corner with no ground at all, on road it
				// was plainly still standing on, so the wheel dropped through anything
				// small enough to sit beside it. Reported as debris and a test cube
				// being driven straight through, and as switching the sweep off curing
				// it - which it did, because the line never saw them in the first place.
				//
				// The line from the hub answers this case correctly: a lateral way out
				// is itself evidence the hub is not inside the floor. With the fallback
				// the swept path can only add information, never remove it.
				bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, GroundTraceChannel, Params);
				SamplePointWS = Hit.ImpactPoint;
			}
		}
		else
		{
			bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, GroundTraceChannel, Params);
			SamplePointWS = Hit.ImpactPoint;
		}

		if (!bHit)
		{
			continue;
		}

		// Drivable-slope guard, Doc (W.48). Discarded before the fit, so a kerb face
		// or wall beside the wheel cannot tip the plane and fire a large lateral
		// load into the chassis.
		//
		// Against the BODY's up, not the sampling frame's. "Can the car drive on
		// this" is a question about the car, and measuring it against a cambered
		// wheel plane would move the threshold by the camber angle - a surface would
		// become drivable or not depending on which corner asked.
		if (FVector::DotProduct(Hit.ImpactNormal, BodyUpWS) < MinNormalZ)
		{
			continue;
		}

		GroundSampleHitsWS.Add(SamplePointWS);
		GroundSampleStartsWS.Add(Start);   // same index, appended together
		SampleFaceNormals.Add(Hit.ImpactNormal);
	}

	bGroundPlaneFitted = false;
	bGroundHit = GroundSampleHitsWS.Num() > 0;

	if (!bGroundHit)
	{
		return;
	}

	// ---------- Bridged samples ----------
	//
	// A tyre is a circle, so at in-plane offset d from where it touches, its own
	// underside already sits R - sqrt(R^2 - d^2) above that contact. Ground deeper
	// than that is SPANNED rather than touched - the tyre is a bridge over it - and a
	// sample there describes a surface the wheel is not standing on.
	//
	// Those samples were steering the fit. Beside a ledge a probe past the edge
	// legitimately finds floor 60 cm down, and 60 cm across a 10 cm baseline fits as
	// a near-vertical plane. GroundEnvelopingBias at 1 hides that for the HEIGHT,
	// since the peak survives by construction - but nothing protected the NORMAL, and
	// at bias 0 nothing protects the height either, because the mean is dragged down
	// by precisely the samples the tyre cannot reach.
	//
	// Measured against the PEAK whatever the bias. The highest sample is the one the
	// tyre is certainly on; averaging is a decision about the surface, taken after
	// deciding what counts as surface at all. The peak always survives its own test,
	// so this can never empty the set.
	//
	// DELIBERATELY NOT DEFLECTION-AWARE, which is the interesting part. A loaded tyre
	// flattens into a patch of half-length sqrt(2*R*x - x^2) - 13.8 cm at 3 cm of
	// deflection, wider than the default pattern - inside which the geometric answer
	// is exactly zero and this would reject every sample that is not the peak. And
	// deflection depends on the plane, which would then depend on which samples
	// survived, which would depend on deflection: a feedback loop closed inside a
	// single solve, chattering with every load change. The unloaded circle is
	// load-independent, stable and generous, which is what a gross-outlier reject
	// wants to be. It is a ceiling, not a measurement.
	if (bRejectBridgedSamples && GroundSampleHitsWS.Num() > 1)
	{
		// The peak, and CRUCIALLY where its probe stood - the tyre pivots to rest on
		// the highest point it can reach, so that point is the contact and every
		// offset in this test has to be measured from THERE, not from the hub.
		//
		// Measuring from the hub was wrong in a way that only shows when the peak is
		// off-centre, which is exactly the ledge and rock cases this exists for. It
		// also made a zero tolerance pathological: the sample under the hub has zero
		// offset and therefore zero reach, so the instant the peak was anywhere else,
		// the centre sample failed its own test and was thrown out. Carried far
		// enough, only the peak itself survives, the fit goes singular and the whole
		// thing silently falls back to a face normal.
		//
		// Identical on flat ground, where the peak IS the centre sample.
		float PeakAlongUp = -BIG_NUMBER;
		int32 PeakIndex = 0;
		for (int32 i = 0; i < GroundSampleHitsWS.Num(); ++i)
		{
			const float HeightCm = FVector::DotProduct(GroundSampleHitsWS[i], Up);
			if (HeightCm > PeakAlongUp)
			{
				PeakAlongUp = HeightCm;
				PeakIndex = i;
			}
		}
		const FVector PeakStartWS = GroundSampleStartsWS[PeakIndex];

		// Sagitta: how far a circle of radius Rad has risen at offset d from its
		// lowest point. Clamped past the radius, where the circle has simply ended.
		auto Sagitta = [](float OffsetCm, float RadiusCm)
		{
			return (OffsetCm >= RadiusCm)
				? RadiusCm
				: RadiusCm - FMath::Sqrt(FMath::Max((RadiusCm * RadiusCm) - (OffsetCm * OffsetCm), 0.f));
		};

		for (int32 i = GroundSampleHitsWS.Num() - 1; i >= 0; --i)
		{
			// SPLIT FORE/AFT FROM LATERAL, because a tyre is a torus and not a sphere.
			//
			// This first went in using the straight radial distance from the wheel
			// centre and the wheel radius for both, which says the tread curves away
			// across its WIDTH as sharply as it does around its circumference. It does
			// not - it is very nearly flat across the width, with a crown radius of
			// something like half a metre against 33 cm of rolling radius.
			//
			// Near the lowest point a torus separates: the surface rises by the fore/aft
			// sagitta plus the lateral one, each about its own radius. That is exact to
			// second order, which is the only order that matters at these offsets.
			const FVector FromPeak = GroundSampleStartsWS[i] - PeakStartWS;
			const float LongCm = FMath::Abs(FVector::DotProduct(FromPeak, ForwardDir));
			const float LatCm  = FMath::Abs(FVector::DotProduct(FromPeak, LateralDir));

			const float ReachCm = Sagitta(LongCm, WheelRadiusCm)
				+ Sagitta(LatCm, FMath::Max(TyreCrownRadiusCm, WheelRadiusCm * 0.25f));

			const float DipCm = PeakAlongUp - FVector::DotProduct(GroundSampleHitsWS[i], Up);
			if (DipCm > ReachCm + BridgedSampleToleranceCm)
			{
				// Kept for the debug draw before it leaves the fit - see the member.
				GroundSampleBridgedWS.Add(GroundSampleHitsWS[i]);

				GroundSampleHitsWS.RemoveAt(i, 1, VP_NO_SHRINK);
				GroundSampleStartsWS.RemoveAt(i, 1, VP_NO_SHRINK);
				SampleFaceNormals.RemoveAt(i, 1, VP_NO_SHRINK);
			}
		}
	}

	// Summed AFTER any rejection, so a bridged sample cannot steer the fallback
	// normal either - which is the one the fit falls back TO when it goes singular.
	FVector FaceNormalSum = FVector::ZeroVector;
	for (const FVector& FaceNormal : SampleFaceNormals)
	{
		FaceNormalSum += FaceNormal;
	}

	// ---------- Least-squares plane through the hit POSITIONS ----------
	//
	// Positions, not the reported normals, and that is the whole point. Surface
	// HEIGHT is continuous across a collision triangle's edges even though its face
	// NORMAL is not, so the sample points slide smoothly as the wheel rolls and the
	// plane through them turns smoothly with them. Averaging face normals instead
	// would still step at every edge crossing, just by 1/N of the jump at a time.
	//
	// Solved in the sampling frame as h = a*u + b*v + c. Centring the coordinates
	// drops the constant out and leaves a 2x2 system - no 3x3 inverse, no eigen
	// solve. Doc (W.49).
	const int32 Count = GroundSampleHitsWS.Num();

	FVector Centroid = FVector::ZeroVector;
	for (const FVector& Point : GroundSampleHitsWS)
	{
		Centroid += Point;
	}
	Centroid /= static_cast<float>(Count);

	float Suu = 0.f, Svv = 0.f, Suv = 0.f, Suh = 0.f, Svh = 0.f;
	for (const FVector& Point : GroundSampleHitsWS)
	{
		const FVector Delta = Point - Centroid;
		const float U = FVector::DotProduct(Delta, ForwardDir);
		const float V = FVector::DotProduct(Delta, LateralDir);
		const float H = FVector::DotProduct(Delta, Up);

		Suu += U * U;
		Svv += V * V;
		Suv += U * V;
		Suh += U * H;
		Svh += V * H;
	}

	// Averaged face normals. Used only when the fit cannot run - which for a Single
	// pattern is always, making that path bit-for-bit the original behaviour.
	GroundNormalWS = FaceNormalSum.GetSafeNormal();

	// Singular when the surviving samples are COLLINEAR: a single point, a
	// one-axis pattern, or dropouts that happened to leave a line. There is
	// genuinely no plane through those, so fall back rather than invent a tilt.
	const float Det = (Suu * Svv) - (Suv * Suv);
	if (FMath::Abs(Det) > SMALL_NUMBER)
	{
		const float A = ((Suh * Svv) - (Svh * Suv)) / Det;
		const float B = ((Svh * Suu) - (Suh * Suv)) / Det;

		// Gradient (a, b) of h(u, v) maps to the normal (-a, -b, 1) in the same
		// frame, which is why (Forward, Lateral, Up) had to be right-handed.
		const FVector Fitted = (Up - (ForwardDir * A) - (LateralDir * B)).GetSafeNormal();

		// The guard again, now on the RESULT: a fit dominated by one steep sample
		// that individually passed can still come out past the limit.
		// Against the BODY's up again, for the same reason as the per-sample guard.
		if (FVector::DotProduct(Fitted, BodyUpWS) >= MinNormalZ)
		{
			GroundNormalWS = Fitted;
			bGroundPlaneFitted = true;
		}
	}

	if (GroundNormalWS.IsNearlyZero())
	{
		// Face normals cancelled (a perfect V under the wheel). Nothing sane to
		// report; level is the least-wrong answer and the deflection still gates it.
		GroundNormalWS = Up;
	}

	// ---------- Where the plane sits along that normal ----------
	//
	// Slide it out to the SUPPORTING position - resting on the highest sample -
	// because a tyre rides the peaks and bridges what lies between them. Taking the
	// mean instead lets the wheel settle to the average height, i.e. sink into the
	// terrain, which is the failure this whole mechanism exists to prevent.
	// GroundEnvelopingBias blends back toward that mean for A/B. Doc (W.50).
	float MeanDot = 0.f;
	float MaxDot = -BIG_NUMBER;
	for (const FVector& Point : GroundSampleHitsWS)
	{
		const float Dot = FVector::DotProduct(Point, GroundNormalWS);
		MeanDot += Dot;
		MaxDot = FMath::Max(MaxDot, Dot);
	}
	MeanDot /= static_cast<float>(Count);

	const float TargetDot = FMath::Lerp(MeanDot, MaxDot, FMath::Clamp(GroundEnvelopingBias, 0.f, 1.f));

	GroundPointWS = Centroid + GroundNormalWS * (TargetDot - FVector::DotProduct(Centroid, GroundNormalWS));
}

// ---------------------------------------------------------------------------
// Integrator
// ---------------------------------------------------------------------------

void UWishboneSuspension::IntegrateSubstep(float SubstepTime, const FTransform& BodyToWorld)
{
	const float ThetaDeg = FMath::RadiansToDegrees(ThetaRad);

	// ---------- Geometry at the current angle ----------
	//
	// The damper mount is rigid to the lower arm, so it orbits that arm's hinge.
	// The wheel centre is rigid to the KNUCKLE, so it translates with the lower
	// ball joint and rotates only with the kingpin. Two mountings, two formulas.
	const FVector WheelCentreCS = WheelCentreAtAngle(ThetaDeg); // W(theta), Doc (W.18)
	const FVector DamperMountCS = WishboneGeometry::OrbitPoint(LCA_F_Rest, DamperMount_Rest, LCA_HingeAxis, ThetaDeg); // D(theta), Doc (W.9)

	// How far this corner has risen relative to the chassis. Published because the
	// anti-roll bar on the other end of the axle is fed from it - see
	// GetWheelTravelCm for why it is a height and not theta, and
	// WheelTravelAtAngle for why it is not just WheelCentreCS.Z.
	WheelTravelCm = WheelTravelAtAngle(ThetaDeg); // z_w, Doc (W.28a)

	const FVector WheelCentreWS = BodyToWorld.TransformPosition(WheelCentreCS);
	const FVector DamperMountWS = BodyToWorld.TransformPosition(DamperMountCS);
	const FVector CoiloverTopWS = BodyToWorld.TransformPosition(CoiloverTop_Rest);

	// Tangents: d(point)/d(theta) in world metres per radian. Direction says what
	// a force at that point can do to theta; magnitude is the moment arm.
	const FVector WheelTangent = BodyToWorld.TransformVector(WheelTangentAtAngle(ThetaDeg)) * WishboneCmToM; // t_W, Doc (W.19)
	const FVector DamperTangent = BodyToWorld.TransformVector(
		WishboneGeometry::OrbitTangent(LCA_F_Rest, DamperMountCS, LCA_HingeAxis)) * WishboneCmToM; // t_D, Doc (W.8)

	// Generalized inertia. |WheelTangent| is already the perpendicular distance
	// from the hinge axis, so this is m*r^2 with no extra bookkeeping.
	const float EffectiveInertia = UnsprungMassKg * WheelTangent.SizeSquared(); // I_eff, Doc (W.30)
	if (EffectiveInertia < MinEffectiveInertia)
	{
		return;
	}

	// ---------- Forces as real 3D world vectors ----------

	// GetGravityZ() is cm/s^2.
	const FVector GravityForce = FVector::UpVector * (GetWorld()->GetGravityZ() * WishboneCmToM) * UnsprungMassKg; // F_g, Doc (W.20)

	// Coilover axis: chassis mount -> damper mount, i.e. the direction the mount
	// travels when the strut extends. Length is measured in body space and only
	// the direction is taken in world.
	const FVector CoiloverDir = (DamperMountWS - CoiloverTopWS).GetSafeNormal(); // e_c, Doc (W.21)
	const float CoiloverLengthCm = FVector::Dist(DamperMountCS, CoiloverTop_Rest); // l_c, Doc (W.21)

	// Where the coil is right now. Subtracting a CONSTANT, so it tracks the strut
	// one-for-one - the rigid hardware cannot take a share of the travel.
	SpringLengthCm = CoiloverLengthCm - SpringMountingOffsetCm; // Doc (W.22a)

	// A coil squeezed shorter than its free length pushes the mount away from the
	// chassis; reach that length and it leaves its perch and carries nothing.
	//
	// Preload lives inside SpringFreeLengthCm rather than being added on after, so
	// it passes through the SAME can-only-push clamp. It must: as a bare added
	// force it would outlive the coil going slack and keep pushing at full droop
	// and in mid-air.
	const float CompressionM = (SpringFreeLengthCm - SpringLengthCm) * WishboneCmToM;
	SpringForceN = SpringRateNPerM * FMath::Max(CompressionM, 0.f);
	const FVector SpringForce = CoiloverDir * SpringForceN; // F_s, Doc (W.22)

	// Damper resists the strut's rate of change of length. Written against the
	// mount's own tangent rather than theta' directly, so the motion ratio is
	// picked up automatically and the term is guaranteed dissipative.
	const float StrutExtensionRate = FVector::DotProduct(DamperTangent * ThetaDotRad, CoiloverDir); // l_c-dot, Doc (W.23)
	DamperForceN = -DamperRateNsPerM * StrutExtensionRate;
	const FVector DamperForce = CoiloverDir * DamperForceN; // F_d, Doc (W.23)

	const float WheelTangentLength = WheelTangent.Size();
	MotionRatio = WheelTangentLength > KINDA_SMALL_NUMBER
		? FMath::Abs(FVector::DotProduct(DamperTangent, CoiloverDir)) / WheelTangentLength
		: 0.f; // MR, Doc (W.24)

	// Anti-roll bar. A spring on how far this corner has moved away from the MEAN
	// height of its axle, which is the whole of what a bar does: split any pair of
	// travels into a common part and an opposite part, and the bar only ever sees
	// the second. In pure ride both corners deviate by nothing and the bar
	// contributes nothing, however stiff it is.
	//
	// Against the mean rather than straight against the partner, and the factor of
	// two between them is not cosmetic. In pure roll this corner's deviation IS its
	// own travel, so k_a is the force per metre THIS wheel moves - the same thing a
	// spring's wheel rate means, giving the same K_roll = k_a*t^2/2. Differencing
	// the partner directly would double every deviation and quietly make a bar
	// twice as stiff as the number written on it.
	//
	// The rate is quoted at the wheel, so the force belongs at the wheel centre and
	// along the axis the travel was measured on: body up. Chassis-relative, not
	// world - the bar is bolted to the chassis and rolls with it.
	//
	// No damping term. A real bar is a steel torsion spring, and what damping it
	// has is bushing friction, not something worth a tunable.
	const float AxleMeanTravelCm = 0.5f * (WheelTravelCm + AntiRollPartnerTravelCm);
	AntiRollForceN = -AntiRollRateNPerM * ((WheelTravelCm - AxleMeanTravelCm) * WishboneCmToM); // F_a, Doc (W.28c)
	const FVector AntiRollForce = BodyToWorld.TransformVector(FVector::UpVector) * AntiRollForceN;

	// ---------- Bump rubbers, Doc (W.33) ----------
	//
	// Measured in WHEEL TRAVEL, not in theta, for the same reason the spring and the
	// bar are: a rate in N/m only means something against a distance, and degrees of
	// arm rotation are not one. The engage band is quoted in degrees because that is
	// how the travel limits themselves are written, and converted through the same
	// WheelTravelAtAngle the bar uses.
	BumpStopForceN = 0.f;
	float Q_BumpStop = 0.f;

	if (BumpStopRateNPerM > KINDA_SMALL_NUMBER)
	{
		const float BumpEdgeDeg = MaxArticulationAngleDeg - BumpStopEngageDeg;
		const float DroopEdgeDeg = MinArticulationAngleDeg + BumpStopEngageDeg;

		float OvertravelM = 0.f;
		float StopSign = 0.f;   // which way the rubber pushes theta

		if (ThetaDeg > BumpEdgeDeg)
		{
			OvertravelM = FMath::Abs(WheelTravelCm - WheelTravelAtAngle(BumpEdgeDeg)) * WishboneCmToM;
			StopSign = -1.f;
		}
		else if (ThetaDeg < DroopEdgeDeg)
		{
			OvertravelM = FMath::Abs(WheelTravelCm - WheelTravelAtAngle(DroopEdgeDeg)) * WishboneCmToM;
			StopSign = 1.f;
		}

		if (OvertravelM > 0.f)
		{
			// F = k*x*(1 + x/x0). Soft on the first millimetres, unyielding at the
			// end - which is what lets a rubber take a big hit without either
			// bottoming through or kicking the car back off the ground. A linear stop
			// at this rate is a wall from first touch.
			const float Progressive = OvertravelM
				* (1.f + (OvertravelM / FMath::Max(BumpStopProgressiveM, KINDA_SMALL_NUMBER)));

			// Speed the rubber is being crushed at. The wheel travels
			// |WheelTangent| metres per radian, so this is the wheel's own speed along
			// its path, and the sign convention falls out of which stop was hit.
			const float TangentLengthM = WheelTangent.Size();
			const float ClosingRate = -StopSign * ThetaDotRad * TangentLengthM;
			const float DampN = (ClosingRate > 0.f) ? (BumpStopDampingNsPerM * ClosingRate) : 0.f;

			BumpStopForceN = (BumpStopRateNPerM * Progressive) + DampN;

			// Acts at the WHEEL and along the wheel's own path, so it projects on the
			// wheel tangent rather than the damper's.
			Q_BumpStop = StopSign * BumpStopForceN * TangentLengthM;
		}
	}

	// ---------- Tyre vertical load ----------
	//
	// Deflection is measured along the ground normal, so a slope is handled
	// correctly even though the trace itself was vertical. This deflection is
	// rho_z - what a Magic Formula or brush tyre model needs as its input.
	FVector ContactForce = FVector::ZeroVector;
	ContactForceN = 0.f;
	TyreDeflectionCm = 0.f;
	bInContact = false;

	// Somewhere harmless to point when there is no contact; the force is zero, so
	// the reaction accumulator ignores it either way.
	ContactPointWS = WheelCentreWS;

	if (bGroundHit)
	{
		// Solved against the wheel's real orientation, so camber and a tilted
		// chassis both move the patch where they should - see SolveDiscContact. The
		// spindle costs an extra upper-arm solve, so it is only paid for when there
		// is ground to contact.
		const FVector SpindleWS = BodyToWorld.TransformVector(SpindleAxisAtAngle(ThetaDeg)).GetSafeNormal();
		const WishboneGeometry::FDiscContact Contact = WishboneGeometry::SolveDiscContact(
			WheelCentreWS, SpindleWS, WheelRadiusCm, GroundPointWS, GroundNormalWS);

		TyreDeflectionCm = Contact.DeflectionCm; // rho_z, Doc (W.25)

		if (TyreDeflectionCm > 0.f)
		{
			bInContact = true;

			// Where the tyre actually touches, not the wheel centre dropped straight
			// down. Deliberately not GroundPointWS either: that came from one vertical
			// trace at frame start, so it slides away from the wheel as theta moves,
			// and the tyre load is what sets the roll and pitch moments - it has to
			// act where the patch really is.
			ContactPointWS = Contact.PointWS; // C, Doc (W.26)

			// The whole ground force, straight from the tyre model - normal load and
			// in-plane in one vector. This corner no longer owns a vertical spring of
			// its own: it says how far the carcass is squashed (TyreDeflectionCm, pure
			// geometry) and the tyre says what that costs. See SetTyreContactForceN
			// for why the answer is a substep old, and for what happens with no tyre
			// attached.
			ContactForce = ExternalContactForceN; // N, Doc (W.28)

			// Published for the readouts. The normal component only - the in-plane
			// part is not a "load", and a reader comparing this against corner weight
			// wants the vertical number.
			ContactForceN = FVector::DotProduct(ContactForce, GroundNormalWS); // F_z, Doc (W.27)
		}
	}

	// ---------- Generalized forces ----------
	//
	// Each force is projected onto the tangent of ITS OWN point of application.
	// Gravity and tyre load act at (or below) the wheel centre; spring and damper
	// act on the arm at the damper mount. Projecting the coilover forces onto the
	// wheel tangent instead would quietly discard the motion ratio.
	//
	// The anti-roll bar goes on the WHEEL tangent, unlike the coilover beside it,
	// because its rate is already quoted at the wheel - see AntiRollRateNPerM. Put
	// it on the damper tangent and the motion ratio would be applied twice.
	const float Q_Gravity = FVector::DotProduct(GravityForce, WheelTangent);   // Q_g, Doc (W.29)
	const float Q_Contact = FVector::DotProduct(ContactForce, WheelTangent);   // Q_N, Doc (W.29)
	const float Q_Spring = FVector::DotProduct(SpringForce, DamperTangent);    // Q_s, Doc (W.29)
	const float Q_Damper = FVector::DotProduct(DamperForce, DamperTangent);    // Q_d, Doc (W.29)
	const float Q_AntiRoll = FVector::DotProduct(AntiRollForce, WheelTangent); // Q_a, Doc (W.29)

	const float ThetaAccel =
		(Q_Gravity + Q_Contact + Q_Spring + Q_Damper + Q_AntiRoll + Q_BumpStop) / EffectiveInertia; // theta-double-dot, Doc (W.31)

	// ---------- Semi-implicit Euler ----------
	// Velocity first, then position from the NEW velocity. Same cost as explicit
	// Euler and it does not pump energy into a stiff spring. Doc (W.32).
	const float ThetaDotBefore = ThetaDotRad;
	ThetaDotRad += ThetaAccel * SubstepTime;
	ThetaRad += ThetaDotRad * SubstepTime;

	// ---------- Reaction back on the chassis ----------
	// Before the travel stops, not after: the clamp kills theta' in a single step,
	// and differencing the wheel velocity across that would invent an a_wc of
	// essentially arbitrary size. See the class comment.
	AccumulateChassisReaction(SubstepTime, BodyToWorld, WheelCentreWS, WheelTangent, ThetaDotBefore,
		GravityForce, ContactForce, SpringForce + DamperForce, CoiloverTopWS);

	// ---------- Travel stops ----------
	//
	// Still here, but no longer the mechanism - the bump rubbers above are, and they
	// engage BumpStopEngageDeg before this. What is left is a backstop for the case
	// the rubbers cannot cover: a hit hard enough to cross the whole engage band
	// inside one substep, where no finite rate could have turned the wheel around in
	// time. Reaching this is a sign the stop wants more rate or the substep wants to
	// be shorter, not that the geometry has failed.
	//
	// It absorbs energy, which is exactly why it cannot be the only stop: a corner
	// that only ever clamps hands the chassis nothing, and the car falls through its
	// own suspension. Doc (W.33).
	const float MinRad = FMath::DegreesToRadians(MinArticulationAngleDeg);
	const float MaxRad = FMath::DegreesToRadians(MaxArticulationAngleDeg);

	if (ThetaRad <= MinRad)
	{
		ThetaRad = MinRad;
		ThetaDotRad = FMath::Max(ThetaDotRad, 0.f);
	}
	else if (ThetaRad >= MaxRad)
	{
		ThetaRad = MaxRad;
		ThetaDotRad = FMath::Min(ThetaDotRad, 0.f);
	}
}

// ---------------------------------------------------------------------------
// Chassis reaction
// ---------------------------------------------------------------------------

void UWishboneSuspension::AccumulateChassisReaction(float SubstepTime, const FTransform& BodyToWorld,
	const FVector& WheelCentreWS, const FVector& WheelTangentOld, float ThetaDotBefore,
	const FVector& GravityForce, const FVector& ContactForce,
	const FVector& CoiloverForceOnArm, const FVector& CoiloverTopWS)
{
	// ---------- Wheel-centre acceleration, m/s^2, relative to the body ----------
	//
	// By differencing the velocity across the step rather than evaluating
	// theta''*Wt + theta'^2*dWt/dtheta directly. The two agree to about 0.1%, this
	// costs one extra tangent instead of a second derivative, and - the reason it
	// matters - it captures the curvature term. On a knuckle-coupled path the wheel
	// centre does not travel a circle, and that term runs at roughly half the size
	// of the obvious tangential one, so dropping it is not a rounding error.
	FVector WheelAccel = FVector::ZeroVector;
	if (SubstepTime > SMALL_NUMBER)
	{
		const FVector WheelTangentNew = BodyToWorld.TransformVector(
			WheelTangentAtAngle(FMath::RadiansToDegrees(ThetaRad))) * WishboneCmToM;

		WheelAccel = ((WheelTangentNew * ThetaDotRad) - (WheelTangentOld * ThetaDotBefore)) / SubstepTime; // a_W, Doc (W.34), equiv. (W.35)
	}
	WheelAccelRelativeMPerSec2 = WheelAccel;

	// ---------- Diagnostic: what the strut tower carries ----------
	// Newton's third law on the coilover, which is a massless two-force member, so
	// its two ends carry equal and opposite loads along its own axis. Real, and NOT
	// applied - it cancels against the arm bushes. See the header. -F_c, Doc (W.40).
	CoiloverForceOnChassisN = -CoiloverForceOnArm;
	CoiloverForceOnChassisMagN = CoiloverForceOnArm.Size();
	CoiloverApplicationWS = CoiloverTopWS;

	// ---------- The wrench ----------
	// Tyre load at the patch, unsprung weight less unsprung inertia at the wheel
	// centre. Derived in the class comment; verified against an exactly-solved
	// constrained linkage, force and moment both.
	//
	// The anti-roll bar is absent here for the same reason the coilover is, one
	// step further out. Its force reaches the chassis twice: once through the arms
	// as +F and -F at the two wheel centres - a roll couple - and once through its
	// own bushes, which have to carry the opposite couple for the bar itself to be
	// in equilibrium. The two cancel exactly, because the model makes the pair
	// antisymmetric by construction: -k(z_a - z_b) and -k(z_b - z_a).
	//
	// That cancellation is the real result, not a modelling shortcut - a bar adds
	// NO direct roll moment. It resists roll by moving vertical load from the
	// inside tyre to the outside one, and that shows up here in ContactForce,
	// which is where it should.
	const FVector InertialForceN = GravityForce - (UnsprungMassKg * WheelAccel); // (F_g - m_u*a_W) at W, Doc (W.38)
	ChassisReactionForceN = ContactForce + InertialForceN; // R, Doc (W.39)

	// Everything above is a readout and is computed either way, so a corner running
	// as a pure observer still reports what it WOULD hand the body. The impulse
	// accumulators below are a different matter: only SolveSubstep clears them, so
	// filling them when nobody is going to drain them would let them grow without
	// bound - invisible, since nothing reads them, right up until someone does.

	// J, H accumulators - Doc (W.41).
	auto Accumulate = [this, SubstepTime](const FVector& ForceN, const FVector& PointWS)
	{
		const FVector ForceUU = ForceN * WishboneNToUU;
		ReactionImpulseUU += ForceUU * SubstepTime;

		// Moment about the centre of mass, which is the reference AddTorqueInRadians
		// works in. Force at the COM plus this torque is exactly AddForceAtPosition,
		// just split so the two halves can be time-averaged independently.
		ReactionAngularImpulseUU +=
			FVector::CrossProduct(PointWS - ReactionRefWS, ForceUU) * SubstepTime;
	};

	Accumulate(ContactForce, ContactPointWS);
	Accumulate(InertialForceN, WheelCentreWS);
}

void UWishboneSuspension::SolveSubstep(float SubstepTime, const FTransform& BodyToWorld,
	const FVector& BodyCOMWS)
{
	// Sweep mode drives theta by hand from the tick; there is nothing to integrate
	// and nothing meaningful to hand the body.
	if (!bGeometryReady || bDebugSweepTravel || SubstepTime <= 0.f)
	{
		return;
	}

	// Steer smoothing, here rather than in the tick. The tick moved to TG_PostPhysics
	// for the ground pass, and slewing there would have meant this frame's input not
	// reaching the wheel until next frame's substeps - a frame of steering latency
	// added for the sake of a debug draw. Run per substep it costs nothing and
	// arrives sooner.
	//
	// Rate is unchanged: applying Lerp(dt*k) across N substeps of a frame approaches
	// the same exponential as one Lerp(frame*k), for the small dt*k this uses.
	SteerAngleDeg = FMath::Lerp(SteerAngleDeg, TargetSteerAngleDeg,
		FMath::Clamp(SubstepTime * SteeringResponsiveness, 0.f, 1.f));

	// The ground, sampled HERE against the live body transform rather than reused
	// from the frame's opening tick.
	//
	// The tick pass is a frame stale by construction, and at 200 km/h a 60 Hz frame
	// is 0.93 m of travel - so the last substep of a frame was solving against a
	// plane sampled most of a metre behind the wheel. The contact solve re-ran every
	// substep against that stale plane, which made the geometry current and its
	// provenance not, and on a curving surface those are not the same thing.
	if (bSampleGroundPerSubstep)
	{
		UpdateGroundPlane(BodyToWorld);
	}

	ReactionRefWS = BodyCOMWS;
	ReactionWindowSeconds = SubstepTime;
	ReactionImpulseUU = FVector::ZeroVector;
	ReactionAngularImpulseUU = FVector::ZeroVector;

	// Subdivide the owner's substep so the corner keeps its own fixed step. If the
	// owner is already stepping finer than SubstepSeconds, take the owner's step -
	// never a coarser one, since coarser is the direction that loses stability, and
	// this also stops an owner substep shorter than SubstepSeconds from alternating
	// between zero internal steps and one.
	//
	// BodyToWorld is ONE snapshot for the whole call, deliberately, not resampled
	// per internal step: this call IS one AddCustomPhysics invocation for one
	// engine substep, and PhysX does not advance the body until every callback for
	// that substep has returned and its queued forces are applied (see
	// FPhysSubstepTask::SubstepInterpolation) - so BodyInstance's own state cannot
	// change between one internal 1 ms step and the next here regardless of how
	// often it is re-queried. The freshness that matters is that ACar re-samples
	// BodyToWorld once per CALL, which is once per actual engine substep - already
	// as fresh as the engine's own MaxSubstepDeltaTime allows.
	const float Step = FMath::Min(SubstepSeconds, SubstepTime);

	SubstepAccumulator += SubstepTime;

	int32 StepsTaken = 0;
	while (SubstepAccumulator >= Step && StepsTaken < MaxSubstepsPerFrame)
	{
		IntegrateSubstep(Step, BodyToWorld);
		SubstepAccumulator -= Step;
		++StepsTaken;
	}

	if (StepsTaken >= MaxSubstepsPerFrame)
	{
		// Hitch: drop the backlog rather than paying it off over later substeps.
		SubstepAccumulator = 0.f;
	}

	ArticulationAngleDeg = FMath::RadiansToDegrees(ThetaRad);
	UpdateSteeringGeometry(ArticulationAngleDeg, SteerAngleDeg);
}

void UWishboneSuspension::ApplyReactionToBody(FBodyInstance* BodyInstance)
{
	if (BodyInstance == nullptr || !bApplyReactionToChassis || ReactionWindowSeconds <= 0.f)
	{
		return;
	}

	// Handed over as the accumulated impulse and the window it covers. What happens
	// next differs by engine, and that difference is the whole reason this is a
	// function rather than two lines here - see VehiclePhysicsBody.
	VehiclePhysicsBody::ApplyLinearReaction(BodyInstance, ReactionImpulseUU, ReactionWindowSeconds);
	VehiclePhysicsBody::ApplyAngularReaction(BodyInstance, ReactionAngularImpulseUU, ReactionWindowSeconds);

	ReactionImpulseUU = FVector::ZeroVector;
	ReactionAngularImpulseUU = FVector::ZeroVector;
	ReactionWindowSeconds = 0.f;
}

FWheelContactFrame UWishboneSuspension::GetContactFrame(const FTransform& BodyToWorld, const FVector& BodyVelocityWS,
	const FVector& BodyAngularVelocityRad, const FVector& BodyCOMWS) const
{
	FWheelContactFrame Frame;
	Frame.bInContact = bInContact;
	if (!bInContact || !bGeometryReady)
	{
		return Frame;
	}
	
	const float ThetaDeg = FMath::RadiansToDegrees(ThetaRad);
	
	Frame.ContactPointWS = ContactPointWS;
	Frame.NormalWS = GroundNormalWS;
	Frame.DeflectionCm = TyreDeflectionCm;
	Frame.LoadN = ContactForceN;

	
	// ---------- Road-plane frame ----------
	// The wheel's own forward, carrying steer and camber through the knuckle
	// rotation, then flattened into the road plane so slip angles are measured
	// where the tyre actually is rather than where the body is pointing.
	const FVector WheelForwardWS = BodyToWorld.TransformVector(
		KnuckleRotationAtAngle(ThetaDeg).RotateVector(ForwardAxis_Rest));
	
	Frame.ForwardWS = (WheelForwardWS
		- Frame.NormalWS * FVector::DotProduct(WheelForwardWS, Frame.NormalWS)).GetSafeNormal();
	if (Frame.ForwardWS.IsNearlyZero())
	{
		// Wheel plane parallel to the road: no meaningful heading. Bail rather
		// than hand the tyre model a zero axis.
		Frame.bInContact = false;
		return Frame;
	}
	Frame.LateralWS = FVector::CrossProduct(Frame.NormalWS, Frame.ForwardWS);
	
	// ---------- Camber against the road ----------
	// The spindle is flipped to lie along +Lateral first, so a given physical
	// lean reports the same sign on both sides of the car.
	const FVector SpindleWS = BodyToWorld.TransformVector(SpindleAxisAtAngle(ThetaDeg)).GetSafeNormal();
	const FVector AxleRight = SpindleWS
		* FMath::Sign(FVector::DotProduct(SpindleWS, Frame.LateralWS));
	Frame.CamberRad = -FMath::Asin(
		FMath::Clamp(FVector::DotProduct(AxleRight, Frame.NormalWS), -1.f, 1.f));
	
	// ---------- Velocity of the material point at the patch ----------
	// Three terms, and the third is the one a raycast model cannot produce: the
	// wheel centre travels t_W*theta' relative to the body, and the patch rides
	// with it. UWheelCollider has only the first two, so its slip angle misses
	// the suspension's own motion entirely over bumps.
	const FVector SuspensionVelWS =
		BodyToWorld.TransformVector(WheelTangentAtAngle(ThetaDeg)) * ThetaDotRad;

	Frame.ContactVelocityWS = BodyVelocityWS
		+ FVector::CrossProduct(BodyAngularVelocityRad, Frame.ContactPointWS - BodyCOMWS)
		+ SuspensionVelWS;

	return Frame;
}