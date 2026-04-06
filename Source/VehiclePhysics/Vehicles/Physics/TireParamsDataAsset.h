#pragma once

#include <ThirdParty/Vulkan/Include/vulkan/vulkan_core.h>

#include "PacejkaResult.h"
#include "TransientState.h"
#include "Engine/DataAsset.h"
#include "TireParamsDataAsset.generated.h"

UCLASS(BlueprintType)
class VEHICLEPHYSICS_API UTireParamsDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true"))
	float SlipRatio = 0.f;    // Nominal load
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true"))
	float SlipAngle = 0.f;     // effective unloaded radius [m]
	UPROPERTY(EditAnywhere, Category = "Tire|Editor", meta = (AllowPrivateAccess = "true"))
	float Camber = 0.f;  	// Nominal (reference) tire inflation pressure
	
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	float FZ0 = 2500.f;    // Nominal load
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	float R0 = 0.32f;     // effective unloaded radius [m]
	UPROPERTY(EditAnywhere, Category = "Tire|General", meta = (AllowPrivateAccess = "true"))
	float pi0 = 200000.f;  	// Nominal (reference) tire inflation pressure
	
	// Actual tire inflation pressure
	float pi = 200000.f;

private:
	
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
	float PKY1 = -15.324f;    // Maximum value of stiffness Kfy/Fznom
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
	float PHY1 = -0.001806f;     // Horizontal shift Shy at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PHY2 = 0.00352f;     // Variation of shift Shy with load
	
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY1 = -0.00661f;     // Vertical shift in Svy/Fz at Fznom
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY2 = 0.03592f;     // Variation of shift Svy/Fz with load
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY3 = -0.162f;     // Variation of shift Svy/Fz with camber
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float PVY4 = -0.4864f;     // Variation of shift Svy/Fz with camber and load
	
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
	float RVY4 = 94.63f;     // Variation of Svyk/Muy*Fz with alpha
	UPROPERTY(EditAnywhere, Category = "Tire|Lateral", meta = (AllowPrivateAccess = "true"))
	float RBY5 = 1.8914f;     // Variation of Svyk/Muy*Fz with kappa
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
	
	
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Longitudinal", meta = (AllowPrivateAccess = "true"))
	float epsilon = 0.1f; 



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
	float epsilon_F = 0.05f; // Small constant to avoid singularity

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tire|Transient", meta = (AllowPrivateAccess = "true"))
	float sigma_min = 0.05f; // Minimum relaxation length [m]

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
		FPacejkaResult result;

		float Fz0_prime = LFZ0 * FZ0; // Book (4.E1)
		float df_z = (Fz - Fz0_prime) / Fz0_prime; // Book (4.E2a)
		float dpi = (pi - pi0)/pi0; // Book (4.E2b)
		dpi = 0.f;
		
		float Re = R0;
		float Vsx = Vcx - Re * Omega;
		float kappa = -Vsx / FMath::Max(FMath::Abs(Vcx), epsilon_v); // Book (4.E5)
		kappa = FMath::Clamp(kappa, -3.f, 3.f);

		float alpha = -FMath::Atan2(Vcy, FMath::Abs(Vcx));

		alpha = FMath::Clamp(alpha, -1.4f, 1.4f);
		alpha = FMath::Clamp(alpha, -1.4f, 1.4f);
		float alpha_star = -Vcy / FMath::Max(FMath::Abs(Vcx), epsilon_v); // Book (4.E3)


		float Gamma_star = FMath::Sin(Gamma); // Book (4.E4)

		// Ensure normal force is positive
		Fz = FMath::Max(Fz, 1.0f);

		float Vc_prime = Vc + epsilon_v; // Book (4.E6a)
		float CosPrimeAlpha = Vcx/Vc_prime; // Book (4.E6)
		float V0 = FMath::Sqrt(9.81f * R0);
		float Vs = FMath::Sqrt(Vsx * Vsx + Vcy * Vcy);
		float lambda_mu_x_star = LMUX / (1 + LMUV * Vs / V0); // Book (4.E7)
		float lambda_mu_y_star = LMUY / (1 + LMUV * Vs / V0); // Book (4.E7)
		//Has to be reset since its not working now!
		lambda_mu_x_star = 1.f;
		lambda_mu_y_star = 1.f;
		float Au = 10.f;
		float lambda_mu_x_prime = Au * lambda_mu_x_star / (1 + (Au - 1) * lambda_mu_x_star); // Book (4.E8)
		float lambda_mu_y_prime = Au * lambda_mu_x_star / (1 + (Au - 1) * lambda_mu_x_star); // Book (4.E8)

        
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
		float S_Hy = PHY1 + PHY2 * LHY +
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
		//By_kappa = FMath::Max(By_kappa, 0.001f);
		float Cy_kappa = RCY1; // Book (4.E63)
		float Ey_kappa = REY1 + REY2 * df_z; // Book (4.E64)
		float S_Hy_kappa = RHY1 + RHY2 * df_z; // Book (4.E55)
		float kappa_s = kappa + S_Hy_kappa; // Book (4.E61)
		float Gy_kappa0 = FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * S_Hy_kappa - Ey_kappa *
			(By_kappa * S_Hy_kappa - FMath::Atan(By_kappa * S_Hy_kappa)))); // Book (4.E60)
		//Gy_kappa0 = FMath::Max(Gy_kappa0, 0.001f);
		float Gy_kappa = FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * kappa_s - Ey_kappa *
			(By_kappa * kappa_s - FMath::Atan(By_kappa * kappa_s)))) / Gy_kappa0; // Book (4.E59)
		float D_Vy_k = Uy * Fz * (RVY1 + RVY2 * df_z + RVY3 * Gamma_star) *
			FMath::Cos(FMath::Atan(RVY4 * alpha_star)) * zeta_2; // Book (4.E67)
		float S_Vy_k = D_Vy_k * FMath::Sin(RBY5 * FMath::Atan(RVY6 * kappa)) * LVYK; // Book (4.E66)
		float Fy = Fy0 * Gy_kappa + S_Vy_k; // Book (4.E58)

		/*  ------- AligningTorque(combined slip) ------- */

		float alpha_t_eq = FMath::Sqrt(
			alpha_t * alpha_t + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_t); // Book (4.E77)
		float alpha_r_eq = FMath::Sqrt(
			alpha_r * alpha_r + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_r); // Book (4.E78)
		
		float Fy_prime = Gy_kappa * Fy0; // Book (4.E74) IMPORTANT This is not correct, Gy_kappa and Fy0 need to be calculated with zero camber
		float Mzr = Dr * FMath::Cos(Cr * FMath::Atan(Br * alpha_r_eq)) * CosPrimeAlpha; // Book (4.E75)
		float s = R0 * (SSZ1 + SSZ2 * (Fy/Fz0_prime) + (SSZ3 + SSZ4 * df_z) * Gamma_star) * LS; // Book (4.E76)
		float t = Dt * FMath::Cos(Ct * FMath::Atan(Bt * alpha_t_eq - FMath::Atan(Bt * alpha_t_eq))) * CosPrimeAlpha; // Book(4.E73)
		float Mz_prime = -t * Fy_prime; // Book (4.E72)
		float Mz = Mz_prime + Mzr + s * Fx; // Book (4.E71)

		// Apply Forces and Torque
		Fx0 *= 100.f;
		Fy0 *= 100.f;
		Fx *= 100.f;
		Fy *= 100.f;

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
		
		result.pneumaticTrail = t0;
		result.Mz = Mz;
		result.Mz_normalized = Mz / 500.f;

		return result;
		
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
		float dpi = (pi - pi0)/pi0; // Book (4.E2b)
		dpi = 0.f;
		
		float Re = R0;
		float Vsx = Vcx - Re * Omega;

		alpha = FMath::Clamp(alpha, -1.4f, 1.4f);
		alpha = FMath::Clamp(alpha, -1.4f, 1.4f);
		float alpha_star = FMath::Tan(alpha);

		float Gamma_star = FMath::Sin(Gamma); // Book (4.E4)

		// Ensure normal force is positive
		Fz = FMath::Max(Fz, 1.0f);

		float Vc_prime = Vc + epsilon_v; // Book (4.E6a)
		float CosPrimeAlpha = Vcx/Vc_prime; // Book (4.E6)
		float V0 = FMath::Sqrt(9.81f * R0);
		float Vs = FMath::Sqrt(Vsx * Vsx + Vcy * Vcy);
		float lambda_mu_x_star = LMUX / (1 + LMUV * Vs / V0); // Book (4.E7)
		float lambda_mu_y_star = LMUY / (1 + LMUV * Vs / V0); // Book (4.E7)
		//Has to be reset since its not working now!
		lambda_mu_x_star = 1.f;
		lambda_mu_y_star = 1.f;
		float Au = 10.f;
		float lambda_mu_x_prime = Au * lambda_mu_x_star / (1 + (Au - 1) * lambda_mu_x_star); // Book (4.E8)
		float lambda_mu_y_prime = Au * lambda_mu_x_star / (1 + (Au - 1) * lambda_mu_x_star); // Book (4.E8)

        
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
		float S_Hy = PHY1 + PHY2 * LHY +
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
		//By_kappa = FMath::Max(By_kappa, 0.001f);
		float Cy_kappa = RCY1; // Book (4.E63)
		float Ey_kappa = REY1 + REY2 * df_z; // Book (4.E64)
		float S_Hy_kappa = RHY1 + RHY2 * df_z; // Book (4.E55)
		float kappa_s = kappa + S_Hy_kappa; // Book (4.E61)
		float Gy_kappa0 = FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * S_Hy_kappa - Ey_kappa *
			(By_kappa * S_Hy_kappa - FMath::Atan(By_kappa * S_Hy_kappa)))); // Book (4.E60)
		//Gy_kappa0 = FMath::Max(Gy_kappa0, 0.001f);
		float Gy_kappa = FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * kappa_s - Ey_kappa *
			(By_kappa * kappa_s - FMath::Atan(By_kappa * kappa_s)))) / Gy_kappa0; // Book (4.E59)
		float D_Vy_k = Uy * Fz * (RVY1 + RVY2 * df_z + RVY3 * Gamma_star) *
			FMath::Cos(FMath::Atan(RVY4 * alpha_star)) * zeta_2; // Book (4.E67)
		float S_Vy_k = D_Vy_k * FMath::Sin(RBY5 * FMath::Atan(RVY6 * kappa)) * LVYK; // Book (4.E66)
		float Fy = Fy0 * Gy_kappa + S_Vy_k; // Book (4.E58)

		/*  ------- AligningTorque(combined slip) ------- */

		float alpha_t_eq = FMath::Sqrt(
			alpha_t * alpha_t + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_t); // Book (4.E77)
		float alpha_r_eq = FMath::Sqrt(
			alpha_r * alpha_r + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_r); // Book (4.E78)
		
		float Fy_prime = Gy_kappa * Fy0; // Book (4.E74) IMPORTANT This is not correct, Gy_kappa and Fy0 need to be calculated with zero camber
		float Mzr = Dr * FMath::Cos(Cr * FMath::Atan(Br * alpha_r_eq)) * CosPrimeAlpha; // Book (4.E75)
		float s = R0 * (SSZ1 + SSZ2 * (Fy/Fz0_prime) + (SSZ3 + SSZ4 * df_z) * Gamma_star) * LS; // Book (4.E76)
		float t = Dt * FMath::Cos(Ct * FMath::Atan(Bt * alpha_t_eq - FMath::Atan(Bt * alpha_t_eq))) * CosPrimeAlpha; // Book(4.E73)
		float Mz_prime = -t * Fy_prime; // Book (4.E72)
		float Mz = Mz_prime + Mzr + s * Fx; // Book (4.E71)

		// Apply Forces and Torque
		Fx0 *= 100.f;
		Fy0 *= 100.f;
		Fx *= 100.f;
		Fy *= 100.f;

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
		
		result.pneumaticTrail = t0;
		result.Mz = Mz;
		result.Mz_normalized = Mz / 500.f;

		return result;
		
	}
	
	/// 
	/// @param Vc Velocity of wheel center [m/s]
	/// @param Vcx Forward velocity of wheel at wheel center [m/s]
	/// @param Vcy Lateral velocity of wheel at wheel center [m/s]
	/// @param Omega wheel angular velocity [Rad/s]
	/// @param Gamma Camber angle [Rad]
	/// @param Fx_SS Longitudinal force at steady state (No transient slip) [N]
	/// @param Fy_SS Lateral force at steady state (No transient slip) [N]
	/// @param Fz Normal force [N]
	/// @param DeltaTime Simulation delta time [s]
	/// @param TransientState Transient state
	/// @return The updated transient state
	FTransientTireState UpdateTransientState(float Vc,
	float Vcx, float Vcy, float Gamma, float Omega,
	float Fx_SS, float Fy_SS, float Fz,
	float DeltaTime,
	FTransientTireState TransientState)
	{
		// Convert to SI units (m/s)
		float Vx_abs = FMath::Max(FMath::Abs(Vcx), 0.1f);
	
		// === 1. Slip velocities (Fig. 7.1) ===
		float Vsx = Vcx - Omega * R0;  // V_sx (rim point S) or = |Vx| * kappa
		float Vsy = Vcy;

		// === 2. Dynamic relaxation lengths ===
	
		// Get current relaxation lengths based on transient slip
		float sigma_kappa_c = ComputeRelaxationLengthKappa(Vc, Vcy, Vcx, Omega, Gamma, Fx_SS, Fz, TransientState);
		float sigma_alpha_c = ComputeRelaxationLengthAlpha(Vc, Vcy, Vcx, Omega, Gamma, Fy_SS, Fz, TransientState);

		// Longitudinal deflection (Equation 7.9)
		const float denom_u = 1.f + (DeltaTime * Vx_abs / sigma_kappa_c);
		const float du_term = -Vsx * DeltaTime;
		TransientState.u = (TransientState.u + du_term) / denom_u;
		//float du_dt = Vx_abs * (kappa - TransientState.kappa_prime);
		//TransientState.u += du_dt * DeltaTime;

		// Lateral deflection due to side slip (Equation 7.7)
		const float denom_v = 1.f + (DeltaTime * Vx_abs / sigma_alpha_c);
		const float dv_term = -Vsy * DeltaTime;
		TransientState.v_alpha = (TransientState.v_alpha + dv_term) / denom_v;
		//float dv_alpha_dt = Vx_abs * (FMath::Tan(alpha_ss) - FMath::Tan(TransientState.alpha_prime));
		//TransientState.v_alpha += dv_alpha_dt * DeltaTime;
	
		// === Camber deflection (Eq. 7.11) ===
		// dv_gamma/dt + (|Vx|/σ_α) * v_gamma = (C_Fγ / C_Fy) * |Vx| * γ
		const float dv_gamma_term = (C_Fgamma / C_Fy) * Vx_abs * Gamma * DeltaTime;
		TransientState.v_gamma = (TransientState.v_gamma + dv_gamma_term) / denom_v; // same denom_v for σ_α

		// === Compute transient slips from deflections (Eq. 7.14–7.16) ===
		if (sigma_kappa_c > epsilon_F)
			TransientState.kappa_prime = TransientState.u / sigma_kappa_c;

		if (sigma_alpha_c > epsilon_F)
		{
			const float v_total = TransientState.v_alpha + TransientState.v_gamma;
			TransientState.alpha_prime = v_total / sigma_alpha_c;
			TransientState.gamma_prime = (C_Fy / C_Fgamma) * TransientState.v_gamma / sigma_alpha_c;
		}
		
		return TransientState;
	}

private:
	
	/// 
	/// @param Vc Velocity of wheel center in world [m/s]
	/// @param Vcy Lateral velocity of wheel at wheel center [m/s]
	/// @param Vcx Longitudinal velocity of wheel at wheel center [m/s]
	/// @param Omega wheel angular velocity [Rad/s]
	/// @param Gamma Camber angle [Rad]
	/// @param Fx_SS Longitudinal force in steady state [N]
	/// @param Fz Normal force [N]
	/// @param TransientState transient state
	/// @return 
	float ComputeRelaxationLengthKappa(float Vc, float Vcy, float Vcx, float Omega, float Gamma, float Fx_SS, float Fz, FTransientTireState& TransientState)
	{
		// Book eqn 7.38: sigma_kappa* = (1 / C_Fx) * (F_x / kappa')
		// Regularized form (book eqn 7.40 style) used below to avoid singularity.
		const float eps = FMath::Max(epsilon_F, 0.f);
		const float sigma0 = FMath::Max(sigma_kappa0, 0.f);
		const float sigma_max = FMath::Max(0.9f, 0.f);

		const float absK = FMath::Abs(TransientState.kappa_prime);
		const float absFx = FMath::Abs(Fx_SS);
		const float Cfx = FMath::Max(C_Fx, 1e-6f);

		// Compute derivative robustly
		const float dFx_dk = Compute_dFx_dKappaNumerical(Vc, Vcy, Vcx, Omega, Gamma, Fz, TransientState.kappa_prime);
		if (dFx_dk <= eps || !FMath::IsFinite(dFx_dk))
		{
			return sigma0; // fallback
		}

		float sigma = C_Fx /*or C_Fk? see note below*/ / dFx_dk;

		// clamp to realistic bounds
		sigma = FMath::Clamp(sigma, sigma_min, sigma_max);

		// optional smoothing (store prev_sigma in TransientState)
		const float alpha = 0.15f;
		float prev = FMath::Max(TransientState.prev_sigma_kappa, sigma0);
		float smoothed = FMath::Lerp(prev, sigma, alpha);
		TransientState.prev_sigma_kappa = smoothed;
		return smoothed;

		/*float sigma_star = sigma0;
	
		if (absK < eps)
		{
			// small slip: use baseline
			sigma_star = sigma0;
		}
		else
		{
			// core book formula (7.38)
			float sigma_from_core = (1.0f / Cfx) * (Fx / kappa_prime_IN);
	
			// regularized/robust approximation (book suggests smoothing to avoid singularity) (7.40-style)
			float sigma_reg = sigma0 * (absFx + Cfx * eps) / (absK + eps);
	
			// prefer the regularized value, but keep sign positive
			sigma_star = FMath::Abs(sigma_reg);
	
			// But if core result is reasonable and within expected bounds, prefer it:
			if (FMath::IsFinite(sigma_from_core) && sigma_from_core > 0.0f &&
				sigma_from_core >= sigma_min && sigma_from_core <= sigma_max)
			{
				sigma_star = sigma_from_core;
			}
		}
	
		// clamp to realistic bounds
		sigma_star = FMath::Clamp(sigma_star, sigma_min, sigma_max);
	
		// Exponential smoothing to avoid instantaneous jumps (prevents oscillations)
		// alpha chosen relative to dt: alpha_small = 1 - exp(-k * dt) but simpler linear blending ok
		const float alpha = FMath::Clamp(0.15f, 0.01f, 0.6f);
		// Use previous stored sigma if available (TransientState.prev_sigma_kappa) — add to your transient state
		float prev_sigma = FMath::Max(TransientState.prev_sigma_kappa, sigma0);
		float sigma_smoothed = FMath::Lerp(prev_sigma, sigma_star, alpha);
	
		// store for next frame
		TransientState.prev_sigma_kappa = sigma_smoothed;
	
		return sigma_smoothed;*/
	
		// Eq. 7.38: σ_κ* = (1 / C_Fx) * (F_x / κ')
		// Eq. 7.40: smoothed variant to avoid singularity at κ'→0

		const float absKappa = FMath::Abs(TransientState.kappa_prime);
		const float absFx2 = FMath::Abs(Fx_SS);

		if (absKappa < epsilon_F)
			return sigma_kappa0;

		// Equation 7.38 core
		float sigma_kappa_star = (1.0f / C_Fx) * (Fx_SS / TransientState.kappa_prime);

		// Equation 7.40 (smooth regularized form)
		sigma_kappa_star = sigma_kappa0 *
			(absFx2 + C_Fx * epsilon_F) /
			(absKappa + epsilon_F);

		// Clamp to a minimum realistic length
		return FMath::Max(sigma_kappa_star, sigma_min);
	}
	
	/// 
	/// @param Vc Velocity of wheel center in world [m/s]
	/// @param Vcy Lateral velocity of wheel at wheel center [m/s]
	/// @param Vcx Longitudinal velocity of wheel at wheel center [m/s]
	/// @param Omega wheel angular velocity [Rad/s]
	/// @param Gamma Camber angle [Rad]
	/// @param Fy_SS Lateral force in steady state [N]
	/// @param Fz Normal force [N]
	/// @param TransientState transient state
	/// @return 
	float ComputeRelaxationLengthAlpha(float Vc, float Vcy, float Vcx, float Omega, float Gamma, float Fy_SS, float Fz, FTransientTireState& TransientState)
	{
		// Eq 7.30: sigma_alpha* = (1 / C_Fy) * (F_y / tan(alpha'))
		const float eps = FMath::Max(epsilon_F, 0.f);
		const float sigma0 = FMath::Max(sigma_alpha0, 0.f);
		const float sigma_max = 0.9f;

		const float tanAlpha = FMath::Tan(TransientState.alpha_prime);
		const float absTan = FMath::Abs(tanAlpha);
		const float absFy = FMath::Abs(Fy_SS);
		const float Cfy = FMath::Max(C_Fy, 1e-6f);

		const float dFy_dalpha = Compute_dFy_dAlphaNumerical(Vc, Vcy, Vcx, Omega, Gamma, Fz, TransientState.alpha_prime);
		if (dFy_dalpha <= eps || !FMath::IsFinite(dFy_dalpha))
		{
			return sigma0;
		}

		float sigma = C_Fgamma /*or C_Fy? see note below*/ / dFy_dalpha;

		sigma = FMath::Clamp(sigma, sigma_min, sigma_max);

		const float alpha = 0.15f;
		float prev = FMath::Max(TransientState.prev_sigma_alpha, sigma0);
		float smoothed = FMath::Lerp(prev, sigma, alpha);
		TransientState.prev_sigma_alpha = smoothed;
		return smoothed;

		/*float sigma_star = sigma0;
	
		if (absTan < eps)
		{
			sigma_star = sigma0;
		}
		else
		{
			float sigma_from_core = (1.0f / Cfy) * (Fy / tanAlpha);
	
			float sigma_reg = sigma0 * (absFy + Cfy * eps) / (absTan + eps);
	
			sigma_star = FMath::Abs(sigma_reg);
	
			if (FMath::IsFinite(sigma_from_core) && sigma_from_core > 0.0f &&
				sigma_from_core >= sigma_min && sigma_from_core <= sigma_max)
			{
				sigma_star = sigma_from_core;
			}
		}
	
		sigma_star = FMath::Clamp(sigma_star, sigma_min, sigma_max);
	
		const float alpha = FMath::Clamp(0.15f, 0.01f, 0.6f);
		float prev_sigma = FMath::Max(TransientState.prev_sigma_alpha, sigma0);
		float sigma_smoothed = FMath::Lerp(prev_sigma, sigma_star, alpha);
		TransientState.prev_sigma_alpha = sigma_smoothed;
	
		return sigma_smoothed;*/


	
		// Eq. 7.30: σ_α* = (1 / C_Fy) * (F_y / tan(α'))
		// Eq. 7.32: smoothed version for small α'

		const float tanAlpha2 = FMath::Tan(TransientState.alpha_prime);
		const float absTanAlpha = FMath::Abs(tanAlpha2);
		const float absFy2 = FMath::Abs(Fy_SS);

		if (absTanAlpha < epsilon_F)
			return sigma_alpha0;

		// Equation 7.30 core
		float sigma_alpha_star = (1.0f / C_Fy) * (Fy_SS / tanAlpha2);

		// Equation 7.32 (regularized smooth approximation)
		sigma_alpha_star =sigma_alpha0 *
			(absFy2 + C_Fy * epsilon_F) /
			(absTanAlpha + epsilon_F);

		// Clamp to a reasonable lower limit
		return FMath::Max(sigma_alpha_star, sigma_min);
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
	
	
	FVector ComputeNonLaggingCamberForce(float gamma, float Fz)
	{
		// Equations 7.41-7.44 for non-lagging camber force
    
		// Simplified implementation - you can expand this based on the full equations in the book
		float epsilon_c = p_NL5 / (1.0f + p_NL7 * (Fz / FZ0) + 
											  p_NL8 * FMath::Square(Fz / FZ0));
    
		// Non-lagging force fraction
		float Fy_NL = epsilon_c * C_Fgamma * gamma;
    
		return FVector(0.0f, Fy_NL, 0.0f);
	}

	float ComputeGyroscopicCouple(float dvy_dt, float Fz, float Omega)
	{
		// Equation 7.49
		// For simplicity, using lateral deflection rate approximation
		float m_t = 10.0f; // Tire mass [kg] - should be a parameter
		float r_e = R0; // Effective rolling radius
		
		float Mz_gyr = c_gyr * m_t * r_e * Omega * dvy_dt * (Fz / FZ0);
    
		return Mz_gyr;
	}
};