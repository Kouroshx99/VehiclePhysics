// Fill out your copyright notice in the Description page of Project Settings.

#include "CarAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "VehiclePhysics/Vehicles/Physics/Car.h"
#include "VehiclePhysics/Vehicles/Physics/WishboneGeometry.h"
#include "VehiclePhysics/Vehicles/Physics/WishboneSuspension.h"

namespace
{
	/**
	 * Position of the LCA damper mount, expressed as barycentric weights over the
	 * arm's three hardpoints plus an offset along the plane normal - the mount is
	 * a lug hanging off the arm rather than one of its hardpoints.
	 *
	 * Animation-only. The physics component reads the damper mount as a plain
	 * hardpoint, so these weights exist purely to place the rendered coilover and
	 * live here rather than in the shared geometry header.
	 *
	 * Weights were solved in-engine against the rest triangle. They go stale if
	 * the arm's hardpoints move, so re-solve them after any geometry change.
	 */
	struct FDamperMountCoords
	{
		float wF; // front bush
		float wB; // rear bush
		float wU; // outer ball joint (wF + wB + wU = 1)
		float H;  // offset along the arm-plane normal, cm
	};

	const FDamperMountCoords FrontDamperMount = { 0.0664f, 0.2781f, 0.6555f, 4.5f };

	/** Live damper-mount position. The plane normal tilts as U orbits. */
	FVector DamperMountPoint(const FVector& F, const FVector& B, const FVector& U_Live,
		const FDamperMountCoords& Coords)
	{
		const FVector PlaneNormal = FVector::CrossProduct(B - U_Live, F - U_Live).GetSafeNormal();

		return (Coords.wF * F) + (Coords.wB * B) + (Coords.wU * U_Live) + (PlaneNormal * Coords.H);
	}
}

void UCarAnimInstance::NativeBeginPlay()
{
	UAnimInstance::NativeBeginPlay();
	CaptureRestPose();
}

void UCarAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Car = Cast<ACar>(TryGetPawnOwner());
	CaptureRestPose();
}

void UCarAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Car == nullptr)
	{
		Car = Cast<ACar>(TryGetPawnOwner());
		if (Car == nullptr)
			return;
	}

	FrontRight = Car->GetWheelRotation(EWheel::EW_FrontRight);
	FrontLeft = Car->GetWheelRotation(EWheel::EW_FrontLeft);
	RearRight = Car->GetWheelRotation(EWheel::EW_RearRight);
	RearLeft = Car->GetWheelRotation(EWheel::EW_RearLeft);

	FrontRightRotation -= Car->GetWheelSpeed(EWheel::EW_FrontRight) * DeltaSeconds;
	FrontLeftRotation -= Car->GetWheelSpeed(EWheel::EW_FrontLeft) * DeltaSeconds;
	RearRightRotation -= Car->GetWheelSpeed(EWheel::EW_RearRight) * DeltaSeconds;
	RearLeftRotation -= Car->GetWheelSpeed(EWheel::EW_RearLeft) * DeltaSeconds;

	FrontRightCenterZ = Car->GetWheelZPosition(EWheel::EW_FrontRight) + RestFR;
	FrontLeftCenterZ = Car->GetWheelZPosition(EWheel::EW_FrontLeft) + RestFR;
	RearRightCenterZ = Car->GetWheelZPosition(EWheel::EW_RearRight) + RestFL;
	RearLeftCenterZ = Car->GetWheelZPosition(EWheel::EW_RearLeft) + RestRL;

	FrontRightAngle = Car->GetWheelAngle(EWheel::EW_FrontRight);
	FrontLeftAngle = Car->GetWheelAngle(EWheel::EW_FrontLeft);

	FrontRightRoll = FrontRight.Roll;
	FrontLeftRoll = FrontLeft.Roll;
	RearRightRoll = RearRight.Roll;
	RearLeftRoll = RearLeft.Roll;

	PosFR = Car->GetWheelPosition(EWheel::EW_FrontRight) + RestFR;
	PosFL = Car->GetWheelPosition(EWheel::EW_FrontLeft) + RestFL;
	PosRR = Car->GetWheelPosition(EWheel::EW_RearRight) + RestRR;
	PosRL = Car->GetWheelPosition(EWheel::EW_RearLeft) + RestRL;
	
	// ---------- 0. Take the suspension angle from the physics layer ----------
	// The physics integrates theta natively, so there is nothing to invert here.
	// If no suspension component is present the property is left alone, which
	// keeps hand-authored angles usable for rig debugging.
	if (const UWishboneSuspension* FRSuspension = Car->GetFrontRightSuspension())
	{
		FR_LCA_ArticulationAngle = FRSuspension->GetArticulationAngleDeg();
	}
	// Parallelogram simplification: exact ONLY because LCA and UCA are
	// currently equal-length and parallel (the degenerate geometry flagged
	// earlier). Replace with a real circle-intersection solve once arm
	// lengths differ.
	FR_UCA_ArticulationAngle = FR_LCA_ArticulationAngle;

	// ---------- 1. LCA / UCA orbit ----------
	// Same closed-form orbit the physics layer integrates against - shared on
	// purpose, so the wheel can never render somewhere other than where it was
	// simulated.
	LCA_U_Live = WishboneGeometry::OrbitPoint(LCA_F_Rest, LCA_U_Rest, LCA_HingeAxis, FR_LCA_ArticulationAngle);
	UCA_U_Live = WishboneGeometry::OrbitPoint(UCA_F_Rest, UCA_U_Rest, UCA_HingeAxis, FR_UCA_ArticulationAngle);

	// ---------- 2. Knuckle Master: swing to re-aim kingpin, twist to preserve tie-rod reference ----------
	KnuckleMasterPositionLive = LCA_U_Live;

	// Ground-truth rest data - Master_Rest_Transform.GetRotation() is read
	// directly off the skeleton (see CaptureRestPose), not reconstructed
	// from any basis-building formula. This is what makes swing/twist both
	// collapse to identity at rest, by construction.
	const FVector KingpinAxisRest = (UCA_U_Rest - LCA_U_Rest).GetSafeNormal();
	const FVector KingpinAxisLive = (UCA_U_Live - LCA_U_Live).GetSafeNormal();

	const FQuat SwingRot = FQuat::FindBetweenNormals(KingpinAxisRest, KingpinAxisLive);
	const FQuat IntermediateRot = SwingRot * Master_Rest_Transform.GetRotation();

	const FVector TieRodDirRest = (TieRodInner_Rest - LCA_U_Rest).GetSafeNormal();
	const FVector TieRodRefLocal = Master_Rest_Transform.GetRotation().Inverse().RotateVector(TieRodDirRest);
	const FVector CurrentRefDir = IntermediateRot.RotateVector(TieRodRefLocal);

	const FVector TieRodDirLive = (TieRodInner_Rest - LCA_U_Live).GetSafeNormal();

	// project both onto the plane perpendicular to the live kingpin axis, compare pure twist
	const FVector A = (CurrentRefDir - KingpinAxisLive * FVector::DotProduct(CurrentRefDir, KingpinAxisLive)).GetSafeNormal();
	const FVector B = (TieRodDirLive - KingpinAxisLive * FVector::DotProduct(TieRodDirLive, KingpinAxisLive)).GetSafeNormal();
	const FQuat TwistCorrection = FQuat::FindBetweenNormals(A, B);

	const FQuat MasterRotLive = TwistCorrection * IntermediateRot;
	KnuckleMasterRotationLive = MasterRotLive.Rotator();

	// FIX: this was missing - step 5 needs it and won't compile without it.
	const FTransform MasterLiveTransform(MasterRotLive, KnuckleMasterPositionLive);

	// ---------- 3. LCA damper mount: barycentric position + normal offset ----------
	// Shared with the physics layer, which needs this exact point to know where
	// the coilover pushes on the arm.
	DamperMountLive = DamperMountPoint(LCA_F_Rest, LCA_B_Rest, LCA_U_Live, FrontDamperMount);

	// ---------- 4. Coilover body/rod + spring, aimed between chassis and damper mount ----------
	CoiloverBodyPositionLive = CoiloverTop_Rest;
	CoiloverBodyRotationLive = UKismetMathLibrary::FindLookAtRotation(CoiloverTop_Rest, DamperMountLive);

	CoiloverRodPositionLive = DamperMountLive;
	CoiloverRodRotationLive = UKismetMathLibrary::FindLookAtRotation(DamperMountLive, CoiloverTop_Rest);

	SpringTopPositionLive = CoiloverTop_Rest;
	SpringBotPositionLive = DamperMountLive;

	// ---------- 5. Tie rod body: aim at the tie rod's LIVE outer point ----------
	const FVector TieRodRod_Live = MasterLiveTransform.TransformPosition(TieRodRod_LocalOffsetFromMaster);

	TieRodBodyPositionLive = TieRodInner_Rest;
	TieRodBodyRotationLive = UKismetMathLibrary::FindLookAtRotation(TieRodInner_Rest, TieRodRod_Live);

	// ---------- 6. Wheel offset: additive slide along the hub's spindle axis ----------
	const FQuat HubLiveRotation = MasterRotLive * Hub_LocalRotationFromMaster;
	const FVector SpindleAxisLive = HubLiveRotation.RotateVector(FVector(0.f, 1.f, 0.f)); // assumes Hub local Y = spindle axis - verify against the rig
	WheelOffsetTranslationLive = SpindleAxisLive * FR_WheelOffset;
}

void UCarAnimInstance::CaptureRestPose()
{
	// One-time offline capture tool. Leave the early-return in place for
	// normal play; temporarily comment it out, run once, copy the logged
	// values into the UPROPERTY defaults in the header, then restore it.
	USkeletalMeshComponent* MeshComp = GetSkelMeshComponent();
	if (MeshComp)
	{
		LCA_F_Rest = MeshComp->GetBoneLocation(LCA_F_Bone, EBoneSpaces::ComponentSpace);
		LCA_B_Rest = MeshComp->GetBoneLocation(LCA_B_Bone, EBoneSpaces::ComponentSpace);
		LCA_U_Rest = MeshComp->GetBoneLocation(LCA_U_Bone, EBoneSpaces::ComponentSpace);
		LCA_HingeAxis = (LCA_B_Rest - LCA_F_Rest).GetSafeNormal();

		UCA_F_Rest = MeshComp->GetBoneLocation(UCA_F_Bone, EBoneSpaces::ComponentSpace);
		UCA_B_Rest = MeshComp->GetBoneLocation(UCA_B_Bone, EBoneSpaces::ComponentSpace);
		UCA_U_Rest = MeshComp->GetBoneLocation(UCA_U_Bone, EBoneSpaces::ComponentSpace);
		UCA_HingeAxis = (UCA_B_Rest - UCA_F_Rest).GetSafeNormal();

		TieRodInner_Rest = MeshComp->GetBoneLocation(TieRodInnerBone, EBoneSpaces::ComponentSpace);
		CoiloverTop_Rest = MeshComp->GetBoneLocation(CoiloverTopBone, EBoneSpaces::ComponentSpace);

		// GROUND TRUTH - read directly off the skeleton, no basis
		// reconstruction, no formula to get subtly wrong or go stale
		// again if the twist math changes later.
		const FQuat MasterRestRotWorld = MeshComp->GetBoneQuaternion(KnuckleMasterBone, EBoneSpaces::ComponentSpace);
		Master_Rest_Transform = FTransform(MasterRestRotWorld, LCA_U_Rest);

		const FVector TieRodRod_Rest = MeshComp->GetBoneLocation(TieRodRodBone, EBoneSpaces::ComponentSpace);
		TieRodRod_LocalOffsetFromMaster = Master_Rest_Transform.InverseTransformPosition(TieRodRod_Rest);

		const FVector Hub_Rest = MeshComp->GetBoneLocation(HubBone, EBoneSpaces::ComponentSpace);
		Hub_LocalOffsetFromMaster = Master_Rest_Transform.InverseTransformPosition(Hub_Rest);
		const FQuat HubRestRotWorld = MeshComp->GetBoneQuaternion(HubBone, EBoneSpaces::ComponentSpace);
		Hub_LocalRotationFromMaster = Master_Rest_Transform.GetRotation().Inverse() * HubRestRotWorld;

		bRestCaptured = true;
	}

	UE_LOG(LogTemp, Warning, TEXT(
		"REST CAPTURE:\n"
		"LCA_F=(%f,%f,%f) LCA_B=(%f,%f,%f) LCA_U=(%f,%f,%f) LCA_Axis=(%f,%f,%f)\n"
		"UCA_F=(%f,%f,%f) UCA_B=(%f,%f,%f) UCA_U=(%f,%f,%f) UCA_Axis=(%f,%f,%f)\n"
		"TieRodInner=(%f,%f,%f) CoiloverTop=(%f,%f,%f)\n"
		"MasterRest Loc=(%f,%f,%f) Rot=(%f,%f,%f,%f) [RotAsEuler P=%f Y=%f R=%f]\n"
		"TieRodRodLocalOffset=(%f,%f,%f)\n"
		"HubLocalOffset=(%f,%f,%f) HubLocalRot=(%f,%f,%f,%f) [RotAsEuler P=%f Y=%f R=%f]"),
		LCA_F_Rest.X, LCA_F_Rest.Y, LCA_F_Rest.Z,
		LCA_B_Rest.X, LCA_B_Rest.Y, LCA_B_Rest.Z,
		LCA_U_Rest.X, LCA_U_Rest.Y, LCA_U_Rest.Z,
		LCA_HingeAxis.X, LCA_HingeAxis.Y, LCA_HingeAxis.Z,
		UCA_F_Rest.X, UCA_F_Rest.Y, UCA_F_Rest.Z,
		UCA_B_Rest.X, UCA_B_Rest.Y, UCA_B_Rest.Z,
		UCA_U_Rest.X, UCA_U_Rest.Y, UCA_U_Rest.Z,
		UCA_HingeAxis.X, UCA_HingeAxis.Y, UCA_HingeAxis.Z,
		TieRodInner_Rest.X, TieRodInner_Rest.Y, TieRodInner_Rest.Z,
		CoiloverTop_Rest.X, CoiloverTop_Rest.Y, CoiloverTop_Rest.Z,
		Master_Rest_Transform.GetLocation().X, Master_Rest_Transform.GetLocation().Y, Master_Rest_Transform.GetLocation().Z,
		Master_Rest_Transform.GetRotation().X, Master_Rest_Transform.GetRotation().Y, Master_Rest_Transform.GetRotation().Z, Master_Rest_Transform.GetRotation().W,
		Master_Rest_Transform.GetRotation().Rotator().Pitch, Master_Rest_Transform.GetRotation().Rotator().Yaw, Master_Rest_Transform.GetRotation().Rotator().Roll,
		TieRodRod_LocalOffsetFromMaster.X, TieRodRod_LocalOffsetFromMaster.Y, TieRodRod_LocalOffsetFromMaster.Z,
		Hub_LocalOffsetFromMaster.X, Hub_LocalOffsetFromMaster.Y, Hub_LocalOffsetFromMaster.Z,
		Hub_LocalRotationFromMaster.X, Hub_LocalRotationFromMaster.Y, Hub_LocalRotationFromMaster.Z, Hub_LocalRotationFromMaster.W,
		Hub_LocalRotationFromMaster.Rotator().Pitch, Hub_LocalRotationFromMaster.Rotator().Yaw, Hub_LocalRotationFromMaster.Rotator().Roll
	);
}
