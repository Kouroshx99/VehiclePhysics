# Vehicle tuning parameters

Every number that defines a car in this plugin, where it lives, and what the two
worked setups use. Written so a car can be tuned from the editor without reading the
source, and so a new setup has something to diverge from deliberately rather than by
omission.

Four vehicles are defined in C++ and are the reference points throughout:

| | `AReferenceCar` | `ASportsCar` | `ARallyCar` | `AOffroadCar` |
|---|---|---|---|---|
| Character | mid-size saloon | Supra-proportioned coupe | gravel rally car | heavy 4x4 |
| Sprung mass | 1500 kg | 1350 kg | 1300 kg | 2200 kg |
| Track / wheelbase | 1.55 / 2.65 m | 1.60 / 2.50 m | 1.60 / 2.55 m | 1.75 / 3.00 m |
| Drive | (unset — see below) | rear | all four | all four |
| Wheel radius | 33 cm | 33 cm | 33 cm | **42 cm** |
| Travel at the wheel | 16.0 cm | 16.5 cm | 26.6 cm | **37.3 cm** |
| Roll gradient | 4.44 deg/g | 2.16 | 5.15 | **8.35** |

**They exist to disagree with each other.** A tyre model that only ever sees one setup is
not being tested — every coefficient can be quietly wrong in a way that one car happens to
hide. These four span 1300–2200 kg, 1.05–1.80 Hz of ride frequency, 2.16–8.35 deg/g of
roll, 16–37 cm of travel, two tyre radii and four drivetrain layouts. A model that stays
plausible across all of that is telling you something about the MODEL.

**Blueprints created *from* a C++ class inherit its values. Blueprints that already
exist do not** — they stored the old values when they were saved, and a stored value
beats a changed class default. If a number below does not match what the editor shows,
that is why: make a new Blueprint from the class, or right-click the property and
Reset to Default.

---

## 1. Suspension — per corner

`UWishboneSuspension`, four components. Applied via `ApplySetup(FWishboneCornerSetup)`.

| Property | Saloon F / R | Sports F / R | Rally F / R | Offroad F / R |
|---|---|---|---|---|
| `SpringRateNPerM` | 62166 / 82464 | 86167 / 140843 | 64001 / 69604 | 65610 / 80828 |
| `DamperRateNsPerM` | 5541 / 6805 | 7200 / 10461 | 7522 / 7334 | 8950 / 10068 |
| `AntiRollRateNPerM` | 14000 / 2000 | 17510 / **0** | 4729 / **0** | 7465 / **0** |
| `StaticCamberDeg` | −1.0 / −0.8 | −2.2 / −1.6 | −1.8 / −1.3 | −0.5 / −0.3 |
| `StaticToeDeg` | +0.08 / +0.12 | +0.05 / +0.15 | **−0.10** / +0.20 | +0.15 / +0.20 |
| `UnsprungMassKg` | 40 | 38 | 45 | 62 |
| `WheelRadiusCm` | 33 | 33 | 33 | **42** |
| `Min/MaxArticulationAngleDeg` | ∓12 | ∓9 | **∓20** | **∓28** |
| `PreloadForceN` | 5945 / 6328 | 5083 / 6015 | 5797 / 4804 | 9109 / 8869 |

Spring rates are **at the spring**, not the wheel; damper likewise. Anti-roll is quoted at
the wheel. Positive toe is toe-in, so the rally car's front is the only **toe-out** setup
here — straight-line calm traded for turn-in, because on a stage the corner is always
arriving. `WheelRadiusCm` **must match the tyre asset's `R0`**, and the offroad car is the
only one where that is not 0.33.

### Where the rates come from

Not typed. Pick a ride frequency and a damping ratio, and the rest is arithmetic:

```
corner mass      m  = M · (axle fraction) / 2
wheel rate       k  = m (2πf)²
damper at wheel  c  = 2ζ √(k m)
at the spring       divide both by MR²        MR = 0.616 front, 0.584 rear
roll stiffness   K  = 0.5 k t²                per axle, springs and bar alike
roll gradient    θ  = M g h / K_total         falls out; it is not chosen
```

| | saloon target | saloon measured | sports target | sports measured |
|---|---|---|---|---|
| Ride frequency | 1.25 / 1.35 Hz | 1.262 / 1.379 | 1.60 / 1.80 Hz | 1.562 / 1.893 |
| Damping ratio ζ | 0.35 | 0.353 / 0.357 | 0.42 | 0.410 / 0.442 |
| Roll gradient | 5.05 deg/g | 4.306 | 2.49 deg/g | 2.163 |
| Front roll-stiffness share | 56.2% | 55.5% | 52.0% | 51.1% |
| Motion ratio | 0.610 / 0.570 | 0.616 / 0.584 | 0.610 / 0.570 | 0.614 / 0.582 |

Both roll gradients come in under target because both were derived assuming the centre of
mass sat 0.40 m above the roll axis, and the real collision puts it lower. The cars roll
**less** than the derivation intended — the sports car in particular is very flat, and
softening the front bar is the lever if that is not wanted.

**The springs alone put the roll stiffness rearward** on both cars — the rear carries
more mass at a higher frequency and both raise its rate. The front bar is what makes
the balance a choice, not an afterthought.

**Change the hardpoints and every rate above stops being right**, because the motion
ratio moves and the frequencies move with it.

### Preload, which is not optional

The coil is squeezed by exactly the preload at the design position, so the spring force
there **is** `PreloadForceN`. Leave it at zero and the spring holds nothing at design
height: the corner sinks until it generates its own load.

```
sag with zero preload  =  corner load / WHEEL rate
preload that cancels it =  corner load / MOTION ratio
```

The first of those was confirmed against a settled bench run to three decimals. At zero
preload the saloon's front sags **15.5 cm** against roughly 8 cm of bump — a car sitting
on its bump stops with no travel left.

| | saloon F / R | sports F / R |
|---|---|---|
| Sag at zero preload | 15.5 / 13.1 cm | 9.6 / 7.3 cm |
| `PreloadForceN` | 5945 / 6328 | 5083 / 6015 |

**This matters beyond ride height.** Every geometry figure in this document and in the
bench report — roll centre, camber curve, motion ratio, KPI — is quoted at the design
position. A car resting anywhere else is running geometry nobody specified, and it shows
up as static camber not matching `StaticCamberDeg`.

### Droop travel against static deflection

A consequence of preload worth knowing, because it is invisible until a wheel does not
lift when you expect it to.

Preload deflection is how far the coil is squeezed at design — `PreloadForceN / rate`.
Droop travel is how far the strut can extend before the arm hits its stop. If the first
exceeds the second, **the spring can never go slack and the wheel can never fully
unload**.

| | preload deflection | droop at the strut | residual at full droop |
|---|---|---|---|
| saloon F | 9.56 cm | 4.92 cm | **1777 N at the wheel** |
| saloon R | 7.67 cm | 4.64 cm | 1463 N |
| sports F | 5.90 cm | 5.07 cm | 441 N |
| sports R | 4.27 cm | 4.77 cm | goes slack 0.5 cm early |

The saloon's front spring is still compressed 4.6 cm at full droop, pushing half its
static load, because its static deflection at the wheel (15.5 cm) is nearly twice the
droop travel available (8 cm). The sports car's rear is the opposite — the coil leaves
its perch before the stop, so that wheel unloads completely.

That is a real difference in how the two cars behave over a crest, and it was chosen by
nobody: it falls out of soft springs plus ±12° of articulation. **The levers are more
articulation or a stiffer spring**, and both trade against rates that are already tuned.
Nothing misbehaves either way — the spring force is clamped at zero (`FMath::Max`) so a
slack coil correctly carries nothing, and bump stops catch both ends
(`BumpStopRateNPerM` 250 kN/m, engaging 2° from each limit).

### Verifying it

Run the bench's Kinematics test. Three lines tell you whether the car sits where it was
designed to:

```
FR rest offset from design    0.009 cm     want zero
FR preload correction            2 N       ADD this to PreloadForceN
FR static camber             -1.001 deg    should equal StaticCamberDeg
```

Static camber matching what you configured is the single best check that everything
upstream is consistent, because it can only happen when the car is resting at design.

**Measure it after the car has settled.** Sampled at bench start those figures are the
car mid-drop — the saloon front reads about 2 cm before settling and 15 cm after. The
bench now waits for the ride height to stop moving and reports which it got.

---

## 2. Rotating inertias

`FShaft::Inertia`, kg·m². **Every one of these defaults to 1.0**, which is the inertia
of a road wheel — so until set, an engine, a gearbox shaft and a differential each
carry a wheel's worth of rotating mass.

| Shaft | Where | Saloon | Sports | Rally | Offroad |
|---|---|---|---|---|---|
| Wheel ×4 | `UPacejkaTyreComponent::WheelShaft` | 1.57 | 1.44 | 1.65 | **3.30** |
| Engine | `UVehicleEngine::Shaft` | 0.22 | 0.18 | 0.20 | 0.32 |
| Front diff | `ACar::FrontDiffShaft` | 0.05 | 0.05 | 0.06 | 0.09 |
| Rear diff | `ACar::RearDiffShaft` | 0.05 | 0.05 | 0.06 | 0.09 |
| Transmission | `UDrivetrain::TransmissionShaft` | 0.03 | 0.03 | 0.04 | 0.06 |
| Clutch | `ACar::ClutchShaft` | 0.20 | 0.20 | 0.20 | 0.20 |

The offroad car's wheel inertia is double the saloon's — a 42 cm mud-terrain tyre — while
its cornering stiffness is the lowest here. Those push the wheel-spin number G in opposite
directions, so read that block of the bench rather than assuming it lands where the others
do.

**Wheel inertia is the denominator of the wheel-spin stability number.**

```
G = C · (dκ/dΩ) · R₀ · dt / I
```

Running 1.0 where the physics wants 1.57 inflates G by 57% on every corner. The
implicit treatment in `UPacejkaTyreComponent` absorbs it, so this never diverged — it
only meant the stability margin was smaller than the analysis claimed.

`ClutchShaft` is left at 0.20 on both. That is high for a clutch disc and input shaft —
0.02 would be nearer — but it was chosen rather than defaulted, and the anti-stall and
engagement timing were tuned against it.

---

## 3. Engine

`UVehicleEngine`.

| Property | Saloon | Sports | Rally | Offroad |
|---|---|---|---|---|
| `MaxEngineTorque` | 250 N·m | 430 N·m | 420 N·m | 560 N·m |
| `MaxRPM` | 8000 | 7000 | 7000 | 5200 |
| `IdleRPM` | 800 | 800 | 900 | 700 |
| `TorqueCurve` | asset | — | — | — |
| `InlineTorqueCurve` | — | 13 keys | 13 keys | 13 keys |
| `RedLineRPMRatio` | 0.95 | 0.95 | 0.95 | 0.95 |

**The curve SHAPE is the character, not the peak.** The rally car's turbo four is 0.32 at
idle and 1.00 by 3600 — a step, not a ramp, which is why it is driven on the throttle
rather than with it. The offroad car is the exact opposite: 0.72 at 1000 rpm, 96% of peak
by 2000, finished by 5000. A curve with no powerband, because the whole range is the
powerband — what a vehicle needs when it is asked for torque at walking pace against an
obstacle.

```
Torque = curve(rpm) · MaxEngineTorque · throttle
```

so `MaxEngineTorque` scales the curve and does not reshape it.

**The curve is looked up by absolute rpm, not normalised.** `MaxRPM` is therefore where
the limiter cuts, *not* where the curve ends — lowering it under a curve authored to
8000 quietly stops the engine reaching the top of its own powerband. Move one only if
you own the other.

`TorqueCurve` (an asset) wins over `InlineTorqueCurve` (by value) when both are set. The
inline one exists because a `UCurveFloat` created in a constructor cannot be referenced
by a saved Blueprint — it lives in the class's package, and the editor refuses to save
the reference and then crashes.

### The sports car's curve

Big-turbo straight six. Normalised.

| rpm | 1000 | 1500 | 2000 | 2500 | 3000 | 3600 | 4200 | 4800 | 5400 | 6000 | 6600 | 7000 | 7400 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| | 0.50 | 0.66 | 0.79 | 0.90 | 0.97 | **1.00** | **1.00** | 0.96 | 0.90 | 0.82 | 0.71 | 0.62 | 0.50 |

297 hp at 6000 rpm, 220 hp/tonne. Authored past the 7000 limiter so the lookup at the
cut is defined rather than running off the end of the data.

---

## 4. Drivetrain

`UDrivetrain`.

| Property | Saloon | Sports | Rally | Offroad |
|---|---|---|---|---|
| `DrivetrainType` | **`EDT_FrontWheelDrive`** (class default) | `EDT_RearWheelDrive` | `EDT_AllWheelDrive` | `EDT_AllWheelDrive` |
| `FinalDriveRatio` | 3.15 | 3.13 | 4.30 | 4.10 |
| `PreloadTorque` | 50 | 80 | 120 | 200 |
| `PowerLockCoeff` | 0.35 | 0.50 | 0.60 | 0.75 |
| `CoastLockCoeff` | 0.15 | 0.25 | 0.40 | 0.50 |
| `ClutchMaxTorque` | 400 | 400 | 500 | 700 |
| `SolverIterations` | 10 | 10 | 10 | 10 |

Gear ratios, index 0 being **reverse and necessarily negative**:

| | ratios | spread |
|---|---|---|
| Saloon | −3.71, 5.25, 3.36, 2.17, 1.72, 1.32, 1.00, 0.82, 0.64 | 8.2 |
| Sports | −3.90, 3.562, 2.457, 1.742, 1.272, 0.956, 0.747 | 4.48 |
| Rally | −3.20, 2.790, 2.180, 1.760, 1.450, 1.210, 1.010 | **2.76** |
| Offroad | −4.50, 4.200, 2.560, 1.720, 1.240, 0.950, 0.740 | **5.68** |

The rally and offroad boxes are opposite answers to the same shortage of grip. The rally
car's spread is the tightest here because its engine has a narrow plateau that has to be
kept inside; the offroad car's is the widest because its engine has a plateau everywhere
and needs RANGE instead — a walking-pace first gear at one end and something usable on a
road at the other.

**The offroad final drive is doing more than 4.10 suggests.** Final drive is geared
against rolling radius, and that car's is 42 cm rather than 33 — so 4.10 there pulls like
about 3.22 would on the others. A bigger tyre is a gearing change whether or not anyone
treats it as one.

**`DrivetrainType` defaults to front-wheel drive.** A rear-drive car that does not set
it is a front-drive car wearing rear-drive geometry.

**Index 0 of `GearRatios` is reverse and must be negative.** The ratio goes straight
into a gear constraint, so its sign is the direction of travel; a positive value there
is one more forward gear wearing reverse's label.

### Where the ratios come from

Two ends and a rule for the middle:

```
top gear     from top speed:  total = ω_engine / (v / r)
first gear   from traction:   total = k · μ · N_driven · r / T_peak     k ≈ 2.3
middle       tapered geometric — 1.45, 1.41, 1.37, 1.33, 1.28
```

A *constant* step is the textbook answer and no gearbox uses it: low gears want to be
far apart because there is torque to spare, high ones close together because there is
not. The sports car's spread is 4.48 against the real V160's 4.83.

`CoastLockCoeff` below `PowerLockCoeff` deliberately — a differential that locks hard
off-throttle drags the car straight on entry.

---

## 5. Tyre

`UTireParamsDataAsset`, 179 parameters. `UTireParamsSportsCar` is a tuned subclass; make
an asset from it with right-click → Miscellaneous → Data Asset.

Twelve carry the difference between a touring tyre and a performance one. The rest are
left alone deliberately — the shape of a tyre curve is largely conserved across
compounds, and changing coefficients whose effect you cannot predict is how a parameter
set stops describing a tyre.

| Parameter | Saloon | Sports | What it is |
|---|---|---|---|
| `PDY1` / `PDX1` | 0.8785 / 1.0422 | 1.15 / 1.30 | peak friction, lateral / longitudinal |
| `PDY2` / `PDX2` | −0.0645 / −0.0829 | −0.075 / −0.095 | how peak falls with load |
| `PKY1` | −15.324 *(header)* | **+19.5** | cornering stiffness — **sign matters, see below** |
| `PKX1` / `PKX2` | 21.687 / 13.728 | 27.0 / 15.0 | longitudinal slip stiffness |
| `PCY1` / `PCX1` | 1.337 / 1.579 | 1.45 / 1.62 | shape — peak sharpness |
| `PEY1` / `PEX1` | −0.8057 / 0.1111 | −0.65 / 0.12 | curvature |
| `FZ0` | 2500 N | 3300 N | nominal load |
| `R0` | 0.32 m | 0.33 m | unloaded radius |

### Presets

`TireParamsPresets.h` carries four more, alongside `UTireParamsSportsCar`. All derive
from the base defaults — which are `TP_GenericSedan`'s driven values, a touring tyre —
so each states only what makes it different.

| | PDY1 | PDX1 | PDY2 | PKY1 | PKX1 | PCY1 | PEY1 |
|---|---|---|---|---|---|---|---|
| Touring *(base)* | 0.878 | 1.042 | −0.0645 | 15.32 | 21.69 | 1.34 | −0.81 |
| Performance summer | 1.150 | 1.300 | −0.0750 | 19.50 | 27.00 | 1.45 | −0.65 |
| Track semi-slick | 1.350 | 1.500 | −0.0850 | 23.00 | 31.00 | 1.52 | −0.55 |
| Wet | 0.650 | 0.720 | −0.0700 | 14.00 | 19.50 | 1.30 | −0.85 |
| Gravel | 0.600 | 0.650 | −0.0450 | 9.50 | 13.00 | 1.20 | −1.10 |
| Offroad *(R0 0.42)* | 0.700 | 0.780 | −0.0400 | 8.00 | 11.00 | 1.15 | −1.20 |

Peaks are taken from published ranges: street/touring 0.7–0.9 dry, performance summer
1.0–1.2, semi-slick 1.2–1.4, racing slick 1.4–1.8 (GT measures 1.30–1.35, F1 1.55–1.60),
wet 0.4–0.8, gravel ≈0.60.

**The offroad preset sits ABOVE gravel at 0.70**, which reads backwards until the
mechanism does. A block tread on loose ground digs rather than slides, and part of what
resists it is the shear strength of material piling up in front of each block. What it
gives away is stiffness — `PKY1` 8.0 against a road tyre's 15.3, barely half — and
warning, since no carcass is stiff enough to report what a tread block is doing.

It is also **the only preset that changes `R0`**, to 0.42, and the corner's
`WheelRadiusCm` has to match it. A bigger tyre is not a cosmetic choice: it clears
obstacles, it lowers effective gearing, and it drops the contact patch 9 cm further from
the wheel centre — which moves scrub radius and roll centre height whether or not anyone
intended it. On `AOffroadCar` that takes scrub *negative*, −12.6 mm front.

**Read down the columns, not across the rows.** Friction and stiffness rise together
for dry compounds, `PCY1` rises with them so grip costs warning — and **gravel breaks
the pattern on every count**, because it grips by shearing loose material rather than
by adhesion. That is also why it is the *least* load-sensitive of the five.

`FZ0` is left at the base 3000 N in all three presets. It is the load they are quoted
*at*, so it belongs to the car the tyre is fitted to, not to the compound.

> **These are not fitted parameter sets.** A real one comes off a flat-track rig with
> 179 coefficients that agree with each other. These are the baseline with a dozen
> moved to land the right peak, stiffness and shape. They behave plausibly; they are
> not measurements. Where a number matters more than plausibility, a measured `.tir`
> file should replace them.

**Load sensitivity goes *more* negative on the sports tyre, and that is not a slip.**
Stickier compounds lose more peak with load, because the mechanism that makes them grip
is the one that saturates. It is why a sports car is more sensitive to load transfer
despite having more grip.

**`R0` must match the corner's `WheelRadiusCm`.** A mismatch is a silent error in both
Fz and slip ratio; the base default of 0.32 against a 33 cm wheel is 3% on both.

**`FZ0` should sit near the operating load.** Every load-sensitivity term is relative to
it, so a nominal 25% under the corner's actual load puts every friction term on the
wrong part of its own curve before anything is tuned.

**`PKY1`'s SIGN sets which way the car turns.** In the Magic Formula `B = Kya/(C·D)`
and `Kya` comes from `PKY1`, so a negative value gives Fy a negative slope through the
origin — a positive slip angle then produces force the wrong way and the car steers
into the corner backwards. Plotted against Fx it is unmistakable: the two curves cross
the origin with opposite slopes.

**The header's defaults are not the working parameter set.** `UTireParamsSportsCar`
derives from `UTireParamsDataAsset`, so its baseline is the *header's* values — not
`TP_GenericSedan`, which is what has actually been driven. Wherever the asset differs
from the header, the subclass silently inherits the header's value, and `PKY1`'s sign
was one such difference. It announced itself only because an inverted tyre is obvious;
a merely *mistuned* parameter would not.

> **Build the next tyre by duplicating `TP_GenericSedan` and applying the twelve
> changes above to the copy.** That starts from a known-good baseline instead of an
> unverified one. The subclass is still worth keeping as the record of *which* twelve
> and why — which a duplicated asset cannot carry.

**Raising `PKX1` raises G proportionally** — see the inertia section. The sports tyre's
+25% and its +44% wheel inertia very nearly cancel, by coincidence.

---

## 6. Body — mass, inertia and collision

Not on any component. `ACar` takes **mass, centre of mass and rotational inertia** from
the skeletal mesh's physics asset, and none of the three is set anywhere in this plugin.
It is the largest single lever on how a car behaves and the easiest one to leave wrong.

| | Saloon | Sports | Rally | Offroad |
|---|---|---|---|---|
| Mass | 1500 kg | 1350 kg | 1300 kg | 2200 kg |
| Physics asset | `SK_SaloonBody_PhysicsAsset` | `SK_SportsBody_PhysicsAsset` | *none yet* | *none yet* |
| Collision hulls | 22 | 21 | — | — |
| CoG height | 48.951 cm | 43.076 cm | — | — |
| Roll `Ixx` | 485.8 kg·m² | 395.2 | — | — |
| Pitch `Iyy` | 2233.1 kg·m² | 1828.4 | — | — |
| Yaw `Izz` | 2471.7 kg·m² | 2054.1 | — | — |
| Ground clearance | 15.4 cm | 12.9 cm | — | — |

**The rally and offroad cars have no hulls of their own yet**, so a Blueprint made from
either inherits whatever physics asset it points at. A saloon's hulls under a 2200 kg 4x4
read a CoG far too low and inertias barely half what they should be. Check the bench's
sprung figure before trusting a single frequency for those two.

**Mass:** a setup derived for one mass and run at another is wrong by the square root of
the ratio in every frequency and proportionally in the roll gradient. The sports car ran
at 477 kg against its intended 1350 for a while: every inertia read 0.3533 of target,
which is 477/1350, and eight metrics were out of band from that one number.

**Inertia is the part with no fallback.** Springs and dampers set how fast the body
returns; inertia sets how hard it is to start moving at all, and no damper value
compensates for getting it wrong. A single box the size of a chassis — which is what both
cars had — carries roughly **40% of a real car's inertia on all three axes**. It rotates
too eagerly in yaw and snaps in roll and pitch, and it is invisible in every other metric.

### How the collision is built

Not by hand. `Art/Scripts/shapes_saloon.py` and `shapes_sports.py` define the hulls as
point clouds with exact convex-hull mass properties by tetrahedron integration, so the
inertias are known before anything is exported. `build_saloon.py` and `build_sports.py`
turn them into the two FBX files each car ships:

| file | import as | purpose |
|---|---|---|
| `SK_*Body.fbx` | Skeletal | the visual, bound rigidly to a `Chassis` bone |
| `SM_*Body.fbx` | Static | holds the UCX hulls; the source Copy Collision reads |

Import the static mesh with **Auto Generate Collision OFF** and **One Convex Hull Per UCX
ON** — off merges all the hulls into one, the cabin stops being empty, and the whole point
is lost. Then in the physics asset: right-click the Chassis body → **Advanced → Copy
Collision From StaticMesh**, and delete the existing primitives first.

Three things the hull set does that a box cannot:

- **The cabin is empty**, and that is the mechanism rather than a detail. One density
  serves the whole body, so no volume in the cabin means no mass there — which is what
  pulls the centre of mass down without any offset being dialled in by hand.
- **The floorpan carries approach and departure ramps.** A flat slab sits at one height
  the whole length while the bodywork above it climbs, hanging below the car at both ends
  and halving both angles. Saloon: 10.9° → 21.7° approach, 10.9° → 22.4° departure.
- **The wheels are discs on the wheel axis.** `UnsprungMassKg` drives a separate 1-DOF
  wheel model that cannot contribute rotational inertia to anything, so four 40 kg wheels
  are worth roughly 377 kg·m² of yaw that nothing else accounts for.

Each hull carries a `part` property naming what it represents. That is the welded batch
if per-part density is ever wanted — `USkinnedMeshComponent::PhysicsAssetOverride` lets
two cars differ without duplicating the mesh, and welded bodies sum their mass properties
with no constraints. **Constraints would be wrong here:** they make the chassis an
articulated assembly rather than one rigid body, and `ACar` applies every wheel force to a
single body.

### Sanity numbers

Absolute inertias only mean something against a car of the same mass and size, so the
bench also reports two dimensionless figures that travel between vehicles:

| | saloon | sports | reading |
|---|---|---|---|
| Dynamic index `Iyy/(m·a·b)` | 0.848 | 0.870 | 1.0 decouples front and rear suspension |
| Yaw gyration / wheelbase | 0.484 | 0.493 | passenger cars 0.45–0.50 |

`m·a·b` is the pitch inertia of a car whose mass sits entirely on its axles, so the
dynamic index measures yours against that. At exactly 1.0 a bump at one axle produces no
motion at the other. Below it, as both cars are, the ends still talk to each other — which
is why the flat-ride split (rear frequency above front) is doing real work.

---

## 7. Ground sampling — per corner

Not vehicle character, but it decides what the tyre model is handed. Defaults suit both
cars; see `WishboneSuspension.h` for the reasoning on each.

| Property | Default | Note |
|---|---|---|
| `GroundSamplePattern` | Cross | Single disables the plane fit entirely |
| `GroundSampleHalfLengthCm` | 10 | enveloping reach — see below |
| `GroundSampleHalfWidthCm` | 10 | **is** half the tyre's section width |
| `GroundEnvelopingBias` | 1.0 | 1 rests on the highest sample, 0 on the mean |
| `MaxDrivableSlopeDeg` | 55 | rejects samples twice — per sample and on the fit |
| `bSampleGroundPerSubstep` | true | off is 0.93 m of stale ground at 200 km/h |
| `bUseSweptContact` | false | sphere probes; adds rounding and recovery |
| `bRejectBridgedSamples` | false | drops samples the tyre is spanning |

A tyre of radius *R* first touches a step of height *h* when its centre is
`√(R² − (R−h)²)` short of the edge — 17.5 cm for a 5 cm step at R = 33. At the default
10 cm half-length the plane starts rising later than the real tyre would, so the wheel
climbs late and hard. Around 20 cm is closer to the truth.
