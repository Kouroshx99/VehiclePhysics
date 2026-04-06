// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyState.h"
#include "Car.h"
#include "DrawDebugHelpers.h"


///Postponed
void ACar::RK2(float DeltaTime, FBodyInstance* BodyInstance, FBodyState CurrentState)
{
	TArray<FWheelHit> k1Hits, k2Hits;
	TArray<FWheelSolverData> SolverData;
	SolverData.Add(FrontRight->GetSolverData());
	SolverData.Add(FrontLeft->GetSolverData());
	SolverData.Add(RearRight->GetSolverData());
	SolverData.Add(RearLeft->GetSolverData());
    
	// Stage 1: k1
	ComputeWheelHits(CurrentState, k1Hits, BodyInstance, DeltaTime);
	FBodyDerivative k1 = ComputeDerivative(CurrentState, BodyInstance, k1Hits);
	TArray<FWheelSolverData> SolverDataK1;
	SolverDataK1.Add(FrontRight->GetSolverData());
	SolverDataK1.Add(FrontLeft->GetSolverData());
	SolverDataK1.Add(RearRight->GetSolverData());
	SolverDataK1.Add(RearLeft->GetSolverData());
	//FrontRight->SetSolverData((SolverData[0] + SolverDataK1[0])/2);
	//FrontLeft->SetSolverData((SolverData[1] + SolverDataK1[1])/2);
	//RearRight->SetSolverData((SolverData[2] + SolverDataK1[2])/2);
	//RearLeft->SetSolverData((SolverData[3] + SolverDataK1[3])/2);
	
	// Stage 2: k2
	FBodyState state2;
	state2.Location = CurrentState.Location + k1.dPosition * (0.5f * DeltaTime);
	state2.Velocity = CurrentState.Velocity + k1.dVelocity * (0.5f * DeltaTime);
	FVector Axis;
	float Angle;
	k1.dRotation.ToAxisAndAngle(Axis, Angle);
	float ScaledAngle = Angle * DeltaTime;
	FQuat k1RotationDelta = FQuat(Axis, ScaledAngle);
	k1RotationDelta.Normalize();
	state2.Rotation = (k1RotationDelta * CurrentState.Rotation).GetNormalized();
	state2.AngularVelocity = CurrentState.AngularVelocity + k1.dAngularVelocity * (0.5f * DeltaTime);
	ComputeWheelHits(state2, k2Hits, BodyInstance, DeltaTime);
	TArray<FWheelSolverData> SolverDataK2;
	SolverDataK2.Add(FrontRight->GetSolverData());
	SolverDataK2.Add(FrontLeft->GetSolverData());
	SolverDataK2.Add(RearRight->GetSolverData());
	SolverDataK2.Add(RearLeft->GetSolverData());
	//FrontRight->SetSolverData((SolverDataK1[0] + SolverDataK2[0])/2);
	//FrontLeft->SetSolverData((SolverDataK1[1] + SolverDataK2[1])/2);
	//RearRight->SetSolverData((SolverDataK1[2] + SolverDataK2[2])/2);
	//RearLeft->SetSolverData((SolverDataK1[3] + SolverDataK2[3])/2);
	FrontRight->Integrate(k2Hits[0], BodyInstance);
	FrontLeft->Integrate(k2Hits[1], BodyInstance);
	RearRight->Integrate(k2Hits[2], BodyInstance);
	RearLeft->Integrate(k2Hits[3], BodyInstance);
	//DrawDebugBox(GetWorld(), CurrentState.Location, FVector(200.f, 100.f, 30.f), state2.Rotation, FColor::Purple);
}

///Postponed
void ACar::RK4(float DeltaTime, FBodyInstance* BodyInstance, FBodyState CurrentState)
{
	TArray<FWheelHit> k1Hits, k2Hits, k3Hits, k4Hits;
	TArray<FWheelSolverData> SolverData;
	SolverData.Add(FrontRight->GetSolverData());
	SolverData.Add(FrontLeft->GetSolverData());
	SolverData.Add(RearRight->GetSolverData());
	SolverData.Add(RearLeft->GetSolverData());
    
	// Stage 1: k1
	ComputeWheelHits(CurrentState, k1Hits, BodyInstance, DeltaTime);
	FBodyDerivative k1 = ComputeDerivative(CurrentState, BodyInstance, k1Hits);
	TArray<FWheelSolverData> SolverDataK1;
	SolverDataK1.Add(FrontRight->GetSolverData());
	SolverDataK1.Add(FrontLeft->GetSolverData());
	SolverDataK1.Add(RearRight->GetSolverData());
	SolverDataK1.Add(RearLeft->GetSolverData());
	
	// Stage 2: k2
	FBodyState state2;
	state2.Location = CurrentState.Location + k1.dPosition * (0.5f * DeltaTime);
	state2.Velocity = CurrentState.Velocity + k1.dVelocity * (0.5f * DeltaTime);
	FVector Axis;
	float Angle;
	k1.dRotation.ToAxisAndAngle(Axis, Angle);
	float ScaledAngle = Angle * DeltaTime;
	FQuat k1RotationDelta = FQuat(Axis, ScaledAngle);
	k1RotationDelta.Normalize();
	state2.Rotation = (k1RotationDelta * CurrentState.Rotation).GetNormalized();
	state2.AngularVelocity = CurrentState.AngularVelocity + k1.dAngularVelocity * (0.5f * DeltaTime);

	//FrontRight->SetSolverData((SolverData[0] + SolverDataK1[0])/2);
	//FrontLeft->SetSolverData((SolverData[1] + SolverDataK1[1])/2);
	//RearRight->SetSolverData((SolverData[2] + SolverDataK1[2])/2);
	//RearLeft->SetSolverData((SolverData[3] + SolverDataK1[3])/2);
	ComputeWheelHits(state2, k2Hits, BodyInstance, DeltaTime);
	FBodyDerivative k2 = ComputeDerivative(state2, BodyInstance, k2Hits);
	TArray<FWheelSolverData> SolverDataK2;
	SolverDataK2.Add(FrontRight->GetSolverData());
	SolverDataK2.Add(FrontLeft->GetSolverData());
	SolverDataK2.Add(RearRight->GetSolverData());
	SolverDataK2.Add(RearLeft->GetSolverData());
	//FrontRight->SetSolverData(SolverData[0]);
	//FrontLeft->SetSolverData(SolverData[1]);
	//RearRight->SetSolverData(SolverData[2]);
	//RearLeft->SetSolverData(SolverData[3]);

	
	// Stage 3: k3
	FBodyState state3;
	state3.Location = CurrentState.Location + k2.dPosition * (0.5f * DeltaTime);
	state3.Velocity = CurrentState.Velocity + k2.dVelocity * (0.5f * DeltaTime);
	k2.dRotation.ToAxisAndAngle(Axis, Angle);
	ScaledAngle = Angle * DeltaTime;
	FQuat k2RotationDelta = FQuat(Axis, ScaledAngle);
	k2RotationDelta.Normalize();
	state3.Rotation = (k2RotationDelta * CurrentState.Rotation).GetNormalized();
	state3.AngularVelocity = CurrentState.AngularVelocity + k2.dAngularVelocity * (0.5f * DeltaTime);
	//FrontRight->SetSolverData((SolverDataK1[0] + SolverDataK2[0])/2);
	//FrontLeft->SetSolverData((SolverDataK1[1] + SolverDataK2[1])/2);
	//RearRight->SetSolverData((SolverDataK1[2] + SolverDataK2[2])/2);
	//RearLeft->SetSolverData((SolverDataK1[3] + SolverDataK2[3])/2);
	//FrontRight->SetSolverData((SolverData[0] + FrontRight->GetSolverData())/2.f);
	//FrontLeft->SetSolverData((SolverData[1] + FrontLeft->GetSolverData())/2.f);
	//RearRight->SetSolverData((SolverData[2] + RearRight->GetSolverData())/2.f);
	//RearLeft->SetSolverData((SolverData[3] + RearLeft->GetSolverData())/2.f);
	ComputeWheelHits(state3, k3Hits, BodyInstance, DeltaTime);
	FBodyDerivative k3 = ComputeDerivative(state3, BodyInstance, k3Hits);
	TArray<FWheelSolverData> SolverDataK3;
	SolverDataK3.Add(FrontRight->GetSolverData());
	SolverDataK3.Add(FrontLeft->GetSolverData());
	SolverDataK3.Add(RearRight->GetSolverData());
	SolverDataK3.Add(RearLeft->GetSolverData());

	// Stage 4: k4
	FBodyState state4;
	state4.Location = CurrentState.Location + k3.dPosition * DeltaTime;
	state4.Velocity = CurrentState.Velocity + k3.dVelocity * DeltaTime;
	k3.dRotation.ToAxisAndAngle(Axis, Angle);
	ScaledAngle = Angle * DeltaTime;
	FQuat k3RotationDelta = FQuat(Axis, ScaledAngle);
	k3RotationDelta.Normalize();
	state4.Rotation = (k3RotationDelta * CurrentState.Rotation).GetNormalized();
	state4.AngularVelocity = CurrentState.AngularVelocity + k3.dAngularVelocity * DeltaTime;
	//FrontRight->SetSolverData(SolverDataK3[0]);
	//FrontLeft->SetSolverData(SolverDataK3[1]);
	//RearRight->SetSolverData(SolverDataK3[2]);
	//RearLeft->SetSolverData(SolverDataK3[3]);
	ComputeWheelHits(state4, k4Hits, BodyInstance, DeltaTime);
	FBodyDerivative k4 = ComputeDerivative(state4, BodyInstance, k4Hits);

	TArray<FWheelHit> Final;
	Final.Add((k1Hits[0] + k2Hits[0] * 2 + k3Hits[0] * 2 + k4Hits[0] * 0)/6);
	Final.Add((k1Hits[1] + k2Hits[1] * 2 + k3Hits[1] * 2 + k4Hits[1] * 0)/6);
	Final.Add((k1Hits[2] + k2Hits[2] * 2 + k3Hits[2] * 2 + k4Hits[2] * 0)/6);
	Final.Add((k1Hits[3] + k2Hits[3] * 2 + k3Hits[3] * 2 + k4Hits[3] * 0)/6);

	FrontRight->Integrate(Final[0], BodyInstance);
	FrontLeft->Integrate(Final[1], BodyInstance);
	RearRight->Integrate(Final[2], BodyInstance);
	RearLeft->Integrate(Final[3], BodyInstance);
	return;

	// Combine derivatives using RK4 weights
	FBodyDerivative finalDeriv;
	finalDeriv.dPosition = (k1.dPosition +
		2*k2.dPosition +
		2*k3.dPosition +
		k4.dPosition) * (1.f/6.f);
	finalDeriv.dVelocity = (k1.dVelocity +
		2*k2.dVelocity +
		2*k3.dVelocity +
		k4.dVelocity) * (1.f/6.f);
	finalDeriv.dRotation = (k1.dRotation +
		k2.dRotation * 2 +
		k3.dRotation * 2 +
		k4.dRotation) * (1.f/6.f);
	finalDeriv.dAngularVelocity = (k1.dAngularVelocity +
		2*k2.dAngularVelocity +
		2*k3.dAngularVelocity +
		k4.dAngularVelocity) * (1.f/6.f);

	// Final integration
	FBodyState finalState;
	finalState.Location = CurrentState.Location + finalDeriv.dPosition * DeltaTime;
	finalState.Velocity = CurrentState.Velocity + finalDeriv.dVelocity * DeltaTime;
	finalState.Rotation = (CurrentState.Rotation + finalDeriv.dRotation * DeltaTime).GetNormalized();
	finalState.AngularVelocity = CurrentState.AngularVelocity + finalDeriv.dAngularVelocity * DeltaTime;

	DrawDebugBox(GetWorld(), CurrentState.Location, FVector(200.f, 100.f, 30.f), CurrentState.Rotation, FColor::Red);
	DrawDebugBox(GetWorld(), state2.Location, FVector(200.f, 100.f, 30.f), state2.Rotation, FColor::Purple);
	DrawDebugBox(GetWorld(), state3.Location, FVector(200.f, 100.f, 30.f), state3.Rotation, FColor::Purple);
	DrawDebugBox(GetWorld(), state4.Location, FVector(200.f, 100.f, 30.f), state4.Rotation, FColor::Purple);
	DrawDebugBox(GetWorld(), finalState.Location, FVector(200.f, 100.f, 30.f), finalState.Rotation, FColor::White);

	// Compute final wheel hits and integrate
	TArray<FWheelHit> finalHits;
	//SolverData.Empty();
	ComputeWheelHits(finalState, finalHits, BodyInstance, DeltaTime);
    
	FrontRight->Integrate(finalHits[0], BodyInstance);
	FrontLeft->Integrate(finalHits[1], BodyInstance);
	RearRight->Integrate(finalHits[2], BodyInstance);
	RearLeft->Integrate(finalHits[3], BodyInstance);
}

///Postponed
///Compute orientation derivative (dOrientation/dt = 0.5 * orientation * angularVelocity)
FQuat ACar::ComputeOrientationDerivative(const FQuat& Orientation, const FVector& AngularVelocity) {
	FQuat OmegaQuat(AngularVelocity.X, AngularVelocity.Y, AngularVelocity.Z, 0.0f);
	return Orientation * OmegaQuat * 0.5f; //Why multiply by 0.5?
}

FBodyDerivative ACar::ComputeDerivative(const FBodyState& State, FVector Force, FVector Torque, float Mass,
	const FMatrix& InertiaInverse)
{
	FVector CalculatedAcceleration = Force / Mass;
	FVector AngularAcceleration = InertiaInverse.TransformVector(Torque);
	FQuat AngularVelocity = ComputeOrientationDerivative(State.Rotation, State.AngularVelocity);
	return {State.Velocity, CalculatedAcceleration, AngularVelocity, AngularAcceleration};
}

FBodyDerivative ACar::ComputeDerivative(const FBodyState& State, FBodyInstance* BodyInstance,
	TArray<FWheelHit>& Hits) const
{
	FBodyDerivative Derivative;

	// Derivative of position (velocity)
	Derivative.dPosition = State.Velocity;
	
	const float Mass = BodyInstance->GetBodyMass();

	// Calculate total force (gravity + wheel forces)
	FVector TotalForce = Mass * FVector(0.f, 0.f, GetWorld()->GetGravityZ());
	for (FWheelHit& Hit : Hits)
	{
		TotalForce += Hit.CalculateTotalForce();
	}

	// Derivative of velocity (acceleration)
	Derivative.dVelocity = TotalForce / Mass;

	// Calculate total torque from wheels
	FVector TotalTorque = FVector::ZeroVector;
	for (FWheelHit& Hit : Hits)
	{
		TotalTorque += Hit.CalculateTotalTorque(State.Location);
	}

	// Get inertia tensor and its inverse (assuming diagonal)
	const FVector InertiaTensor = BodyInstance->GetBodyInertiaTensor();
	FVector InertiaInverse(
		InertiaTensor.X != 0.f ? 1.f / InertiaTensor.X : 0.f,
		InertiaTensor.Y != 0.f ? 1.f / InertiaTensor.Y : 0.f,
		InertiaTensor.Z != 0.f ? 1.f / InertiaTensor.Z : 0.f
	);

	// Calculate angular acceleration in local space
	FVector LocalTorque = State.Rotation.UnrotateVector(TotalTorque);
	FVector LocalAngularAcceleration(
		LocalTorque.X * InertiaInverse.X,
		LocalTorque.Y * InertiaInverse.Y,
		LocalTorque.Z * InertiaInverse.Z
	);

	// Transform back to world space
	Derivative.dAngularVelocity = State.Rotation.RotateVector(LocalAngularAcceleration);

	// Derivative of orientation (quaternion rate of change)
	FQuat omegaQuat = FQuat(State.AngularVelocity.GetSafeNormal(), State.AngularVelocity.Size());
	Derivative.dRotation = (State.Rotation * omegaQuat) * 0.5f;
	Derivative.dRotation = omegaQuat;
	Derivative.dRotation.Normalize();
	//DrawDebugBox(GetWorld(), State.Location, FVector(200.f, 100.f, 30.f), Derivative.dRotation, FColor::White);
	//Derivative.dRotation = omegaQuat;

	return Derivative;
}