# VehiclePhysics

A vehicle dynamics simulation plugin for Unreal Engine, written from scratch — not a
wrapper around Chaos Vehicles.

Every input is a physical quantity you can look up or measure: mass and inertia, centre
of gravity height, spring and damper rates in N/m and Ns/m, anti-roll stiffness, gear
and final drive ratios, engine torque curves, tyre coefficients. Setting up a car is
meant to be a research task rather than a tuning task — find the real numbers and enter
them.

It ships with the test bench that measures the result, so the behaviour a given set of
parameters produces can be checked rather than asserted.

Supports **UE 4.27 and UE 5.x from one set of sources**. Win64.

---

## Contents

- [Installing](#installing)
- [Quick start](#quick-start)
- [Building a car](#building-a-car)
- [What is modelled](#what-is-modelled)
- [The test bench](#the-test-bench)
- [The test circuit generator](#the-test-circuit-generator)
- [Switching cars at runtime](#switching-cars-at-runtime)
- [Units](#units)
- [Physics settings](#physics-settings)
- [Documentation](#documentation)
- [Engine version support](#engine-version-support)
- [Licence and citation](#licence-and-citation)

---

## Installing

Drop the `VehiclePhysics` folder into your project's `Plugins/` directory and rebuild.
The project must be a C++ project — a Blueprint-only project has no build step to
compile the plugin into.

```
YourProject/
  Plugins/
    VehiclePhysics/
      Source/
      Content/
      ...
```

Input bindings come with the plugin. `Config/DefaultInput.ini` layers into your
project's input settings and **adds** to whatever you already have — it cannot
overwrite your existing bindings, and they cannot overwrite it.

| Action | Keyboard | Gamepad |
|---|---|---|
| Throttle | `W` | Right trigger |
| Brake | `S` | Left trigger |
| Steering | `A` / `D` | Left stick X |
| Gear up / down | `E` / `Q` | Face button right / left |
| Reset car | `R` | D-Pad down |
| Next / previous car | `N` / `B` | D-Pad right / left |

There is no collision detection on these. If your project already binds `N`, both
actions will fire — rebind whichever you prefer in Project Settings.

## Quick start

The plugin ships an example map at `Content/VehiclePhysics/Maps/Test`, and four ready-made
cars under `Content/VehiclePhysics/Vehicles/` — `BP_Saloon`, `BP_SportCar`, `BP_Rally` and
`BP_Offroad`.

If you are evaluating the simulation rather than the example content, [Building a
car](#building-a-car) is the more useful path — it takes about ten minutes and exercises
the parts that matter.

## Building a car

**The C++ car classes carry no assets.** `ASaloonCar`, `ASportsCar`, `ARallyCar` and
`AOffroadCar` hold the physical setup — masses, spring rates, geometry, gear ratios —
but no skeletal mesh. They exist to be subclassed. A car with no mesh has no physics
body and will log an error and refuse to simulate.

So the workflow is:

1. Create a Blueprint deriving from one of the four car classes (or from `ACar` directly).
2. Assign a skeletal mesh with a **physics asset**. Without one there is nothing for the
   suspension to push against.
3. Set the four wheel colliders and tyre components on their bones.
4. Adjust the setup values to match the vehicle you are modelling.

`VehicleTuning.md` covers what each parameter means and where to find it for a real car.

## What is modelled

**Tyres** — Pacejka Magic Formula 6.1, including combined slip, transient relaxation,
inflation-pressure dependency and turn slip,
so longitudinal and lateral demands interact as they do on a real contact patch. Four
presets ship: `TrackDay`, `Wet`, `Gravel`, `Offroad`.

**Suspension** — double wishbone described by its actual hardpoints: wishbone pivots,
damper mount, coilover top, wheel centre. The motion ratio between spring travel and
wheel travel is *computed from that geometry* rather than authored, so moving the damper
mount changes the wheel rate as it does on a real car.

**Unsprung mass** — genuinely simulated, not a coefficient. Each wheel carries its own
inertia along the suspension arc, and its acceleration produces an inertial reaction on
the contact patch. Wheel hop and tyre load fluctuation over bumps emerge from that.

**Driveline** — a sequential-impulse constraint solver over gearing, differentials,
limited-slip behaviour and brakes, rather than scripted torque splitting. Behaviour such
as an unloaded inner wheel dragging the axle emerges from the model instead of being
special-cased.

**Engine** — torque curve to torque to RPM, with a rev limiter and engine braking.

## The test bench

`AVehicleTestBench` drives a vehicle through an automated suite and reports measured
results. Place one in a level and point it at a car, or drive it from the command line.

| `-benchtest=` | What it does |
|---|---|
| `Kinematics` | Static report — masses, inertias, CoG and roll-centre heights, camber, roll gradient, roll-stiffness split. No driving, no ground needed |
| `Skidpad` | Fixed steer, slow speed ramp. Understeer gradient by the ISO 4138 constant-steer method, roll gradient, peak lateral grip |
| `StepSteer` | Straight run, then a steer step. Yaw response time and overshoot |
| `Articulation` | One wheel over a block. What the anti-roll bar does to the diagonal load |
| `StraightLine` | Full throttle to target, then full brake. Pitch, traction, stopping distance |
| `PowerOnCornering` | Fixed steer on a circle, then throttle. What power does to the balance |

```bash
# UE 4.27 - UE4Editor-Cmd.exe;  UE 5.x - UnrealEditor-Cmd.exe
UnrealEditor-Cmd.exe YourProject.uproject /Game/Maps/BenchMap -game -runbench -benchtest=Skidpad -usefixedtimestep -fps=60 -log
```

**`-usefixedtimestep -fps=60` is not optional for a headless run.** Without it the world
ticks as fast as the machine allows. The vehicle substeps internally and copes, but the
bench's own speed and steering controllers run per frame, and lateral acceleration is
differentiated from velocity — so a twentieth of the timestep is twenty times the
numerical noise. A car whose tyre peaks at 1.15 g has been measured at 2.331 g this way.
A number twice its true value with no warning attached is worse than a crash.

Other flags: `-benchspawn=<class path>` replaces the level's car, `-benchtarget=<name>`
picks one when several are present.

## The test circuit generator

`AVehicleTestCircuit` builds complete test courses procedurally — rally, asphalt circuit
and offroad — with kerbs, banking, edge ramps and optional landscape sculpting. Offroad
courses include stations for the geometric limits:

- **Grade test** — ramps steepening 15° to 35° in 5° steps. The top of that range sits at
  the traction ceiling: on four driven wheels the limit is `atan(mu)`, about 35° for the
  offroad tyre.
- **Breakover ridge** — crests steepening 8° to 20°. A symmetric ridge is cleared while
  `tan(theta) <= 2h/L` for clearance `h` and wheelbase `L`, so these bracket where the
  belly grounds out.

## Switching cars at runtime

Place a **Vehicle Roster** actor in a level to enable car switching there. Press `N` /
`B` — or D-Pad right / left — to cycle through its list, replacing the car in place, same
position and heading, so you can drive to a corner and try it again in something else.

It arrives preloaded with the four cars the plugin ships, so it works the moment you drop
it in. Edit its `Cars` array to reorder it, trim it, or point it at your own Blueprints.
They want to be Blueprints: the C++ car classes carry the simulation but assign no mesh,
and a car with no mesh has no physics body.

**Switching is deliberately per-level.** A level with no roster actor has no switching,
and nothing is logged about it — installing this plugin does not change how any of your
existing levels behave, and there is no global setting to discover or forget.

## Units

Two domains, with `UWheelCollider` as the boundary:

- **Unreal domain** — distance cm, velocity cm/s, force in Unreal force units
  (UU = kg·cm/s², so **1 N = 100 UU**). Suspension maths stays here.
- **SI domain** — the tyre model and shafts work in N, m, m/s, rad, kg·m².

Conversions into the tyre model are explicit. Conversions out are asymmetric: the tyre
model returns `Fx`/`Fy` already scaled to UU but `Mz` in N·m and pneumatic trail in m.

## Physics settings

The plugin works with default settings. It runs the solve from the engine's physics
callback on both engines, and with no fixed timestep configured it integrates once per
frame at the frame delta.

For **reproducible, frame-rate-independent results** — which you want for any measurement
— give the solver a fixed step:

- **UE 5.x** — Project Settings → Physics → `Tick Physics Async` on, with
  `Async Fixed Time Step Size` at `0.004166667` (240 Hz).
- **UE 4.27** — Project Settings → Physics → `Substepping` on, with a max substep delta
  around `0.004166667`.

The plugin does not change these for you: they are global physics settings affecting
every physics actor in your project, not just vehicles.

## Documentation

| File | Covers |
|---|---|
| `Docs/WishboneSuspension.md` | the suspension model derived in full — 50 numbered equations, each referenced from the code that implements it |
| `VehicleTuning.md` | what each setup parameter means and how to choose it |
| `TireParams_Baseline.md` | the tyre coefficient set and where the values come from |

## Engine version support

One set of sources builds on 4.27 and 5.x. Version branching is concentrated: eleven
`#if` sites in four files, all named and indexed in
`Source/VehiclePhysics/Vehicles/VehiclePhysicsCompat.h`. The simulation itself — tyre
model, driveline solver, suspension mathematics — contains none.

The seams exist where the engines genuinely differ, not merely where an API was renamed:

- **The physics tick entry.** 4.27 dispatches through PhysX substepping
  (`AddCustomPhysics`); 5.x overrides `AsyncPhysicsTickActor`, because Chaos's
  `AddCustomPhysics` is a stub that calls the delegate inline with the frame delta.
- **Reading the body.** 4.27 reads `FBodyInstance`, which is authoritative inside a
  substep callback. 5.x prefers the physics-thread particle; the game-thread copy is a
  frame stale inside an async tick.
- **Applying an accumulated reaction.** 4.27 divides by the substep window and adds a
  force. Chaos does not clear its external force accumulator per step, so the same
  approach delivers roughly N times the intended impulse — it applies the impulse
  directly instead.
- **Landscape edit layers**, which are 5.x only.

## Licence and citation

This repository is made available solely for **non-commercial academic research and
educational purposes**.

Any commercial use, redistribution for commercial purposes, or incorporation into a
commercial product or service is prohibited without prior written authorization.
Requests should be directed to itc@lsu.edu.

Use is conditioned upon proper citation of the following in any resulting publication,
presentation, or derivative work:

> K. Afsharpour, *VehiclePhysics: A Modular Vehicle Dynamics Plugin for Unreal Engine*,
> Louisiana State University, 2026.
> GitHub repository: https://github.com/Kouroshx99/VehiclePhysics

See `LICENCE` for the full terms.
