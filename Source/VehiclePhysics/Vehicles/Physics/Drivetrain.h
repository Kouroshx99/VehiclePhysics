// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Connection.h"
#include "Components/ActorComponent.h"
#include "Drivetrain.generated.h"

class UVehicleEngine;

UENUM(BlueprintType)
enum class EDrivetrainType: uint8
{
	EDT_FrontWheelDrive UMETA(DisplayName="FrontWheelDrive"),
	EDT_RearWheelDrive UMETA(DisplayName="RearWheelDrive"),
	EDT_AllWheelDrive UMETA(DisplayName="AllWheelDrive")
};

/**
 * Where a gear change has got to.
 *
 * A shift is a process, not an instant, which is why GearUp/GearDown only record
 * a target and the work happens over the substeps that follow. Changing the ratio
 * while the clutch still carries torque is exactly the lock-up that shocks the
 * driveline and spins the driven wheels.
 */
UENUM()
enum class EShiftPhase : uint8
{
    Engaged     UMETA(DisplayName = "Engaged"),
    Opening     UMETA(DisplayName = "Opening"),
    Changing    UMETA(DisplayName = "Changing"),
    Closing     UMETA(DisplayName = "Closing")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VEHICLEPHYSICS_API UDrivetrain : public UActorComponent
{
	GENERATED_BODY()

public:
	UDrivetrain();
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	int32 SolverIterations = 10;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	FShaft _TransmissionShaft;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	EDrivetrainType DrivetrainType = EDrivetrainType::EDT_FrontWheelDrive;

	/**
	 * Throws the constraint graph away and builds it again from the CURRENT
	 * DrivetrainType and differential settings.
	 *
	 * Needed because InitializeShafts runs once at BeginPlay, so the layout is
	 * baked from whatever the enum said then. Writing DrivetrainType afterwards
	 * changes the field and nothing else - the shafts stay wired the way they
	 * were, and the car keeps driving through the same axle while the details
	 * panel claims otherwise. Anything that retunes the drivetrain while running
	 * has to call this, or it is only pretending to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Drivetrain")
	void RebuildDrivetrain();

	/**
	 * True while a gear change is running, i.e. the clutch is not fully clamped.
	 *
	 * Anything deciding when to shift has to check this. Engine speed is only a
	 * meaningful signal while the clutch is connected - mid-shift the plate is open
	 * and the engine flares against nothing, which reads as "time to upshift" to any
	 * controller watching revs alone.
	 */
	UFUNCTION(BlueprintPure, Category = "Drivetrain")
	bool IsShifting() const { return ShiftPhase != EShiftPhase::Engaged; }

	/** How clamped the clutch is, 0 open to 1 locked. Diagnostic. */
	UFUNCTION(BlueprintPure, Category = "Drivetrain")
	float GetClutchEngagement() const
	{
		return ClutchConstraints.Num() > 0 ? ClutchConstraints[0].Engagement : 1.f;
	}
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	EDifferentialType FrontDifferentialType = EDifferentialType::Open;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	EDifferentialType RearDifferentialType = EDifferentialType::Open;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential", meta=(unit="N.m"))
	float PreloadTorque = 50;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential")
	float PowerLockCoeff = 0.35f;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential")
	float CoastLockCoeff = 0.15f;
	
	UPROPERTY(EditAnywhere, Category="Limited Slip Differential", meta=(unit="N.m"))
	float MaxCorrectionTorque = 700;

	// ---------------- Clutch ----------------

	/**
	 * Torque the plate carries fully clamped, N*m.
	 *
	 * The whole tuning surface, and a real number rather than a feel knob: size it
	 * at 1.3-1.5x peak engine torque, as manufacturers do. Too high and it never
	 * slips, which is the instant lock-up this exists to remove; too low and it
	 * slips permanently and the car will not drive.
	 */
	UPROPERTY(EditAnywhere, Category = "Clutch", meta = (ClampMin = "0.0"))
	float ClutchMaxTorque = 400.f;

	UPROPERTY(EditAnywhere, Category = "Clutch", meta = (ClampMin = "0.0", Units = "s"))
	float ClutchOpenSeconds = 0.08f;

	UPROPERTY(EditAnywhere, Category = "Clutch", meta = (ClampMin = "0.01", Units = "s"))
	float ClutchCloseSeconds = 0.25f;

	/**
	 * Throttle applied per rad/s of engine shortfall while re-engaging a downshift.
	 * Zero disables the blip entirely - the clutch still matches the speeds.
	 */
	UPROPERTY(EditAnywhere, Category = "Clutch", meta = (ClampMin = "0.0"))
	float RevMatchGain = 0.02f;

	/** Driver's pedal, 1 = up. Only read while Engaged; a shift overrides it. */
	UPROPERTY(BlueprintReadWrite, Category = "Clutch")
	float ClutchPedal = 1.f;

	/**
	 * Ease the clutch off rather than let the wheels drag the engine below idle.
	 *
	 * With the plate clamped and the car stopped, the wheels hold the engine and it
	 * stalls. That is exactly right for a manual and almost never what is wanted, so
	 * this does what a driver's left foot does, and what an automated box does for
	 * them. Turn it off for a car that should be stallable.
	 *
	 * It is also where creep comes from: the plate slips, the engine keeps turning,
	 * and the torque that does cross moves the car.
	 */
	UPROPERTY(EditAnywhere, Category = "Clutch")
	bool bAntiStall = true;

	/** Anti-stall starts easing off at this multiple of the reference, out at the reference. */
	UPROPERTY(EditAnywhere, Category = "Clutch", meta = (ClampMin = "1.01", ClampMax = "3.0"))
	float StallGuardRatio = 1.25f;

	/**
	 * Engine speed the anti-stall guard defends AT FULL THROTTLE, rpm.
	 *
	 * Idle is the right thing to defend when the driver is off the throttle - that
	 * is creep. It is the wrong thing to defend when they are asking for everything,
	 * because a car pulling away wants the plate slipping while the engine sits up
	 * on the torque curve, not pinned at the bottom of it.
	 *
	 * Set this near peak torque. Below it, the guard trades clutch slip for engine
	 * speed, which is exactly what a driver's left foot does on a hard launch.
	 */
	UPROPERTY(EditAnywhere, Category = "Clutch", meta = (ClampMin = "0.0"))
	float LaunchRPM = 2500.f;

	/**
	 * Wheel speed at which launch assistance has faded out entirely, rpm.
	 *
	 * Measured at the WHEEL deliberately, so it means one road speed rather than one
	 * per gear. Full throttle at low revs in sixth is lugging, not launching, and
	 * must not open the plate; at 150 rpm on a 0.33 m wheel this stops helping at
	 * about 19 km/h, by which point first gear is turning the engine fast enough to
	 * pull on its own.
	 */
	UPROPERTY(EditAnywhere, Category = "Clutch", meta = (ClampMin = "1.0"))
	float LaunchFadeWheelRPM = 150.f;

	/**
	 * Throttle the shift wants, or -1 when the driver's own throttle should apply.
	 *
	 * Only ever raises the engine to meet a faster gearbox on a downshift. The
	 * clutch would drag the two together regardless; blipping just means the plate
	 * absorbs less of that energy, and the car is not slowed by hauling the engine
	 * up on its own.
	 */
	UFUNCTION(BlueprintPure, Category = "Clutch")
	float GetShiftThrottleOverride() const;

	/** 1 at a standstill, 0 once rolling - see ComputeLaunchBlend's own note. */
	float ComputeLaunchBlend() const;
	
	UPROPERTY(EditAnywhere, Category="Drivetrain")
	float FinalDriveRatio = 3.15f;

	UPROPERTY(EditAnywhere, Category="Drivetrain")
	// Index 0 is REVERSE and it is NEGATIVE. The ratio is fed straight into a
	// FGearConstraint between the transmission shaft and the differential, so the sign
	// is the direction of travel: a positive value at index 0 is a ninth forward gear
	// wearing reverse's label, and selecting it drives the car forwards.
	//
	// It was +3.71 here, which means reverse has never worked on this default set. Left
	// at the same magnitude and negated rather than retuned - that is a separate
	// question from whether the sign was right.
	TArray<float> GearRatios {-3.71f, 5.25f, 3.36f, 2.17f, 1.72f, 1.32f, 1.f, 0.82f, 0.64f};
	
	int32 CurrentGear = 1;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void GearUp();
	void GearDown();

	//Shafts for the chain
	UPROPERTY()
	UVehicleEngine* VehicleEngine;
	
	FShaft* EngineShaft;
	FShaft* ClutchShaft;
	FShaft* TransmissionShaft;
	FShaft* TransferCaseShaft;
	FShaft* FrontDiffShaft;
	FShaft* RearDiffShaft;
	FShaft* FrontRightWheelShaft;
	FShaft* FrontLeftWheelShaft;
	FShaft* RearRightWheelShaft;
	FShaft* RearLeftWheelShaft;

	TArray<FGearConstraint> Connections;
	TArray<FGearConstraint> CopyConnections;
	
	TArray<FGearConstraint> GearConstraints;
	TArray<FBrakeConstraint> BrakeConstraints;
	TArray<FDiffCarrierConstraint> DiffCarrierConstraints;
	TArray<FLimitedSlipConstraint> LimitedSlipConstraints;
	TArray<FClutchConstraint> ClutchConstraints;

	/** Gear the box is heading for. GearUp/GearDown set it; UpdateShift executes it. */
	int32 TargetGear = 1;
	EShiftPhase ShiftPhase = EShiftPhase::Engaged;
	float ShiftTimer = 0.f;

	/**
	 * Which GearConstraint carries the selected ratio.
	 *
	 * Recorded rather than hardcoded. GearUp/GearDown used to write
	 * GearConstraints[2], correct only for the exact set InitializeShafts happened
	 * to build - and replacing the engine-to-clutch rigid gear with the clutch
	 * removes an entry and shifts that index by one, silently retuning the wrong
	 * constraint.
	 */
	int32 GearRatioConstraintIndex = INDEX_NONE;
	TArray<FConstraintRef> SolveOrder;

private:

	void InitializeShafts();

	/** Advances the gear change. Called once per substep from PreSolveConstraints. */
	void UpdateShift(float DT);

	void SetSelectedGearRatio(float Ratio);
	
	int32 AddGearConstraint(const FGearConstraint& Constraint)
	{
		const int32 Index = GearConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::Gear, Index));
		return Index;
	}
	
	int32 AddBrakeConstraint(const FBrakeConstraint& Constraint)
	{
		const int32 Index = BrakeConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::Brake, Index));
		return Index;
	}

	int32 AddDiffCarrierConstraint(const FDiffCarrierConstraint& Constraint)
	{
		const int32 Index = DiffCarrierConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::DiffCarrier, Index));
		return Index;
	}

	void AddClutchConstraint(const FClutchConstraint& Constraint)
	{
		const int32 Index = ClutchConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::Clutch, Index));
	}

	int32 AddLimitedSlipConstraint(const FLimitedSlipConstraint& Constraint)
	{
		const int32 Index = LimitedSlipConstraints.Add(Constraint);
		SolveOrder.Add(FConstraintRef(EConstraintKind::LimitedSlip, Index));
		return Index;
	}
public:
	void PreSolveConstraints(float DT);
	void SolveConstraints(TArray<FGearConstraint>& Conns, float DT);
	void SolveGearConstraint(FGearConstraint& Constraint);
	void SolveBrakeConstraint(FBrakeConstraint& Constraint);
	void SolveDiffCarrierConstraint(FDiffCarrierConstraint& Constraint);
	void SolveLimitedSlipConstraint(FLimitedSlipConstraint& LSD);
	void SolveClutchConstraint(FClutchConstraint& Clutch);
	
};