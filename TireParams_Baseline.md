# Tyre parameter baseline

Snapshot of `UTireParamsDataAsset`'s C++ defaults taken **before** they were
changed to match `TP_GenericSedan`, the asset that had actually been driven.
Kept so nothing is lost and the previous set can be restored from one file.

Taken from `TireParamsDataAsset.h` at change 40522, 283 defaults.

## What changed, and why

Eleven parameters differed between the header and the working asset. The header
was the unverified one — a subclass deriving from it inherited values no car had
ever been driven with, which is how `UTireParamsSportsCar` shipped with an
inverted lateral curve.

| Parameter | Header (old) | Asset (now the default) | Why |
|---|---|---|---|
| `FZ0` | 2500. | 3000. | nominal load, moved to the car it is used on |
| `R0` | 0.32 | 0.33 | matches WheelRadiusCm; 0.32 was a 3% error in Fz and slip ratio |
| `PKY1` | -15.324 | 15.324 | SIGN FLIPPED - see the note; published sets carry this negative |
| `PHY1` | -0.001806 | 0. | lateral shift zeroed: no force at zero slip |
| `PHY2` | 0.00352 | 0. | lateral shift zeroed |
| `PVY1` | -0.00661 | 0. | lateral shift zeroed |
| `PVY2` | 0.03592 | 0. | lateral shift zeroed |
| `PVY3` | -0.162 | 0. | lateral shift zeroed |
| `PVY4` | -0.4864 | 0. | lateral shift zeroed |
| `RVY4` | 94.63 | 0. | combined-slip lateral shift zeroed |
| `RVY5` | 1.8914 | 0. | combined-slip lateral shift zeroed |

**The zeroing is a coherent choice, not drift.** `PHY*`, `PVY*` and `RVY4/5` are
the lateral horizontal and vertical shift terms — they model conicity and
ply-steer, the small force a real tyre makes at zero slip. Zeroed, the tyre is
perfectly symmetric and a car tracks straight with no steering input. That is
usually what you want in a simulator and rarely what a measured `.tir` file gives
you.

## The PKY1 sign, which is a trap worth knowing about

Published Magic Formula parameter sets carry **`PKY1` negative** — that is the
ISO convention and it is what you will find in any `.tir` file or paper. This
implementation needs it **positive**: with a negative value, Fy comes out with
the opposite slope to Fx through the origin and the car steers the wrong way.

So the header was not wrong about the tyre. It was right about the literature and
wrong about this code. **Anyone importing a parameter set from a `.tir` file or a
paper must flip `PKY1`** — and should check `PHY*`/`PVY*` too, since those carry
the same lateral sign convention.

## Full previous defaults

| Parameter | Value | Comment |
|---|---|---|
| `SlipRatio` | 0. | kappa, dimensionless |
| `SlipAngle` | 0. | alpha [rad] |
| `Camber` | 0. | gamma [rad] |
| `FZ0` **(changed)** | 2500. | Nominal load |
| `R0` **(changed)** | 0.32 | effective unloaded radius [m] |
| `pi0` | 200000. | Nominal (reference) tire inflation pressure |
| `pi` | 200000. |  |
| `bEnableSpeedDependentFriction` | false |  |
| `MzGraphScale` | 500. |  |
| `GraphLoadN` | 2637. |  |
| `bEnablePressureSensitivity` | true |  |
| `qFz1` | 15.0 | Linear vertical stiffness |
| `qFz2` | 0.0 | Quadratic (progressive) vertical stiffness |
| `qFz3` | 0.0 | Camber effect on vertical stiffness |
| `qV2` | 0.0 | Centrifugal stiffening with wheel speed |
| `qFcx` | 0.0 | Vertical stiffness loss under longitudinal force |
| `qFcy` | 0.0 | Vertical stiffness loss under lateral force |
| `ppFz1` | 0.7 | Inflation pressure effect on vertical stiffness |
| `LFZ0` | 1. | Scale factor of nominal (rated) load |
| `LCX` | 1.0 | Scale factor of Fx shape factor |
| `LMUX` | 1.0 | Scale factor of Fx peak friction coefficient |
| `LEX` | 1.0 | Scale factor of Fx curvature factor |
| `LHX` | 1.0 | Scale factor of Fx horizontal shift |
| `LVX` | 1.0 | Scale factor of Fx vertical shift |
| `LXAL` | 1. | Scale factor of Fx vertical shift |
| `LCY` | 1.0 | Scale factor of Fy shape factor |
| `LMUY` | 1.0 | Scale factor of Fy peak friction coefficient |
| `LEY` | 1.0 | Scale factor of Fy curvature factor |
| `LKYAL` | 1. | Scale factor of cornering stiffness |
| `LKZC` | 1.0 | Scale factor of camber moment stiffness |
| `LHY` | 1.0 | Scale factor of Fy horizontal shift |
| `LVY` | 1.0 | Scale factor of Fy vertical shift |
| `LT` | 1. | Scale factor of pneumatic trail |
| `LYK` | 1. | Scale factor of k influence on Fy(a) |
| `LVYK` | 1. | Scale factor of k induced ‘ply-steer’ Fy |
| `LMUV` | 0.05 | Scale factor of with slip speed Vs decaying friction |
| `LS` | 1. | Scale factor of moment arm of Fx |
| `LMR` | 1. | Scale factor aligning torque |
| `qsy1` | 0.01 |  |
| `qsy2` | 0. |  |
| `qsy3` | 0. |  |
| `qsy4` | 0. |  |
| `qsy5` | 0. |  |
| `qsy6` | 0. |  |
| `qsy7` | 0.85 |  |
| `qsy8` | -0.4 |  |
| `LMY` | 1. | Scale factor rolling resistance moment |
| `zeta_0` | 1.0 |  |
| `zeta_1` | 1.0 |  |
| `zeta_2` | 1.0 |  |
| `zeta_3` | 1.0 |  |
| `zeta_4` | 1.0 |  |
| `zeta_5` | 1. |  |
| `zeta_6` | 1.0 |  |
| `zeta_7` | 1.0 |  |
| `zeta_8` | 1.0 |  |
| `PCX1` | 1.579 | Shape factor Cfx for longitudinal force |
| `PDX1` | 1.0422 | Longitudinal friction Mux at Fznom |
| `PDX2` | -0.08285 | Variation of friction Mux with load |
| `PDX3` | 0. | Variation of friction Mux with camber |
| `PEX1` | 0.11113 | Longitudinal curvature Efx at Fznom |
| `PEX2` | 0.3143 | Variation of curvature Efx with load |
| `PEX3` | 0.0 | Variation of curvature Efx with load squared |
| `PEX4` | 0.001719 | Factor in curvature Efx while driving |
| `PKX1` | 21.687 | Longitudinal slip stiffness Kfx/Fz at Fznom |
| `PKX2` | 13.728 | Variation of slip stiffness Kfx/Fz with load |
| `PKX3` | -0.4098 | Exponent in slip stiffness Kfx/Fz with load |
| `PHX1` | 2.1615e-04 | Horizontal shift Shx at Fznom |
| `PHX2` | 0.0011598 | Variation of shift Shx with load |
| `PVX1` | 2.20283e-5 | Vertical shift Svx/Fz at Fznom |
| `PVX2` | 1.0568e-4 | Variation of shift Svx/Fz with load |
| `RBX1` | 13.046 | Slope factor for combined slip Fx reduction |
| `RBX2` | 9.718 | Variation of slope Fx reduction with kappa |
| `RBX3` | 0.0 | Influence of camber on stiffness for Fx combined |
| `RCX1` | 0.9995 | Shape factor for combined slip Fx reduction |
| `REX1` | -0.4403 | Curvature factor of combined Fx |
| `REX2` | -0.4663 | Curvature factor of combined Fx with load |
| `RHX1` | -9.968e-5 | Shift factor for combined slip Fx reduction |
| `PPX1` | -0.3485 | Linear pressure effect on slip stiffness |
| `PPX2` | 0.37824 | Quadratic pressure effect on slip stiffness |
| `PPX3` | -0.09603 | Linear pressure effect on longitudinal friction |
| `PPX4` | 0.06518 | Quadratic pressure effect on longitudinal friction |
| `PCY1` | 1.337 | Shape factor Cfy for lateral forces |
| `PDY1` | 0.8785 | Lateral friction Muy |
| `PDY2` | -0.06452 | Variation of friction Muy with load |
| `PDY3` | 0. | Variation of friction Muy with squared camber |
| `PEY1` | -0.8057 | Lateral curvature Efy at Fznom |
| `PEY2` | -0.6046 | Variation of curvature Efy with load |
| `PEY3` | 0.09854 | Zero order camber dependency of curvature Efy |
| `PEY4` | -6.697 | Variation of curvature Efy with camber |
| `PEY5` | 0.0 | Camber curvature Efc |
| `PKY1` **(changed)** | -15.324 | Maximum value of stiffness Kfy/Fznom |
| `PKY2` | 1.715 | Load at which Kfy reaches maximum value |
| `PKY3` | 0.3695 | Variation of Kfy/Fznom with camber |
| `PKY4` | 2.0005 | Peak stiffness variation with camber squared |
| `PKY5` | 0. | Lateral stiffness dependency with camber |
| `PKY6` | -0.8987 | Camber stiffness factor |
| `PKY7` | -0.23303 | Load dependency of camber stiffness factor |
| `PHY1` **(changed)** | -0.001806 | Horizontal shift Shy at Fznom |
| `PHY2` **(changed)** | 0.00352 | Variation of shift Shy with load |
| `PVY1` **(changed)** | -0.00661 | Vertical shift in Svy/Fz at Fznom |
| `PVY2` **(changed)** | 0.03592 | Variation of shift Svy/Fz with load |
| `PVY3` **(changed)** | -0.162 | Variation of shift Svy/Fz with camber |
| `PVY4` **(changed)** | -0.4864 | Variation of shift Svy/Fz with camber and load |
| `RBY1` | 10.622 | Slope factor for combined Fy reduction |
| `RBY2` | 7.82 | Variation of slope Fy reduction with alpha |
| `RBY3` | 0.002037 | Shift term for alpha in slope Fy reduction |
| `RBY4` | 0.0 | Influence of camber on stiffness of Fy combined |
| `RCY1` | 1.0587 | Shape factor for combined Fy reduction |
| `REY1` | 0.3148 | Curvature factor of combined Fy |
| `REY2` | 0.004867 | Curvature factor of combined Fy with load |
| `RHY1` | 0.009472 | Shift factor for combined Fy reduction |
| `RHY2` | 0.009754 | Shift factor for combined Fy reduction with load |
| `RVY1` | 0.05187 | Kappa induced side force Svyk/Muy*Fz at Fznom |
| `RVY2` | 4.853e-4 | Variation of Svyk/Muy*Fz with load |
| `RVY3` | 0. | Variation of Svyk/Muy*Fz with camber |
| `RVY4` **(changed)** | 94.63 | Variation of Svyk/Muy*Fz with alpha |
| `RVY5` **(changed)** | 1.8914 | Variation of Svyk/Muy*Fz with kappa |
| `RVY6` | 23.8 | Variation of Svyk/Muy*Fz with atan (kappa) |
| `PPY1` | -0.6255 | Pressure effect on cornering stiffness magnitude |
| `PPY2` | -0.06523 | Pressure effect on location of cornering stiffness peak |
| `PPY3` | -0.16666 | Linear pressure effect on lateral friction |
| `PPY4` | -0.2811 | Quadratic pressure effect on lateral friction |
| `PPY5` | 0. | Influence of inflation pressure on camber stiffness |
| `QBZ1` | 12.035 | TTrail slope factor for trail Bpt at Fznom |
| `QBZ2` | -1.33 | Variation of slope Bpt with load |
| `QBZ3` | 0.0 | Variation of slope Bpt with load squared |
| `QBZ5` | 0.176 | Variation of slope Bpt with absolute camber |
| `QBZ6` | -0.14853 | //Variation of slope Bpt with camber squared |
| `QBZ9` | 34.5 | Slope factor Br of residual torque Mzr |
| `QBZ10` | 0.0 | Slope factor Br of residual torque Mzr |
| `QCZ1` | 1.2923 | Shape factor Cpt for pneumatic trail |
| `QDZ1` | 0.09068 | Peak trail Dpt" = Dpt*(Fz/Fznom*R0) |
| `QDZ2` | -0.00565 | Variation of peak Dpt with load |
| `QDZ3` | 0.3778 | Variation of peak Dpt with camber |
| `QD4` | 0.0 | Variation of peak Dpt with camber squared |
| `QDZ6` | 0.0017015 | Peak residual torque Dmr = Dmr/(Fz*R0) |
| `QDZ7` | -0.002091 | Variation of peak factor Dmr with load |
| `QDZ8` | -0.1428 | Variation of peak factor Dmr with camber |
| `QDZ9` | 0.00915 | Variation of peak factor Dmr with camber and load |
| `QDZ10` | 0.0 | Variation of peak factor Dmr with camber squared |
| `QDZ11` | 0.0 | Variation of Dmr with camber squared and load |
| `QEZ1` | -1.7924 | Trail curvature Ept at Fznom |
| `QEZ2` | 0.8975 | Variation of curvature Ept with load |
| `QEZ3` | 0.0 | Variation of curvature Ept with load squared |
| `QEZ4` | 0.2895 | Variation of curvature Ept with sign of Alpha-t |
| `QEZ5` | -0.6786 | Variation of Ept with camber and sign Alpha-t |
| `QHZ1` | 0.0014333 | Trail horizontal shift Sht at Fznom |
| `QHZ2` | 0.0024087 | Variation of shift Sht with load |
| `QHZ3` | 0.24973 | Variation of shift Sht with camber |
| `QHZ4` | -0.21205 | Variation of shift Sht with camber and load |
| `SSZ1` | 0.00918 | Nominal value of s/R0: effect of Fx on Mz |
| `SSZ2` | 0.03869 | Variation of distance s/R0 with Fy/Fznom |
| `SSZ3` | 0.0 | Variation of distance s/R0 with camber |
| `SSZ4` | 0.0 | Variation of distance s/R0 with load and camber |
| `PPZ1` | -0.4408 | Linear pressure effect on pneumatic trail |
| `PPZ2` | 0.0 | Influence of inflation pressure on residual aligning torque |
| `sigma_kappa0` | 0.3 | Initial longitudinal relaxation length [m] |
| `sigma_alpha0` | 0.3 | Initial lateral relaxation length [m] |
| `C_Fx` | 200000.0 | Longitudinal carcass stiffness [N/m] |
| `C_Fy` | 130000.0 | Lateral carcass stiffness [N/m] |
| `C_Fgamma` | 3000.0 | Camber stiffness [N/rad] |
| `epsilon_F` | 0.05 |  |
| `sigma_min` | 0.05 | Minimum relaxation length [m] |
| `SlipRatioCap` | 3.0 |  |
| `SlipAngleCapDeg` | 80. |  |
| `kVlow0` | 8000. |  |
| `Vlow` | 2.5 | Upper bound on the relaxation length [m]. Numerical guard, not from the book: |
| `sigma_max` | 0.9 | Blend factor for smoothing the relaxation length across steps (0 = frozen, |
| `sigma_smoothing` | 0.15 | Non-lagging camber parameters |
| `p_NL1` | 2.5 |  |
| `p_NL2` | 0.8 |  |
| `p_NL3` | 0.0 |  |
| `p_NL4` | 3.0 |  |
| `p_NL5` | 1.0 |  |
| `p_NL6` | 2.0 |  |
| `p_NL7` | -2.5 |  |
| `p_NL8` | 10.0 | Gyroscopic couple |
| `c_gyr` | 0.5 | Gyroscopic coefficient |
| `epsilon_v` | 0.1 |  |
| `epsilon_x` | 0.01 |  |
| `epsilon_k` | 0.01 |  |
| `epsilon_y` | 0.01 | / |
| `kappa` | -Vsx / FMath::Max(FMath::Abs(Vcx), epsilon_v) | Book (4.E5) |
| `alpha` | -FMath::Atan2(Vcy, FMath::Abs(Vcx)) |  |
| `Fz0_prime` | LFZ0 * FZ0 | Book (4.E1) |
| `df_z` | (Fz - Fz0_prime) / Fz0_prime | Book (4.E2a) |
| `Re` | R0 |  |
| `Vsx` | Vcx - Re * Omega | Clamp kappa to the same range ComputeTireForces uses. This overload takes |
| `alpha_star` | FMath::Tan(alpha) |  |
| `Gamma_star` | FMath::Sin(Gamma) | Book (4.E4) |
| `Vc_prime` | Vc + epsilon_v | Book (4.E6a) |
| `CosPrimeAlpha` | Vcx/Vc_prime | Book (4.E6) |
| `V0` | FMath::Sqrt(9.81f * R0) |  |
| `Vs` | FMath::Sqrt(Vsx * Vsx + Vcy * Vcy) | Book (4.E7): composite friction scaling, decaying with slip speed. Disabled |
| `lambda_mu_x_star` | 1. |  |
| `lambda_mu_y_star` | 1. |  |
| `lambda_mu_x_prime` | Au * lambda_mu_x_star / (1 + (Au - 1) * lambda_mu_x_star) | Book (4.E8) |
| `lambda_mu_y_prime` | Au * lambda_mu_y_star / (1 + (Au - 1) * lambda_mu_y_star) | Book (4.E8) |
| `S_Hx` | (PHX1 + PHX2 * df_z) * LHX | Book (4.E17) |
| `Kx` | kappa + S_Hx | Book (4.E10) |
| `Cx` | PCX1 * LCX | Book (4.E11) |
| `Ux` | (PDX1 + PDX2 * df_z) * (1 + PPX3 * dpi + PPX4 * dpi * dpi) * (1 - PDX3 * Gamma * Gamma) * lambda_mu_x_star | Book (4.E13) |
| `Kxk` | Fz * (PKX1 + PKX2 * df_z) * FMath::Exp(PKX3 * df_z) * (1 + PPX1 * dpi + PPX2 * dpi * dpi) | Book (4.E15) |
| `Dx` | Ux * Fz * zeta_1 | Book (4.E12) Consider adding zeta later, check the book for more info |
| `Bx` | Kxk / (Cx * Dx + epsilon_x) | Book (4.E16) |
| `Ex` | (PEX1 + PEX2 * df_z + PEX3 * df_z * df_z) * (1.0f - PEX4 * FMath::Sign(Kx)) * LEX | Book (4.E14) |
| `US_Vx` | (PVX1 + PVX2 * df_z) *
			(FMath::Abs(Vcx)/(FMath::Abs(Vcx) + 0.1f)) * LVX * lambda_mu_x_prime * zeta_1 | Book (4.E18) |
| `S_Vx` | Fz * US_Vx | Book (4.E18) |
| `Expx` | FMath::Sin(Cx * FMath::Atan(Bx * Kx - Ex * (Bx * Kx - FMath::Atan(Bx * Kx)))) | Book (4.E9) |
| `Ux0` | Ux * Expx + US_Vx | Book (4.E9) |
| `Fx0` | Dx * Expx + S_Vx | Book (4.E9) |
| `Ky_alpha` | PKY1 * Fz0_prime * (1.f + PPY1 * dpi) * (1.0f - PKY3 * FMath::Abs(Gamma_star)) *
			FMath::Sin(PKY4 * FMath::Atan((Fz/Fz0_prime) /
				((PKY2 + PKY5 * Gamma_star * Gamma_star) * (1.f + PPY2 * dpi)))) *
			zeta_3 * LKYAL | Book (4.E25) |
| `US_Vy_gamma` | (PVY3 + PVY4 * df_z) * Gamma_star * LKZC * lambda_mu_y_prime * zeta_2 | Book (4.E28 |
| `S_Vy_gamma` | Fz * US_Vy_gamma | Book (4.E28) |
| `K_y_gamma_0` | Fz * (PKY6 + PKY7 * df_z) * (1.0f + PPY5 * dpi) * LKZC | Book (4.E30) |
| `S_Hy` | (PHY1 + PHY2 * df_z) * LHY +
			(K_y_gamma_0 * Gamma_star - S_Vy_gamma) * zeta_0 / (Ky_alpha + epsilon_k) + zeta_4 - 1. | Book (4.E27) |
| `alpha_y` | alpha_star + S_Hy | Book (4.E20) |
| `Cy` | PCY1 * LCY | Book (4.E21) |
| `Uy` | (PDY1 + PDY2 * df_z) * (1.f + PPY3 * dpi + PPY4 * dpi * dpi) * (1.0f - PDY3 * Gamma_star * Gamma_star) * lambda_mu_y_star | Book (4.E23) |
| `Dy` | Uy * Fz * zeta_2 | Book (4.E22) Check the book for the full implementation. |
| `By` | Ky_alpha / (Cy * Dy + epsilon_y) | Book (4.E26) |
| `Ey` | (PEY1 + PEY2 * df_z) * ( 1.f + PEY5 * Gamma_star * Gamma_star - (PEY3 + PEY4 * Gamma_star) * FMath::Sign(alpha_y)) * LEY | Book (4.E24) |
| `US_Vy` | (PVY1 + PVY2 * df_z) * LVY * lambda_mu_y_prime * zeta_2 | Book (4.E29) |
| `S_Vy` | Fz * US_Vy + S_Vy_gamma | Book (4.E29) |
| `Expy` | FMath::Sin(Cy * FMath::Atan(By * alpha_y - Ey * (By * alpha_y - FMath::Atan(By * alpha_y)))) | Book (4.E19) |
| `Uy0` | Uy * Expy + US_Vy | Book (4.E19) |
| `Fy0` | Dy * Expy + S_Vy | Book (4.E19) |
| `S_Ht` | QHZ1 + QHZ2 * df_z + (QHZ3 + QHZ4 * df_z) * Gamma_star | Book (4.E35) |
| `alpha_t` | alpha_star + S_Ht | Book (4.E34) |
| `Bt` | (QBZ1 + QBZ2 * df_z + QBZ3 * df_z * df_z) *
			(1 + QBZ5 * FMath::Abs(Gamma_star) + QBZ6 * Gamma_star * Gamma_star) * LKYAL/lambda_mu_y_star | Book (4.E40) |
| `Ct` | QCZ1 | Book (4.E41) |
| `Et` | (QEZ1 + QEZ2 * df_z + QEZ3 * df_z * df_z) *
			(1 + (QEZ4 + QEZ5 * Gamma_star) * (2/PI) * FMath::Atan(Bt * Ct * alpha_t)) | Book (4.E44) |
| `Dt0` | Fz * (R0/Fz0_prime) * (QDZ1 + QDZ2 * df_z) * (1.0f - PPZ1 * dpi) * LT * FMath::Sign(Vcx) | Book (4.E42) |
| `Dt` | Dt0 * (1.f + QDZ3 * FMath::Abs(Gamma_star) + QD4 * Gamma_star * Gamma_star) * zeta_5 | Book (4.E43) |
| `t0` | Dt * FMath::Cos(Ct *
			FMath::Atan(Bt * alpha_t - Et *
				(Bt * alpha_t - FMath::Atan(Bt * alpha_t)))) * CosPrimeAlpha | Book (4.E33) |
| `Br` | ((QBZ9 * LKYAL / lambda_mu_y_star) + QBZ10 * By * Cy) * zeta_6 | Book (4.E45) |
| `Cr` | zeta_7 | Book (4.E46) |
| `Ky_alpha_prime` | FMath::Max(Ky_alpha, epsilon_k) | Book (4.E39) |
| `S_Hf` | S_Hy + S_Vy / Ky_alpha_prime | Book (4.E38) |
| `alpha_r` | alpha_star + S_H | Book (4.E37) |
| `term1` | (QDZ6 + QDZ7 * df_z) * LMR * zeta_2 |  |
| `term2` | ((QDZ8 + QDZ9 * df_z) * (1.0f + PPZ2 * dpi) + (QDZ10 + QDZ11 * df_z) * FMath::Abs(Gamma_star)) * Gamma_star * LKZC * zeta_0 |  |
| `Dr` | Fz * R0 * (term1 + term2) * lambda_mu_y_star * FMath::Sign(Vcx) * CosPrimeAlpha + zeta_8 - 1.0 | Book (4.E47) |
| `Mzr0` | Dr * FMath::Cos(Cr * FMath::Atan(Br * alpha_r)) * CosPrimeAlpha | Book (4.E36) |
| `Mz0Prime` | -t0 * Fy0 | Book (4.E32) |
| `Mz0` | Mz0Prime + Mzr0 | Book (4.E31) |
| `Bx_alpha` | (RBX1 + RBX3 * Gamma_star * Gamma_star) * FMath::Cos(FMath::Atan(RBX2 * kappa)) * LXAL | Book (4.E54) |
| `Cx_alpha` | RCX1 | Book (4.E55) |
| `Ex_alpha` | REX1 + REX2 * df_z | Book (4.E56) |
| `S_Hx_alpha` | RHX1 | Book (4.E57) |
| `alpha_s` | alpha_star + S_Hx_alpha | Book (4.E53) |
| `Gx_alpha0` | FMath::Cos(Cx_alpha * FMath::Atan(Bx_alpha * S_Hx_alpha - Ex_alpha * (Bx_alpha * S_Hx_alpha - FMath::Atan(Bx_alpha * S_Hx_alpha)))) | Book (4.E52) |
| `Gx_alpha` | FMath::Cos(Cx_alpha * FMath::Atan(Bx_alpha * alpha_s - Ex_alpha * (Bx_alpha * alpha_s - FMath::Atan(Bx_alpha * alpha_s)))) / Gx_alpha0 | Book (4.E51) |
| `Fx` | Fx0 * Gx_alpha | Book (4.E50) |
| `By_kappa` | (RBY1 + RBY4 * Gamma_star * Gamma_star) * FMath::Cos(FMath::Atan(RBY2 * (alpha_star - RBY3))) * LYK | Book (4.E62) |
| `Cy_kappa` | RCY1 | Book (4.E63) |
| `Ey_kappa` | REY1 + REY2 * df_z | Book (4.E64) |
| `S_Hy_kappa` | RHY1 + RHY2 * df_z | Book (4.E65) |
| `kappa_s` | kappa + S_Hy_kappa | Book (4.E61) |
| `Gy_kappa0` | FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * S_Hy_kappa - Ey_kappa *
			(By_kappa * S_Hy_kappa - FMath::Atan(By_kappa * S_Hy_kappa)))) | Book (4.E60) |
| `Gy_kappa` | FMath::Max(FMath::Cos(Cy_kappa * FMath::Atan(By_kappa * kappa_s - Ey_kappa *
			(By_kappa * kappa_s - FMath::Atan(By_kappa * kappa_s)))) / Gy_kappa0, 0.f) | Book (4.E59) |
| `D_Vy_k` | Uy * Fz * (RVY1 + RVY2 * df_z + RVY3 * Gamma_star) *
			FMath::Cos(FMath::Atan(RVY4 * alpha_star)) * zeta_2 | Book (4.E67) |
| `S_Vy_k` | D_Vy_k * FMath::Sin(RVY5 * FMath::Atan(RVY6 * kappa)) * LVYK | Book (4.E66) |
| `Fy` | Fy0 * Gy_kappa + S_Vy_k | Book (4.E58) |
| `alpha_t_eq` | FMath::Sqrt(
			alpha_t * alpha_t + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_t) | Book (4.E77) |
| `alpha_r_eq` | FMath::Sqrt(
			alpha_r * alpha_r + FMath::Pow(Kx/Ky_alpha_prime,2) * kappa * kappa) * FMath::Sign(alpha_r) | Book (4.E78) |
| `Mzr` | Dr * FMath::Cos(Cr * FMath::Atan(Br * alpha_r_eq)) * CosPrimeAlpha | Book (4.E75) |
| `s` | R0 * (SSZ1 + SSZ2 * (Fy/Fz0_prime) + (SSZ3 + SSZ4 * df_z) * Gamma_star) * LS | Book (4.E76) |
| `t` | Dt * FMath::Cos(Ct *
			FMath::Atan(Bt * alpha_t_eq - Et *
				(Bt * alpha_t_eq - FMath::Atan(Bt * alpha_t_eq)))) * CosPrimeAlpha | Book (4.E73) |
| `Mz_prime` | -t * Fy_prime | Book (4.E72) |
| `Mz` | Mz_prime + Mzr + s * Fx | Book (4.E71) |
| `Sigma` | (FMath::IsFinite(RawSigma) && RawSigma > KINDA_SMALL_NUMBER)
			? RawSigma
			: Fallback |  |
| `k_plus` | IN_kappa_prime + dk |  |
| `k_minus` | IN_kappa_prime - dk |  |
| `slipRatio` | k_plus |  |
| `slipAngle` | 0 | Compute forces at k_plus |
| `Fx_plus` | rPlus.Fx / 100.0 | convert back to N (your ComputeTireForces multiplies by 100) |
| `Fx_minus` | rMinus.Fx / 100.0 |  |
| `slipRatio` | 0 |  |
| `slipAngle` | IN_alpha_prime + da |  |
| `Fy_plus` | rPlus.Fy / 100.0 |  |
| `Fy_minus` | rMinus.Fy / 100.0 |  |
