#pragma once

#include "CoreMinimal.h"
#include "Misc/EngineVersionComparison.h"
#include "Containers/Ticker.h"

/**
 * ENGINE-VERSION SEAMS, all of them, in one file.
 *
 * This plugin builds against UE 4.27 and UE 5.x from one set of sources. The rule that
 * keeps that survivable is that version branching is CONCENTRATED: every #if lives
 * either here or at one of a small number of named seams that say in a comment why they
 * exist. The simulation itself - tyre model, suspension mathematics, drivetrain solver,
 * roughly twenty-two thousand lines of it - contains no version checks at all, because
 * none of it depends on the engine beyond arithmetic.
 *
 * UE_VERSION_NEWER_THAN and UE_VERSION_OLDER_THAN come from Misc/EngineVersionComparison.h,
 * which is present in 4.27 as well as 5.x - so the readable form works on both sides and
 * there is no need to compare ENGINE_MAJOR_VERSION by hand.
 *
 * The seams outside this file are:
 *
 *   ACar                    the physics tick entry. 4.27 registers a substepping
 *                           delegate through AddCustomPhysics; 5.x overrides
 *                           AsyncPhysicsTickActor. Both funnel into the same step.
 *
 *   ACar::ReadBodyState     4.27 reads the body through FBodyInstance, which inside a
 *                           substep callback is the authoritative copy. 5.x prefers the
 *                           physics-thread particle, because the game-thread copy is a
 *                           frame stale inside an async tick.
 *
 *   VehiclePhysicsBody      how an accumulated reaction reaches the body. The two
 *                           engines genuinely differ here - see the comment there, it is
 *                           not a rename.
 *
 *   AVehicleTestCircuit     landscape edit layers, which only exist in 5.x.
 */

/**
 * True where Chaos offers a fixed-rate asynchronous physics tick.
 *
 * 4.27 ships Chaos but drives the vehicle through PhysX substepping, so this is the line
 * between two execution models rather than between two spellings.
 */
#define VP_HAS_ASYNC_PHYSICS (!UE_VERSION_OLDER_THAN(5, 0, 0))

/**
 * Third argument to TArray::RemoveAt and friends.
 *
 * The bool became EAllowShrinking in 5.5; the bool overload still existed, deprecated,
 * before that. Guarded at 5.5 rather than at 5.0 so intermediate versions keep compiling.
 */
#if UE_VERSION_OLDER_THAN(5, 5, 0)
	#define VP_NO_SHRINK false
#else
	#define VP_NO_SHRINK EAllowShrinking::No
#endif

/**
 * True where the ULandscapeEditLayerBase API used by AVehicleTestCircuit exists.
 *
 * Guarded at 5.5 rather than 5.0 because it is that API specifically - GetEditLayerConst
 * returning a ULandscapeEditLayerBase - that arrived then. 4.27 landscapes have no edit
 * layers at all, so the accessor writes the final heightmap and nothing discards it.
 */
#define VP_HAS_LANDSCAPE_EDIT_LAYERS (!UE_VERSION_OLDER_THAN(5, 5, 0))

/**
 * The core ticker.
 *
 * UE5 replaced FTicker with the thread-safe FTSTicker outright rather than keeping both,
 * so the class name has to be bridged. FTickerDelegate survived the rename unchanged,
 * which is why only the ticker itself appears here.
 */
#if UE_VERSION_OLDER_THAN(5, 0, 0)
	using FVehiclePhysicsTicker = FTicker;
#else
	using FVehiclePhysicsTicker = FTSTicker;
#endif
