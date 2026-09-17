#include "WishboneSuspensionVisualizer.h"

#include "SceneManagement.h"
#include "VehiclePhysics/Vehicles/Physics/WishbonePose.h"
#include "VehiclePhysics/Vehicles/Physics/WishboneSuspension.h"

namespace
{
	// Markers are drawn as three short crossed segments rather than DrawWireSphere:
	// a hardpoint is a POINT, and at the zoom levels you inspect a corner at a
	// sphere large enough to see reads as a ball joint with a diameter, which
	// invites measuring off its edge instead of its centre.
	constexpr float MarkerHalfSizeCm = 1.4f;

	// Enough segments that the tyre reads as a circle at the zoom where camber is
	// being judged by eye. Cheap - this is one component, drawn when selected.
	constexpr int32 WheelCircleSegments = 48;

	// Arc resolution for the travel sweep. The arcs are a shape check, not a
	// measurement, so this only has to look smooth.
	constexpr int32 TravelArcSegments = 24;

	void DrawMarker(FPrimitiveDrawInterface* PDI, const FVector& P, const FColor& C, float Thickness)
	{
		for (const FVector& Axis : { FVector::ForwardVector, FVector::RightVector, FVector::UpVector })
		{
			PDI->DrawLine(P - Axis * MarkerHalfSizeCm, P + Axis * MarkerHalfSizeCm,
				C, SDPG_Foreground, Thickness);
		}
	}
}

void FWishboneSuspensionVisualizer::DrawPose(const FWishbonePose& Pose, FPrimitiveDrawInterface* PDI,
	float Brightness, float Thickness, bool bDrawWheel)
{
	using namespace WishbonePalette;

	const FColor ChassisC = Dim(Chassis, Brightness);
	const FColor ArmC = Dim(Arm, Brightness);
	const FColor KingpinC = Dim(Kingpin, Brightness);
	const FColor CoiloverC = Dim(Coilover, Brightness);
	const FColor WheelC = Dim(Wheel, Brightness);

	// A failed knuckle solve means the drawn kingpin is NOT KingpinLengthCm long -
	// the linkage would have had to come apart. Flag it in the contact red rather
	// than presenting a stretched link as if it were geometry.
	const FColor KingpinDrawC = Pose.bUpperArmSolveFailed ? Dim(Contact, Brightness) : KingpinC;

	// ---------- Chassis-side hardpoints and hinge axes ----------
	DrawMarker(PDI, Pose.LcaFront, ChassisC, Thickness);
	DrawMarker(PDI, Pose.LcaRear, ChassisC, Thickness);
	DrawMarker(PDI, Pose.UcaFront, ChassisC, Thickness);
	DrawMarker(PDI, Pose.UcaRear, ChassisC, Thickness);
	DrawMarker(PDI, Pose.CoiloverTop, CoiloverC, Thickness);

	// The hinge axes themselves, drawn bush to bush. Their direction in plan and
	// side view is what sets camber gain, caster gain and anti-dive, so seeing them
	// is the point rather than a nicety.
	PDI->DrawLine(Pose.LcaFront, Pose.LcaRear, ChassisC, SDPG_Foreground, Thickness);
	PDI->DrawLine(Pose.UcaFront, Pose.UcaRear, ChassisC, SDPG_Foreground, Thickness);

	// ---------- Arms ----------
	// Drawn as triangles from both bushes, which is what an A-arm is; a single line
	// to the ball joint would hide which way the arm is swept in plan.
	PDI->DrawLine(Pose.LcaFront, Pose.LowerBallJoint, ArmC, SDPG_Foreground, Thickness);
	PDI->DrawLine(Pose.LcaRear, Pose.LowerBallJoint, ArmC, SDPG_Foreground, Thickness);
	PDI->DrawLine(Pose.UcaFront, Pose.UpperBallJoint, ArmC, SDPG_Foreground, Thickness);
	PDI->DrawLine(Pose.UcaRear, Pose.UpperBallJoint, ArmC, SDPG_Foreground, Thickness);

	DrawMarker(PDI, Pose.LowerBallJoint, ArmC, Thickness);
	DrawMarker(PDI, Pose.UpperBallJoint, ArmC, Thickness);

	// ---------- Kingpin ----------
	// Its lean in side view is caster, in front view is KPI. Extended past both
	// joints so those angles can be read against the world grid instead of guessed
	// from a segment only as long as the knuckle.
	const FVector KingpinDir = (Pose.UpperBallJoint - Pose.LowerBallJoint).GetSafeNormal();
	PDI->DrawLine(Pose.LowerBallJoint, Pose.UpperBallJoint, KingpinDrawC, SDPG_Foreground, Thickness * 1.5f);
	if (!KingpinDir.IsNearlyZero())
	{
		PDI->DrawLine(Pose.LowerBallJoint, Pose.LowerBallJoint - KingpinDir * 12.f,
			Dim(KingpinDrawC, 0.5f), SDPG_Foreground, Thickness);
		PDI->DrawLine(Pose.UpperBallJoint, Pose.UpperBallJoint + KingpinDir * 12.f,
			Dim(KingpinDrawC, 0.5f), SDPG_Foreground, Thickness);
	}

	// ---------- Coilover ----------
	PDI->DrawLine(Pose.CoiloverTop, Pose.DamperMount, CoiloverC, SDPG_Foreground, Thickness * 2.f);
	DrawMarker(PDI, Pose.DamperMount, CoiloverC, Thickness);
	// The mount is rigid to the LOWER ARM, not the knuckle - that is the whole
	// reason a motion ratio exists, so the tie back to the arm is drawn.
	PDI->DrawLine(Pose.LowerBallJoint, Pose.DamperMount, Dim(ArmC, 0.6f), SDPG_Foreground, Thickness);

	// ---------- Knuckle and wheel ----------
	// Hub arm: the wheel centre hangs off the KNUCKLE, so it translates with the
	// lower ball joint and rotates with the kingpin. It does not orbit the arm.
	PDI->DrawLine(Pose.LowerBallJoint, Pose.WheelCentre, WheelC, SDPG_Foreground, Thickness);
	DrawMarker(PDI, Pose.WheelCentre, WheelC, Thickness);

	if (bDrawWheel && Pose.WheelRadiusCm > 0.f)
	{
		DrawCircle(PDI, Pose.WheelCentre, Pose.WheelPlaneU, Pose.WheelPlaneV,
			WheelC, Pose.WheelRadiusCm, WheelCircleSegments, SDPG_Foreground, Thickness);

		// Spindle, drawn through the hub. Its tilt off horizontal IS the camber and
		// its yaw off the body's forward IS the toe, so the two alignment angles are
		// legible from this one segment.
		PDI->DrawLine(Pose.WheelCentre - Pose.SpindleAxis * (Pose.WheelRadiusCm * 0.55f),
			Pose.WheelCentre + Pose.SpindleAxis * (Pose.WheelRadiusCm * 0.55f),
			WheelC, SDPG_Foreground, Thickness * 1.5f);
	}
}

void FWishboneSuspensionVisualizer::DrawVisualization(const UActorComponent* Component,
	const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FComponentVisualizer::DrawVisualization(Component, View, PDI);

	const UWishboneSuspension* Corner = Cast<UWishboneSuspension>(Component);
	if (Corner == nullptr)
	{
		return;
	}

#if WITH_EDITORONLY_DATA
	if (!Corner->bEditorPreview)
	{
		return;
	}

	const float PreviewAngleDeg = Corner->TravelFractionToAngleDeg(Corner->EditorPreviewTravel);
	const float PreviewSteerDeg = Corner->EditorPreviewSteerDeg;

	// ---------- Rest pose first, so the live one draws over it ----------
	// Dimmed and thin: this is the reference the preview is measured against, and
	// the eye should read the bright pose as "now" without being told.
	if (Corner->bEditorPreviewRestPose
		&& (!FMath::IsNearlyZero(PreviewAngleDeg) || !FMath::IsNearlyZero(PreviewSteerDeg)))
	{
		const FWishbonePose RestPose = Corner->GetPose(0.f, 0.f);
		if (RestPose.bValid)
		{
			DrawPose(RestPose, PDI, /*Brightness*/ 0.45f, /*Thickness*/ 0.6f,
				Corner->bEditorPreviewWheel);
		}
	}

	// ---------- Travel arcs ----------
	// The actual path each moving point sweeps between the limits. An arc cutting
	// through the bodywork means the hardpoints are wrong, which is a geometry
	// problem you want to see before ever pressing Play.
	if (Corner->bEditorPreviewTravelArcs)
	{
		const float MinDeg = Corner->GetMinArticulationAngleDeg();
		const float MaxDeg = Corner->GetMaxArticulationAngleDeg();

		FWishbonePose Previous = Corner->GetPose(MinDeg, PreviewSteerDeg);
		for (int32 Step = 1; Step <= TravelArcSegments; ++Step)
		{
			const float Alpha = static_cast<float>(Step) / static_cast<float>(TravelArcSegments);
			const FWishbonePose Current = Corner->GetPose(FMath::Lerp(MinDeg, MaxDeg, Alpha), PreviewSteerDeg);
			if (!Previous.bValid || !Current.bValid)
			{
				Previous = Current;
				continue;
			}

			PDI->DrawLine(Previous.WheelCentre, Current.WheelCentre,
				WishbonePalette::Dim(WishbonePalette::Wheel, 0.5f), SDPG_World, 0.5f);
			PDI->DrawLine(Previous.LowerBallJoint, Current.LowerBallJoint,
				WishbonePalette::Dim(WishbonePalette::Arm, 0.5f), SDPG_World, 0.5f);
			PDI->DrawLine(Previous.UpperBallJoint, Current.UpperBallJoint,
				WishbonePalette::Dim(WishbonePalette::Arm, 0.5f), SDPG_World, 0.5f);
			PDI->DrawLine(Previous.DamperMount, Current.DamperMount,
				WishbonePalette::Dim(WishbonePalette::Coilover, 0.5f), SDPG_World, 0.5f);

			Previous = Current;
		}
	}

	// ---------- The previewed pose ----------
	const FWishbonePose Pose = Corner->GetPose(PreviewAngleDeg, PreviewSteerDeg);
	if (Pose.bValid)
	{
		DrawPose(Pose, PDI, /*Brightness*/ 1.f, /*Thickness*/ 1.2f, Corner->bEditorPreviewWheel);
	}
#endif
}
