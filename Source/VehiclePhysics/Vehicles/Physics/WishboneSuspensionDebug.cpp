// Fill out your copyright notice in the Description page of Project Settings.

// Debug visualisation for UWishboneSuspension.
//
// Split from WishboneSuspension.cpp purely by size: the draw was 522 lines
// against ~450 for the whole of the physics, and reading either meant scrolling
// past the other. Nothing here participates in the simulation - every function
// is called from the post-physics tick and only reads state.
//
// It shares WishboneGeometry with the integrator on purpose, SolveDiscContact
// above all: the drawn contact patch is produced by the same call that loads it,
// so the two cannot disagree.

#include "WishboneSuspension.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

// ---------------------------------------------------------------------------
// Debug draw tick
// ---------------------------------------------------------------------------

void FWishboneDebugDrawTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType,
	ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	if (IsValid(Target))
	{
		FActorComponentTickFunction::ExecuteTickHelper(Target, /*bTickInEditor*/ false, DeltaTime, TickType,
			[this](float /*DilatedTime*/) { Target->DrawDebugPass(); });
	}
}

FString FWishboneDebugDrawTickFunction::DiagnosticMessage()
{
	return (Target != nullptr ? Target->GetFullName() : FString(TEXT("<null>"))) + TEXT("[WishboneDebugDraw]");
}

// ---------------------------------------------------------------------------
// Debug drawing
// ---------------------------------------------------------------------------

void UWishboneSuspension::DrawDebugPass()
{
	const USceneComponent* Body = GetBodyComponent();
	if (!bDrawDebug || !bGeometryReady || Body == nullptr)
	{
		return;
	}

	// TG_PostUpdateWork, so this transform is the one that gets rendered this frame.
	//
	// It is NOT the pose the solve ran in. That was true under 4.27, where physics
	// stepped inside the frame; under Chaos async the solver runs on its own clock and
	// this is the marshalled result, a measured 12.5 ms behind it. Anything built from
	// this transform is consistent with the visible car, and anything stored as an
	// absolute world point during the solve is not - see the sample correction in
	// DrawDebugGeometry.
	DrawDebugGeometry(Body->GetComponentTransform());
}

void UWishboneSuspension::DrawDebugGeometry(const FTransform& BodyToWorld)
{
	UWorld* World = GetWorld();

	// THE GROUND SAMPLES BROUGHT INTO THE POSE BEING DRAWN.
	//
	// Everything else here is built from BodyToWorld, so it lands wherever the body is
	// drawn. The ground samples cannot be - they are absolute world points, traced
	// against the terrain in whatever pose the solve was in. Those two used to be the
	// same instant. Under async physics they are not: the solver runs a measured 12.5 ms
	// ahead of the marshalled pose this receives, which is 28 cm at 80 km/h, so the
	// samples drew ahead of the wheel and outboard of it in a corner.
	//
	// Undoing it is one rigid transform: where the body has moved from the sample pose
	// to this one, move the samples with it. That is exact for the body and therefore
	// exact for the samples' position RELATIVE to the wheel, which is what the overlay
	// is read for - is the contact under the tyre, how far apart are the probes, is one
	// bridging a gap.
	//
	// It is not exact against the terrain: the ground did not move with the body, so a
	// sample's drawn height is off by however much the body rose in those 12.5 ms. Flat
	// ground, nothing; over a kerb, a few millimetres. The alternative - drawing the
	// whole overlay in the solve pose - is exact against the terrain and puts the entire
	// suspension 28 cm ahead of the car instead, which trades a small error for a
	// confusing picture.
	const FTransform SampleToDrawn = GroundSamplePoseWS.GetRotation().IsIdentity() && GroundSamplePoseWS.GetLocation().IsZero()
		? FTransform::Identity                                    // never solved yet
		// Order: FTransform's C = A * B applies A THEN B, so this is "un-apply the pose
		// the sample was traced in, then apply the pose being drawn" - not the reverse,
		// which would move the samples further out rather than back.
		: GroundSamplePoseWS.Inverse() * BodyToWorld;

	TArray<FVector, TInlineAllocator<16>> DrawnSampleHits;
	TArray<FVector, TInlineAllocator<16>> DrawnSampleStarts;
	TArray<FVector, TInlineAllocator<16>> DrawnSampleBridged;
	DrawnSampleHits.Reserve(GroundSampleHitsWS.Num());
	DrawnSampleStarts.Reserve(GroundSampleStartsWS.Num());
	DrawnSampleBridged.Reserve(GroundSampleBridgedWS.Num());
	for (const FVector& V : GroundSampleHitsWS)    { DrawnSampleHits.Add(SampleToDrawn.TransformPosition(V)); }
	for (const FVector& V : GroundSampleStartsWS)  { DrawnSampleStarts.Add(SampleToDrawn.TransformPosition(V)); }
	for (const FVector& V : GroundSampleBridgedWS) { DrawnSampleBridged.Add(SampleToDrawn.TransformPosition(V)); }

	// The fitted plane's own point is a solve-time world value for the same reason.
	const FVector DrawnGroundPointWS = SampleToDrawn.TransformPosition(GroundPointWS);
	if (World == nullptr)
	{
		return;
	}

	const float Theta = ArticulationAngleDeg;

	// Everything below is drawn from the SAME geometry the physics integrates, so
	// a marker sitting where the mesh does not means the hardpoints are wrong -
	// the drawing cannot disagree with the simulation by construction.
	//
	// Resolved ONCE here and threaded through every section, rather than each one
	// re-running the upper-arm solve for the points it happens to want. It is also
	// the same query the editor visualizer draws from, so the viewport and play
	// cannot disagree either.
	const FWishbonePose LivePose = GetPose(Theta, SteerAngleDeg);
	if (!LivePose.bValid)
	{
		return;
	}

	auto ToWorld = [&BodyToWorld](const FVector& PointCS) { return BodyToWorld.TransformPosition(PointCS); };

	const uint8 Fg = 0;

	// ---------- Palette ----------
	//
	// One hue per SUBSYSTEM, used identically for every marker, line and label
	// that belongs to it - a point and the line reaching it are always the same
	// colour, so the eye groups by subsystem rather than by individual element.
	//
	// Taken from WishbonePalette, which the EDITOR visualizer draws from too, so a
	// corner cannot be one set of colours in the viewport and another in play. Same
	// numbers as the reference doc's CSS.
	//
	// UE4's debug line-batcher does not support alpha blending - Color.A is ignored,
	// everything draws fully opaque - so the "is this the important thing or just
	// context" job that transparency would normally do is carried by
	// MarkerRadius/LineWidth below instead: live, meaningful elements are drawn
	// bigger and bolder, reference-only ones (rest pose, travel arcs) thinner.
	const FColor ChassisColour  = WishbonePalette::Chassis;
	const FColor ArmColour      = WishbonePalette::Arm;
	const FColor KingpinColour  = WishbonePalette::Kingpin;
	const FColor CoiloverColour = WishbonePalette::Coilover;
	const FColor WheelColour    = WishbonePalette::Wheel;
	const FColor ContactColour  = WishbonePalette::Contact;

	// Deliberately outside the palette and outside the hue scheme: the tangents are
	// kinematic DIRECTIONS, the same kind of thing as each other rather than part of
	// any subsystem, so they share one neutral language instead of competing for a
	// hue. Not in WishbonePalette because the editor draws no tangents.
	const FColor TangentColour(220, 220, 220);

	// Live, meaningful markers/lines vs reference-only ones - since colour and
	// alpha cannot carry that distinction here, size and weight do.
	const float LiveMarkerRadius  = 1.3f;
	const float FixedMarkerRadius = 1.1f;
	// 6 segments read as faceted/spiky at close range rather than round - a
	// wireframe sphere is three orthogonal rings, and a hexagonal ring has
	// visible points. This is plenty for round without costing much.
	const int32 SphereSegments = 14;
	const float LiveLineWidth  = 0.8f;
	const float FixedLineWidth = 0.6f;

	// Same hue as the live element, just darker - "this is the envelope it sweeps
	// through, not where it is right now" - rather than an unrelated colour. Shared
	// with the editor visualizer for the same reason the palette is.
	auto Dim = [](const FColor& C, float Factor) { return WishbonePalette::Dim(C, Factor); };

	// A tangent (d(point)/d(theta)) is a cross product against the hinge axis,
	// and mirroring a corner negates one axis of everything that feeds it - which
	// flips the tangent's OTHER out-of-plane component (a right-hand-rule
	// consequence of reflecting through a single axis, not a mistake in either
	// corner's geometry). bPositiveThetaIsBump already tracks this exact flip for
	// the physics, since theta's own sign convention flips the same way; reusing
	// it here makes a tangent arrow point toward bump on both sides, so "up"
	// means the same thing regardless of which corner you are looking at.
	auto TowardBump = [this](const FVector& RawTangent)
	{
		return (bPositiveThetaIsBump ? RawTangent : -RawTangent).GetSafeNormal();
	};

	// ---------- Labels ----------
	//
	// Short symbols matching the reference doc exactly, so a point on screen and
	// a point on the page are never in doubt. Offset above the marker so the text
	// does not sit on top of it.
	//
	// Routes through AHUD::PostRender, which only runs while an actual
	// APlayerController is driving a HUD - i.e. in Play, not in a bare editor
	// viewport or Simulate. Every other marker here is a ULineBatchComponent
	// primitive and has no such requirement, which is why labels can go missing
	// while everything else still renders.
	auto AddLabel = [&](const FVector& WorldPos, const FString& Symbol, const FColor& Colour)
	{
		if (bDrawLabels)
		{
			DrawDebugString(World, WorldPos + FVector::UpVector * 3.f, Symbol, nullptr, Colour, 0.f, true, 0.9f);
		}
	};

	// ---------- Chassis-side hardpoints: fixed, never articulate ----------
	if (bDrawHardpoints)
	{
		// Chassis-fixed, so any pose carries them - they do not move with theta.
		const FVector LcaFWS = LivePose.LcaFront;
		const FVector LcaBWS = LivePose.LcaRear;
		const FVector UcaFWS = LivePose.UcaFront;
		const FVector UcaBWS = LivePose.UcaRear;
		const FVector CoiloverTopWSForLabel = LivePose.CoiloverTop;

		for (const FVector& Bush : { LcaFWS, LcaBWS, UcaFWS, UcaBWS })
		{
			DrawDebugSphere(World, Bush, FixedMarkerRadius, SphereSegments, ChassisColour, false, -1.f, Fg, FixedLineWidth);
		}
		DrawDebugSphere(World, CoiloverTopWSForLabel, FixedMarkerRadius, SphereSegments, CoiloverColour, false, -1.f, Fg, FixedLineWidth);

		AddLabel(LcaFWS, TEXT("F_L"), ChassisColour);
		AddLabel(LcaBWS, TEXT("B_L"), ChassisColour);
		AddLabel(UcaFWS, TEXT("F_U"), ChassisColour);
		AddLabel(UcaBWS, TEXT("B_U"), ChassisColour);
		AddLabel(CoiloverTopWSForLabel, TEXT("T"), CoiloverColour);

		// Hinge axes, extended past the bushes so the rotation axis is unmistakable.
		const FVector LcaAxisWS = BodyToWorld.TransformVector(LCA_HingeAxis).GetSafeNormal();
		const FVector UcaAxisWS = BodyToWorld.TransformVector(UCA_HingeAxis).GetSafeNormal();
		DrawDebugLine(World, LcaFWS - LcaAxisWS * 8.f, LcaBWS + LcaAxisWS * 8.f, ChassisColour, false, -1.f, Fg, FixedLineWidth);
		DrawDebugLine(World, UcaFWS - UcaAxisWS * 8.f, UcaBWS + UcaAxisWS * 8.f, ChassisColour, false, -1.f, Fg, FixedLineWidth);
	}

	// ---------- The corner at a given angle ----------
	//
	// Every point comes from GetPose, the same query the editor visualizer draws
	// from, so the two cannot end up disagreeing about where the linkage is. It
	// also carries bUpperArmSolveFailed, which this draw had no way to report
	// before - a kingpin the solve could not satisfy was drawn at the wrong length
	// with nothing to say so.
	// Takes the pose rather than an angle so the live call can reuse LivePose above
	// instead of solving the same linkage twice. AngleDeg comes along only for the
	// damper tangent, which is the one thing here the pose does not carry.
	auto DrawCornerAtPose = [&](const FWishbonePose& Pose, float AngleDeg, bool bGhost)
	{
		if (!Pose.bValid)
		{
			return;
		}

		// A dimmed hue, not a flat grey. Grey threw the subsystem colour away, so a
		// ghost arm and a ghost strut were indistinguishable - and it contradicted
		// the rule Dim() states and the travel arcs below already follow.
		const float Ghosting = 0.45f;
		const FColor ThisArmColour = bGhost ? Dim(ArmColour, Ghosting) : ArmColour;
		const FColor ThisKingpinColour = bGhost ? Dim(KingpinColour, Ghosting) : KingpinColour;
		const FColor ThisStrutColour = bGhost ? Dim(CoiloverColour, Ghosting) : CoiloverColour;
		const float Thickness = bGhost ? 0.5f : LiveLineWidth;

		const FVector LcaU = Pose.LowerBallJoint;
		const FVector Mount = Pose.DamperMount;
		const FVector Wheel = Pose.WheelCentre;

		// Lower arm triangle, plus where the coilover picks up on it.
		DrawDebugLine(World, Pose.LcaFront, LcaU, ThisArmColour, false, -1.f, Fg, Thickness);
		DrawDebugLine(World, Pose.LcaRear, LcaU, ThisArmColour, false, -1.f, Fg, Thickness);
		DrawDebugLine(World, LcaU, Mount, ThisArmColour, false, -1.f, Fg, Thickness);

		// Upper arm at its SOLVED angle, not the lower arm's - using the lower
		// arm's angle is what makes the knuckle stretch through travel.
		if (bHasUpperArm)
		{
			const FVector UcaU = Pose.UpperBallJoint;
			DrawDebugLine(World, Pose.UcaFront, UcaU, ThisArmColour, false, -1.f, Fg, Thickness);
			DrawDebugLine(World, Pose.UcaRear, UcaU, ThisArmColour, false, -1.f, Fg, Thickness);

			// Red when the rigid-knuckle solve could not be satisfied: the line being
			// drawn is then NOT KingpinLengthCm long, which means the travel limits are
			// wider than the geometry allows.
			const FColor KingpinDrawColour = Pose.bUpperArmSolveFailed
				? Dim(ContactColour, bGhost ? Ghosting : 1.f)
				: ThisKingpinColour;
			DrawDebugLine(World, LcaU, UcaU, KingpinDrawColour, false, -1.f, Fg, Thickness);

			if (!bGhost)
			{
				DrawDebugSphere(World, UcaU, LiveMarkerRadius, SphereSegments, ArmColour, false, -1.f, Fg, LiveLineWidth);
				AddLabel(UcaU, TEXT("U"), ArmColour);
			}
		}

		const FVector StrutTopWS = Pose.CoiloverTop;
		DrawDebugLine(World, StrutTopWS, Mount, ThisStrutColour, false, -1.f, Fg, bGhost ? 0.5f : 1.f);

		// ---------- Coil versus rigid mounts ----------
		//
		// Recomputed from the geometry being DRAWN rather than read off the live
		// members, so it cannot disagree with the strut line above it - the same
		// reason the contact patch is re-derived at draw time.
		if (bDrawSpring && !bGhost)
		{
			// Measured off the pose's world points rather than the body-space ones. A
			// distance survives a rigid transform unchanged, and this file already
			// assumes the body is unscaled, so it is the same number from one source
			// instead of two.
			const float DrawnStrutCm = FVector::Dist(Mount, StrutTopWS);
			const FVector StrutDirWS = (Mount - StrutTopWS).GetSafeNormal();

			// Shift the whole breakdown sideways, or every length lands on the strut
			// line and on top of each other. Crossed with the hinge axis rather than
			// taking an arbitrary perpendicular, so it comes out lateral (the hinge
			// runs fore-aft) instead of pointing somewhere different each frame.
			FVector SideDirWS = FVector::CrossProduct(
				StrutDirWS, BodyToWorld.TransformVector(LCA_HingeAxis).GetSafeNormal()).GetSafeNormal();
			if (SideDirWS.IsNearlyZero())
			{
				FVector Unused;
				StrutDirWS.FindBestAxisVectors(SideDirWS, Unused);
			}

			// Force it outboard on both sides. The cross product's sign flips with
			// the mirrored geometry, which would otherwise put the bar inboard - and
			// through the chassis - on one side of the car only.
			const FVector OutboardWS = BodyToWorld.TransformVector(
				FVector(0.f, WheelCentre_Rest.Y >= 0.f ? 1.f : -1.f, 0.f)).GetSafeNormal();
			if (FVector::DotProduct(SideDirWS, OutboardWS) < 0.f)
			{
				SideDirWS = -SideDirWS;
			}

			const FVector Shift = SideDirWS * SpringDrawOffsetCm;
			const FVector BarTopWS = StrutTopWS + Shift;
			const FVector BarMountWS = Mount + Shift;

			// Tie the bar back to the strut it describes, so the two read as one
			// thing rather than a stray line floating beside the corner.
			const FColor TieColour = Dim(CoiloverColour, 0.35f);
			DrawDebugLine(World, StrutTopWS, BarTopWS, TieColour, false, -1.f, Fg, 0.5f);
			DrawDebugLine(World, Mount, BarMountWS, TieColour, false, -1.f, Fg, 0.5f);

			// Mounts at the chassis end - a drawing convention; only the total
			// offset is modelled. The coil is whatever is left, down to the mount.
			const FVector CoilTopWS = BarTopWS + StrutDirWS * SpringMountingOffsetCm;

			if (SpringMountingOffsetCm > KINDA_SMALL_NUMBER)
			{
				// Dimmed and thin: this length cannot compress.
				DrawDebugLine(World, BarTopWS, CoilTopWS, Dim(CoiloverColour, 0.45f),
					false, -1.f, Fg, 1.5f);
				AddLabel(BarTopWS + StrutDirWS * (SpringMountingOffsetCm * 0.5f),
					TEXT("mounts"), Dim(CoiloverColour, 0.7f));
			}

			// The coil itself, fat enough to read as the compliant part.
			DrawDebugLine(World, CoilTopWS, BarMountWS, CoiloverColour, false, -1.f, Fg, 3.f);
			AddLabel(CoilTopWS, TEXT("coil"), CoiloverColour);

			// Where the mount would sit with the coil relaxed. Beyond this the
			// spring is slack, so the gap is the droop left before the corner stops
			// carrying load - and once the mount passes it, it reads as overshoot.
			const FVector FreeEndWS = CoilTopWS + StrutDirWS * SpringFreeLengthCm;
			const bool bSlack = DrawnStrutCm - SpringMountingOffsetCm >= SpringFreeLengthCm;
			const FColor FreeMarkColour = bSlack ? Dim(CoiloverColour, 0.5f) : CoiloverColour;

			DrawDebugLine(World, FreeEndWS - SideDirWS * 2.5f, FreeEndWS + SideDirWS * 2.5f,
				FreeMarkColour, false, -1.f, Fg, 2.f);

			// Only worth drawing while there is a gap to show.
			if (!bSlack)
			{
				DrawDebugLine(World, BarMountWS, FreeEndWS, Dim(CoiloverColour, 0.55f),
					false, -1.f, Fg, 0.5f);
			}

			AddLabel(FreeEndWS, bSlack ? TEXT("slack") : TEXT("free"), FreeMarkColour);
		}

		if (!bGhost)
		{
			DrawDebugSphere(World, LcaU, LiveMarkerRadius, SphereSegments, ArmColour, false, -1.f, Fg, LiveLineWidth);
			DrawDebugSphere(World, Mount, LiveMarkerRadius, SphereSegments, CoiloverColour, false, -1.f, Fg, LiveLineWidth);
			DrawDebugSphere(World, Wheel, LiveMarkerRadius, SphereSegments, WheelColour, false, -1.f, Fg, LiveLineWidth);

			AddLabel(LcaU, TEXT("L"), ArmColour);
			AddLabel(Mount, TEXT("D"), CoiloverColour);
			AddLabel(Wheel, TEXT("W"), WheelColour);

			// Hub arm. Rigid, so its length and its angle to the kingpin must not
			// change through travel - the quickest check that the wheel is mounted
			// to the knuckle rather than dragged around by the lower arm. Same
			// colour as the wheel centre it reaches - they are one subsystem.
			DrawDebugLine(World, LcaU, Wheel, WheelColour, false, -1.f, Fg, Thickness);

			// Damper-mount tangent - t_D, the direction Q_Spring/Q_Damper are
			// projected against (W.29). Same neutral colour as the wheel tangent
			// below: both are the SAME kind of thing, a kinematic direction rather
			// than a subsystem, so they share one visual language instead of
			// competing for a hue.
			//
			// The only geometry here still computed outside the pose, and deliberately
			// so: a tangent is a central difference, so carrying the two of them in
			// FWishbonePose would triple what every GetPose costs - including the 25
			// the travel arcs make, none of which want a tangent.
			const FVector DamperTangentWS = BodyToWorld.TransformVector(WishboneGeometry::OrbitTangent(
				LCA_F_Rest,
				WishboneGeometry::OrbitPoint(LCA_F_Rest, DamperMount_Rest, LCA_HingeAxis, AngleDeg),
				LCA_HingeAxis));
			DrawDebugDirectionalArrow(World, Mount, Mount + TowardBump(DamperTangentWS) * 12.f,
				3.f, TangentColour, false, -1.f, Fg, 0.6f);
		}
	};

	if (bDrawRestPose)
	{
		// Rest TRAVEL but live steer, so the ghost isolates what articulation did and
		// does not also swing away under steering input.
		DrawCornerAtPose(GetPose(0.f, SteerAngleDeg), 0.f, /*bGhost*/ true);
	}
	if (bDrawHardpoints)
	{
		DrawCornerAtPose(LivePose, Theta, /*bGhost*/ false);
	}

	// ---------- Travel arcs: the actual paths, min limit to max limit ----------
	if (bDrawTravelArcs)
	{
		const int32 Segments = 24;

		// One pose per step yields every arc at once. Of the four point-at-angle
		// lambdas this replaced, two ran the upper-arm solve (the wheel centre and the
		// upper ball joint) and two did not, so 25 steps cost 50 solves where they now
		// cost 25. Also the path the editor visualizer walks.
		//
		// Dimmed versions of the SAME hue the live point uses, rather than an
		// unrelated colour - "this is the swept envelope" reads from brightness, not
		// from having to learn a fourth shade of green/orange/yellow.
		const FColor WheelArcColour = Dim(WheelColour, 0.55f);
		const FColor ArmArcColour = Dim(ArmColour, 0.55f);
		const FColor StrutArcColour = Dim(CoiloverColour, 0.55f);

		FWishbonePose Previous = GetPose(MinArticulationAngleDeg, SteerAngleDeg);
		for (int32 i = 1; i <= Segments; ++i)
		{
			const float Alpha = static_cast<float>(i) / static_cast<float>(Segments);
			const FWishbonePose Current = GetPose(
				FMath::Lerp(MinArticulationAngleDeg, MaxArticulationAngleDeg, Alpha), SteerAngleDeg);

			if (Previous.bValid && Current.bValid)
			{
				DrawDebugLine(World, Previous.WheelCentre, Current.WheelCentre, WheelArcColour, false, -1.f, Fg, 0.5f);
				DrawDebugLine(World, Previous.LowerBallJoint, Current.LowerBallJoint, ArmArcColour, false, -1.f, Fg, 0.5f);
				DrawDebugLine(World, Previous.DamperMount, Current.DamperMount, StrutArcColour, false, -1.f, Fg, 0.5f);
				if (bHasUpperArm)
				{
					DrawDebugLine(World, Previous.UpperBallJoint, Current.UpperBallJoint, ArmArcColour, false, -1.f, Fg, 0.5f);
				}
			}
			Previous = Current;
		}
	}

	// ---------- Tyre, spindle, ground trace, contact ----------
	//
	// Either toggle brings the block in, because the contact solve at the top of it
	// feeds both halves - the disc needs the wheel pose and the arrows need the patch
	// that pose resolves to.
	if (bDrawWheelDisc || bDrawContactForces)
	{
		// From the live pose, so the wheel is bolted to exactly the knuckle the arms
		// above were drawn from. The pose carries the wheel-plane basis too - the
		// wheel is drawn in its OWN plane, perpendicular to the live spindle, rather
		// than as a screen-aligned silhouette, which is what makes camber visible.
		// That lean IS camber gain.
		const FVector WheelWS = LivePose.WheelCentre;
		const FVector SpindleWS = LivePose.SpindleAxis;
		const FVector WheelPlaneU = LivePose.WheelPlaneU;
		const FVector WheelPlaneV = LivePose.WheelPlaneV;

		// ContactPointWS (the member) is a world point cached mid-substep, so it is
		// stale by whatever the body moved after the substep that wrote it - while
		// everything else here is rebuilt through THIS frame's transform. That
		// mismatch is why the patch used to trail the rig it should sit under, worse
		// the faster the car went, even though the rig itself never lagged.
		//
		// So re-solve it here against THIS frame's wheel and spindle, through the
		// very same function the integrator uses - which is also what keeps the
		// drawn patch honest about camber and slope rather than quietly reverting
		// to the old under-the-hub approximation.
		const FVector DrawnContactPointWS = WishboneGeometry::SolveDiscContact(
			WheelWS, SpindleWS, WheelRadiusCm, GroundPointWS, GroundNormalWS).PointWS;

		// The tyre silhouette is a reference outline, not a functional element - it
		// exists to show camber, not to be looked AT - so it is dimmed the same way
		// the rest-pose ghost and the travel arcs are, rather than fighting the small
		// markers for the same saturated green.
		if (bDrawWheelDisc)
		{
			// Rim and a tread line inboard of it, so the outline reads as a wheel with
			// a width rather than as a hoop.
			DrawDebugCircle(World, WheelWS, WheelRadiusCm, 40, Dim(WheelColour, 0.6f), false, -1.f, Fg, 0.5f,
				WheelPlaneU, WheelPlaneV, /*bDrawAxis*/ false);
			DrawDebugCircle(World, WheelWS, WheelRadiusCm * 0.62f, 28, Dim(WheelColour, 0.35f), false, -1.f, Fg, 0.5f,
				WheelPlaneU, WheelPlaneV, /*bDrawAxis*/ false);

			// Spokes, turned by the shaft's own angle. A circle cannot show rotation -
			// it is symmetric, so a locked wheel and one at 2000 rpm draw identically,
			// and the slip ratio is the difference between them. Five is deliberate:
			// an odd count means the pattern never looks stationary under a strobing
			// frame rate the way an even one can.
			//
			// NOT drawn in WheelPlaneU/V. Those come from SpindleAxis.FindBestAxisVectors,
			// which returns *some* orthonormal pair perpendicular to the spindle and
			// promises nothing about which way round it is - and the spindle itself
			// mirrors between the two sides of the car. Handedness therefore flipped
			// across the car, so a positive shaft speed spun the left wheels forward
			// and the right wheels backward. The circle did not care; the spokes did.
			//
			// So the basis is rebuilt from something physical instead. The axle is
			// forced to point to the car's RIGHT on both sides, and then forward
			// rolling is a positive rotation about it on both sides: with the axle
			// along +Y and the top of the wheel at +Z, cross(+Y, +Z) is +X, so the
			// top of the wheel advances as the angle grows. That is what rolling
			// forward means, and it now means it on both sides.
			const FVector BodyRightWS = BodyToWorld.GetUnitAxis(EAxis::Y);
			const FVector AxleRightWS = SpindleWS
				* FMath::Sign(FVector::DotProduct(SpindleWS, BodyRightWS));

			FVector SpokeUp = BodyToWorld.GetUnitAxis(EAxis::Z);
			SpokeUp = (SpokeUp - (AxleRightWS * FVector::DotProduct(SpokeUp, AxleRightWS))).GetSafeNormal();
			if (SpokeUp.IsNearlyZero())
			{
				SpokeUp = WheelPlaneU;   // axle straight up: a nonsense rig, but do not divide by it
			}
			const FVector SpokeFwd = FVector::CrossProduct(AxleRightWS, SpokeUp);

			const int32 SpokeCount = 5;
			for (int32 SpokeIndex = 0; SpokeIndex < SpokeCount; ++SpokeIndex)
			{
				const float SpokeAngle = DebugWheelSpinRad
					+ ((2.f * PI * SpokeIndex) / SpokeCount);
				const FVector Dir = (SpokeUp * FMath::Cos(SpokeAngle))
					+ (SpokeFwd * FMath::Sin(SpokeAngle));

				// ONE spoke is bright and full length - the valve stem. Direction of
				// rotation is ambiguous from a symmetric pattern, and a single marked
				// spoke resolves it, as well as making a slow creep visible when five
				// identical spokes would just shimmer.
				const bool bMarker = (SpokeIndex == 0);
				DrawDebugLine(World,
					WheelWS + (Dir * (bMarker ? 0.f : WheelRadiusCm * 0.62f)),
					WheelWS + (Dir * WheelRadiusCm),
					bMarker ? WheelColour : Dim(WheelColour, 0.45f),
					false, -1.f, Fg, bMarker ? 1.4f : 0.6f);
			}
		}

		// Same colour as the wheel centre and the hub arm - spindle, hub arm and W
		// are all one subsystem, the knuckle.
		DrawDebugLine(World, WheelWS - SpindleWS * WheelRadiusCm * 0.6f,
			WheelWS + SpindleWS * WheelRadiusCm * 0.6f, WheelColour, false, -1.f, Fg, LiveLineWidth);

		// Instantaneous direction of travel - the tangent forces project onto.
		// Flipped toward bump (see TowardBump) so "up" means the same thing on
		// both sides of the car - the raw d(WheelCentre)/d(theta) points toward
		// INCREASING theta, and theta's own "which sign is bump" convention is
		// deliberately opposite between mirrored corners.
		const FVector TangentWS = BodyToWorld.TransformVector(WheelTangentAtAngle(Theta));
		DrawDebugDirectionalArrow(World, WheelWS, WheelWS + TowardBump(TangentWS) * 16.f,
			4.f, TangentColour, false, -1.f, Fg, 0.6f);

		// The raw once-per-frame ground trace (GroundPointWS/GroundNormalWS) is not
		// drawn. It is still used for the actual contact math below - only its own
		// dedicated marker is gone, since it added a second dot next to the live
		// patch with no benefit for tuning the corner.

		// ---------- The wrench actually handed to the chassis ----------
		//
		// The tyre load's application point, drawn where the force is really
		// applied, so a wrong load path is visible rather than inferred. Should sit
		// under the wheel centre along the ground normal and track it through
		// travel. The other half of the wrench - the inertial term at the wheel
		// centre - is small enough at rest that it draws as a bare arrowhead with
		// no visible shaft, which read as a stray marker rather than a vector; it is
		// reported numerically in the on-screen readout instead (see below) rather
		// than drawn here.
		if (bInContact && bDrawContactForces)
		{
			// Newtons to a drawable length. Arbitrary, but shared with the coilover
			// diagnostic arrow below so their relative sizes mean something.
			const float ForceDrawScale = 0.01f;

			// Tyre load at the patch - the term that carries the car. The one red
			// thing on screen; its presence at all already says "loaded". Drawn at
			// DrawnContactPointWS, not the member - see the comment above.
			DrawDebugSphere(World, DrawnContactPointWS, 2.f, SphereSegments, ContactColour, false, -1.f, Fg, LiveLineWidth);
			// THE FORCE THAT IS ACTUALLY APPLIED, along its own direction.
			//
			// This drew GroundNormalWS * ContactForceN - the road normal, hardcoded -
			// so it could not move whatever the tyre did with the load. That hid a
			// working toggle well enough to look broken, and the toggle has since been
			// removed as a bad idea in its own right; the drawing stays, because a
			// picture of the force that is actually applied beats a reconstruction of
			// it whether or not anything can change the axis.
			//
			// ExternalContactForceN is the vector the tyre handed over and the same one
			// IntegrateSubstep applies, so drawing it is drawing the thing itself
			// rather than a reconstruction of it. It carries Fx and Fy as well as the
			// normal load, which is why it now leans under traction and braking - that
			// lean is real, and was invisible before.
			DrawDebugDirectionalArrow(World, DrawnContactPointWS,
				DrawnContactPointWS + (ExternalContactForceN * ForceDrawScale),
				6.f, ContactColour, false, -1.f, Fg, 1.2f);

			// A short reference stub along the road normal. Without something to read
			// the angle AGAINST, a tilted arrow just looks like an arrow - which is the
			// same reason the hardcoded version went unnoticed for as long as it did.
			DrawDebugLine(World, DrawnContactPointWS,
				DrawnContactPointWS + (GroundNormalWS * 10.f),
				Dim(ContactColour, 0.45f), false, -1.f, Fg, FixedLineWidth);
			// THE ANGLE, ON SCREEN, because "the arrow points the wrong way" is not
			// answerable by looking at an arrow in a perspective view - and there are two
			// red things in this drawing, this and the body trail, which makes it worse
			// rather than better.
			//
			// The normal load rides the road normal, so a loaded tyre on flat ground
			// reads a few degrees at most: Fx and Fy tilt it and nothing else should.
			// Past about 45 degrees the in-plane forces exceed the normal load, which is
			// a tyre being asked for more than it has rather than a drawing fault.
			const float ForceN = ExternalContactForceN.Size();
			const float FromNormalDeg = (ForceN > 1.f)
				? FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(
					FVector::DotProduct(ExternalContactForceN / ForceN, GroundNormalWS), -1.f, 1.f)))
				: 0.f;

			AddLabel(DrawnContactPointWS,
				FString::Printf(TEXT("C %.0fN %.0f deg off normal"), ForceN, FromNormalDeg),
				ContactColour);

			// Strut-tower load. Same hue as the rest of the coilover family - it IS
			// the coilover, read from its other end - but thin, to mark it as a
			// DIAGNOSTIC, not part of the applied wrench (see the class comment).
			//
			// Starts a little short of T along the coilover's own axis rather than
			// exactly AT it: T's marker sphere would otherwise swallow this arrow's
			// base whenever the force is small, since the two would be concentric.
			// A vector that legitimately has near-zero magnitude (spring near its
			// free length - see the class comment on SpringForceN) should still be
			// visible as a short, offset stub, not disappear into the marker.
			if (!CoiloverApplicationWS.IsNearlyZero() && !CoiloverForceOnChassisN.IsNearlyZero())
			{
				const FVector DiagnosticDir = CoiloverForceOnChassisN.GetSafeNormal();
				const FVector DiagnosticStart = CoiloverApplicationWS + DiagnosticDir * (FixedMarkerRadius + 1.5f);
				DrawDebugDirectionalArrow(World, DiagnosticStart,
					DiagnosticStart + CoiloverForceOnChassisN * ForceDrawScale,
					4.f, CoiloverColour, false, -1.f, Fg, 0.5f);
			}
		}
	}

	// ---------- Ground sampling ----------
	//
	// Its own hue rather than one of the six corner colours, because the palette
	// groups by subsystem and this is the ROAD - not a part of the linkage.
	// Gated on CONTACT, not on the trace finding something. A wheel in the air still
	// has ground under it and the trace still reports it - and drawn, that reads as
	// contact however it is styled. Dimming the plane was not enough; nor was hiding
	// the plane and leaving the samples, because dots under a wheel look like a
	// wheel on the ground. So the whole group goes, and only the gap is labelled.
	if (bDrawGroundSamples && bGroundHit && (bInContact || bDrawGroundWhenAirborne))
	{
		const FColor GroundColour(80, 220, 230);

		// WHERE TO DRAW THE PLANE. Not at GroundPointWS, which is why the plane
		// appeared to lag the wheel it belongs to.
		//
		// The tick group was never the problem - this draw already runs at
		// TG_PostUpdateWork, after the body is synced. The lag is in the DATA, and it
		// is one substep of it: UpdateGroundPlane samples at the START of a substep,
		// from the body transform passed in, and the substep then integrates the body
		// forward. So by the time anything is drawn the wheel has moved on by a
		// substep of travel and GroundPointWS has not. At 200 Hz that is 5 ms, which
		// is 28 cm at 200 km/h and perfectly visible.
		//
		// The honest fix is that a PLANE HAS NO POSITION ALONG ITSELF. GroundPointWS
		// is just "some point on it" - SolveDiscContact proves that, using it only
		// through dot(CentreWS - PlanePointWS, PlaneNormalWS), a pure height query in
		// which sliding the point along the plane changes nothing at all. So the disc
		// is anchored under the wheel being drawn instead. Same plane, same normal,
		// same height, same physics: only the arbitrary anchor moves, to the one
		// place a reader is actually looking.
		// STRAIGHT DOWN from the wheel to the plane - not the nearest point on it.
		//
		// 40425 used the perpendicular foot, wheel minus normal times distance. That
		// is only under the wheel while the normal is parallel to Up. Tilt the plane
		// and the foot slides sideways by distance*sin(tilt): 60 cm of drop beside a
		// ledge with a 40 degree fitted normal puts it 38 cm off to one side. So the
		// circle still wandered away from its own wheel, for a second and different
		// reason than the one 40425 fixed - and lowering the enveloping bias moves the
		// plane further from the wheel, which makes exactly this term bigger.
		//
		// Down the BODY's own axis instead, which is also the axis every probe was
		// cast along, so the circle lands among its sample dots rather than off past
		// them. Solving dot(W - t*Up - P, N) = 0 gives t = dot(W-P, N)/dot(Up, N).
		// The divisor is safe: GroundNormalWS only reaches here having passed the
		// drivable-slope guard, so dot(Up, N) >= cos(MaxDrivableSlopeDeg) > 0. The
		// perpendicular foot is kept as the degenerate fallback.
		//
		// Cosmetic in the strict sense, and worth being explicit about why: nothing
		// outside this file reads PlaneAnchorWS, and GroundPointWS is itself consumed
		// only as dot(Centre - Point, Normal) - a pure height, in which any slide
		// along the plane cancels exactly. Where the disc is DRAWN cannot move the
		// car. What the bias changes about the plane's HEIGHT and NORMAL can, and
		// does.
		const float AnchorDenom = FVector::DotProduct(BodyToWorld.GetUnitAxis(EAxis::Z), GroundNormalWS);
		const float WheelAbovePlaneCm =
			FVector::DotProduct(LivePose.WheelCentre - DrawnGroundPointWS, GroundNormalWS);

		const FVector PlaneAnchorWS = (FMath::Abs(AnchorDenom) > KINDA_SMALL_NUMBER)
			? LivePose.WheelCentre - (BodyToWorld.GetUnitAxis(EAxis::Z) * (WheelAbovePlaneCm / AnchorDenom))
			: LivePose.WheelCentre - (GroundNormalWS * WheelAbovePlaneCm);

		// The SAMPLES are deliberately left where they are. Those are real trace hits
		// at real world positions, and shifting them to chase the wheel would be a
		// lie about which geometry was struck. If they visibly trail the wheel at
		// speed, that is not a drawing artefact - it is the substep of latency above,
		// which is worth being able to see. The residual below is unaffected either
		// way: dot(Sample - P, N) is the same for every P on the plane.
		// In-plane axes, needed by the probe footprints below as well as by the plane
		// itself. FindBestAxisVectors rather than a cross against world up, which
		// degenerates on exactly the flat ground this runs on most.
		FVector PlaneU, PlaneV;
		GroundNormalWS.FindBestAxisVectors(PlaneU, PlaneV);

		// THE PROBE AT ITS REAL SIZE, whenever the swept path is on.
		//
		// Before this, every sample was a 1.2 cm dot regardless of what had actually
		// been cast - so a full-radius sweep, which swings a 33 cm sphere per sample,
		// drew as a tidy little constellation of points across the contact patch. That
		// reads as "many small probes covering the patch". It is the opposite of the
		// truth: at scale 1.0 the probes are each as big as the wheel and overlap
		// almost entirely, and the mental model the picture created is what made the
		// behaviour at a ledge impossible to reason about.
		//
		// Drawn flat on the plane rather than as a sphere: nine wireframe spheres of a
		// third of a metre is an unreadable ball of lines, and the footprint is the
		// part that matters - it shows the reach of each probe against the spacing
		// between them. When those two are comparable the pattern and the sweep are
		// describing the same ground twice, which is the condition to look for.
		if (bUseSweptContact)
		{
			const float ProbeRadiusCm = GetSweepRadiusCm();
			for (const FVector& SampleWS : DrawnSampleHits)
			{
				DrawDebugCircle(World, SampleWS, ProbeRadiusCm, 20, Dim(GroundColour, 0.30f),
					false, -1.f, Fg, FixedLineWidth, PlaneU, PlaneV, /*bDrawAxis*/ false);
			}
		}

		// THE PROBE AXIS: where each probe started, and the line down to what it found.
		//
		// This exists to settle one question by looking rather than by arguing, because
		// arguing about it from screenshots has now failed twice. Every path in the
		// sampler is supposed to put a sample on its own probe's vertical line - the
		// swept sphere's centre travels straight down that line, so centre-minus-radius
		// is on it; a line trace's impact point is on it by construction; and the
		// penetrating case is now lifted along Up for exactly this reason.
		//
		// So: if each sample sits at the FOOT of its own line, the sampler is correct
		// and any apparent sideways offset is depth seen in perspective - at a ledge
		// the trace reaches ~86 cm below the hub, which in a perspective view looks
		// like a large lateral displacement and is not one. If a sample sits OFF the
		// foot of its line, there is still a displacement bug and this shows which
		// probe and by how much.
		//
		// The crosses are at hub height, which is also worth seeing: they are the
		// pattern itself, so their spacing against the footprint circles above is the
		// probe-reach-versus-probe-spacing comparison in one picture.
		if (DrawnSampleStarts.Num() == DrawnSampleHits.Num())
		{
			const FColor AxisColour = Dim(GroundColour, 0.45f);
			for (int32 i = 0; i < DrawnSampleHits.Num(); ++i)
			{
				const FVector StartWS = DrawnSampleStarts[i];
				DrawDebugLine(World, StartWS, DrawnSampleHits[i], AxisColour,
					false, -1.f, Fg, FixedLineWidth);
				DrawDebugLine(World, StartWS - PlaneU * 1.6f, StartWS + PlaneU * 1.6f,
					AxisColour, false, -1.f, Fg, FixedLineWidth);
				DrawDebugLine(World, StartWS - PlaneV * 1.6f, StartWS + PlaneV * 1.6f,
					AxisColour, false, -1.f, Fg, FixedLineWidth);
			}
		}

		// REJECTED samples, struck through. Same family as the survivors because they
		// are still ground - just ground this tyre is bridging rather than standing
		// on - so the same hue, dimmed, with an X through it.
		//
		// Drawn rather than silently dropped so the bridging test can be checked from
		// outside. A deep sample still on screen is otherwise ambiguous: it could have
		// passed the test, or the test could not have run at all, and nothing
		// distinguishes those by eye. With this, no X anywhere means the test is not
		// running; an X on the deep one means it is working and the fit is already
		// clean.
		for (const FVector& BridgedWS : DrawnSampleBridged)
		{
			const FColor BridgedColour = Dim(GroundColour, 0.40f);
			const FVector A = (PlaneU + PlaneV).GetSafeNormal() * 2.2f;
			const FVector B = (PlaneU - PlaneV).GetSafeNormal() * 2.2f;

			DrawDebugLine(World, BridgedWS - A, BridgedWS + A, BridgedColour, false, -1.f, Fg, FixedLineWidth);
			DrawDebugLine(World, BridgedWS - B, BridgedWS + B, BridgedColour, false, -1.f, Fg, FixedLineWidth);
		}

		// Each surviving sample, with a tie line back to the plane along the normal.
		// That line IS the residual: a fit tracking the ground shows short stubs all
		// round, while one being dragged by an outlier shows a single long one.
		//
		// Note that in swept mode a sample is NOT a point on the geometry. It is the
		// sphere's lowest point, and a sphere resting on a corner has its lowest point
		// out in open air beside that corner - a 33 cm sphere on a corner 25 cm off
		// the probe axis sits 11.5 cm BELOW the ledge top, touching nothing. That is
		// the effective height a tyre of that radius rides at, which is the whole
		// intent, but it is why these stop coinciding with the surface near an edge.
		// DrawnSampleHits, not the raw array - these are the sample dots themselves and
		// they were the last thing still drawing in the solve pose. The residual is taken
		// against the corrected plane point for the same reason: both sides of that
		// subtraction have to be in one frame or the tie-line length is wrong.
		for (const FVector& SampleWS : DrawnSampleHits)
		{
			const float ResidualCm = FVector::DotProduct(SampleWS - DrawnGroundPointWS, GroundNormalWS);

			DrawDebugSphere(World, SampleWS, 1.2f, 8, GroundColour, false, -1.f, Fg, FixedLineWidth);
			DrawDebugLine(World, SampleWS, SampleWS - GroundNormalWS * ResidualCm,
				Dim(GroundColour, 0.55f), false, -1.f, Fg, FixedLineWidth);
		}

		// Not drawn at all when the wheel is off the ground.
		//
		// Dimming it was not enough: a plane under an airborne wheel still reads as
		// contact at a glance, and the whole reason it is drawn is to be glanced at.
		// The samples themselves still draw, so where the ground IS remains visible -
		// what goes away is the surface that implies the wheel is on it.
		//
		// NOT a return - the numeric readout below still has to run.
		if (!bInContact)
		{
			AddLabel(PlaneAnchorWS, FString::Printf(TEXT("AIR %.0fcm"), -TyreDeflectionCm),
				Dim(GroundColour, 0.5f));
		}
		else
		{
		// The plane, when the wheel is genuinely on it.
		//
		// The plane is drawn whenever a trace found ground within reach, which is a
		// wheel radius plus the remaining travel plus the slope allowance - about
		// 55 cm below the hub on this car. A wheel held 20 cm in the air therefore
		// still has a plane under it, correctly, because the ground IS down there.
		// Drawn at full strength that reads as "this wheel is on the ground", which
		// it is not, and it cost real debugging time before the distinction was
		// made visible.
		//
		// bInContact is the honest answer, and it comes from TyreDeflectionCm > 0
		// against the solved disc rather than from the trace finding anything.
		const float PlaneRadiusCm = FMath::Max3(GroundSampleHalfLengthCm, GroundSampleHalfWidthCm, 2.f);

		DrawDebugCircle(World, PlaneAnchorWS, PlaneRadiusCm, 24, GroundColour, false, -1.f, Fg,
			LiveLineWidth, PlaneU, PlaneV, /*bDrawAxis*/ false);
		DrawDebugDirectionalArrow(World, PlaneAnchorWS, PlaneAnchorWS + GroundNormalWS * 12.f,
			4.f, GroundColour, false, -1.f, Fg, LiveLineWidth);

		// Counts on the label, so the test is readable even where the X marks are too
		// small to pick out: kept over total.
		AddLabel(PlaneAnchorWS,
			FString::Printf(TEXT("%s %d/%d"),
				bGroundPlaneFitted ? TEXT("fit") : TEXT("face"),
				GroundSampleHitsWS.Num(),
				GroundSampleHitsWS.Num() + GroundSampleBridgedWS.Num()),
			GroundColour);
		}
	}
	else if (bDrawGroundSamples && bGroundHit && !bInContact)
	{
		// Nothing drawn on the road, but say how far off it this corner is - the one
		// number that was worth reading out of all that geometry.
		AddLabel(LivePose.WheelCentre, FString::Printf(TEXT("AIR %.0fcm"), -TyreDeflectionCm),
			FColor(80, 220, 230));
	}

	// ---------- Numeric readout ----------
	if (bDrawReadout && GEngine != nullptr)
	{
		const int32 BaseKey = static_cast<int32>(GetUniqueID() & 0x0FFFFFFF);
		const FColor Ink = FColor::White;

		// Rigid lengths. If either drifts from its rest value through the sweep, a
		// link is being stretched and the kinematics are wrong.
		// Off the pose, so these check the geometry that was actually DRAWN. Measuring
		// a re-solve instead could in principle agree while the drawing disagreed,
		// which is the one thing a rigid-length check must not do. Distances are
		// unaffected by the world transform, so world points give the same numbers.
		const float LiveKingpinCm = FVector::Dist(LivePose.UpperBallJoint, LivePose.LowerBallJoint);
		const float HubArmCm = FVector::Dist(LivePose.WheelCentre, LivePose.LowerBallJoint);

		// The knuckle is rigid in two independent senses, and the check needs both:
		// the ball joints hold their DISTANCE, and the spindle holds its ORIENTATION
		// to the line between them. A length check alone would pass a knuckle that
		// was twisting about its own kingpin. Notably NOT IncludedAngleDeg, which
		// swings over 10 deg under steer with nothing wrong - see its header comment.
		const bool bKingpinLengthHolds = FMath::Abs(LiveKingpinCm - KingpinLengthCm) <= 0.05f;
		const bool bSpindleAngleHolds =
			FMath::Abs(SpindleToKingpinDeg - RestSpindleToKingpinDeg) <= 0.05f;
		const bool bLengthsHold = bKingpinLengthHolds && bSpindleAngleHolds;

		// A corner in contact with zero ground force is not airborne - it means
		// nothing is feeding SetTyreContactForceN, which otherwise just looks like a
		// corner that sagged.
		const bool bStarvedOfLoad = bInContact && ExternalContactForceN.IsNearlyZero();
		GEngine->AddOnScreenDebugMessage(BaseKey + 0, 0.f,
			bStarvedOfLoad ? FColor::Red : Ink,
			FString::Printf(TEXT("[%s] %s%s"), *GetName(),
				bDebugSweepTravel ? TEXT("SWEEP MODE - physics paused") : TEXT("physics running"),
				bStarvedOfLoad ? TEXT("  NO TYRE FORCE - is a tyre component attached?") : TEXT("")));
		GEngine->AddOnScreenDebugMessage(BaseKey + 1, 0.f, Ink,
			FString::Printf(TEXT("  theta %+7.2f deg  [%.1f .. %.1f]   theta' %+8.2f deg/s"),
				ArticulationAngleDeg, MinArticulationAngleDeg, MaxArticulationAngleDeg,
				FMath::RadiansToDegrees(ThetaDotRad)));
		GEngine->AddOnScreenDebugMessage(BaseKey + 2, 0.f, Ink,
			FString::Printf(TEXT("  contact %s  load %8.1f N  tyre deflection %6.3f cm (rho_z)"),
				bInContact ? TEXT("YES") : (bGroundHit ? TEXT("no ") : TEXT("NO TRACE")),
				ContactForceN, TyreDeflectionCm));

		// Where the ground plane came from. Amber on the fallback, because a fit
		// silently degenerating to face normals is exactly the regression that would
		// otherwise be invisible - the corner keeps working, just jitters again.
		TArray<FVector2D> PatternOffsets;
		BuildGroundSampleOffsets(PatternOffsets);
		GEngine->AddOnScreenDebugMessage(BaseKey + 10, 0.f,
			bGroundPlaneFitted ? Ink : FColor(255, 200, 120),
			FString::Printf(TEXT("  ground %d/%d samples hit   normal from %s   enveloping %.2f"),
				GroundSampleHitsWS.Num(), PatternOffsets.Num(),
				bGroundPlaneFitted ? TEXT("plane fit") : TEXT("face normals (fit degenerate)"),
				GroundEnvelopingBias));
		// "slack" is worth seeing directly: it means the strut has extended past the
		// spring's free length, so the corner is carrying nothing here and preload
		// is correctly NOT propping it up.
		const bool bSpringSlack = SpringForceN <= KINDA_SMALL_NUMBER;
		GEngine->AddOnScreenDebugMessage(BaseKey + 3, 0.f, Ink,
			FString::Printf(TEXT("  spring %8.1f N%s   damper %+8.1f N   motion ratio %.3f"),
				SpringForceN, bSpringSlack ? TEXT(" (SLACK)") : TEXT("    "), DamperForceN, MotionRatio));
		GEngine->AddOnScreenDebugMessage(BaseKey + 9, 0.f, Ink,
			FString::Printf(TEXT("  coil %.2f cm  free %.2f cm  (%+.2f cm squeezed)   mounts %.2f cm"),
				SpringLengthCm, SpringFreeLengthCm, SpringFreeLengthCm - SpringLengthCm,
				SpringMountingOffsetCm));
		// Both travels, not just the difference: seeing them together is what tells
		// you whether the bar is answering roll or just an unwired partner reading
		// a flat zero.
		GEngine->AddOnScreenDebugMessage(BaseKey + 11, 0.f, Ink,
			FString::Printf(TEXT("  travel %+6.2f cm   partner %+6.2f cm   anti-roll %+8.1f N @ %.0f N/m"),
				WheelTravelCm, AntiRollPartnerTravelCm, AntiRollForceN, AntiRollRateNPerM));
		GEngine->AddOnScreenDebugMessage(BaseKey + 4, 0.f,
			bLengthsHold ? Ink : FColor::Red,
			FString::Printf(TEXT("  kingpin %.3f cm (rest %.3f)%s   spindle-kingpin %.3f deg (rest %.3f)%s   "
								 "hub arm %.3f cm   arm sweep L %.2f / U %.2f"),
				LiveKingpinCm, KingpinLengthCm, bKingpinLengthHolds ? TEXT("") : TEXT(" STRETCHED"),
				SpindleToKingpinDeg, RestSpindleToKingpinDeg, bSpindleAngleHolds ? TEXT("") : TEXT(" TWISTED"),
				HubArmCm, LowerArmSweepRadiusCm, UpperArmSweepRadiusCm));
		GEngine->AddOnScreenDebugMessage(BaseKey + 5, 0.f, Ink,
			FString::Printf(TEXT("  camber %+6.2f (gain %+6.2f)   caster %+6.2f (gain %+6.2f)   KPI %+6.2f (gain %+6.2f)"),
				LiveCamberDeg, LiveCamberDeg - RestCamberDeg,
				CasterAngleDeg, CasterAngleDeg - RestCasterAngleDeg,
				KingpinInclinationDeg, KingpinInclinationDeg - RestKingpinInclinationDeg));
		GEngine->AddOnScreenDebugMessage(BaseKey + 6, 0.f, Ink,
			FString::Printf(TEXT("  steer %+6.2f deg   scrub %+.1f mm   included %+.2f = KPI + camber"),
				SteerAngleDeg, ScrubRadiusMm, IncludedAngleDeg));

		// The reaction, and whether it is reaching the body at all. Read the vertical
		// component against the corner's share of sprung weight: parked and settled
		// they should agree, and if they do not the spring rate, the preload or the
		// body mass is the thing to look at - not this.
		const bool bReactionLive = bApplyReactionToChassis;
		GEngine->AddOnScreenDebugMessage(BaseKey + 7, 0.f,
			bReactionLive ? Ink : FColor(170, 170, 170),
			FString::Printf(TEXT("  chassis reaction %8.1f N (up %+8.1f)   strut tower %8.1f N (diagnostic)   %s"),
				ChassisReactionForceN.Size(), ChassisReactionForceN.Z, CoiloverForceOnChassisMagN,
				bReactionLive ? TEXT("APPLIED")
					: TEXT("not applied (standalone tick)")));
		GEngine->AddOnScreenDebugMessage(BaseKey + 8, 0.f, Ink,
			FString::Printf(TEXT("  wheel accel (rel. body) %8.2f m/s2   inertial term %8.1f N"),
				WheelAccelRelativeMPerSec2.Size(), UnsprungMassKg * WheelAccelRelativeMPerSec2.Size()));
	}
}