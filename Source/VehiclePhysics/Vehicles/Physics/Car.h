// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BodyDerivative.h"
#include "Transmission.h"
#include "VehicleEngine.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/BodyInstance.h"
#include "VehiclePhysics/Vehicles/Types/Wheel.h"
#include "Car.generated.h"

class UECU;
class UTorqueConverterComponent;
class UCameraComponent;
class USpringArmComponent;
class UDrivetrain;
class UDifferential;
class UTransmission;
class UVehicleEngine;
class UWheelCollider;
class STireGraphWidget;

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

	UPROPERTY(EditAnywhere, Category="Car", meta=(AllowPrivateAccess=true))
	FShaft ClutchShaft;

	UPROPERTY(EditAnywhere, Category="Car", meta=(AllowPrivateAccess=true))
	FShaft TransferCaseShaft;

	TSharedPtr<STireGraphWidget> TireGraphWidget;

	float GetEngineRPM() const {return Engine->CurrentEngineRPM;}
	int32 GetGear() const {return Drivetrain->CurrentGear;}

protected:
	UPROPERTY(EditAnywhere, Category="SceneComponents")
	UWheelCollider* FrontRight;
	
	UPROPERTY(EditAnywhere, Category="SceneComponents")
	UWheelCollider* FrontLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SceneComponents", meta=(AllowPrivateAccess=true))
	UWheelCollider* RearRight;
	
	UPROPERTY(EditAnywhere,Category="SceneComponents")
	UWheelCollider* RearLeft;
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RK2(float DeltaTime, FBodyInstance* BodyInstance, FBodyState CurrentState);
	void RK4(float DeltaTime, FBodyInstance* BodyInstance, FBodyState CurrentState);
	void DrawCOM(FBodyInstance* BodyInstance);
	void Heun(float DeltaTime, FBodyInstance* BodyInstance,
	          FBodyState CurrentBodyInfo);
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

	UPROPERTY(EditAnywhere, Category="Components")
	UTransmission* TransmissionComponent;

	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess=true))
	UDrivetrain* Drivetrain;
	
	UPROPERTY(EditAnywhere, Category="Components", meta=(AllowPrivateAccess=true))
	UECU* ECU;

	UPROPERTY(EditAnywhere, Category="Car Propperties", meta=(AllowPrivateAccess=true))
	int32 HeunNumIterations = 1;

	UPROPERTY(EditAnywhere, Category="Car Propperties", meta=(AllowPrivateAccess=true)) //Center of mass offset relative to the default position.
	FVector COMOffset; 

	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	float MaxSteerAngle = 25.f;
	
	UPROPERTY(EditAnywhere, Category="Wheel", meta=(AllowPrivateAccess=true))
	float WheelAnimZOffset = 0.f;

	UPROPERTY(EditAnywhere, Category="Car Propperties", meta=(AllowPrivateAccess=true)) //Amount of steering applied versus speed, more speed less steering
	UCurveFloat* SteerCurve; 

	UPROPERTY(EditAnywhere, Category = "Steering", meta=(AllowPrivateAccess=true)) // 100 = full Ackerman, 0 = parallel, -100 = full anti-Ackerman
	float AckermanPercent = 100.0f; 

	UPROPERTY(EditAnywhere, Category="Debug", meta=(AllowPrivateAccess=true))
	bool bVisualizeCOM = false;

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
	

	void SubstepPhysics(float DeltaTime, FBodyInstance* BodyInstance);

	FQuat ComputeOrientationDerivative(const FQuat& Orientation, const FVector& AngularVelocity);

	FBodyDerivative ComputeDerivative(
		const FBodyState& State,
		FVector Force,
		FVector Torque, float Mass, const FMatrix& InertiaInverse);

	FBodyDerivative ComputeDerivative(
		const FBodyState& State,
		FBodyInstance* BodyInstance,
		TArray<FWheelHit>& Hits) const;

	void ComputeWheelHits(
		const FBodyState& State,
		TArray<FWheelHit>& Hits,
		FBodyInstance* BodyInstance,
		float DeltaTime);

	/**
* Predicts the state of a body given the forces and torques that will be applied.
*
* @param BodyInstance               The body instance whose state we want to predict.
* @param WheelHits                  The hit information of each wheel.
* @param PredictedBodyInfo          Predicted state of the body instance.
* @param PredictionTime             The time step over which to predict (e.g. 0.5f * DeltaSeconds).
*/
	void PredictBodyState(
		FBodyInstance* BodyInstance,
		TArray<FWheelHit>& WheelHits,
		FBodyState CurrentBodyInfo,
		FBodyState& PredictedBodyInfo,
		float PredictionTime);
	
	FORCEINLINE void SetThrottle(float InputThrottle) {Throttle = InputThrottle;}
	FORCEINLINE void SetBrake(float InputBrake) {Brake = InputBrake;}
	
	FORCEINLINE void GearUp()
	{
		Drivetrain->GearUp();
		TransmissionComponent->CurrentGear = FMath::Clamp(++TransmissionComponent->CurrentGear, 0, TransmissionComponent->GearRatios.Num()-1);
	}
	
	FORCEINLINE void GearDown()
	{
		Drivetrain->GearDown();
		TransmissionComponent->CurrentGear = FMath::Clamp(--TransmissionComponent->CurrentGear, 0, TransmissionComponent->GearRatios.Num()-1);
	}
	float LastSteer = 0;
	float CurrentSteer = 0;
	void SetSteering(float Steering);
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
	// cm, distance front to rear axle
	float WheelBase; 
	// cm, distance left to right wheel
	float TrackWidth; 
	FRotator OGSpringArmRotation = FRotator(0.f, 0.f, 0.f);
	FCalculateCustomPhysics CustomPhysics = FCalculateCustomPhysics();
	float ForwardVelocityKMH = 0.f;
	float Acceleration = 0.f;
	UPROPERTY()
	TArray<UWheelCollider*> WheelColliders;

	UFUNCTION()
	void ResetCar();
};