// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Drivetrain.h"
#include "VehicleEngine.h"

UDrivetrain::UDrivetrain()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TransmissionShaft = &_TransmissionShaft;
}

void UDrivetrain::BeginPlay()
{
	Super::BeginPlay();
	
	if (TransmissionShaft == nullptr)
		TransmissionShaft = &_TransmissionShaft;

	InitializeShafts();
}

void UDrivetrain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDrivetrain::GearUp()
{
	// A request, not the change itself. The ratio only moves once the clutch is
	// open - see UpdateShift.
	TargetGear = FMath::Clamp(CurrentGear + 1, 0, GearRatios.Num() - 1);
}

void UDrivetrain::GearDown()
{
	TargetGear = FMath::Clamp(CurrentGear - 1, 0, GearRatios.Num() - 1);
}

void UDrivetrain::SetSelectedGearRatio(float Ratio)
{
	if (GearConstraints.IsValidIndex(GearRatioConstraintIndex))
	{
		GearConstraints[GearRatioConstraintIndex].Ratio = Ratio;
	}
}

void UDrivetrain::UpdateShift(float DT)
{
	if (ClutchConstraints.Num() == 0)
	{
		return;
	}
	FClutchConstraint& Clutch = ClutchConstraints[0];

	switch (ShiftPhase)
	{
	case EShiftPhase::Engaged:
	{
		float Target = FMath::Clamp(ClutchPedal, 0.f, 1.f);

		// Anti-stall: give the plate up before the engine. Slipping costs a little
		// heat; being dragged under the reference costs the engine entirely, and the
		// governor cannot save it because torque has to fight the load like anything
		// else.
		if (bAntiStall && VehicleEngine != nullptr)
		{
			const float Idle = VehicleEngine->GetIdleRPM();

			// WHAT the guard defends, which is not always idle.
			//
			// Defending idle unconditionally is what strangled the launch. With idle
			// at 800 and a 25% band the plate only locked past 1000 rpm, and 1000 rpm
			// in first through 5.25 x 3.15 is 7.5 km/h of road speed. Below that the
			// clutch was held part open with the engine at the bottom of its torque
			// curve, so the car crawled to 7 km/h and then leapt as it locked. It
			// showed on all three drivetrains because it was never the axle.
			//
			// Under power the guard defends LaunchRPM instead, so the engine is
			// allowed - required - to sit up where it makes torque while the plate
			// takes the difference. That IS a launch.
			const float LaunchBlend = ComputeLaunchBlend();
			const float Reference = FMath::Lerp(Idle, FMath::Max(LaunchRPM, Idle),
				FMath::Clamp(VehicleEngine->GetThrottle(), 0.f, 1.f) * LaunchBlend);

			const float Headroom = Reference * (StallGuardRatio - 1.f);
			if (Reference > KINDA_SMALL_NUMBER && Headroom > KINDA_SMALL_NUMBER)
			{
				// 1 at StallGuardRatio x reference, 0 at the reference: fully clamped
				// while the engine has revs in hand, fully open by the time it does not.
				const float Slack = (VehicleEngine->CurrentEngineRPM - Reference) / Headroom;
				Target = FMath::Min(Target, FMath::Clamp(Slack, 0.f, 1.f));
			}
		}

		Clutch.Engagement = Target;
		if (TargetGear != CurrentGear)
		{
			ShiftPhase = EShiftPhase::Opening;
			ShiftTimer = 0.f;
		}
		break;
	}

	case EShiftPhase::Opening:
		ShiftTimer += DT;
		Clutch.Engagement = FMath::Clamp(
			1.f - (ShiftTimer / FMath::Max(ClutchOpenSeconds, KINDA_SMALL_NUMBER)), 0.f, 1.f);
		if (ShiftTimer >= ClutchOpenSeconds)
		{
			Clutch.Engagement = 0.f;
			ShiftPhase = EShiftPhase::Changing;
		}
		break;

	case EShiftPhase::Changing:
		// Capacity is zero, so nothing is being transmitted and the ratio can change
		// without a torque step. That is the entire reason for the opening phase, and
		// what the gearbox lacked - it used to swap ratios under full lock.
		CurrentGear = FMath::Clamp(TargetGear, 0, GearRatios.Num() - 1);
		SetSelectedGearRatio(GearRatios[CurrentGear]);
		ShiftTimer = 0.f;
		ShiftPhase = EShiftPhase::Closing;
		break;

	case EShiftPhase::Closing:
		// Capacity ramps back. While it sits below what the speed mismatch demands the
		// plate slips, and in slipping it drags the two sides together - that IS the
		// rev match, and it needs no logic of its own. A slipping clutch equalises
		// speeds by construction; that is what the element is.
		ShiftTimer += DT;
		Clutch.Engagement = FMath::Clamp(
			ShiftTimer / FMath::Max(ClutchCloseSeconds, KINDA_SMALL_NUMBER), 0.f, 1.f);
		if (ShiftTimer >= ClutchCloseSeconds)
		{
			ShiftPhase = EShiftPhase::Engaged;
		}
		break;
	}
}

float UDrivetrain::ComputeLaunchBlend() const
{
	// How much of a launch this is: 1 at a standstill, 0 once the car is rolling
	// well enough that first gear turns the engine fast enough to pull on its own.
	//
	// Taken at the WHEEL rather than at the clutch, so it means one road speed
	// instead of one per gear. Scaling it by the gear would make full throttle at
	// low revs in sixth look like a launch and open the plate, when the right
	// behaviour there is to lug.
	float WheelRadPerSec = 0.f;
	int32 Counted = 0;
	const FShaft* const Wheels[4] = {
		FrontRightWheelShaft, FrontLeftWheelShaft, RearRightWheelShaft, RearLeftWheelShaft };
	for (const FShaft* W : Wheels)
	{
		if (W != nullptr)
		{
			WheelRadPerSec += FMath::Abs(W->GetAngularVelocity());
			++Counted;
		}
	}
	if (Counted == 0 || LaunchFadeWheelRPM <= KINDA_SMALL_NUMBER)
	{
		return 0.f;
	}

	const float WheelRPM = (WheelRadPerSec / Counted) * (60.f / (2.f * PI));
	return FMath::Clamp(1.f - (WheelRPM / LaunchFadeWheelRPM), 0.f, 1.f);
}

float UDrivetrain::GetShiftThrottleOverride() const
{
	if (ClutchConstraints.Num() == 0)
	{
		return -1.f;
	}

	// Throttle CLOSED while the plate comes apart and the ratio swaps.
	//
	// These phases used to fall through to -1, which means "the driver's throttle
	// applies" - so a car shifting at full throttle KEPT full throttle with the
	// clutch open, and the engine, driving nothing, ran away for the whole of
	// ClutchOpenSeconds. Closing then had to re-engage that excess speed, and a
	// plate rated at ClutchMaxTorque delivers it in one go: 400 Nm through 2.99 and
	// a 2.87 final drive is about 3400 Nm at the wheels, which is at the rear
	// tyres' limit before any cornering is asked of them. That is what broke the
	// rear away on the 2-3 upshift.
	//
	// An open differential does not help with this, which is worth stating because
	// it looks as though it should: it equalises torque between the two wheels but
	// does nothing about the total arriving at the axle.
	if (ShiftPhase == EShiftPhase::Opening || ShiftPhase == EShiftPhase::Changing)
	{
		return 0.f;
	}

	if (ShiftPhase != EShiftPhase::Closing || RevMatchGain <= 0.f)
	{
		return -1.f;
	}
	const FClutchConstraint& Clutch = ClutchConstraints[0];
	if (Clutch.Input == nullptr || Clutch.Output == nullptr)
	{
		return -1.f;
	}

	// Only where the gearbox is turning faster than the engine, i.e. a downshift.
	// Upshifts want the opposite and are better served by lifting, which the driver
	// is usually doing anyway.
	const float Shortfall = Clutch.Output->GetAngularVelocity() - Clutch.Input->GetAngularVelocity();
	return Shortfall > 0.f ? FMath::Clamp(Shortfall * RevMatchGain, 0.f, 1.f) : 0.f;
}

void UDrivetrain::RebuildDrivetrain()
{
	// SolveOrder FIRST, because it is the one that bites. It is not a constraint
	// list but the index table the solver walks - Add*Constraint appends a (kind,
	// index) pair to it - so clearing the constraint arrays and leaving it behind
	// keeps refs pointing past the end of the new, possibly shorter, arrays. AWD
	// builds more constraints than FWD, so switching down from it indexed off the
	// end and took the editor with it.
	SolveOrder.Reset();

	Connections.Reset();
	CopyConnections.Reset();
	GearConstraints.Reset();
	BrakeConstraints.Reset();
	DiffCarrierConstraints.Reset();
	LimitedSlipConstraints.Reset();
	ClutchConstraints.Reset();

	GearRatioConstraintIndex = INDEX_NONE;
	TargetGear = CurrentGear;
	ShiftPhase = EShiftPhase::Engaged;
	ShiftTimer = 0.f;

	InitializeShafts();
}

void UDrivetrain::InitializeShafts()
{
	// Define connections (from input to output, ratio >1 for reduction)
	//
	// Engine to clutch shaft is the CLUTCH, not a rigid gear. Below its capacity it
	// locks and behaves exactly as that gear did; above it the plate slips, which is
	// what stops a gear change dumping a torque step into the driven wheels.
	AddClutchConstraint(FClutchConstraint(EngineShaft, ClutchShaft, ClutchMaxTorque));
	AddGearConstraint(FGearConstraint(ClutchShaft, TransmissionShaft, 1.0f));
	
	if (DrivetrainType == EDrivetrainType::EDT_RearWheelDrive)
	{
		// Recorded so a gear change retunes THIS constraint rather than a hardcoded
		// index that moves whenever the set of constraints changes.
		GearRatioConstraintIndex = GearConstraints.Num();
		AddGearConstraint(FGearConstraint(TransmissionShaft, RearDiffShaft, GearRatios[CurrentGear]));
		
		const int32 RearDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				RearDiffShaft,
				RearRightWheelShaft,
				RearLeftWheelShaft,
				FinalDriveRatio));
		
		if (RearDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(RearRightWheelShaft,
				RearLeftWheelShaft,
				1.f));
		
		if (RearDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(RearLeftWheelShaft, RearRightWheelShaft,
				RearDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
	}
	
	if (DrivetrainType == EDrivetrainType::EDT_FrontWheelDrive)
	{
		// Recorded so a gear change retunes THIS constraint rather than a hardcoded
		// index that moves whenever the set of constraints changes.
		GearRatioConstraintIndex = GearConstraints.Num();
		AddGearConstraint(FGearConstraint(TransmissionShaft, FrontDiffShaft, GearRatios[CurrentGear]));
		
		const int32 FrontDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				FrontDiffShaft,
				FrontRightWheelShaft,
				FrontLeftWheelShaft,
				FinalDriveRatio));
		
		if (FrontDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(FrontRightWheelShaft,
				FrontLeftWheelShaft,
				1.f));
		
		if (FrontDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(FrontLeftWheelShaft, FrontRightWheelShaft,
				FrontDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
	}
	
	if (DrivetrainType == EDrivetrainType::EDT_AllWheelDrive)
	{
		// Recorded so a gear change retunes THIS constraint rather than a hardcoded
		// index that moves whenever the set of constraints changes.
		GearRatioConstraintIndex = GearConstraints.Num();
		AddGearConstraint(FGearConstraint(TransmissionShaft, RearDiffShaft, GearRatios[CurrentGear]));
		
		const int32 RearDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				RearDiffShaft,
				
				RearRightWheelShaft,
				RearLeftWheelShaft,
				FinalDriveRatio));
		
		if (RearDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(RearRightWheelShaft,
				RearLeftWheelShaft,
				1.f));
		
		if (RearDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(RearLeftWheelShaft, RearRightWheelShaft,
				RearDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
		
		AddGearConstraint(FGearConstraint(RearDiffShaft, FrontDiffShaft, 1));
		
		const int32 FrontDiffCarrierIndex =
			AddDiffCarrierConstraint(FDiffCarrierConstraint(
				FrontDiffShaft,
				FrontRightWheelShaft,
				FrontLeftWheelShaft,
				FinalDriveRatio));
		
		if (FrontDifferentialType == EDifferentialType::Locked)
			AddGearConstraint(FGearConstraint(FrontRightWheelShaft,
				FrontLeftWheelShaft,
				1.f));
		
		if (FrontDifferentialType == EDifferentialType::LimitedSlip)
			AddLimitedSlipConstraint(FLimitedSlipConstraint(FrontLeftWheelShaft, FrontRightWheelShaft,
				FrontDiffCarrierIndex, PreloadTorque, PowerLockCoeff, CoastLockCoeff, MaxCorrectionTorque));
	}
	AddBrakeConstraint(FBrakeConstraint(
		FrontRightWheelShaft,
		1.f,
		3000));
	AddBrakeConstraint(FBrakeConstraint(
		FrontLeftWheelShaft,
		1.f,
		3000));
	AddBrakeConstraint(FBrakeConstraint(
		RearRightWheelShaft,
		1.f,
		3000));
	AddBrakeConstraint(FBrakeConstraint(
		RearLeftWheelShaft,
		1.f,
		3000));
}

void UDrivetrain::PreSolveConstraints(float DT)
{
	for (FGearConstraint& C : GearConstraints)
	{
		C.AccumJ = 0.f;
		C.DeltaTime = DT;
	}

	for (FDiffCarrierConstraint& C : DiffCarrierConstraints)
	{
		C.AccumJ = 0.f;
		C.DeltaTime = DT;
	}

	for (FLimitedSlipConstraint& C : LimitedSlipConstraints)
	{
		C.AccumJ = 0.f;
		C.DeltaTime = DT;
	}

	for (FClutchConstraint& C : ClutchConstraints)
	{
		C.AccumJ = 0.f;
		C.DeltaTime = DT;
	}

	// Per SUBSTEP, not per frame. Engagement feeds the solve immediately below, and
	// the ramp is the thing keeping a shift smooth - at frame rate an 80 ms open is
	// five steps, which is barely a ramp at all.
	UpdateShift(DT);
}

int32 offset = 0;
void UDrivetrain::SolveConstraints(TArray<FGearConstraint>& Conns, float DT)
{
	PreSolveConstraints(DT);
	for(FGearConstraint& Connection : GearConstraints)
	{
		Connection.Shaft1->ResetAccumImpulse();
		Connection.Shaft2->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for(FBrakeConstraint& Connection : BrakeConstraints)
	{
		Connection.Shaft->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for(FDiffCarrierConstraint& Connection : DiffCarrierConstraints)
	{
		Connection.Input->ResetAccumImpulse();
		Connection.Left->ResetAccumImpulse();
		Connection.Right->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	for(FLimitedSlipConstraint& Connection : LimitedSlipConstraints)
	{
		Connection.Left->ResetAccumImpulse();
		Connection.Right->ResetAccumImpulse();
		Connection.AccumJ = 0.f;
		Connection.DeltaTime = DT;
	}
	const int32 NumRefs = SolveOrder.Num();
	if (NumRefs == 0)
		return;
	
	for (int32 Iter = 0; Iter < SolverIterations; ++Iter)
	{
		offset = offset % NumRefs;
		const int32 IterOffset = (offset + Iter) % NumRefs;
		
		for (int32 j = 0; j < NumRefs; ++j)
		{
			const int32 RefIndex = (j + IterOffset) % NumRefs;
			const FConstraintRef& Ref = SolveOrder[RefIndex];

			switch (Ref.Kind)
			{
			case EConstraintKind::Gear:
				SolveGearConstraint(GearConstraints[Ref.Index]);
				break;

			case EConstraintKind::DiffCarrier:
				SolveDiffCarrierConstraint(DiffCarrierConstraints[Ref.Index]);
				break;

			case EConstraintKind::LimitedSlip:
				SolveLimitedSlipConstraint(LimitedSlipConstraints[Ref.Index]);
				break;

			case EConstraintKind::Clutch:
				SolveClutchConstraint(ClutchConstraints[Ref.Index]);
				break;

			case EConstraintKind::Brake:
				SolveBrakeConstraint(BrakeConstraints[Ref.Index]);
				break;
			}
		}
		//offset = (offset + 1) % NumRefs;
	}
	offset = (offset + 1) % NumRefs;
}

void UDrivetrain::SolveGearConstraint(FGearConstraint&Constraint)
{
	const float R = Constraint.Ratio;

	float Win = Constraint.Shaft1->GetAngularVelocity();
	float Wout = Constraint.Shaft2->GetAngularVelocity();

	float DeltaOmega = Win - Wout * R;

	if (FMath::Abs(DeltaOmega) < KINDA_SMALL_NUMBER)
		return;

	float InvIin = Constraint.Shaft1->GetInvInertia();
	float InvIout = Constraint.Shaft2 ? Constraint.Shaft2->GetInvInertia() * FMath::Square(R) : 0.f;

	float InvISum = InvIin + InvIout;

	if (FMath::Abs(InvISum) < KINDA_SMALL_NUMBER)
		return;

	float J = DeltaOmega / InvISum;

	Constraint.AccumJ += J;

	Constraint.Shaft1->ApplyImpulse(-J);
	Constraint.Shaft2->ApplyImpulse(J * R);
}

void UDrivetrain::SolveBrakeConstraint(FBrakeConstraint& Constraint)
{
	const float R = Constraint.Ratio;

	float WheelAngularVelocity = Constraint.Shaft->GetAngularVelocity();

	float DeltaOmega = WheelAngularVelocity;

	if (FMath::Abs(DeltaOmega) < KINDA_SMALL_NUMBER)
		return;

	float InvI = Constraint.Shaft->GetInvInertia();
		
	if (FMath::Abs(InvI) < KINDA_SMALL_NUMBER)
		return;

	float J = DeltaOmega / InvI;
	J *= Constraint.ConnectionStrength;

	Constraint.AccumJ += J;

	Constraint.Shaft->ApplyImpulse(-J);
}

void UDrivetrain::SolveDiffCarrierConstraint(FDiffCarrierConstraint& Constraint)
{
	const float R = Constraint.Ratio;

	float Win = Constraint.Input->GetAngularVelocity();
	float Wl  = Constraint.Left->GetAngularVelocity();
	float Wr  = Constraint.Right->GetAngularVelocity();
		
	float Cdot = Win - (Wl + Wr) * R * 0.5f;

	float InvIin = Constraint.Input->GetInvInertia();
	float InvIl  = Constraint.Left->GetInvInertia() * FMath::Square(R * 0.5f);
	float InvIr  = Constraint.Right->GetInvInertia() * FMath::Square(R * 0.5f);

	float InvISum = InvIin + InvIl + InvIr;
	if (FMath::Abs(InvISum) < KINDA_SMALL_NUMBER)
		return;

	float J = Cdot / InvISum;
	Constraint.AccumJ += J;
	Constraint.Input->ApplyImpulse(-J);
	Constraint.Left->ApplyImpulse(J * R * 0.5f);
	Constraint.Right->ApplyImpulse(J * R * 0.5f);
}

void UDrivetrain::SolveClutchConstraint(FClutchConstraint& Clutch)
{
	if (Clutch.Input == nullptr || Clutch.Output == nullptr)
	{
		return;
	}

	const float InvISum = Clutch.Input->GetInvInertia() + Clutch.Output->GetInvInertia();
	if (InvISum < KINDA_SMALL_NUMBER)
	{
		return;
	}

	// The impulse that would lock the two sides together this substep.
	const float DeltaOmega = Clutch.Input->GetAngularVelocity() - Clutch.Output->GetAngularVelocity();
	float J = DeltaOmega / InvISum;

	// Capacity is a property of the plate, not something derived from solver state,
	// which is why this needs no second MaxCorrectionTorque ceiling the way the LSD
	// does. That one takes its capacity from the carrier's own accumulated impulse -
	// a feedback path that can wind up. There is no such loop here.
	const float Capacity = Clutch.MaxTorque * FMath::Clamp(Clutch.Engagement, 0.f, 1.f);
	const float MaxAccum = Capacity * Clutch.DeltaTime;

	// Clamp the ACCUMULATED impulse, not this iteration's share of it. The
	// per-iteration form in SolveLimitedSlipConstraint under-delivers whenever an
	// iteration fails to saturate, so its effective capacity drifts with
	// SolverIterations - change the iteration count and the diff quietly behaves
	// differently. Worth back-porting there.
	const float Old = Clutch.AccumJ;
	Clutch.AccumJ = FMath::Clamp(Clutch.AccumJ + J, -MaxAccum, MaxAccum);
	J = Clutch.AccumJ - Old;

	Clutch.Input->ApplyImpulse(-J);
	Clutch.Output->ApplyImpulse(J);
}

void UDrivetrain::SolveLimitedSlipConstraint(FLimitedSlipConstraint& LSD)
{
	float Wl = LSD.Left->GetAngularVelocity();
	float Wr = LSD.Right->GetAngularVelocity();

	float DeltaOmega = Wl - Wr;
	if (FMath::Abs(DeltaOmega) < KINDA_SMALL_NUMBER)
		return;

	float InvIl = LSD.Left->GetInvInertia();
	float InvIr = LSD.Right->GetInvInertia();

	float InvISum = InvIl + InvIr;
	if (FMath::Abs(InvISum) < KINDA_SMALL_NUMBER)
		return;

	float J = DeltaOmega / InvISum;

	float CarrierTorque = 0.f;
	if (DiffCarrierConstraints.IsValidIndex(LSD.CarrierConstraintIndex))
	{
		const FDiffCarrierConstraint& Carrier = DiffCarrierConstraints[LSD.CarrierConstraintIndex];
		CarrierTorque = Carrier.AccumJ != 0.f ? FMath::Abs(Carrier.AccumJ / LSD.DeltaTime) : 0.f;
	}

	float AllowedTorque = LSD.PreloadTorque + LSD.PowerLockCoeff * CarrierTorque;
	AllowedTorque = FMath::Min(AllowedTorque, LSD.MaxCorrectionTorque);

	float MaxJ = AllowedTorque * LSD.DeltaTime / SolverIterations;
	J = FMath::Clamp(J, -MaxJ, MaxJ);

	LSD.AccumJ += J;
	LSD.Left->ApplyImpulse(-J);
	LSD.Right->ApplyImpulse(J);
}