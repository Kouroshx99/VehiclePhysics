#pragma once

#include "PacejkaResult.h"
#include "TransientState.h"
#include "Engine/DataAsset.h"
#include "TireParamsDataAsset.generated.h"

UCLASS(BlueprintType)
class VEHICLEPHYSICS_API UTireParamsDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	// Operating point previewed by the tire graph editor. These drive the asset
	// editor's plot only - the runtime gets its slips from UWheelCollider.
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true"))
	float SlipRatio = 0.f;    // kappa, dimensionless
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true"))
	float SlipAngle = 0.f;     // alpha [rad]
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true"))
	float Camber = 0.f;  	// gamma [rad]

	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	float FZ0 = 3000.f;    // Nominal load
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	float R0 = 0.33f;     // effective unloaded radius [m]
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	float pi0 = 200000.f;  	// Nominal (reference) tire inflation pressure
	
	// Actual tire inflation pressure [Pa]. An authored setup value, not something
	// derived - road cars run roughly 200-250 kPa cold. Only the offset from pi0
	// matters: dpi = (pi - pi0)/pi0, which scales every pp** term in the force
	// equations and ppFz1 in the vertical model (4.E68). Gated by
	// bEnablePressureSensitivity.
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	float pi = 200000.f;

	/**
	 * Enables the speed-dependent friction decay of (4.E7)/(4.E8). Off by default:
	 * the decay rate LMUV has never been tuned for this tire, so switching it on is
	 * a handling change to be made deliberately, not a correctness fix.
	 */
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	bool bEnableSpeedDependentFriction = false;

	/**
	 * Display scale for FPacejkaResult::Mz_normalized, used only by the tire graph
	 * so the aligning-torque curve shares an axis with the friction coefficients.
	 * Not physical - Mz itself is unscaled N*m.
	 */
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true"))
	float MzGraphScale = 500.f;

	/**
	 * Vertical load the tire graph is drawn at, N. DISPLAY ONLY - nothing in the
	 * simulation reads it.
	 *
	 * Worth sweeping, because the Magic Formula's coefficients are load-scaled and
	 * a curve drawn at one load says very little about the shape at another. dfz
	 * (4.E2a) is how far Fz sits from FZ0, and it runs through very nearly every
	 * term: peak factors, stiffnesses, curvatures, the lot.
	 *
	 * What it shows most clearly is LOAD SENSITIVITY - that grip does not rise in
	 * proportion to load, so mu falls as Fz climbs. That single fact is what makes
	 * lateral load transfer cost an axle grip, and therefore what decides the
	 * balance of the car. Draw the same tire at FZ0/2, FZ0 and 2*FZ0 and the peak
	 * of mu drops each time; the size of that drop IS the car's sensitivity to
	 * where its weight sits.
	 */
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float GraphLoadN = 2637.f;

	/**
	 * Enables the pp** pressure terms throughout the force equations and the ppFz1
	 * term in the vertical model. Safe to leave on: with pi == pi0 the pressure
	 * offset dpi is zero, so nothing changes until the actual pressure is altered.
	 */
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	bool bEnablePressureSensitivity = true;

	/* ---------- [VERTICAL_COEFFICIENTS] - book (4.E68) ---------- */
	//
	// These turn a radial deflection into a normal load, i.e. they ARE the tire's
	// vertical spring. qFz1 is the one that corresponds to "tire spring rate":
	// linearising (4.E68) at small deflection gives dFz/drho_z = qFz1 * Fz0 / R0,
	// so with qFz1 = 15, Fz0 = 2500 N and R0 = 0.32 m that is ~117 N/mm.

	UPROPERTY(EditAnywhere, Category = "Tire|Vertical", meta = (AllowPrivateAccess = "true"))
	float qFz1 = 15.0f;    // Linear vertical stiffness

	UPROPERTY(EditAnywhere, Category = "Tire|Vertical", meta = (AllowPrivateAccess = "true"))
	float qFz2 = 0.0f;     // Quadratic (progressive) vertical stiffness

	UPROPERTY(EditAnywhere, Category = "Tire|Vertical", meta = (AllowPrivateAccess = "true"))
	float qFz3 = 0.0f;     // Camber effect on vertical stiffness

	UPROPERTY(EditAnywhere, Category = "Tire|Vertical", meta = (AllowPrivateAccess = "true"))
	float qV2 = 0.0f;      // Centrifugal stiffening with wheel speed

	UPROPERTY(EditAnywhere, Category = "Tire|Vertical", meta = (AllowPrivateAccess = "true"))
	float qFcx = 0.0f;     // Vertical stiffness loss under longitudinal force

	UPROPERTY(EditAnywhere, Category = "Tire|Vertical", meta = (AllowPrivateAccess = "true"))
	float qFcy = 0.0f;     // Vertical stiffness loss under lateral force

	UPROPERTY(EditAnywhere, Category = "Tire|Vertical", meta = (AllowPrivateAccess = "true"))
	float ppFz1 = 0.7f;    // Inflation pressure effect on vertical stiffness

// PROTECTED, not private, so a C++ subclass can carry a tuned parameter set - see
// UTireParamsSportsCar. Every member below is already EditAnywhere with
// AllowPrivateAccess, so the editor can write all of them: private was only stopping
// the one caller that could document WHY a value is what it is, which is the opposite
// of what the access level is for here.
protected:
	
	/* ---------- [SCALING_COEFFICIENTS] ---------- */
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LFZ0 = 1.f;     // Scale factor of nominal (rated) load
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LCX = 1.0f;     // Scale factor of Fx shape factor
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LMUX = 1.0f;     // Scale factor of Fx peak friction coefficient
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LEX = 1.0f;     // Scale factor of Fx curvature factor

	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LHX = 1.0f;      // Scale factor of Fx horizontal shift
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LVX = 1.0f;     // Scale factor of Fx vertical shift
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LXAL = 1.f;     // Scale factor of Fx vertical shift
	
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LCY = 1.0f;     // Scale factor of Fy shape factor
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LMUY = 1.0f;     // Scale factor of Fy peak friction coefficient
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LEY = 1.0f;     // Scale factor of Fy curvature factor
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LKYAL = 1.f;      // Scale factor of cornering stiffness
	
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LKZC = 1.0f; // Scale factor of camber moment stiffness
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LHY = 1.0f; // Scale factor of Fy horizontal shift
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LVY = 1.0f; // Scale factor of Fy vertical shift
	
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LT = 1.f;     // Scale factor of pneumatic trail
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LYK = 1.f;     // Scale factor of k influence on Fy(a)

	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LVYK = 1.f;     // Scale factor of k induced ‘ply-steer’ Fy
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LMUV = 0.05f;     // Scale factor of with slip speed Vs decaying friction

	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LS = 1.f;     // Scale factor of moment arm of Fx
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float LMR = 1.f;     // Scale factor aligning torque

	// ---- Rolling Resistance Moment, book (4.E70) ----
	//
	// My = Fz*Ro*{ qsy1 + qsy2*Fx/Fzo + qsy3*|Vx/Vo| + qsy4*(Vx/Vo)^4
	//              + (qsy5 + qsy6*Fz/Fzo)*gamma^2 }
	//            * { (Fz/Fzo)^qsy7 * (pi/pio)^qsy8 } * lambda_My
	//
	// qsy1 is the one that matters: it IS the rolling resistance coefficient, so
	// 0.01 is about 1% of vertical load and typical for a road tyre.
	//
	// NOTE ON qsy3/qsy4. The book's Vo here is the same reference velocity used in
	// 4.E7, which this file takes as sqrt(g*Ro) - about 1.8 m/s, not the 16.7 m/s
	// LONGVL that MF-Tyre datasets are measured against. A qsy4 lifted from a
	// dataset would therefore be scaled by (16.7/1.8)^4, roughly 7000x, and would
	// swamp every other term. Both speed coefficients default to zero for that
	// reason; rescale before using dataset values.
	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy1 = 0.01f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy2 = 0.f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy3 = 0.f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy4 = 0.f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy5 = 0.f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy6 = 0.f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy7 = 0.85f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float qsy8 = -0.4f;

	UPROPERTY(EditAnywhere, Category = "Rolling Resistance", meta=(AllowPrivateAccess=true))
	float LMY = 1.f;     // Scale factor rolling resistance moment
	
	
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_0 = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_1 = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_2 = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_3 = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_4 = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_5 = 1.f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_6 = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_7 = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Tire|Scaling Coefficients", meta = (AllowPrivateAccess = "true"))
	float zeta_8 = 1.0f;

	/* ---------- [LONGITUDINAL_COEFFICIENTS] ---------- */
	
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PCX1 = 1.579f;    // Shape factor Cfx for longitudinal force
	
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PDX1 = 1.0422f;     // Longitudinal friction Mux at Fznom 
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PDX2 = -0.08285f;   // Variation of friction Mux with load
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PDX3 = 0.f;	// Variation of friction Mux with camber
	
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PEX1 = 0.11113f;     // Longitudinal curvature Efx at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PEX2 = 0.3143f;    // Variation of curvature Efx with load
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PEX3 = 0.0f;    // Variation of curvature Efx with load squared
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PEX4 = 0.001719f;    // Factor in curvature Efx while driving

	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PKX1 = 21.687f;    // Longitudinal slip stiffness Kfx/Fz at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PKX2 = 13.728f;    // Variation of slip stiffness Kfx/Fz with load
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PKX3 = -0.4098f;    // Exponent in slip stiffness Kfx/Fz with load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PHX1 = 2.1615e-04f;     // Horizontal shift Shx at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PHX2 = 0.0011598f;     // Variation of shift Shx with load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PVX1 = 2.20283e-5f;     // Vertical shift Svx/Fz at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PVX2 = 1.0568e-4f;     // Variation of shift Svx/Fz with load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Combined", meta = (AllowPrivateAccess = "true"))
	float RBX1 = 13.046f;     // Slope factor for combined slip Fx reduction
	UPROPERTY(EditAnywhere, Category = "Tire|Combined", meta = (AllowPrivateAccess = "true"))
	float RBX2 = 9.718f;     // Variation of slope Fx reduction with kappa
	UPROPERTY(EditAnywhere, Category = "Tire|Combined", meta = (AllowPrivateAccess = "true"))
	float RBX3 = 0.0f;     // Influence of camber on stiffness for Fx combined
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Combined", meta = (AllowPrivateAccess = "true"))
	float RCX1 = 0.9995f;     // Shape factor for combined slip Fx reduction
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Combined", meta = (AllowPrivateAccess = "true"))
	float REX1 = -0.4403f;     // Curvature factor of combined Fx
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Combined", meta = (AllowPrivateAccess = "true"))
	float REX2 = -0.4663f;     // Curvature factor of combined Fx with load
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Combined", meta = (AllowPrivateAccess = "true"))
	float RHX1 = -9.968e-5f;   // Shift factor for combined slip Fx reduction
	
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PPX1 = -0.3485f;     // Linear pressure effect on slip stiffness
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PPX2 = 0.37824f;     // Quadratic pressure effect on slip stiffness
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PPX3 = -0.09603f;     // Linear pressure effect on longitudinal friction
	UPROPERTY(EditAnywhere, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float PPX4 = 0.06518f;     // Quadratic pressure effect on longitudinal friction
    
	/* ---------- [LATERAL_COEFFICIENTS] ---------- */
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PCY1 = 1.337f;     // Shape factor Cfy for lateral forces
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PDY1 = 0.8785f;     // Lateral friction Muy
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PDY2 = -0.06452f;   // Variation of friction Muy with load
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PDY3 = 0.f;     // Variation of friction Muy with squared camber
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PEY1 = -0.8057f;    // Lateral curvature Efy at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PEY2 = -0.6046f;    // Variation of curvature Efy with load
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PEY3 = 0.09854f;     // Zero order camber dependency of curvature Efy
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PEY4 = -6.697f;     // Variation of curvature Efy with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PEY5 = 0.0f;      // Camber curvature Efc

	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	// POSITIVE, and published parameter sets are NEGATIVE. That is not a typo in either
	// place: PKY1 sets the sign of B in Fy = D sin(C atan(B a - ...)), so a negative
	// value gives Fy the opposite slope to Fx through the origin and the car steers the
	// wrong way in THIS code. ISO sets it negative; this implementation needs it
	// positive.
	//
	// IMPORTING A .tir FILE OR A PUBLISHED SET MEANS FLIPPING THIS, and checking PHY*
	// and PVY* with it, since they carry the same lateral convention. See
	// TireParams_Baseline.md.
	float PKY1 = 15.324f;    // Maximum value of stiffness Kfy/Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PKY2 = 1.715f;   // Load at which Kfy reaches maximum value
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PKY3 = 0.3695f;     // Variation of Kfy/Fznom with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PKY4 = 2.0005f;    // Peak stiffness variation with camber squared
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PKY5 = 0.f;    // Lateral stiffness dependency with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PKY6 = -0.8987f;    // Camber stiffness factor
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PKY7 = -0.23303f;    // Load dependency of camber stiffness factor
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PHY1 = 0.f;     // Horizontal shift Shy at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PHY2 = 0.f;     // Variation of shift Shy with load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY1 = 0.f;     // Vertical shift in Svy/Fz at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY2 = 0.f;     // Variation of shift Svy/Fz with load
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY3 = 0.f;     // Variation of shift Svy/Fz with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY4 = 0.f;     // Variation of shift Svy/Fz with camber and load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RBY1 = 10.622f;     // Slope factor for combined Fy reduction
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RBY2 = 7.82f;     // Variation of slope Fy reduction with alpha
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RBY3 = 0.002037f;     // Shift term for alpha in slope Fy reduction
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RBY4 = 0.0f;     // Influence of camber on stiffness of Fy combined
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RCY1 = 1.0587f;     // Shape factor for combined Fy reduction
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float REY1 = 0.3148f;     // Curvature factor of combined Fy
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float REY2 = 0.004867f;     // Curvature factor of combined Fy with load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RHY1 = 0.009472f;   // Shift factor for combined Fy reduction
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RHY2 = 0.009754f;     // Shift factor for combined Fy reduction with load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RVY1 = 0.05187f;     // Kappa induced side force Svyk/Muy*Fz at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RVY2 = 4.853e-4f;     // Variation of Svyk/Muy*Fz with load
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RVY3 = 0.f;     // Variation of Svyk/Muy*Fz with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RVY4 = 0.f;     // Variation of Svyk/Muy*Fz with alpha
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RVY5 = 0.f;     // Variation of Svyk/Muy*Fz with kappa
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RVY6 = 23.8f;     // Variation of Svyk/Muy*Fz with atan (kappa)
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PPY1 = -0.6255f;    // Pressure effect on cornering stiffness magnitude
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PPY2 = -0.06523f;     // Pressure effect on location of cornering stiffness peak
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PPY3 = -0.16666f;     // Linear pressure effect on lateral friction
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PPY4 = -0.2811f;     // Quadratic pressure effect on lateral friction
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PPY5 = 0.f;     // Influence of inflation pressure on camber stiffness
	
	/* ---------- [ALIGNING_COEFFICIENTS] ---------- */
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QBZ1 = 12.035f;    // TTrail slope factor for trail Bpt at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QBZ2 = -1.33f;     // Variation of slope Bpt with load
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QBZ3 = 0.0f;     // Variation of slope Bpt with load squared
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QBZ5 = 0.176f;     //Variation of slope Bpt with absolute camber
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QBZ6 = -0.14853f;     // //Variation of slope Bpt with camber squared
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QBZ9 = 34.5f;     // Slope factor Br of residual torque Mzr
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QBZ10 = 0.0f;     //Slope factor Br of residual torque Mzr
	
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QCZ1 = 1.2923f;     // Shape factor Cpt for pneumatic trail

	// Dr / Dz family (peak/residual amplitude)
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ1 = 0.09068f;     // Peak trail Dpt" = Dpt*(Fz/Fznom*R0)
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ2 = -0.00565f;   // Variation of peak Dpt with load
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ3 = 0.3778f;     // Variation of peak Dpt with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QD4 = 0.0f;     // Variation of peak Dpt with camber squared
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ6 = 0.0017015f;     // Peak residual torque Dmr = Dmr/(Fz*R0)
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ7 = -0.002091f;     // Variation of peak factor Dmr with load
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ8 = -0.1428f;     // Variation of peak factor Dmr with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ9 = 0.00915f;     // Variation of peak factor Dmr with camber and load
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ10 = 0.0f;     // Variation of peak factor Dmr with camber squared
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QDZ11 = 0.0f;     // Variation of Dmr with camber squared and load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QEZ1 = -1.7924f;    // Trail curvature Ept at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QEZ2 = 0.8975f;     // Variation of curvature Ept with load
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QEZ3 = 0.0f;     // Variation of curvature Ept with load squared
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QEZ4 = 0.2895f;     // Variation of curvature Ept with sign of Alpha-t
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QEZ5 = -0.6786f;     // Variation of Ept with camber and sign Alpha-t
	
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QHZ1 = 0.0014333f;     // Trail horizontal shift Sht at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QHZ2 = 0.0024087f;     // Variation of shift Sht with load
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QHZ3 = 0.24973f;     // Variation of shift Sht with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float QHZ4 = -0.21205f;     // Variation of shift Sht with camber and load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float SSZ1 = 0.00918f;     // Nominal value of s/R0: effect of Fx on Mz
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float SSZ2 = 0.03869f;     // Variation of distance s/R0 with Fy/Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float SSZ3 = 0.0f;     // Variation of distance s/R0 with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float SSZ4 = 0.0f;     // Variation of distance s/R0 with load and camber

	// pressure sensitivity
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float PPZ1 = -0.4408f; // Linear pressure effect on pneumatic trail
	UPROPERTY(EditAnywhere, Category = "Tire|Aligning", meta = (AllowPrivateAccess = "true"))
	float PPZ2 = 0.0f; // Influence of inflation pressure on residual aligning torque

public:
	/** ----------- Transient model parameters ----------- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float sigma_kappa0 = 0.3f; // Initial longitudinal relaxation length [m]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float sigma_alpha0 = 0.3f; // Initial lateral relaxation length [m]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float C_Fx = 200000.0f; // Longitudinal carcass stiffness [N/m]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float C_Fy = 130000.0f; // Lateral carcass stiffness [N/m]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float C_Fgamma = 3000.0f; // Camber stiffness [N/rad]

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	// Regularization constant for the "intersection length" forms of the relaxation
	// length (book 7.30 / 7.38), which divide by the slip itself. Currently UNUSED:
	// the transient model uses the derivative forms (7.36 / 7.39) instead, which have
	// no singularity to regularize. Kept so existing assets keep loading.
	float epsilon_F = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float sigma_min = 0.05f; // Minimum relaxation length [m]

	/**
	 * Hard cap on |kappa| handed to the Magic Formula.
	 *
	 * A RAIL, not a tuning parameter. MF 6.1 is fitted from rig data spanning
	 * roughly |kappa| < 0.25-0.30; past that the formula still returns a smooth,
	 * monotonically decaying force, but one nothing ever measured. The cap stops
	 * the extrapolation running away rather than shaping anything a driver feels -
	 * ordinary cornering on this car runs kappa between 0.016 and 0.043, two orders
	 * inside it.
	 *
	 * It earns its keep on the transient path, where the integrated kappa_prime
	 * reaches O(10-100) during wheelspin from rest.
	 *
	 * Lowering it toward the fitted range makes the tyre give up abruptly at the
	 * cap, which is a discontinuity, not a tighter model. Change it to EXPLORE what
	 * the extrapolated region is contributing, not to tune grip.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Limits", meta = (ClampMin = "0.05"))
	float SlipRatioCap = 3.0f;

	/**
	 * Hard cap on |alpha| handed to the Magic Formula, DEGREES.
	 *
	 * Same reasoning as SlipRatioCap. MF is fitted over roughly +/-15 to 20 degrees
	 * of slip angle; the default 80 is far outside that and exists only to keep a
	 * spinning car from reaching 90 degrees, where the tyre is travelling purely
	 * sideways and the Magic Formula has nothing to say about it - real force there
	 * comes from scrubbing and bulldozing, which this model does not contain.
	 *
	 * Set it near 20 to see the fitted envelope alone.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Limits", meta = (ClampMin = "1.0", ClampMax = "90.0"))
	float SlipAngleCapDeg = 80.f;

	/**
	 * Damping coefficient at standstill, N*s/m. Book 7.26 (Besselink), k_Vlow.
	 *
	 * The book, p.336: "starting from standstill gives rise to oscillations which
	 * are practically undamped. Damping increases when speed is built up." With |Vx|
	 * correctly absent from the denominator the tyre at rest IS a pure spring, so
	 * nothing dissipates and the car rocks on its tyres. A real tyre damps this
	 * through material damping; the model has to be told about it.
	 *
	 * Size it against the standstill spring it is damping: the tyre's longitudinal
	 * rate is C_Fkappa/sigma_kappa, so critical damping for a corner mass m is
	 * 2*sqrt(m * C_Fkappa/sigma_kappa). Half of that is a sensible starting point.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transient", meta = (ClampMin = "0.0"))
	float kVlow0 = 8000.f;

	/**
	 * Speed at which the low-speed damping has been fully suppressed, m/s. Book
	 * 7.26's V_low: k_Vlow "should be gradually suppressed to zero when the speed of
	 * travel Vx approaches a selected low value Vlow. Beyond that value the model
	 * should operate as usual."
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transient", meta = (ClampMin = "0.0"))
	float Vlow = 2.5f;

	// Upper bound on the relaxation length [m]. Numerical guard, not from the book:
	// the derivative forms (7.36/7.39) are well behaved, but a bad coefficient set
	// or a near-zero carcass stiffness could otherwise produce an enormous lag.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float sigma_max = 0.9f;

	// Blend factor for smoothing the relaxation length across steps (0 = frozen,
	// 1 = no smoothing). Damps central-difference noise in the computed slope.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0"))
	float sigma_smoothing = 0.15f;

	// Non-lagging camber parameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL1 = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL2 = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL3 = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL4 = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL5 = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL6 = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL7 = -2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float p_NL8 = 10.0f;

	// Gyroscopic couple
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float c_gyr = 0.5f; // Gyroscopic coefficient
	
	float epsilon_v = 0.1f;
	float epsilon_x = 0.01f;
	float epsilon_k = 0.01f;
	float epsilon_y = 0.01f;
	
	/// 
	/// @param Fz Normal force [N]
	/// @param Gamma Camber angle [Rad]
	/// @param Vc  magnitude of the velocity of the wheel contact center [m/s]
	/// @param Vcy Lateral vehicle motion at the wheel contact patch center [m/s]
	/// @param Vcx Forward vehicle motion at the wheel contact patch center [m/s]
	/// @param Omega Angular velocity of the wheel [Rad/s]
	/// @return 
	UFUNCTION()
	FPacejkaResult ComputeTireForces(float Fz, float Gamma,
					   float Vc, float Vcy, float Vcx, float Omega) const
	{
		// Steady state IS the transient model evaluated at the instantaneous slips.
		// This used to be a full second copy of the Magic Formula, identical to
		// ComputeTireForcesOverriden in every line after the slips were formed - which
		// meant every book correction had to be made twice, and any equation fixed in
		// one place and missed in the other would diverge silently, since both return
		// plausible forces either way.
		//
		// Effective rolling radius is fixed to the unloaded radius for now: the loaded
		// value needs the tyre's radial deflection, which this model is not given. It
		// feeds kappa directly, so the slip ratio carries a systematic rho_z/R0 error
		// that is worst under high load.
		const float Re = R0;
		const float Vsx = Vcx - Re * Omega;

		float kappa = -Vsx / FMath::Max(FMath::Abs(Vcx), epsilon_v); // Book (4.E5)
		kappa = FMath::Clamp(kappa, -SlipRatioCap, SlipRatioCap);

		float alpha = -FMath::Atan2(Vcy, FMath::Abs(Vcx));
		const float AlphaCap = FMath::DegreesToRadians(SlipAngleCapDeg);
		alpha = FMath::Clamp(alpha, -AlphaCap, AlphaCap);

		return ComputeTireForcesOverriden(Fz, kappa, alpha, Gamma, Vc, Vcy, Vcx, Omega);
	}



	/// 
	/// @param Fz Normal force [N]
	/// @param kappa 
	/// @param alpha 
	/// @param Gamma Camber angle [Rad]
	/// @param Vc  magnitude of the velocity of the wheel contact center [m/s]
	/// @param Vcy Lateral vehicle motion at the wheel contact patch center [m/s]
	/// @param Vcx Forward vehicle motion at the wheel contact patch center [m/s]
	/// @param Omega Angular velocity of the wheel [Rad/s]
	/// @param bOverrideSlipRatios 
	/// @return 
	UFUNCTION()
	FPacejkaResult ComputeTireForcesOverriden(float Fz, float kappa, float alpha, float Gamma,
						   float Vc, float Vcy, float Vcx, float Omega) const
	{
		FPacejkaResult result;

		float Fz0_prime = LFZ0 * FZ0; // Book (4.E1)
		float df_z = (Fz - Fz0_prime) / Fz0_prime; // Book (4.E2a)
		// Book (4.E2b). Every pp** coefficient below multiplies dpi, so disabling this
		// zeroes the whole pressure family at once. With pi == pi0 it is zero anyway.
		const float dpi = bEnablePressureSensitivity ? (pi - pi0) / pi0 : 0.f;
		
		// Effective rolling radius. Fixed to the unloaded radius for now: the loaded
		// value needs the tire's radial deflection, which this model is not given.
		// It feeds kappa directly, so slip ratio carries a systematic rho_z/R0 error
		// that is worst under high load.
		float Re = R0;
		float Vsx = Vcx - Re * Omega;

		// Clamp kappa to the same range ComputeTireForces uses. This overload takes
		// the slips as arguments, and the transient path feeds it kappa_prime, which
		// reaches O(10-100) during wheelspin from rest - well outside the range the
		// combined-slip formulas are fitted over.
		kappa = FMath::Clamp(kappa, -SlipRatioCap, SlipRatioCap);
		const float AlphaCapRad = FMath::DegreesToRadians(SlipAngleCapDeg);
		alpha = FMath::Clamp(alpha, -AlphaCapRad, AlphaCapRad);
		// Book (4.E3): alpha* = tan(alpha)*sgn(Vcx) = -Vcy/|Vcx|.
		//
		// NO sgn(Vcx) FACTOR HERE, AND THAT IS CORRECT - do not "restore" it. The sgn in
		// 4.E3 is there to put the sign back when alpha has been formed as
		// -atan2(Vcy, |Vcx|), which throws the direction of travel away. This function is
		// never handed that alpha.
		//
		// What arrives here is the TRANSIENT slip angle, and 7.35 integrates it against a
		// right-hand member of -Vsy, so its steady state is tan(alpha') = -Vcy/|Vcx| -
		// which IS alpha*, sign included. tan() of it recovers alpha* exactly.
		//
		// Applying sgn on top double-corrects. Near standstill, where Vcx dithers either
		// side of zero, it also flips the sign of the held lateral carcass deflection
		// every step and walks the car sideways under its own power.
		float alpha_star = FMath::Tan(alpha);

		float Gamma_star = FMath::Sin(Gamma); // Book (4.E4)

		// Ensure normal force is positive
		Fz = FMath::Max(Fz, 1.0f);

		float Vc_prime = Vc + epsilon_v; // Book (4.E6a)
		float CosPrimeAlpha = Vcx/Vc_prime; // Book (4.E6)
		float V0 = FMath::Sqrt(9.81f * R0);
		float Vs = FMath::Sqrt(Vsx * Vsx + Vcy * Vcy);
		// Book (4.E7): composite friction scaling, decaying with slip speed. Disabled
		// by default (both factors forced to 1) because it has never been tuned here -
		// LMUV is the knob that controls the decay rate. Flip bEnableSpeedDependentFriction
		// to switch it on; that is a handling change, not a bug fix.
		float lambda_mu_x_star = 1.f;
		float lambda_mu_y_star = 1.f;
		if (bEnableSpeedDependentFriction)
		{
			lambda_mu_x_star = LMUX / (1 + LMUV * Vs / V0); // Book (4.E7)
			lambda_mu_y_star = LMUY / (1 + LMUV * Vs / V0); // Book (4.E7)
		}

		// Book (4.E8): degressive friction factor. The y variant previously read
		// lambda_mu_x_star - harmless while both are pinned to 1, wrong the moment
		// (4.E7) above is enabled.
		const float Au = 10.f;
		float lambda_mu_x_prime = Au * lambda_mu_x_star / (1 + (Au - 1) * lambda_mu_x_star); // Book (4.E8)
		float lambda_mu_y_prime = Au * lambda_mu_y_star / (1 + (Au - 1) * lambda_mu_y_star); // Book (4.E8)

        
		/* --------- Longitudinal Force (Pure Longitudinal Slip, alpha = 0) -------- */
		float S_Hx = (PHX1 + PHX2 * df_z) * LHX; // Book (4.E17)
		float Kx = kappa + S_Hx; // Book (4.E10)
		float Cx = PCX1 * LCX; // Book (4.E11)
		//Cx = Cx <= 0.f ? 0.001f : Cx;
		float Ux = (PDX1 + PDX2 * df_z) * (1 + PPX3 * dpi + PPX4 * dpi * dpi) * (1 - PDX3 * Gamma * Gamma) * lambda_mu_x_star; // Book (4.E13)
		float Kxk = Fz * (PKX1 + PKX2 * df_z) * FMath::Exp(PKX3 * df_z) * (1 + PPX1 * dpi + PPX2 * dpi * dpi); // Book (4.E15)
		float Dx = Ux * Fz * zeta_1; // Book (4.E12) Consider adding zeta later, check the book for more info
		//Dx = Dx <= 0.f ? 0.001f : Dx;
		float Bx = Kxk / (Cx * Dx + epsilon_x); // Book (4.E16)
		float Ex = (PEX1 + PEX2 * df_z + PEX3 * df_z * df_z) * (1.0f - PEX4 * FMath::Sign(Kx)) * LEX; // Book (4.E14)
		//Ex = Ex > 1.f ? 1.f : Ex;
		// Book (4.E18) plus a deliberate DEVIATION: the |Vcx|/(|Vcx|+0.1) factor is not
		// in the book. It fades the vertical shift out at standstill so the tire does
		// not produce a residual longitudinal force with the vehicle stopped.
		float US_Vx = (PVX1 + PVX2 * df_z) *
			(FMath::Abs(Vcx)/(FMath::Abs(Vcx) + 0.1f)) * LVX * lambda_mu_x_prime * zeta_1; // Book (4.E18)
		float S_Vx = Fz * US_Vx; // Book (4.E18)
		float Expx = FMath::Sin(Cx * FMath::Atan(Bx * Kx - Ex * (Bx * Kx - FMath::Atan(Bx * Kx)))); // Book (4.E9)
		float Ux0 = Ux * Expx + US_Vx; // Book (4.E9)
		float Fx0 = Dx * Expx + S_Vx; // Book (4.E9)
		//UE_LOG(LogTemp, Warning, TEXT("Kx %f Cx %f Bx %f Ex %f Ux %f Fx0 %f"), Kx, Cx, Bx, Ex, Dx * Expx/Fz, Fx0);
		
		/* ------------ Lateral Force (Pure Side Slip, kappa = 0) ---------- */
		float Ky_alpha = PKY1 * Fz0_prime * (1.f + PPY1 * dpi) * (1.0f - PKY3 * FMath::Abs(Gamma_star)) *
			FMath::Sin(PKY4 * FMath::Atan((Fz/Fz0_prime) /
				((PKY2 + PKY5 * Gamma_star * Gamma_star) * (1.f + PPY2 * dpi)))) *
			zeta_3 * LKYAL; // Book (4.E25)
		
		float US_Vy_gamma = (PVY3 + PVY4 * df_z) * Gamma_star * LKZC * lambda_mu_y_prime * zeta_2; // Book (4.E28
		float S_Vy_gamma = Fz * US_Vy_gamma; // Book (4.E28)
		
		float K_y_gamma_0 = Fz * (PKY6 + PKY7 * df_z) * (1.0f + PPY5 * dpi) * LKZC; // Book (4.E30)
		// Book (4.E27): SHy = (pHy1 + pHy2*dfz)*lHy + (Kyg0*g* - SVyg)/(Kya + eK)*z0 + z4 - 1
		// The load term pHy2*dfz was missing and lHy scaled only pHy2 instead of the
		// whole bracket, which made the lateral curve's horizontal shift independent
		// of vertical load.
		float S_Hy = (PHY1 + PHY2 * df_z) * LHY +
			(K_y_gamma_0 * Gamma_star - S_Vy_gamma) * zeta_0 / (Ky_alpha + epsilon_k) + zeta_4 - 1.f; // Book (4.E27)
		float alpha_y = alpha_star + S_Hy; // Book (4.E20)
		float Cy = PCY1 * LCY; // Book (4.E21)
		//Cy = Cy <= 0.f ? 0.001f : Cy;
		float Uy = (PDY1 + PDY2 * df_z) * (1.f + PPY3 * dpi + PPY4 * dpi * dpi) * (1.0f - PDY3 * Gamma_star * Gamma_star) * lambda_mu_y_star; // Book (4.E23)
		float Dy = Uy * Fz * zeta_2; // Book (4.E22) Check the book for the full implementation.
		float By = Ky_alpha / (Cy * Dy + epsilon_y); // Book (4.E26)
		float Ey = (PEY1 + PEY2 * df_z) * ( 1.f + PEY5 * Gamma_star * Gamma_star - (PEY3 + PEY4 * Gamma_star) * FMath::Sign(alpha_y)) * LEY; // Book (4.E24)
		//Ey = Ey > 1.f ? 1.f : Ey;
		float US_Vy = (PVY1 + PVY2 * df_z) * LVY * lambda_mu_y_prime * zeta_2; // Book (4.E29)
		float S_Vy = Fz * US_Vy + S_Vy_gamma; // Book (4.E29)

		float Expy = FMath::Sin(Cy * FMath::Atan(By * alpha_y - Ey * (By * alpha_y - FMath::Atan(By * alpha_y)))); // Book (4.E19)
		float Uy0 = Uy * Expy + US_Vy; // Book (4.E19)
		float Fy0 = Dy * Expy + S_Vy; // Book (4.E19)

		// ---------------- Aligning Torque (Pure Side Slip, kappa = 0) ------------------
		
		float S_Ht = QHZ1 + QHZ2 * df_z + (QHZ3 + QHZ4 * df_z) * Gamma_star; // Book (4.E35)
		float alpha_t = alpha_star + S_Ht; // Book (4.E34)

		float Bt = (QBZ1 + QBZ2 * df_z + QBZ3 * df_z * df_z) *
			(1 + QBZ5 * FMath::Abs(Gamma_star) + QBZ6 * Gamma_star * Gamma_star) * LKYAL/lambda_mu_y_star; // Book (4.E40)
		//Bt = Bt <= 0.f ? 0.0001f : Bt;
		float Ct = QCZ1; // Book (4.E41)
		//Ct = Ct <= 0.f ? 0.0001f : Ct;
		float Et = (QEZ1 + QEZ2 * df_z + QEZ3 * df_z * df_z) *
			(1 + (QEZ4 + QEZ5 * Gamma_star) * (2/PI) * FMath::Atan(Bt * Ct * alpha_t)); // Book (4.E44)
		//Et = Et > 1 ? 1.f : Et; // Book (4.E44)
		float Dt0 = Fz * (R0/Fz0_prime) * (QDZ1 + QDZ2 * df_z) * (1.0f - PPZ1 * dpi) * LT * FMath::Sign(Vcx); // Book (4.E42)
		float Dt = Dt0 * (1.f + QDZ3 * FMath::Abs(Gamma_star) + QD4 * Gamma_star * Gamma_star) * zeta_5; // Book (4.E43)
		float t0 = Dt * FMath::Cos(Ct *
			FMath::Atan(Bt * alpha_t - Et *
				(Bt * alpha_t - FMath::Atan(Bt * alpha_t)))) * CosPrimeAlpha; // Book (4.E33)
		float Br = ((QBZ9 * LKYAL / lambda_mu_y_star) + QBZ10 * By * Cy) * zeta_6; // Book (4.E45)
		float Cr = zeta_7; // Book (4.E46)
		
		float Ky_alpha_prime = FMath::Max(Ky_alpha, epsilon_k); // Book (4.E39)
		float S_Hf = S_Hy + S_Vy / Ky_alpha_prime; // Book (4.E38)

		float alpha_r = alpha_star + S_Hf; // Book (4.E37)

		float term1 = (QDZ6 + QDZ7 * df_z) * LMR * zeta_2;
		float term2 = ((QDZ8 + QDZ9 * df_z) * (1.0f + PPZ2 * dpi) + (QDZ10 + QDZ11 * df_z) * FMath::Abs(Gamma_star)) * Gamma_star * LKZC * zeta_0;
		float Dr = Fz * R0 * (term1 + term2) * lambda_mu_y_star * FMath::Sign(Vcx) * CosPrimeAlpha + zeta_8 - 1.0f; // Book (4.E47)

		float Mzr0 = Dr * FMath::Cos(Cr * FMath::Atan(Br * alpha_r)) * CosPrimeAlpha; // Book (4.E36)
		float Mz0Prime = -t0 * Fy0; // Book (4.E32)
		float Mz0 = Mz0Prime + Mzr0; // Book (4.E31)
		
		/* ---------  Longitudinal Force (Combined Slip) --------- */
		float Bx_alpha = (RBX1 + RBX3 * Gamma_star * Gamma_star) * FMath::Cos(FMath::Atan(RBX2 * kappa)) * LXAL; // Book (4.E54)
		Bx_alpha = Bx_alpha < 0.f ? 0.001f : Bx_alpha;
		float Cx_alpha = RCX1; // Book (4.E55)
		float Ex_alpha = REX1 + REX2 * df_z; // Book (4.E56)
		Ex_alpha = Ex_alpha > 1.f ? 1.f : Ex_alpha; // Book (4.E56)
		float S_Hx_alpha = RHX1; // Book (4.E57)
		float alpha_s = alpha_star + S_Hx_alpha; // Book (4.E53)
		float Gx_alpha0 = FMath::Cos(Cx_alpha * FMath::Atan(Bx_alpha * S_Hx_alpha - Ex_alpha * (Bx_alpha * S_Hx_alpha - FMath::Atan(Bx_alpha * S_Hx_alpha)))); // Book (4.E52)
		Gx_alpha0 = FMath::Max(Gx_alpha0, 0.001f);
		float Gx_alpha = FMath::Cos(Cx_alpha * FMath::Atan(Bx_alpha * alpha_s - Ex_alpha * (Bx_alpha * alpha_s - FMath::Atan(Bx_alpha * alpha_s)))) / Gx_alpha0; // Book (4.E51)
		float Fx = Fx0 * Gx_alpha; // Book (4.E50)
		//UE_LOG(LogTemp, Warning, TEXT("Gx %f Gx0 %f Alpha_S %f Bx %f Cx %f Ex %f"),
		//	Gx_alpha, Gx_alpha0, alpha_s, Bx_alpha, Cx_alpha, Ex_alpha);
		
		/* ---------- Lateral Force (Combined Slip) --------- */
		float By_kappa = (RBY1 + RBY4 * Gamma_star * Gamma_star) * FMath::Cos(FMath::Atan(RBY2 * (alpha_star - RBY3))) * LYK; // Book (4.E62)
		By_kappa = FMath::Max(By_kappa, 0.001f); // Book (4.E62) is annotated (> 0)
		float Cy_kappa = RCY1; // Book (4.E63)
		float Ey_kappa = REY1 + REY2 * df_z; // Book (4.E64)
		float S_Hy_kappa = RHY1 + RHY2 * df_z; // Book (4.E65)
		float kappa_s = kappa + S_Hy_kappa; // Book (4.E61)
		float Gy_kappa0 = FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * S_Hy_kappa - Ey_kappa *
			(By_kappa * S_Hy_kappa - FMath::Atan(By_kappa * S_Hy_kappa)))); // Book (4.E60)
		Gy_kappa0 = FMath::Max(Gy_kappa0, 0.001f); // Book (4.E60) is annotated (> 0)
		// Book (4.E59) is annotated (> 0). The clamp is load-bearing, not cosmetic:
		// RCY1 = 1.0587 > 1 lets Cy_kappa * atan(...) exceed pi/2, so the raw cosine
		// goes NEGATIVE for |kappa| >= ~1.50 (floor ~ -0.09). Unclamped that reverses
		// the direction of the lateral force under heavy wheelspin or lockup, which is
		// destabilising rather than merely lossy.
		float Gy_kappa = FMath::Max(FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * kappa_s - Ey_kappa *
			(By_kappa * kappa_s - FMath::Atan(By_kappa * kappa_s)))) / Gy_kappa0, 0.f); // Book (4.E59)
		float D_Vy_k = Uy * Fz * (RVY1 + RVY2 * df_z + RVY3 * Gamma_star) *
			FMath::Cos(FMath::Atan(RVY4 * alpha_star)) * zeta_2; // Book (4.E67)
		float S_Vy_k = D_Vy_k * FMath::Sin(RVY5 * FMath::Atan(RVY6 * kappa)) * LVYK; // Book (4.E66)
		float Fy = Fy0 * Gy_kappa + S_Vy_k; // Book (4.E58)

		/*  ------- AligningTorque(combined slip) ------- */

		float alpha_t_eq = FMath::Sqrt(
			alpha_t * alpha_t + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_t); // Book (4.E77)
		float alpha_r_eq = FMath::Sqrt(
			alpha_r * alpha_r + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_r); // Book (4.E78)
		
		// Book (4.E74): F'y = Gyk(gamma = phi_t = 0) * Fyo(gamma = phi_t = 0).
		// BOTH factors are re-evaluated at ZERO camber. The aligning torque needs the
		// camber-free side force because camber's contribution to Mz is already carried
		// separately by the residual torque Mzr - feeding the cambered Fy0/Gy_kappa in
		// here double-counts camber. Turn slip is inert (all zeta = 1), so only the
		// camber terms need stripping.
		float Fy_prime;
		{
			// (4.E25) with gamma* = 0
			const float Ky_alpha_g0 = PKY1 * Fz0_prime * (1.f + PPY1 * dpi) *
				FMath::Sin(PKY4 * FMath::Atan((Fz / Fz0_prime) / (PKY2 * (1.f + PPY2 * dpi)))) *
				zeta_3 * LKYAL;
			// (4.E27) with gamma* = 0: the camber shift term vanishes entirely
			const float S_Hy_g0 = (PHY1 + PHY2 * df_z) * LHY + zeta_4 - 1.f;
			const float alpha_y_g0 = alpha_star + S_Hy_g0;                       // (4.E20)
			const float Uy_g0 = (PDY1 + PDY2 * df_z) *
				(1.f + PPY3 * dpi + PPY4 * dpi * dpi) * lambda_mu_y_star;        // (4.E23)
			const float Dy_g0 = Uy_g0 * Fz * zeta_2;                             // (4.E22)
			const float By_g0 = Ky_alpha_g0 / (Cy * Dy_g0 + epsilon_y);          // (4.E26)
			const float Ey_g0 = (PEY1 + PEY2 * df_z) *
				(1.f - PEY3 * FMath::Sign(alpha_y_g0)) * LEY;                    // (4.E24)
			const float S_Vy_g0 = Fz * (PVY1 + PVY2 * df_z) * LVY *
				lambda_mu_y_prime * zeta_2;                                      // (4.E29), SVyg = 0
			const float Fy0_g0 = Dy_g0 * FMath::Sin(Cy * FMath::Atan(By_g0 * alpha_y_g0 -
				Ey_g0 * (By_g0 * alpha_y_g0 - FMath::Atan(By_g0 * alpha_y_g0)))) + S_Vy_g0; // (4.E19)

			// (4.E62)/(4.E60)/(4.E59) with gamma* = 0
			const float By_kappa_g0 = FMath::Max(
				RBY1 * FMath::Cos(FMath::Atan(RBY2 * (alpha_star - RBY3))) * LYK, 0.001f);
			const float Gy_kappa0_g0 = FMath::Max(FMath::Cos(Cy_kappa * FMath::Atan(
				By_kappa_g0 * S_Hy_kappa - Ey_kappa *
				(By_kappa_g0 * S_Hy_kappa - FMath::Atan(By_kappa_g0 * S_Hy_kappa)))), 0.001f);
			const float Gy_kappa_g0 = FMath::Max(FMath::Cos(Cy_kappa * FMath::Atan(
				By_kappa_g0 * kappa_s - Ey_kappa *
				(By_kappa_g0 * kappa_s - FMath::Atan(By_kappa_g0 * kappa_s)))) / Gy_kappa0_g0, 0.f);

			Fy_prime = Gy_kappa_g0 * Fy0_g0; // Book (4.E74)
		}
		float Mzr = Dr * FMath::Cos(Cr * FMath::Atan(Br * alpha_r_eq)) * CosPrimeAlpha; // Book (4.E75)
		float s = R0 * (SSZ1 + SSZ2 * (Fy/Fz0_prime) + (SSZ3 + SSZ4 * df_z) * Gamma_star) * LS; // Book (4.E76)
		// Book (4.E73). Same shape as t0 (4.E33) but evaluated at the equivalent slip
		// angle; the Et curvature factor was missing here while t0 above has it.
		float t = Dt * FMath::Cos(Ct *
			FMath::Atan(Bt * alpha_t_eq - Et *
				(Bt * alpha_t_eq - FMath::Atan(Bt * alpha_t_eq)))) * CosPrimeAlpha; // Book (4.E73)
		float Mz_prime = -t * Fy_prime; // Book (4.E72)
		float Mz = Mz_prime + Mzr + s * Fx; // Book (4.E71)

		// Apply Forces and Torque
		Fx0 *= 100.f;
		Fy0 *= 100.f;
		Fx *= 100.f;
		Fy *= 100.f;

		// Do NOT reassign Fx = Fx0 / Fy = Fy0 here. Doing so discards the combined-slip
		// results (Gx_alpha, Gy_kappa, S_Vy_k) and hands back pure-slip forces, which
		// removes the friction-ellipse coupling entirely - full longitudinal AND full
		// lateral grip at the same time.
		//
		// If a caller needs the pure-slip force, it is already carried separately:
		//   result.Fx0 / result.Fy0  -> pure slip   (alpha = 0 / kappa = 0 respectively)
		//   result.Fx  / result.Fy   -> combined slip
		// Both are populated below, so nothing has to be clobbered to expose either one.
		//UE_LOG(LogTemp, Warning, TEXT("Alpha is %f Fy0 is %f Fy is %f S_Vyk is %f"), alpha, Fy0, Fy, S_Vy_k);
		result.Kappa = kappa;
		result.Alpha = alpha;

		result.Ux0 = Ux0;
		result.Ux = Ux0 * Gx_alpha;
		result.US_Vx = US_Vx;
		result.Fx0 = Fx0;
		result.Fx = Fx;

		result.Uy0 = Uy0;
		result.Uy = Uy0 * Gy_kappa;
		result.US_Vy = US_Vy;
		result.Fy0 = Fy0;
		result.Fy = Fy;
		
		/* ------------------ Rolling Resistance Moment (4.E70) ------------------- */
		//
		// Magnitude only - unsigned. Which way it acts is the vehicle's business, and
		// the answer is "against whichever way the wheel is turning"; the tyre model
		// has no opinion about that.
		const float FzRatio = Fz / FMath::Max(Fz0_prime, KINDA_SMALL_NUMBER);
		const float SpeedRatio = Vcx / FMath::Max(V0, KINDA_SMALL_NUMBER);

		const float MyBracket = qsy1
			+ (qsy2 * (Fx / FMath::Max(Fz0_prime, KINDA_SMALL_NUMBER)))
			+ (qsy3 * FMath::Abs(SpeedRatio))
			+ (qsy4 * FMath::Pow(SpeedRatio, 4.f))
			+ ((qsy5 + (qsy6 * FzRatio)) * Gamma * Gamma);

		// Pow of a ratio that is zero when the wheel is airborne: with qsy7 positive
		// that is zero, which is right - an unloaded tyre has no rolling resistance.
		// A negative qsy7 would send it to infinity instead, so it is clamped at zero
		// rather than left to produce an inf that only shows up as a NaN later.
		const float MyScale = FMath::Pow(FMath::Max(FzRatio, 0.f), qsy7)
			* FMath::Pow(FMath::Max(pi / FMath::Max(pi0, KINDA_SMALL_NUMBER), 0.f), qsy8);

		result.My = Fz * R0 * MyBracket * MyScale * LMY; // Book (4.E70)

		/* ------------- Aligning stiffnesses, book (4.E48) and (4.E49) ------------ */
		//
		// Both are evaluated at zero slip and zero camber - they are the SLOPES of the
		// aligning torque at the origin, so the operating point they describe is the
		// origin, not wherever the tyre happens to be. Kyalpha is therefore
		// recomputed with gamma taken as zero: 4.E25 carries camber through both the
		// (1 - pKy3*|gamma*|) factor and the pKy5*gamma*^2 term inside the arctan, and
		// leaving the live camber in would report the stiffness of a leaning tyre.
		const float Ky_alpha_gamma0 = PKY1 * Fz0_prime * (1.f + PPY1 * dpi) *
			FMath::Sin(PKY4 * FMath::Atan((Fz / Fz0_prime) /
				(PKY2 * (1.f + PPY2 * dpi)))) *
			zeta_3 * LKYAL; // Book (4.E25) at gamma = 0

		// K_y_gamma_0 (4.E30) has no camber dependence of its own, so it is already
		// the Kygammao that 4.E49 asks for.
		result.Kzao = Dt0 * Ky_alpha_gamma0; // Book (4.E48)

		result.Kzgo = (Fz * R0 * (QDZ8 + (QDZ9 * df_z)) * (1.f + (PPZ2 * dpi))
						* LKZC * lambda_mu_y_star)
					- (Dt0 * K_y_gamma_0); // Book (4.E49)

		result.pneumaticTrail = t0;
		result.Mz = Mz;
		result.Mz_normalized = Mz / FMath::Max(MzGraphScale, KINDA_SMALL_NUMBER); // graph display only

		return result;
		
	}
	
	/** Unloaded tire radius in cm, for callers working in Unreal units. */
	FORCEINLINE float GetUnloadedRadiusCm() const { return R0 * 100.f; }

	/**
	 * Normal load from tire radial deflection - book (4.E68).
	 *
	 * THIS IS THE TIRE'S VERTICAL SPRING. It is a separate spring from the
	 * suspension: the suspension acts between chassis and wheel carrier, this acts
	 * between wheel carrier and ground, and they are in series with the unsprung
	 * mass between them.
	 *
	 * Fz = {1 + qV2*(Ro/Vo)*|Omega| - (qFcx*Fx/Fzo)^2 - (qFcy*Fy/Fzo)^2}
	 *    * {(qFz1 + qFz3*gamma^2)*(rho_z/Ro) + qFz2*(rho_z/Ro)^2}
	 *    * (1 + ppFz1*dpi) * Fzo
	 *
	 * The Fx/Fy terms make this implicit (they depend on Fz). Pass the PREVIOUS
	 * step's forces - the same explicit coupling a penalty spring already has, so
	 * it introduces no new stability problem.
	 *
	 * @param RhoZ  Radial deflection [m], positive in compression. Clamped at 0:
	 *              a tire off the ground carries no load and cannot pull.
	 * @param Gamma Camber angle [rad]
	 * @param Omega Wheel angular velocity [rad/s]
	 * @param Fx    Longitudinal force from the previous step [N]
	 * @param Fy    Lateral force from the previous step [N]
	 * @return Normal load [N], never negative.
	 */
	float ComputeNormalLoad(float RhoZ, float Gamma, float Omega, float Fx, float Fy) const
	{
		const float Fz0_prime = LFZ0 * FZ0;
		const float dpi = bEnablePressureSensitivity ? (pi - pi0) / pi0 : 0.f;
		const float SafeR0 = FMath::Max(R0, KINDA_SMALL_NUMBER);
		const float RhoNorm = FMath::Max(RhoZ, 0.f) / SafeR0;

		if (RhoNorm <= 0.f)
		{
			return 0.f;
		}

		// Reference velocity Vo = sqrt(g * Ro), as used throughout the model.
		const float V0 = FMath::Sqrt(9.81f * SafeR0);

		// Speed stiffens the tire (centrifugal); in-plane force softens it.
		const float ScaleTerm = 1.f
			+ (qV2 * (SafeR0 / FMath::Max(V0, KINDA_SMALL_NUMBER)) * FMath::Abs(Omega))
			- FMath::Square(qFcx * Fx / Fz0_prime)
			- FMath::Square(qFcy * Fy / Fz0_prime);

		const float DeflectionTerm =
			((qFz1 + qFz3 * Gamma * Gamma) * RhoNorm) + (qFz2 * RhoNorm * RhoNorm);

		const float PressureTerm = 1.f + (ppFz1 * dpi);

		return FMath::Max(ScaleTerm * DeflectionTerm * PressureTerm * Fz0_prime, 0.f);
	}

	/**
	 * Inverse of (4.E68): the radial deflection that would carry a given load.
	 *
	 * Used to place the tire at rest without having to settle it dynamically, and
	 * to convert a suspension force into a deflection for a massless wheel carrier.
	 * Ignores the qV2 / qFcx / qFcy scale term, which is a small correction and
	 * would otherwise make the inversion implicit.
	 *
	 * @param Fz    Desired normal load [N]
	 * @param Gamma Camber angle [rad]
	 * @return Radial deflection [m], never negative.
	 */
	float ComputeDeflectionFromLoad(float Fz, float Gamma) const
	{
		const float Fz0_prime = LFZ0 * FZ0;
		const float dpi = bEnablePressureSensitivity ? (pi - pi0) / pi0 : 0.f;
		const float PressureTerm = FMath::Max(1.f + (ppFz1 * dpi), KINDA_SMALL_NUMBER);

		// Solve  A*x^2 + B*x - Target = 0  for x = rho_z / R0.
		const float Target = FMath::Max(Fz, 0.f) / (PressureTerm * Fz0_prime);
		const float A = qFz2;
		const float B = qFz1 + (qFz3 * Gamma * Gamma);

		if (Target <= 0.f || B <= KINDA_SMALL_NUMBER)
		{
			return 0.f;
		}

		float RhoNorm;
		if (FMath::Abs(A) < KINDA_SMALL_NUMBER)
		{
			RhoNorm = Target / B;
		}
		else
		{
			const float Disc = (B * B) + (4.f * A * Target);
			if (Disc < 0.f)
			{
				return 0.f;
			}
			RhoNorm = (-B + FMath::Sqrt(Disc)) / (2.f * A);
		}

		return FMath::Max(RhoNorm, 0.f) * R0;
	}

	/**
	 * Advances the single-contact-point transient tire state by one step.
	 *
	 * WHAT THIS IS FOR: a tire cannot build force instantly - the carcass has to
	 * physically deflect first. This integrates those deflections (u longitudinal,
	 * v_alpha from side slip, v_gamma from camber) as first-order lags, then divides
	 * them by the relaxation lengths to get LAGGED slips (kappa', alpha', gamma').
	 * Those lagged slips are what should be fed to the Magic Formula instead of the
	 * raw slips. Skip this and force appears instantly, which feels artificially
	 * sharp. The relaxation length is the rolling distance needed to reach ~63% of
	 * the steady-state force.
	 *
	 * Book: Pacejka, Tire and Vehicle Dynamics 3rd ed, Chapter 7 (single contact
	 * point). Equation references below are to that chapter.
	 *
	 * @param Vc Velocity of wheel center [m/s]
	 * @param Vcx Forward velocity of wheel at wheel center [m/s]
	 * @param Vcy Lateral velocity of wheel at wheel center [m/s]
	 * @param Omega wheel angular velocity [Rad/s]
	 * @param Gamma Camber angle [Rad]
	 * @param Fx_SS Longitudinal force at steady state [N]. Unused - retained for
	 *              call-site compatibility; the relaxation lengths use the local
	 *              slope of the force curve (7.36/7.39), not the force itself.
	 * @param Fy_SS Lateral force at steady state [N]. Unused, as above.
	 * @param Fz Normal force [N]
	 * @param DeltaTime Simulation delta time [s]
	 * @param TransientState Transient state
	 * @return The updated transient state
	 */
	FTransientTireState UpdateTransientState(float Vc,
	float Vcx, float Vcy, float Gamma, float Omega,
	float Fx_SS, float Fy_SS, float Fz,
	float DeltaTime,
	FTransientTireState TransientState)
	{
		// NO FLOOR. Book p.334: 7.7 and 7.9 are written deliberately with Vx out of the
		// denominator and Vsx,y as the right-hand member, "that makes them applicable for
		// simulations of stopping and starting from zero speed occurrences". At Vx = 0
		// the equation becomes a pure integrator, u = -integral(Vsx dt), and the tyre
		// "at standstill acts like a longitudinal or tangential spring" - which is what
		// holds a parked car on an incline instead of letting it creep. Flooring |Vx|
		// turns that integrator into a leaky one with time constant sigma/floor, so the
		// deflection bleeds away and the car walks.
		//
		// Safe without a floor: the backward-Euler denominators below are
		// sigma + |Vx|*dt, and ResolveRelaxationLength clamps sigma to sigma_min, so they
		// stay >= 0.05 however slowly the wheel is going. Vx never divides anything.
		const float Vx_abs = FMath::Abs(Vcx);

		// === 1. Slip velocities (Fig. 7.1) ===
		const float Vsx = Vcx - Omega * R0;
		const float Vsy = Vcy;

		// === 2. Slip stiffnesses at the current operating point ===
		//
		// C_Fkappa = dFx/dkappa'  [N per unit slip]
		// C_Falpha = dFy/dtan(alpha')  [N/rad] - the cornering stiffness.
		//
		// Magnitudes are taken deliberately. A relaxation length is a length, and the
		// book's own regularized forms (7.30, 7.38) use |Fy| and |Fx| for the same
		// reason. With this coefficient set PKY1 < 0, so Ky_alpha and hence the signed
		// dFy/dalpha are negative; taking the sign literally makes every sigma
		// negative, which a positivity guard then swallows - leaving the derivative
		// silently unused.
		//
		// The derivative is taken w.r.t. alpha, while 7.36 wants tan(alpha'); the
		// cos^2 factor converts between them and matters past a few degrees of slip.
		const float dFx_dkappa = FMath::Abs(
			Compute_dFx_dKappaNumerical(Vc, Vcy, Vcx, Omega, Gamma, Fz, TransientState.kappa_prime));

		const float CosAlpha = FMath::Cos(TransientState.alpha_prime);
		const float dFy_dtanalpha = FMath::Abs(
			Compute_dFy_dAlphaNumerical(Vc, Vcy, Vcx, Omega, Gamma, Fz, TransientState.alpha_prime))
			* CosAlpha * CosAlpha;

		const float C_Falpha = FMath::Max(dFy_dtanalpha, KINDA_SMALL_NUMBER);

		// === 3. Relaxation lengths (7.39 and 7.36) ===
		//
		//   sigma_kappa = (1 / C_Fx) * dFx/dkappa'     (7.39)
		//   sigma_alpha = (1 / C_Fy) * dFy/dtan(alpha') (7.36)
		//
		// Note the direction: slip stiffness DIVIDED BY carcass stiffness. Inverting
		// this is dimensionally wrong - (N/m)/N is 1/m, not m - and with these
		// defaults it saturates the clamp instead of varying.
		//
		// These are the derivative forms, which are singularity-free. The
		// "intersection length" forms (7.30, 7.38) divide by the slip itself and need
		// the regularization the book gives them; we do not use those here.
		//
		// The relaxation length shrinks as the sliding range grows (book Fig. 7.4),
		// which falls out naturally: the curve flattens past the force peak so the
		// slope, and hence sigma, drops.
		const float sigma_kappa_c = ResolveRelaxationLength(
			dFx_dkappa / FMath::Max(C_Fx, KINDA_SMALL_NUMBER), sigma_kappa0,
			TransientState.prev_sigma_kappa);

		const float sigma_alpha_c = ResolveRelaxationLength(
			C_Falpha / FMath::Max(C_Fy, KINDA_SMALL_NUMBER), sigma_alpha0,
			TransientState.prev_sigma_alpha);

		TransientState.prev_sigma_kappa = sigma_kappa_c;
		TransientState.prev_sigma_alpha = sigma_alpha_c;

		// === 4. Integrate the transient SLIPS directly (7.35, 7.39) ===
		//
		//   sigma * d(x)/dt + |Vx| * x = rhs
		//     x = tan(alpha'), rhs = -Vsy        (7.35)
		//     x = kappa',      rhs = -Vsx        (7.39 analogue)
		//     x = gamma',      rhs = |Vx| * gamma
		//
		// Backward Euler:  x_new = (sigma * x_old + rhs * dt) / (sigma + |Vx| * dt)
		// Unconditionally stable, and the steady state |Vx| * x = rhs is independent
		// of sigma - so sigma sets only how fast the tire gets there, never where.
		//
		// WHY THE SLIP AND NOT THE DEFLECTION: it is tempting to integrate the carcass
		// deflections (7.7/7.9) and recover the slip with 7.14's tan(alpha') = v/sigma.
		// That only holds while the force curve is linear. 7.14's sigma is the SECANT
		// Fy/tan(alpha') (7.28), whereas 7.36 gives the SLOPE dFy/dtan(alpha'); using
		// the slope as a divisor creates an algebraic feedback loop, because the slope
		// collapses past the force peak, which inflates the recovered slip, which
		// pushes further past the peak. In practice that limit-cycles instead of
		// converging. The book flags exactly this at 7.33 - the deflection form needs
		// the INVERSE force characteristic and warns of "double valued solutions"
		// resolved "through iterations" - and offers 7.35 as the loop-free form. Here
		// sigma multiplies the derivative and never divides the state.
		const float ImplicitDenomAlpha = sigma_alpha_c + (Vx_abs * DeltaTime);
		const float ImplicitDenomKappa = sigma_kappa_c + (Vx_abs * DeltaTime);

		const float TanAlphaPrime =
			((sigma_alpha_c * FMath::Tan(TransientState.alpha_prime)) - (Vsy * DeltaTime))
			/ ImplicitDenomAlpha;

		TransientState.kappa_prime =
			((sigma_kappa_c * TransientState.kappa_prime) - (Vsx * DeltaTime))
			/ ImplicitDenomKappa;

		// 7.11 recast through 7.16: the C_Fgamma / C_Falpha ratio cancels and camber
		// reduces to a plain first-order lag of gamma sharing the side-slip relaxation
		// length. 7.11 notes the camber relaxation length equals the side-slip one,
		// which Higuchi's step-response experiments confirm.
		TransientState.gamma_prime =
			((sigma_alpha_c * TransientState.gamma_prime) + (Vx_abs * Gamma * DeltaTime))
			/ ImplicitDenomAlpha;

		// 7.35 carries the TANGENT of alpha'. The Magic Formula takes the angle and
		// re-applies tan() internally, so invert it here rather than handing a tangent
		// over as if it were an angle.
		TransientState.alpha_prime = FMath::Atan(TanAlphaPrime);

		// === 5. Low-speed damping (7.26, after Besselink) ===
		//
		//   kappa' = u/sigma_kappa - (k_Vlow / C_Fkappa) * Vsx      (7.26)
		//
		// and the book notes "a similar equation may be employed for the lateral
		// transient slip". The state above is u/sigma_kappa already, so this is the
		// second term only.
		//
		// Suppressed with speed exactly as the book asks - full at rest, gone by Vlow.
		// A raised cosine rather than a straight line so the damping arrives and leaves
		// with zero slope; a kink here would be a step in force at the moment the
		// suppression ends.
		const float LowSpeedDamping = (Vlow > KINDA_SMALL_NUMBER && Vx_abs < Vlow)
			? 0.5f * kVlow0 * (1.f + FMath::Cos(PI * Vx_abs / Vlow))
			: 0.f;

		// Both divisors are slip stiffnesses that legitimately collapse past the force
		// peak, so they get the same floor C_Falpha already carries. It matters least
		// exactly where this term is active - at low speed the slip is small and the
		// curve is near its steepest - but a wheel spun up against a stationary car is
		// both slow and saturated, so the case is reachable.
		const float C_Fkappa = FMath::Max(dFx_dkappa, KINDA_SMALL_NUMBER);

		TransientState.kappa_prime_damped =
			TransientState.kappa_prime - ((LowSpeedDamping / C_Fkappa) * Vsx);
		TransientState.alpha_prime_damped =
			FMath::Atan(TanAlphaPrime - ((LowSpeedDamping / C_Falpha) * Vsy));

		// Carcass deflections are no longer state - keep them as derived diagnostics
		// so anything inspecting them still reads something meaningful (7.15, 7.28,
		// and 7.16 solved for v_gamma).
		TransientState.u = TransientState.kappa_prime * sigma_kappa_c;
		TransientState.v_alpha = TanAlphaPrime * sigma_alpha_c;
		TransientState.v_gamma = TransientState.gamma_prime * sigma_alpha_c
			* (C_Fgamma / C_Falpha);

		return TransientState;
	}
	
	/**
	 * Slip stiffness dFx/dkappa at an operating point, N per unit slip.
	 *
	 * Public face of the private numerical derivative. Wanted by the wheel-shaft
	 * update, which needs the slope to damp its own reaction implicitly.
	 */
	float GetSlipStiffnessN(float Vc, float Vcy, float Vcx, float Omega,
		float Gamma, float Fz, float Kappa)
	{
		return Compute_dFx_dKappaNumerical(Vc, Vcy, Vcx, Omega, Gamma, Fz, Kappa);
	}

private:
	
	/**
	 * Turns a raw relaxation length into a usable one: rejects non-finite or
	 * non-positive values, clamps to a sane band, and smooths across steps.
	 *
	 * The clamp and the smoothing are NOT from the book - they are numerical
	 * guards. The lower bound stops a vanishing slope (past the force peak, or at
	 * standstill) from producing a division by ~0 in 7.14/7.15. The smoothing
	 * damps the step-to-step noise of the central-difference derivative; without it
	 * sigma can chatter, and sigma feeds straight into the lag time constant.
	 *
	 * @param RawSigma  Relaxation length straight from 7.36 / 7.39 [m]
	 * @param FallbackSigma  Initial relaxation length to use if RawSigma is unusable
	 *                       (book 7.6 / 7.8: sigma0 = slip stiffness / carcass stiffness) [m]
	 * @param PrevSigma  Previous step's value, for smoothing [m]
	 */
	float ResolveRelaxationLength(float RawSigma, float FallbackSigma, float PrevSigma) const
	{
		const float Fallback = FMath::Max(FallbackSigma, sigma_min);

		float Sigma = (FMath::IsFinite(RawSigma) && RawSigma > KINDA_SMALL_NUMBER)
			? RawSigma
			: Fallback;

		Sigma = FMath::Clamp(Sigma, sigma_min, sigma_max);

		// First step has no history; PrevSigma defaults to 0.
		if (PrevSigma <= KINDA_SMALL_NUMBER)
		{
			return Sigma;
		}

		return FMath::Lerp(PrevSigma, Sigma, FMath::Clamp(sigma_smoothing, 0.f, 1.f));
	}


	// --- Helpers: numerical derivative using central difference ---

	/// 
	/// @param Vc Velocity of wheel center in world [m/s]
	/// @param Vcy Lateral velocity of wheel at wheel center [m/s]
	/// @param Vcx Longitudinal velocity of wheel at wheel center [m/s]
	/// @param Omega wheel angular velocity [Rad/s]
	/// @param Gamma Camber angle [Rad]
	/// @param Fz Normal force [N]
	/// @param IN_kappa_prime Transient state alpha prime
	/// @return dFx/dkappa' [N / unit-kappa]
	float Compute_dFx_dKappaNumerical(float Vc, float Vcy, float Vcx, float Omega, float Gamma, float Fz, float IN_kappa_prime)
	{
		// choose perturbation
		const float base = FMath::Max(1e-5f, FMath::Abs(IN_kappa_prime));
		const float dk = FMath::Max(1e-5f, base * 1e-3f); // scale small relative perturbation

		// Evaluate Fx at kappa + dk and kappa - dk using override flag.
		float k_plus = IN_kappa_prime + dk;
		float k_minus = IN_kappa_prime - dk;

		float slipRatio = k_plus;
		float slipAngle = 0;
		// Compute forces at k_plus
		FPacejkaResult rPlus = ComputeTireForcesOverriden(
			Fz, slipRatio, slipAngle, Gamma,
			Vc,
			Vcy,
			Vcx,
		   Omega);
		float Fx_plus = rPlus.Fx / 100.0f; // convert back to N (your ComputeTireForces multiplies by 100)

		slipRatio = k_minus;
		slipAngle = 0;
		FPacejkaResult rMinus = ComputeTireForcesOverriden(
			Fz, slipRatio, slipAngle, Gamma,
			Vc,
			Vcy,
			Vcx,
			Omega);
		float Fx_minus = rMinus.Fx / 100.0f;

		const float dFx = (Fx_plus - Fx_minus) / (2.0f * dk);
		if (!FMath::IsFinite(dFx))
			return 0.f;
		return dFx;
	}

	/// 
	/// @param Vc Velocity of wheel center in world [m/s]
	/// @param Vcy Lateral velocity of wheel at wheel center [m/s]
	/// @param Vcx Longitudinal velocity of wheel at wheel center [m/s]
	/// @param Omega wheel angular velocity [Rad/s]
	/// @param Gamma Camber angle [Rad]
	/// @param Fz Normal force [N]
	/// @param IN_alpha_prime Transient state alpha prime
	/// @return dFy/dalpha' [N / rad]
	float Compute_dFy_dAlphaNumerical(float Vc, float Vcy, float Vcx, float Omega, float Gamma, float Fz, float IN_alpha_prime)
	{
		const float base = FMath::Max(1e-5f, FMath::Abs(IN_alpha_prime));
		const float da = FMath::Max(1e-5f, base * 1e-3f);

		float slipRatio = 0;
		float slipAngle = IN_alpha_prime + da;
		FPacejkaResult rPlus = ComputeTireForcesOverriden(Fz, slipRatio, slipAngle, Gamma,
		Vc,
		Vcy,
		Vcx,
		Omega);
		float Fy_plus = rPlus.Fy / 100.0f;

		slipAngle = IN_alpha_prime - da;
		FPacejkaResult rMinus = ComputeTireForcesOverriden(Fz, slipRatio, slipAngle, Gamma,
		Vc,
		Vcy,
		Vcx,
		Omega);
		float Fy_minus = rMinus.Fy / 100.0f;

		const float dFy = (Fy_plus - Fy_minus) / (2.0f * da);
		if (!FMath::IsFinite(dFy))
			return 0.f;
		return dFy;
	}
	
	
};
