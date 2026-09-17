// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CarAnimInstance.generated.h"

class ACar;
/**
 * 
 */
UCLASS()
class VEHICLEPHYSICS_API UCarAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ACar* Car;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearRightRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearLeftRotation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightAngle = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftAngle = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightCenterZ = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftCenterZ = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearRightCenterZ = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearLeftCenterZ = 0.f;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontRightRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FrontLeftRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearRightRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RearLeftRoll = 0.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator FrontRight = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator FrontLeft = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator RearRight = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator RearLeft = FRotator::ZeroRotator;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosFR;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosFL;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosRR;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector PosRL;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestFR = 50.f;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestFL = 50.f;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestRR = 50.f;
	UPROPERTY(VisibleDefaultsOnly, Category="Car", meta = (AllowPrivateAccess = "true"))
	float RestRL = 50.f;
	
	
	public:
	// ---------------- Suspension inputs ----------------
	// Driven every tick by the corner's physics component (UWishboneSuspension),
	// which integrates this angle as the suspension's one real degree of freedom.
	// Left hand-settable so the rig can still be posed without physics running.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Suspension|Test")
	float FR_LCA_ArticulationAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Suspension|Test")
	float FR_UCA_ArticulationAngle = 0.f;

	// Wheel spacer / scrub-radius offset along the spindle axis, cm.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Suspension|Test")
	float FR_WheelOffset = 0.f;

	// ---------------- Bone names (re-pointable without recompiling) ----------------
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName LCA_F_Bone = TEXT("FR_LCA_F_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName LCA_B_Bone = TEXT("FR_LCA_B_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName LCA_U_Bone = TEXT("FR_LCA_U_Bone");

	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName UCA_F_Bone = TEXT("FR_UCA_F_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName UCA_B_Bone = TEXT("FR_UCA_B_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName UCA_U_Bone = TEXT("FR_UCA_U_Bone");

	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName KnuckleMasterBone = TEXT("FR_Knuckle_Master_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName TieRodInnerBone = TEXT("FR_TieRodInner_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName TieRodBodyBone = TEXT("FR_TieRodBody_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName TieRodRodBone = TEXT("FR_TieRodRod_Bone");

	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName CoiloverTopBone = TEXT("FR_CoiloverTop_Bone"); // chassis mount, fixed
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName CoiloverBodyBone = TEXT("FR_CoiloverBody_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName CoiloverRodBone = TEXT("FR_CoiloverRod_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName SpringTopBone = TEXT("FR_Spring_Top_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName SpringBotBone = TEXT("FR_Spring_Bot_Bone");

	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName HubBone = TEXT("FR_Hub_Bone");
	UPROPERTY(EditAnywhere, Category="Suspension|Bones")
	FName WheelBone = TEXT("FR_Wheel_Bone");

	// ---------------- Live outputs -> wire these into Transform (Modify) Bone ----------------
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector LCA_U_Live = FVector::ZeroVector;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector UCA_U_Live = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector KnuckleMasterPositionLive = FVector::ZeroVector;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FRotator KnuckleMasterRotationLive = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector DamperMountLive = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector CoiloverBodyPositionLive = FVector::ZeroVector;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FRotator CoiloverBodyRotationLive = FRotator::ZeroRotator;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector CoiloverRodPositionLive = FVector::ZeroVector;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FRotator CoiloverRodRotationLive = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector SpringTopPositionLive = FVector::ZeroVector;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector SpringBotPositionLive = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector TieRodBodyPositionLive = FVector::ZeroVector;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FRotator TieRodBodyRotationLive = FRotator::ZeroRotator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Suspension|Live")
	FVector WheelOffsetTranslationLive = FVector::ZeroVector; // feed as ADDITIVE, bone space

	// ---------------- Rest snapshot (captured once, never re-read live) ----------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector LCA_F_Rest = FVector(143.246658f, 57.374817f, 4.712109f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector LCA_B_Rest = FVector(114.543495f, 57.408588f, 4.699930f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector LCA_U_Rest = FVector(132.274857f, 99.871216f, 4.239235f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector LCA_HingeAxis = FVector(-0.999999f, 0.001177f, -0.000424f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector UCA_F_Rest = FVector(143.245117f, 57.373707f, 38.508877f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector UCA_B_Rest = FVector(114.541969f, 57.407295f, 38.496670f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector UCA_U_Rest = FVector(132.274857f, 99.871216f, 38.034309f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector UCA_HingeAxis = FVector(-0.999999f, 0.001170f, -0.000425f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector TieRodInner_Rest = FVector(142.839233f,57.981926f,21.090454f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector CoiloverTop_Rest = FVector(126.280006f,62.784325f,57.652046f);


	// The LCA damper-mount barycentric weights now live in WishboneGeometry.h,
	// shared with the physics layer - the coilover has to push on the same point
	// the animation draws it attached to.

	// Master's rest transform, used to derive rest-local offsets for
	// everything that's a "rigid attachment" rather than an independent
	// hardpoint (tie rod outer, hub).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FTransform Master_Rest_Transform = FTransform(
		FRotator(0.f, 0.f, 90.f).Quaternion(),  // Pitch, Yaw, Roll
		FVector(132.274857f, 99.871216f, 4.239235f),
		FVector(1.f, 1.f, 1.f));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector TieRodRod_LocalOffsetFromMaster = FVector(-7.758163f, 16.975679f, -7.306236f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FVector Hub_LocalOffsetFromMaster = FVector(-5.740471f, 16.895235f, 7.601523f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Suspension")
	FQuat Hub_LocalRotationFromMaster = FRotator(37.054752f, 0.000004f, 89.997467f).Quaternion(); // Pitch, Yaw, Roll

	bool bRestCaptured = false;

	UFUNCTION(CallInEditor, Category = "Suspension")
	void CaptureRestPose();
};