// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleTestCircuit.h"
#include "VehiclePhysics/Vehicles/VehiclePhysicsCompat.h"

#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "EngineUtils.h"
#include "Misc/Paths.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#if WITH_EDITOR
// Everything the sculpt needs. LandscapeEdit.h is itself #if WITH_EDITOR from line 33
// to line 617, so none of this can be included unguarded.
#include "Landscape.h"
#include "LandscapeProxy.h"
#include "LandscapeInfo.h"
#include "LandscapeComponent.h"
#include "LandscapeDataAccess.h"
#if VP_HAS_LANDSCAPE_EDIT_LAYERS
#include "LandscapeEditLayer.h"
#endif
#include "LandscapeEdit.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogTestCircuit, Log, All);

namespace
{
	/**
	 * Scale that makes a unit mesh come out the requested size, in its own axes.
	 *
	 * Prefixed rather than sharing AVehicleProvingGround's identical helper: both
	 * live in anonymous namespaces, and a unity build puts the two .cpp files in one
	 * translation unit where the names collide. Duplicated deliberately, since the
	 * alternative is a shared header for four lines of arithmetic.
	 */
	FVector CircuitScaleForSize(const UStaticMesh* Mesh, const FVector& SizeCm)
	{
		if (Mesh == nullptr) { return FVector::OneVector; }
		const FVector Bounds = Mesh->GetBoundingBox().GetSize();
		return FVector(
			Bounds.X > KINDA_SMALL_NUMBER ? SizeCm.X / Bounds.X : 1.f,
			Bounds.Y > KINDA_SMALL_NUMBER ? SizeCm.Y / Bounds.Y : 1.f,
			Bounds.Z > KINDA_SMALL_NUMBER ? SizeCm.Z / Bounds.Z : 1.f);
	}

	/** Hermite smoothstep. C1 at both ends, which is what stops the shoulder creasing. */
	FORCEINLINE float SmoothStep01(float T)
	{
		T = FMath::Clamp(T, 0.f, 1.f);
		return T * T * (3.f - 2.f * T);
	}

	const FLinearColor ColourTarmac(0.16f, 0.16f, 0.17f);
	const FLinearColor ColourGravel(0.45f, 0.38f, 0.28f);
	const FLinearColor ColourKerbA(0.85f, 0.15f, 0.15f);
	const FLinearColor ColourKerbB(0.92f, 0.92f, 0.92f);
	const FLinearColor ColourBuilding(0.38f, 0.40f, 0.44f);
	const FLinearColor ColourRock(0.34f, 0.31f, 0.28f);
	const FLinearColor ColourWood(0.30f, 0.22f, 0.14f);
	const FLinearColor ColourFoliage(0.16f, 0.30f, 0.13f);
	const FLinearColor ColourStart(0.95f, 0.85f, 0.10f);
}

AVehicleTestCircuit::AVehicleTestCircuit()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BuildDefaultLayout();
}

void AVehicleTestCircuit::BeginPlay()
{
	Super::BeginPlay();
}

#if WITH_EDITOR
void AVehicleTestCircuit::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// WITHOUT THIS THE VENUE SELECTOR DOES NOTHING, which is exactly how it shipped.
	//
	// Layout is an EditAnywhere UPROPERTY, so it serialises onto the actor in the
	// level, and BuildCentreline only fills it when it is EMPTY. An actor saved with
	// one venue's table therefore keeps that table forever: switching Venue changed a
	// value nothing read, and all three settings built whatever was already there.
	//
	// Repopulating here rather than in Build has a second benefit - the new table
	// appears in the details panel the moment the enum changes, so it is visible
	// rather than a thing that happens later.
	//
	// A hand-edited Layout IS discarded by this. That is the intended trade: the enum
	// is the coarse control and the table is the fine one, so choosing a venue means
	// asking for that venue's table.
	const FName Changed = PropertyChangedEvent.GetPropertyName();
	if (Changed == GET_MEMBER_NAME_CHECKED(AVehicleTestCircuit, Venue))
	{
		Layout.Empty();
		BuildDefaultLayout();
	}
}
#endif

void AVehicleTestCircuit::BuildDefaultLayout()
{
	Layout.Empty();

	switch (Venue)
	{
	case EVehicleTestVenue::RallyStage:   BuildStageLayout();    return;
	case EVehicleTestVenue::OffroadCourse: BuildOffroadLayout(); return;
	case EVehicleTestVenue::CombinedLap:  BuildCombinedLayout(); return;
	case EVehicleTestVenue::Circuit:
	default:                              BuildCircuitLayout();  return;
	}
}

void AVehicleTestCircuit::BuildCircuitLayout()
{
	// 16 segments, 2.31 km, closing to 0.0000 m horizontally and 0.0000 m in height, with
	// the turn angles summing to exactly 360 degrees. Eight corners, 67 to 114 km/h.
	//
	// This is the specification circuit from the design document, which is what decides
	// what gets built here. Two straights carry the closure solve and their lengths are
	// its answer rather than round numbers.
	//
	// ELEVATION, and the second attempt at it. The specification had 9.5 m of relief
	// over 2.31 km - 4.1 m per km, flatter than Monza. The first fix raised that to
	// 34 m but did it with a single dominant sine, which is one hill: the lap went up
	// one side and down the other and read as having no variation at all, correctly.
	//
	// Now a 1/n harmonic series, A(n) = K/n, so every harmonic costs the SAME gradient
	// budget - gradient goes as n*A - and the higher terms become affordable instead of
	// being priced out by the fundamental. That is what buys hills rather than a dome.
	// Every term still vanishes at both ends, so the added rises sum to zero by
	// construction and vertical closure is exact without solving for it.
	//
	// 43.0 m of range, 19 m per km, against Suzuka at 6.9, Spa at 14.6 and Club
	// Motorsports New Hampshire at 18.9. But range is the wrong measure and it is worth
	// saying why: it cannot tell one hill from five. The number that reads as variation
	// is TRAVEL, the metres actually climbed per lap - 151 m here against 73 m for the
	// single dome at the same range. Steepest gradient 9.8%.
	//
	// Barely subdivided, and deliberately: splitting further made it WORSE. A long
	// segment can hold a whole sine lobe inside one rise, where heavy subdivision
	// splits lobes across boundaries and the up and down partly cancel in the sum.
	// 38 segments measured 112 m of travel against 151 m for 16.
	//
	// Length, radius (0 straight, +right, -left), rise, bank, width, gravel.
	Layout.Add(FCircuitSegment(   30673.f,         0.f,    2950.f,   0.0f,   1200.f, false ));  // Start / Finish 1/2
	Layout.Add(FCircuitSegment(   30500.f,         0.f,   -2604.f,   0.0f,   1200.f, false ));  // Start / Finish 2/2
	Layout.Add(FCircuitSegment(   10500.f,      4000.f,    -335.f,   2.0f,   1000.f, false ));  // T1 Hairpin
	Layout.Add(FCircuitSegment(    9200.f,         0.f,      53.f,   0.0f,   1100.f, false ));  // Chute
	Layout.Add(FCircuitSegment(    8400.f,     -8000.f,     493.f,   1.0f,   1100.f, false ));  // T2 Left
	Layout.Add(FCircuitSegment(   25814.f,         0.f,    1891.f,   0.0f,   1100.f, false ));  // Run to T3
	Layout.Add(FCircuitSegment(   10500.f,      6000.f,     532.f,   3.0f,   1000.f, false ));  // T3 Right
	Layout.Add(FCircuitSegment(    8000.f,         0.f,     757.f,   0.0f,   1100.f, false ));  // The Climb
	Layout.Add(FCircuitSegment(   10500.f,    -12000.f,     557.f,   0.0f,   1150.f, false ));  // T4 Crest Left
	Layout.Add(FCircuitSegment(    9000.f,         0.f,    -323.f,   0.0f,   1100.f, false ));  // The Drop
	Layout.Add(FCircuitSegment(   20900.f,     10000.f,     325.f,   4.0f,   1200.f, false ));  // T5 Long Right
	Layout.Add(FCircuitSegment(    8400.f,         0.f,    -274.f,   0.0f,   1000.f, false ));  // Esse Link
	Layout.Add(FCircuitSegment(    3500.f,     -5000.f,    -130.f,   0.0f,    950.f, false ));  // T6 Esse Left
	Layout.Add(FCircuitSegment(    3500.f,      5000.f,    -171.f,   0.0f,    950.f, false ));  // T7 Esse Right
	Layout.Add(FCircuitSegment(   29500.f,         0.f,   -2885.f,   0.0f,   1100.f, false ));  // Back Chute
	Layout.Add(FCircuitSegment(   12200.f,      7000.f,    -836.f,   4.0f,   1200.f, false ));  // T8 Onto Straight
}

void AVehicleTestCircuit::BuildStageLayout()
{
	// 19 segments, %.2f km of gravel, POINT TO POINT - it does not close and it should
	// not. A stage that returns to its own start reads as a circuit with the wrong
	// surface. Net drop 2.0 m, 13.5 m of relief.
	//
	// Two traps, a yump, adverse camber and a section narrow enough to matter. The
	// yump is short on purpose: lengthen it and the crest stops throwing the car.
	Layout.Add(FCircuitSegment(   12000.f,         0.f,       0.f,   0.0f,    900.f, true  ));  // Stage Start
	Layout.Add(FCircuitSegment(    9400.f,     -9000.f,    -200.f,   3.0f,    850.f, true  ));  // Opening Left
	Layout.Add(FCircuitSegment(    7000.f,         0.f,     350.f,   0.0f,    800.f, true  ));  // Run to the Yump
	Layout.Add(FCircuitSegment(    2200.f,         0.f,    -100.f,   0.0f,    750.f, true  ));  // The Yump
	Layout.Add(FCircuitSegment(    5500.f,      4500.f,    -250.f,  -2.0f,    700.f, true  ));  // Blind Right on Landing
	Layout.Add(FCircuitSegment(   11000.f,         0.f,    -700.f,   0.0f,    750.f, true  ));  // The Descent
	Layout.Add(FCircuitSegment(    3700.f,      7000.f,    -100.f,   1.0f,    800.f, true  ));  // Trap Entry
	Layout.Add(FCircuitSegment(    2800.f,      4000.f,     -80.f,   0.0f,    700.f, true  ));  // Trap Tightens
	Layout.Add(FCircuitSegment(    2700.f,      2200.f,     -40.f,  -6.0f,    650.f, true  ));  // Trap Apex
	Layout.Add(FCircuitSegment(    9000.f,         0.f,     150.f,   0.0f,    600.f, true  ));  // Narrow Chute
	Layout.Add(FCircuitSegment(    4800.f,     -5500.f,     100.f,  -8.0f,    650.f, true  ));  // Devils Camber
	Layout.Add(FCircuitSegment(    2400.f,      3000.f,      50.f,  -2.0f,    550.f, true  ));  // Tree Gate Right
	Layout.Add(FCircuitSegment(    2400.f,     -3000.f,      50.f,  -2.0f,    550.f, true  ));  // Tree Gate Left
	Layout.Add(FCircuitSegment(   13000.f,         0.f,     900.f,   0.0f,    800.f, true  ));  // The Climb
	Layout.Add(FCircuitSegment(    8400.f,     12000.f,     100.f,   2.0f,    900.f, true  ));  // Long Right Opens
	Layout.Add(FCircuitSegment(    3500.f,      4500.f,     -50.f,  -4.0f,    750.f, true  ));  // Long Right Shuts
	Layout.Add(FCircuitSegment(   18000.f,         0.f,    -200.f,   0.0f,   1000.f, true  ));  // Open Run
	Layout.Add(FCircuitSegment(    6700.f,     -7000.f,    -150.f,   2.0f,    850.f, true  ));  // Final Left
	Layout.Add(FCircuitSegment(   10000.f,         0.f,     -30.f,   0.0f,   1000.f, true  ));  // Finish Straight
}

void AVehicleTestCircuit::BuildOffroadLayout()
{
	// STATIONS ARE SET AFTER THE TABLE, at the bottom of this function, so the table
	// itself stays six readable columns of numbers. Until they existed the course was a
	// narrow road with some gradients: the names below were comments and nothing built
	// them, which is exactly how it drove.

	// 17 segments, %.0f m, closing to 1.44 m. Nothing here is about speed - every
	// station asks a suspension question instead. Articulation at the ditch, approach
	// angle at the ledge, traction on a 19%% climb, and rollover margin across the
	// side slope.
	//
	// The steepest gradients in the whole project are here, and deliberately: an
	// open differential is most obviously an open differential on a loose 19%% climb.
	Layout.Add(FCircuitSegment(   16500.f,         0.f,       0.f,   0.0f,    600.f, true  ));  // Trailhead
	Layout.Add(FCircuitSegment(    3500.f,         0.f,     100.f,   0.0f,    600.f, true  ));  // Rock Garden
	Layout.Add(FCircuitSegment(    3100.f,      2000.f,      50.f,   0.0f,    550.f, true  ));  // Turn to Ledge
	Layout.Add(FCircuitSegment(    1800.f,         0.f,     120.f,   0.0f,    500.f, true  ));  // The Ledge Step
	Layout.Add(FCircuitSegment(    6000.f,         0.f,       0.f, -14.0f,    550.f, true  ));  // Side Slope Traverse
	Layout.Add(FCircuitSegment(    2400.f,      1500.f,     -50.f,   0.0f,    500.f, true  ));  // Around the Knoll
	Layout.Add(FCircuitSegment(    2600.f,         0.f,    -150.f,   0.0f,    550.f, true  ));  // Ditch Crossing
	Layout.Add(FCircuitSegment(    2200.f,         0.f,      80.f,   0.0f,    500.f, true  ));  // Log Crossing
	Layout.Add(FCircuitSegment(    3400.f,         0.f,     650.f,   0.0f,    600.f, true  ));  // Steep Climb
	Layout.Add(FCircuitSegment(    1600.f,     -2000.f,      30.f,   0.0f,    500.f, true  ));  // Ridge Turn
	Layout.Add(FCircuitSegment(    3800.f,         0.f,    -700.f,   0.0f,    600.f, true  ));  // Steep Descent
	Layout.Add(FCircuitSegment(    4500.f,         0.f,      50.f,   0.0f,    550.f, true  ));  // Rut Section
	Layout.Add(FCircuitSegment(    1200.f,      1500.f,     -20.f,   0.0f,    500.f, true  ));  // Switchback
	Layout.Add(FCircuitSegment(    4000.f,         0.f,       0.f,   0.0f,    650.f, true  ));  // Whoops Run
	Layout.Add(FCircuitSegment(    3100.f,      2000.f,     -40.f,   0.0f,    600.f, true  ));  // Return Sweep
	Layout.Add(FCircuitSegment(   14200.f,         0.f,    -120.f,   0.0f,    600.f, true  ));  // Back to Trailhead
	Layout.Add(FCircuitSegment(    3100.f,      2000.f,       0.f,   0.0f,    600.f, true  ));  // Final Turn In
	// Which segments carry geometry. Indices rather than names because the table has no
	// name column - the comments are the names - and a wrong index shows up instantly
	// as a rock garden in the wrong place.
	auto Station = [this](int32 Index, ECircuitStation What)
	{
		if (Layout.IsValidIndex(Index))
		{
			Layout[Index].Station = What;
		}
	};

	Station(1,  ECircuitStation::RockGarden);    // Rock Garden
	Station(3,  ECircuitStation::LedgeStep);     // The Ledge Step
	Station(7,  ECircuitStation::LogCrossing);   // Log Crossing
	Station(11, ECircuitStation::RutSection);    // Rut Section
	Station(13, ECircuitStation::WhoopsRun);     // Whoops Run

	// ROCK CRAWLING goes on the two steep segments, which is where it belongs: a
	// boulder field on the flat is an obstacle course, and the same field on a 19%
	// climb is a traction problem as well. Segment 8 climbs at +19.1% and segment 10
	// drops at -18.4%, and both were previously bare gradient.
	Station(8,  ECircuitStation::RockCrawl);     // Steep Climb
	Station(10, ECircuitStation::RockCrawl);     // Steep Descent

	// THE TWO GEOMETRIC LIMITS, which the course never tested.
	//
	// Segment 6 was the Ditch Crossing and carried nothing, for the reason below. A
	// BREAKOVER RIDGE goes there instead: same question about belly clearance, asked
	// from above the trail rather than below it, and additive so it can actually be
	// built. Segment 15 is 142 m of straight on a gentle descent with no station on it,
	// which is the only stretch long enough for a GRADE sweep with run-up and run-out.
	Station(6,  ECircuitStation::BreakoverRidge);   // was the bare Ditch Crossing
	Station(15, ECircuitStation::GradeTest);        // Back to Trailhead

	// A DITCH ITSELF still cannot be built this way, and that is why the ridge stands in
	// for it. A 1.5 m V is a hole, everything above is additive, and three ridges
	// standing in for a rut is a fair trade where standing in for a ditch is not - the
	// articulation test IS the depth. It wants a local subtractive carve in
	// SculptLandscape, which is a different job.

}

void AVehicleTestCircuit::BuildCombinedLayout()
{
	// A 4.03 km lap that CLOSES to 0.63 cm horizontally and 0.00 cm vertically, with
	// its finishing heading back at 360.0009 degrees. Those three things do not come
	// out of drawing a track: a layout laid down by eye misses its own start line by
	// hundreds of metres and tens of degrees, and no amount of nudging one corner
	// fixes it without breaking the last.
	//
	// It was solved. Every turn angle is degrees(length/radius), and they are chosen
	// to sum to exactly 360 - which closes heading by construction AND makes position
	// LINEAR in the straight lengths, because with the headings fixed each straight
	// contributes a known direction times its length. That leaves a 2x2 solve. The two
	// straights marked below are not round numbers because they are its answer, and
	// they were picked 90 degrees apart so the matrix is perfectly conditioned - two
	// straights either side of the same 180 degrees of turning would be singular.
	//
	// One corner carries the adjustment that made the turns sum: Combe Right went from
	// 85 to 95 degrees, chosen by searching every corner and every pair for the change
	// that left the least residual. Its radius, bank and width are untouched.
	//
	// The lap holds 46 m of clearance between parts of itself that pass near each
	// other. The only sub-40 m approach anywhere is the T1 hairpin's own two legs,
	// which are 2R = 36 m apart because the radius is 18 m, and that is a hairpin
	// rather than a fault.
	//
	// Change anything and it stops closing; Build logs by how much. To close it again,
	// keep the turns summing to 360 and re-solve two straights rather than nudging
	// corners.
	//
	// ---- the landform ----------------------------------------------------------
	//
	// 61.4 m of elevation range, which is 1.5 m per km. For scale: Monza is 1.7,
	// Suzuka 6.9, Brands Hatch 10.3, Spa 14.6. It reads as a hillside circuit rather
	// than a flat one, which is the point - it had 23.5 m and the steep bits were all
	// short, so the LAP was flat even though individual corners were not.
	//
	// The rises come from a sum of sines in lap fraction, sin(n*pi*x), superimposed on
	// the hand-placed features. Two properties made that the cheap way to do it. Every
	// term vanishes at both ends, so the added rises sum to zero and VERTICAL closure
	// is exact by construction rather than by solving. And LengthCm is the PLAN length
	// here - BuildCentreline advances X and Y by it and adds Z separately - so rise
	// cannot disturb the horizontal closure at all. The 0.63 cm is untouched.
	//
	// Weighted so the landform lands where there is room for it: a segment already
	// doing 12% of its own gradient takes none, a long flat straight takes all of it.
	// Without that, the whole thing is capped by The Wall before the lap gains any
	// topography. The Wall, the yump sequence and the other character corners are
	// therefore bit-identical - the terrain moved around them.
	//
	// It is not harsher for it. Joints exceeding a third of a g vertically went from
	// 4 tarmac and 8 gravel to 4 and 7, and the yump still launches at 1.79 g because
	// its three segments are pinned outright.
	//
	// Length, radius (0 straight, +right, -left), rise, bank, width, gravel.

	// Summit Straight carries a 2 cm trim. Rises are whole centimetres and the
	// landform's are not, so rounding 47 of them left the lap 2 cm high; the longest
	// straight absorbs it at 0.004% of gradient. Vertical closure is exact again.
	// ---- the fast half: 2.92 km of sealed road, 74.8 m of climb ----------------
	Layout.Add(FCircuitSegment(24673.f,      0.f,     -588.f,  0.f, 1400.f, false));  //   +0.00  Grid Straight
	Layout.Add(FCircuitSegment( 5278.f,   1800.f,  -225.f, -1.f, 1000.f, false));  // +168.00  T1 Hairpin
	Layout.Add(FCircuitSegment(19474.f,      0.f,  -1032.f,  0.f, 1300.f, false));  //   +0.00  The Plunge

	// The compression and the wall. Heavily loaded at the bottom of the dip, then
	// +15.9% uphill taken near-flat - the car goes light exactly where it is asked
	// to change direction. The single most revealing corner on the lap.
	Layout.Add(FCircuitSegment( 2967.f,  -8500.f,   145.f,  1.f, 1200.f, false));  //  -20.00  Compression Left
	Layout.Add(FCircuitSegment( 6597.f,  10500.f,  1050.f,  2.f, 1100.f, false));  //  +36.00  The Wall
	Layout.Add(FCircuitSegment( 6109.f, -25000.f,   607.f,  0.f, 1150.f, false));  //  -14.00  Light Crest Left
	Layout.Add(FCircuitSegment(46448.f,      0.f,   1683.f,  0.f, 1350.f, false));  //   +0.00  Summit Straight

	Layout.Add(FCircuitSegment( 7627.f,   4600.f,  225.f,  1.f, 1000.f, false));  //  +95.00  Combe Right   <- turn budget
	Layout.Add(FCircuitSegment( 3449.f,  -3800.f,  -181.f,  0.f,  950.f, false));  //  -52.00  Combe Left
	Layout.Add(FCircuitSegment(12089.f,      0.f,   779.f,  0.f, 1200.f, false));  //   +0.00  Ridge Approach

	// Turns as it drops away at -10.1%. The exit is not visible from the entry.
	Layout.Add(FCircuitSegment( 7959.f,   9500.f,  -803.f, -2.f, 1000.f, false));  //  +48.00  Blind Drop Right
	Layout.Add(FCircuitSegment( 9215.f, -16500.f,  -153.f,  4.f, 1250.f, false));  //  -32.00  Cascade One
	Layout.Add(FCircuitSegment( 6807.f, -13000.f,  -144.f,  4.f, 1150.f, false));  //  -30.00  Cascade Two
	Layout.Add(FCircuitSegment(11151.f,      0.f,  132.f,  0.f, 1250.f, false));  //   +0.00  Valley Run     <- 2x2 solved

	Layout.Add(FCircuitSegment( 4328.f,   6200.f,  -7.f,  0.f, 1000.f, false));  //  +40.00  Esse Right
	Layout.Add(FCircuitSegment( 3519.f,  -5600.f,  -50.f,  0.f,  950.f, false));  //  -36.00  Esse Left

	// Opens, then shuts. A car that settles on the first radius is wrong for the second.
	Layout.Add(FCircuitSegment(14050.f,  11500.f,   639.f,  2.f, 1150.f, false));  //  +70.00  Stavelot Long Right
	Layout.Add(FCircuitSegment( 6283.f,   7200.f,   378.f, -1.f, 1050.f, false));  //  +50.00  Stavelot Tighten
	Layout.Add(FCircuitSegment(26154.f,      0.f,  1634.f,  0.f, 1300.f, false));  //   +0.00  The Climb
	Layout.Add(FCircuitSegment(16616.f, -28000.f,   204.f,  2.f, 1300.f, false));  //  -34.00  Highside Left
	Layout.Add(FCircuitSegment( 8605.f,      0.f,  -207.f,  0.f, 1250.f, false));  //   +0.00  Approach Link  <- 2x2 solved
	Layout.Add(FCircuitSegment(18326.f,  14000.f,  -790.f,  3.f, 1200.f, false));  //  +75.00  Grand Sweep Right
	Layout.Add(FCircuitSegment( 2632.f,   2600.f,  -153.f,  0.f,  900.f, false));  //  +58.00  Chicane In

	// ---- the rally loop: 1.11 km of gravel, spliced INSIDE the late chicane -----
	//
	// The splice point is forced by geometry rather than chosen by taste. The loop's
	// end-to-end displacement is 845.8 m at +3.8 degrees in its own frame, so inserted
	// at heading h it cancels the tarmac lap's own 989.5 m error only near h = 49.8.
	// The lap passes through that heading in exactly two places, and this is the one
	// that leaves the straights least re-proportioned. Every other splice was
	// infeasible - not worse, infeasible.
	Layout.Add(FCircuitSegment( 7878.f,      0.f,   -311.f,  0.f, 1400.f, true ));  //   +0.00  Gravel Entry
	Layout.Add(FCircuitSegment( 9420.f, -12000.f,  -474.f,  3.f, 1300.f, true ));  //  -44.98  Opening Sweeper Left
	Layout.Add(FCircuitSegment( 7000.f,      0.f,   560.f,  0.f, 1200.f, true ));  //   +0.00  Climb to the Yump

	// Held at 1800 cm through the closure solve. Left free, the solver stretched it
	// 38% and the crest stopped throwing the car, which is the only reason it is here.
	Layout.Add(FCircuitSegment( 1800.f,      0.f,   -40.f,  0.f, 1100.f, true ));  //   +0.00  The Yump
	Layout.Add(FCircuitSegment( 4280.f,   7000.f,  -330.f, -2.f, 1000.f, true ));  //  +35.03  Blind Right on Landing
	Layout.Add(FCircuitSegment( 7416.f,      0.f,  -695.f,  0.f, 1000.f, true ));  //   +0.00  The Drop

	// Decreasing radius across three segments - 80 m, then 40 m, then 18 m, off camber
	// at the apex. The classic trap: every input that felt right at the entry is wrong.
	Layout.Add(FCircuitSegment( 2790.f,   8000.f,  -204.f,  1.f, 1000.f, true ));  //  +19.98  The Trap Entry
	Layout.Add(FCircuitSegment( 2440.f,   4000.f,  -163.f,  0.f,  900.f, true ));  //  +34.95  The Trap Tightens
	Layout.Add(FCircuitSegment( 1885.f,   1800.f,   -94.f, -6.f,  900.f, true ));  //  +60.00  The Trap Apex

	Layout.Add(FCircuitSegment( 3488.f,      0.f,   23.f,  0.f,  800.f, true ));  //   +0.00  Narrow Chute

	// -8 degrees of adverse camber costs tan(8) = 0.14 g of a 0.85 g budget, and does
	// it for the whole corner rather than at one point.
	Layout.Add(FCircuitSegment( 4190.f,  -6000.f,    -43.f, -8.f,  850.f, true ));  //  -40.01  Devils Camber
	Layout.Add(FCircuitSegment( 2180.f,   2500.f,    -31.f, -2.f,  750.f, true ));  //  +49.96  Tree Gate Right
	Layout.Add(FCircuitSegment( 1965.f,  -2500.f,    -22.f, -2.f,  750.f, true ));  //  -45.03  Tree Gate Left
	Layout.Add(FCircuitSegment( 6280.f,  -9000.f,   578.f,  4.f, 1100.f, true ));  //  -39.98  Opening Left Climb
	Layout.Add(FCircuitSegment( 6614.f,      0.f,   -4.f,  0.f, 1200.f, true ));  //   +0.00  Rolling Ridge
	Layout.Add(FCircuitSegment( 5235.f, -15000.f,  -267.f,  2.f, 1300.f, true ));  //  -20.00  Long Left Reads Open
	Layout.Add(FCircuitSegment( 3665.f,  -6000.f,  -282.f, -5.f, 1100.f, true ));  //  -35.00  Long Left Shuts Down
	Layout.Add(FCircuitSegment(13095.f,  30000.f,   -197.f,  2.f, 1500.f, true ));  //  +25.01  The Big Right
	Layout.Add(FCircuitSegment(11879.f,      0.f,    -289.f,  0.f, 1500.f, true ));  //   +0.00  Open Run
	Layout.Add(FCircuitSegment( 3490.f,  -5000.f,  -218.f, -6.f, 1200.f, true ));  //  -39.99  Last Chance Left
	Layout.Add(FCircuitSegment( 4197.f,   8000.f,   85.f,  2.f, 1400.f, true ));  //  +30.06  Rejoin Right

	// ---- back onto tarmac and home ---------------------------------------------
	Layout.Add(FCircuitSegment( 3416.f,      0.f,   -127.f,  0.f,  900.f, false));  //   +0.00  Chicane Link
	Layout.Add(FCircuitSegment( 2178.f,  -2400.f,  -134.f,  0.f,  900.f, false));  //  -52.00  Chicane Out
	Layout.Add(FCircuitSegment(15673.f,      0.f,  -464.f,  0.f, 1400.f, false));  //   +0.00  Run to the Line
}

bool AVehicleTestCircuit::LoadAssets()
{
	CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	ConeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone"));
	BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

	if (CubeMesh == nullptr)
	{
		UE_LOG(LogTestCircuit, Error,
			TEXT("Engine basic shapes are missing. Enable 'Show Engine Content' in the Content Browser "
				 "view options, or there is nothing to build the circuit from."));
		return false;
	}
	return true;
}

namespace
{
	// One tag for every piece any circuit actor ever made. Deliberately NOT per-actor:
	// the orphans worth finding are precisely the ones whose actor is gone.
	FName CircuitPieceTag()
	{
		return FName(TEXT("VehicleTestCircuitPiece"));
	}
}

AActor* AVehicleTestCircuit::SpawnPiece(UStaticMesh* Mesh, const FVector& LocalLocation,
	const FRotator& LocalRotation, const FVector& SizeCm, const FLinearColor& Colour, bool bBlocking)
{
	if (Mesh == nullptr)
	{
		return nullptr;
	}

	const FTransform& Base = GetActorTransform();
	const FVector World = Base.TransformPosition(LocalLocation);
	const FRotator WorldRot = (Base.GetRotation() * LocalRotation.Quaternion()).Rotator();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = this;
	AStaticMeshActor* Piece = GetWorld()->SpawnActor<AStaticMeshActor>(World, WorldRot, Params);
	if (Piece == nullptr)
	{
		return nullptr;
	}

	// TAGGED, so Clear can find it even when Spawned cannot. Spawned is serialised on
	// the actor, so a piece outlives its tracking whenever the actor is replaced,
	// duplicated, or deleted without Clear being pressed first - and the orphans then
	// sit in the level looking exactly like the current build being in the wrong place.
	Piece->Tags.AddUnique(CircuitPieceTag());

#if WITH_EDITOR
	// FOLDERED AND NAMED. A circuit is a few thousand actors and they were all called
	// StaticMeshActor5063, in one flat list, which makes the outliner useless for the
	// one thing it is for - finding something. Grouped under this actor's own name so
	// two venues in one level stay apart.
	if (!PieceFolder.IsNone())
	{
		Piece->SetFolderPath(PieceFolder);
	}
	Piece->SetActorLabel(FString::Printf(TEXT("%s_%s_%04d"),
		*GetName(),
		PieceFolder.IsNone() ? TEXT("Piece") : *FPaths::GetCleanFilename(PieceFolder.ToString()),
		Spawned.Num()));
#endif

	Piece->SetMobility(EComponentMobility::Movable);
	if (UStaticMeshComponent* Comp = Piece->GetStaticMeshComponent())
	{
		Comp->SetStaticMesh(Mesh);
		Comp->SetRelativeScale3D(CircuitScaleForSize(Mesh, SizeCm));

		if (bBlocking)
		{
			Comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Comp->SetCollisionProfileName(TEXT("BlockAll"));
		}
		else
		{
			Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (BaseMaterial)
		{
			if (UMaterialInstanceDynamic* MID = Comp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMaterial))
			{
				MID->SetVectorParameterValue(TEXT("Color"), Colour);
			}
		}
	}

	Spawned.Add(Piece);
	return Piece;
}

// ---------------------------------------------------------------------------
// Centreline
// ---------------------------------------------------------------------------

void AVehicleTestCircuit::BuildCentreline()
{
	CentrePos.Reset();
	CentreHeadingDeg.Reset();
	CentreGradeDeg.Reset();
	CentreRollDeg.Reset();
	CentreWidthCm.Reset();
	CentreStepCm.Reset();
	CentreGravel.Reset();
	CentreSegment.Reset();

	if (Layout.Num() == 0)
	{
		BuildDefaultLayout();
	}

	const float Step = FMath::Max(SlabLengthCm, 50.f);

	// Heading is a UE yaw - degrees, zero along +X, increasing to the right - so a
	// positive radius, which is a right-hand corner, increases it.
	FVector Pos(0.f, 0.f, 0.f);
	float HeadingDeg = 0.f;
	float TravelledCm = 0.f;

	CentreLengthCm = 0.f;
	CentreLapSeconds = 0.f;

	for (int32 SegIndex = 0; SegIndex < Layout.Num(); ++SegIndex)
	{
		const FCircuitSegment& Seg = Layout[SegIndex];
		const float SegLen = FMath::Max(Seg.LengthCm, Step);
		const int32 Steps = FMath::Max(FMath::RoundToInt(SegLen / Step), 1);
		const float StepLen = SegLen / Steps;

		// Grade and bank are constant across a segment, so both come from the
		// description rather than from a difference between samples.
		const float GradeDeg = FMath::RadiansToDegrees(FMath::Atan2(Seg.RiseCm, SegLen));

		// Bank raises the OUTSIDE of the corner. Negative roll raises +Y (the car's
		// right) - the convention AVehicleProvingGround::BuildRtiRamp established - so
		// a right-hand corner, whose outside is on the left, wants positive roll and
		// the sign follows the radius.
		const float TurnSign = (Seg.RadiusCm > 0.f) ? 1.f : ((Seg.RadiusCm < 0.f) ? -1.f : 0.f);
		const float RollDeg = Seg.BankDeg * TurnSign;

		// Lap estimate: a corner is limited by grip at its radius, a straight by top
		// speed. Bank belongs in here because it genuinely raises the limit rather
		// than merely looking like it should.
		const float BankRad = FMath::DegreesToRadians(FMath::Abs(Seg.BankDeg));
		const float EffectiveGrip = LapEstimateGrip + FMath::Tan(BankRad);
		const float CornerSpeed = (Seg.RadiusCm != 0.f)
			? FMath::Sqrt(EffectiveGrip * 9.81f * (FMath::Abs(Seg.RadiusCm) * 0.01f))
			: (LapEstimateTopSpeedKph / 3.6f);
		const float SegSpeed = FMath::Max(FMath::Min(CornerSpeed, LapEstimateTopSpeedKph / 3.6f), 5.f);
		CentreLapSeconds += (SegLen * 0.01f) / SegSpeed;

		for (int32 i = 0; i < Steps; ++i)
		{
			const float HeadingRad = FMath::DegreesToRadians(HeadingDeg);
			const float HalfStep = StepLen * 0.5f;

			FVector Sample(
				Pos.X + (FMath::Cos(HeadingRad) * HalfStep),
				Pos.Y + (FMath::Sin(HeadingRad) * HalfStep),
				Pos.Z + ((Seg.RiseCm / Steps) * 0.5f));

			// Gravel undulates on a long wavelength. Short ripples are the proving
			// ground's washboard; what a stage adds that a bump strip cannot is a
			// surface the whole car has to follow rather than a disturbance each
			// wheel takes in turn.
			if (Seg.bGravel)
			{
				const float S = TravelledCm + (i * StepLen);
				Sample.Z += (18.f * FMath::Sin(S * 0.0006f)) + (9.f * FMath::Sin(S * 0.0017f));
			}

			CentrePos.Add(Sample);
			CentreHeadingDeg.Add(HeadingDeg);
			CentreGradeDeg.Add(GradeDeg);
			CentreRollDeg.Add(RollDeg);
			CentreWidthCm.Add(Seg.WidthCm);
			CentreStepCm.Add(StepLen);
			CentreGravel.Add(Seg.bGravel ? 1 : 0);
			CentreSegment.Add(SegIndex);

			Pos.X += FMath::Cos(HeadingRad) * StepLen;
			Pos.Y += FMath::Sin(HeadingRad) * StepLen;
			Pos.Z += Seg.RiseCm / Steps;
			if (Seg.RadiusCm != 0.f)
			{
				HeadingDeg += FMath::RadiansToDegrees(StepLen / Seg.RadiusCm);
			}
		}

		TravelledCm += SegLen;
		CentreLengthCm += SegLen;
	}

	CentreCloseErrorCm = FVector2D(Pos.X, Pos.Y).Size();
	CentreCloseHeightCm = Pos.Z;
}

// ---------------------------------------------------------------------------
// Road and scenery
// ---------------------------------------------------------------------------

void AVehicleTestCircuit::SmoothCentrelineHeight()
{
	const int32 Count = CentrePos.Num();
	if (GradeBlendLengthCm <= 1.f || Count < 5)
	{
		return;
	}

	// Half-width in samples. The steps are near-uniform, so one divide is honest
	// enough here and avoids walking the array to accumulate arc length.
	const float StepCm = FMath::Max(CentreLengthCm / FMath::Max(Count, 1), 1.f);
	const int32 Half = FMath::Clamp(FMath::RoundToInt((GradeBlendLengthCm * 0.5f) / StepCm), 1, Count / 3);

	// Wrapped for a closed venue, clamped for the stage. A symmetric moving average
	// over a PERIODIC sequence stays periodic, so a loop's closure survives untouched;
	// clamping holds the stage's start and finish heights where the layout put them.
	const bool bClosed = (Venue != EVehicleTestVenue::RallyStage);

	auto WorstVerticalG = [this, Count]() -> float
	{
		float Worst = 0.f;
		for (int32 i = 1; i + 1 < Count; ++i)
		{
			const float StepA = FMath::Max(CentreStepCm[i], 1.f);
			const float StepB = FMath::Max(CentreStepCm[i + 1], 1.f);
			const float GradeA = (CentrePos[i].Z - CentrePos[i - 1].Z) / StepA;
			const float GradeB = (CentrePos[i + 1].Z - CentrePos[i].Z) / StepB;
			const float SpeedMs = FMath::Min(LapEstimateTopSpeedKph / 3.6f, 200.f / 3.6f);
			Worst = FMath::Max(Worst,
				(SpeedMs * SpeedMs * FMath::Abs(GradeB - GradeA)) / (StepB * 0.01f) / 9.81f);
		}
		return Worst;
	};

	const float Before = WorstVerticalG();

	// Two passes of a box blur, which is a decent triangular kernel and cheaper than
	// building one. Positions only - X and Y are the closure solve's answer and are
	// not ours to touch.
	for (int32 Pass = 0; Pass < 2; ++Pass)
	{
		TArray<float> Smoothed;
		Smoothed.SetNumUninitialized(Count);

		for (int32 i = 0; i < Count; ++i)
		{
			float Sum = 0.f;
			int32 N = 0;
			for (int32 d = -Half; d <= Half; ++d)
			{
				int32 j = i + d;
				if (bClosed)
				{
					j = ((j % Count) + Count) % Count;
				}
				else
				{
					j = FMath::Clamp(j, 0, Count - 1);
				}
				Sum += CentrePos[j].Z;
				++N;
			}
			Smoothed[i] = Sum / FMath::Max(N, 1);
		}

		for (int32 i = 0; i < Count; ++i)
		{
			CentrePos[i].Z = Smoothed[i];
		}
	}

	// AND THE BANK, which nothing was smoothing at all. The design document's second
	// rule is a roll RATE limit - L >= v * dBank / (4 deg/s) - and it had no more
	// enforcement than the first one did. CentreRollDeg comes straight from the
	// segment, so bank arrives as a step at every corner entry: the ribbon interpolates
	// roll across one component, up to 8 m, where 4 degrees of bank at 100 km/h wants
	// 28 m to come in over.
	//
	// Smoothed over the same window as the height. The two rules are different - one is
	// an acceleration limit, the other a rate limit - but the length they ask for is
	// the same order, and one window is easier to reason about than two.
	{
		const int32 RollHalf = Half;
		for (int32 Pass = 0; Pass < 2; ++Pass)
		{
			TArray<float> SmoothRoll;
			SmoothRoll.SetNumUninitialized(Count);
			for (int32 i = 0; i < Count; ++i)
			{
				float Sum = 0.f;
				int32 N = 0;
				for (int32 d = -RollHalf; d <= RollHalf; ++d)
				{
					int32 j = i + d;
					j = bClosed ? (((j % Count) + Count) % Count) : FMath::Clamp(j, 0, Count - 1);
					Sum += CentreRollDeg[j];
					++N;
				}
				SmoothRoll[i] = Sum / FMath::Max(N, 1);
			}
			for (int32 i = 0; i < Count; ++i)
			{
				CentreRollDeg[i] = SmoothRoll[i];
			}
		}
	}

	// Grade is published for the slab path and the readouts, so it has to come from
	// the heights that actually exist now rather than from the layout's intent.
	for (int32 i = 0; i < Count; ++i)
	{
		const int32 Next = bClosed ? ((i + 1) % Count) : FMath::Min(i + 1, Count - 1);
		const float Run = FMath::Max(CentreStepCm[Next], 1.f);
		CentreGradeDeg[i] = FMath::RadiansToDegrees(FMath::Atan2(CentrePos[Next].Z - CentrePos[i].Z, Run));
	}

	float WorstRollRate = 0.f;
	for (int32 i = 1; i < Count; ++i)
	{
		const float RunM = FMath::Max(CentreStepCm[i], 1.f) * 0.01f;
		const float SpeedMs = FMath::Max(LapEstimateTopSpeedKph / 3.6f, 1.f);
		const float DegPerSec = FMath::Abs(CentreRollDeg[i] - CentreRollDeg[i - 1]) / (RunM / SpeedMs);
		WorstRollRate = FMath::Max(WorstRollRate, DegPerSec);
	}

	UE_LOG(LogTestCircuit, Log,
		TEXT("  blend %.0f m (%d samples each side): worst vertical %.2f g -> %.2f g, "
			 "worst roll rate %.1f deg/s (the rule wants 4)."),
		GradeBlendLengthCm * 0.01f, Half, Before, WorstVerticalG(), WorstRollRate);
}

void AVehicleTestCircuit::BuildRoadRibbon()
{
	// ADAPTIVE SPACING, from the geometry rather than a guess. The chord across a
	// component is short of the arc by L^2/8R, so inverting that for a tolerance gives
	// L = sqrt(8*R*tol) - short components exactly where the curvature earns them and
	// long ones on the straights, which is what "more detail where it is needed" means
	// when it is written down as a number.
	const float TolCm = FMath::Max(RibbonChordToleranceCm, 0.25f);
	const int32 Count = CentrePos.Num();

	TArray<int32> Nodes;
	Nodes.Add(0);
	for (int32 i = 1; i < Count; )
	{
		// Local radius from the heading change across this step. A straight reports an
		// effectively infinite radius and falls through to the span cap.
		const int32 Prev = Nodes.Last();
		const float StepCm = FMath::Max(CentreStepCm[i], 1.f);
		const float TurnDeg = FMath::Abs(FMath::FindDeltaAngleDegrees(
			CentreHeadingDeg[FMath::Max(i - 1, 0)], CentreHeadingDeg[i]));
		const float TurnRad = FMath::DegreesToRadians(FMath::Max(TurnDeg, 0.0001f));
		const float RadiusCm = StepCm / TurnRad;

		const float AllowedCm = FMath::Min(FMath::Sqrt(8.f * RadiusCm * TolCm), RibbonMaxSpanCm);

		float SpanCm = 0.f;
		int32 j = Prev;
		while (j + 1 < Count && SpanCm + CentreStepCm[j + 1] <= AllowedCm)
		{
			SpanCm += CentreStepCm[++j];
		}
		if (j <= Prev)
		{
			j = Prev + 1;   // never stall: one step is the floor
		}
		Nodes.Add(FMath::Min(j, Count - 1));
		i = j + 1;
	}
	if (Nodes.Last() != Count - 1)
	{
		Nodes.Add(Count - 1);
	}

	// CLOSE THE LOOP. BuildCentreline stops one step short of the start line - it
	// emits a sample per step and the last step's end IS the start - so a ribbon built
	// from consecutive pairs leaves that final gap open. The slabs hid it behind their
	// 15% overlap; the ribbon does not overlap and cannot.
	//
	// Not for the stage, which is point to point and whose two ends are half a
	// kilometre apart. Joining them would draw a road through the countryside.
	const bool bCloseLoop = (Venue != EVehicleTestVenue::RallyStage);
	if (bCloseLoop && Nodes.Num() > 2)
	{
		Nodes.Add(0);
	}

	// TANGENTS FROM THE NEIGHBOURS, which is what makes the joints smooth rather than
	// merely gapless. A tangent taken from the segment's own direction alone is
	// discontinuous at every corner entry; taken across the node it is C1, and the
	// deformation on both sides of the joint then agrees on the slope there.
	auto NodeTangent = [this, &Nodes, Count](int32 NodeIdx) -> FVector
	{
		// Neighbours WRAP when the venue is a loop, so the tangent at the start line
		// is continuous with the run-in rather than being clamped flat. Without this
		// the closing joint is smooth on one side and a corner on the other.
		const bool bWrap = (Venue != EVehicleTestVenue::RallyStage);
		const int32 Last = Nodes.Num() - 1;
		const int32 Here = Nodes[NodeIdx];
		const int32 Before = Nodes[(NodeIdx > 0) ? (NodeIdx - 1) : (bWrap ? FMath::Max(Last - 1, 0) : 0)];
		const int32 After = Nodes[(NodeIdx < Last) ? (NodeIdx + 1) : (bWrap ? 1 : Last)];

		FVector Dir = CentrePos[After] - CentrePos[Before];
		if (Dir.IsNearlyZero())
		{
			Dir = CentrePos[FMath::Min(Here + 1, Count - 1)] - CentrePos[Here];
		}

		// Magnitude matters: a cubic Hermite wants the tangent scaled to the span it
		// covers, or the curve either flattens or overshoots between the nodes.
		const float SpanCm = (CentrePos[After] - CentrePos[Here]).Size()
			+ (CentrePos[Here] - CentrePos[Before]).Size();
		return Dir.GetSafeNormal() * FMath::Max(SpanCm * 0.5f, 1.f);
	};

	const float ThicknessCm = 20.f;

	// THE ROLLED FRAME, which the kerbs were missing entirely. They were offset by a
	// right vector built from heading alone, so on a banked corner they sat flat beside
	// a tilted road - floating on the low side, buried on the high side, and never
	// parallel to the surface a wheel would put over them.
	//
	// Positive roll lowers the road's right, which is the same convention the carve
	// uses (RoadZ = CentreZ - lat*tan(roll)). So the rolled right leans DOWN by the
	// roll angle, and the rolled up leans with it - rotating both about the forward
	// axis rather than translating them.
	auto RolledFrame = [this](int32 At, FVector& OutRight, FVector& OutUp)
	{
		const float HeadingRad = FMath::DegreesToRadians(CentreHeadingDeg[At]);
		const float RollRad = FMath::DegreesToRadians(CentreRollDeg[At]);
		const FVector FlatRight(-FMath::Sin(HeadingRad), FMath::Cos(HeadingRad), 0.f);
		const FVector FlatUp(0.f, 0.f, 1.f);

		OutRight = (FlatRight * FMath::Cos(RollRad)) - (FlatUp * FMath::Sin(RollRad));
		OutUp = (FlatUp * FMath::Cos(RollRad)) + (FlatRight * FMath::Sin(RollRad));
	};

	// One lane of ribbon, offset laterally in that frame. The road is the lane at zero
	// offset; a kerb is the same thing pushed out to the edge and raised a little.
	auto LayLane = [this, &NodeTangent](int32 A, int32 B, int32 NodeA, int32 NodeB,
		const FVector& OffsetA, const FVector& OffsetB,
		float WidthA, float WidthB, float ThickCm, const FLinearColor& Colour) -> USplineMeshComponent*
	{
		USplineMeshComponent* Piece = NewObject<USplineMeshComponent>(this, NAME_None, RF_Transactional);
		Piece->SetMobility(EComponentMobility::Movable);
		Piece->SetStaticMesh(CubeMesh);
		Piece->SetForwardAxis(ESplineMeshAxis::X, false);

		Piece->SetStartAndEnd(CentrePos[A] + OffsetA, NodeTangent(NodeA),
			CentrePos[B] + OffsetB, NodeTangent(NodeB), false);
		Piece->SetStartScale(FVector2D(WidthA / 100.f, ThickCm / 100.f), false);
		Piece->SetEndScale(FVector2D(WidthB / 100.f, ThickCm / 100.f), false);
		Piece->SetStartRoll(FMath::DegreesToRadians(CentreRollDeg[A]), false);
		Piece->SetEndRoll(FMath::DegreesToRadians(CentreRollDeg[B]), false);

		Piece->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Piece->SetCollisionObjectType(ECC_WorldStatic);
		Piece->SetCollisionResponseToAllChannels(ECR_Block);
		Piece->bUseDefaultCollision = false;

		if (BaseMaterial != nullptr)
		{
			UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(BaseMaterial, Piece);
			if (Mat != nullptr)
			{
				Mat->SetVectorParameterValue(TEXT("Color"), Colour);
				Piece->SetMaterial(0, Mat);
			}
		}

		Piece->RegisterComponent();
		Piece->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		AddInstanceComponent(Piece);
		Piece->UpdateMesh();
		RibbonPieces.Add(Piece);
		return Piece;
	};

	for (int32 n = 0; n + 1 < Nodes.Num(); ++n)
	{
		const int32 A = Nodes[n];
		const int32 B = Nodes[n + 1];
		if (A == B)
		{
			continue;
		}

		LayLane(A, B, n, n + 1, FVector::ZeroVector, FVector::ZeroVector,
			CentreWidthCm[A], CentreWidthCm[B], ThicknessCm,
			CentreGravel[A] ? ColourGravel : ColourTarmac);

		// KERB GEOMETRY, needed by the fillet below as well as by the kerb itself.
		const float KerbWidthCm = 80.f;
		const float KerbThickCm = 16.f;
		// Raised by half the difference in thickness so the kerb's TOP sits just above
		// the road's, rather than its centre sitting level with it.
		const float KerbRaiseCm = ((ThicknessCm - KerbThickCm) * 0.5f) + 4.f;
		const float KerbTopCm = KerbRaiseCm + (KerbThickCm * 0.5f);

		const bool bKerbHere = bKerbs && !CentreGravel[A]
			&& (!FMath::IsNearlyZero(CentreRollDeg[A], 0.01f) || !FMath::IsNearlyZero(CentreRollDeg[B], 0.01f));

		// EDGE RAMPS, laid before the kerbs so a kerb sits on top where the two meet.
		//
		// Same construction as a kerb - a lane pushed out in the road's own rolled frame -
		// but rolled FURTHER so it falls away outboard, turning the edge wall into a slope.
		//
		// IT HAS TO START OUTBOARD OF THE KERB, and getting that wrong is worse than not
		// having it. A kerb stands only 4 cm above the road, but a fillet that begins at
		// the road edge passes UNDERNEATH it and by the time it emerges 80 cm out it has
		// already fallen 12.4 cm. From the run-off the kerb is then a 16.4 cm wall - a
		// sports car with 12 cm of clearance cannot get back over it, which is exactly the
		// reported symptom. So where a kerb exists the fillet begins at the kerb's outer
		// edge and its top starts level with the kerb's top, and the slope is continuous
		// from the ground to the highest thing at the road edge.
		if (bEdgeRamps && EdgeRampWidthCm > KINDA_SMALL_NUMBER)
		{
			const float InnerLatCm = bKerbHere ? KerbWidthCm : 0.f;
			const float InnerTopCm = bKerbHere ? KerbTopCm : (ThicknessCm * 0.5f);

			// Top surface runs from the inner top down to the carved ground level.
			const float DropCm = InnerTopCm + RoadClearanceCm;
			const float ExtraRollRad = FMath::Asin(
				FMath::Clamp(DropCm / EdgeRampWidthCm, -1.f, 1.f));

			FVector RightA, UpA, RightB, UpB;
			RolledFrame(A, RightA, UpA);
			RolledFrame(B, RightB, UpB);

			// Positioned by its centre, so drop half the slab's thickness below the
			// midpoint of the top surface. The thickness hangs below and is what bridges
			// terrain that does not meet the road - see EdgeRampThicknessCm.
			const float TopMidCm = InnerTopCm - (DropCm * 0.5f);
			const float RaiseCm = TopMidCm - (EdgeRampThicknessCm * 0.5f);

			// Pulled in by a hand's width so surfaces overlap rather than meet, which is
			// the difference between a seam and a lip.
			const float OverlapCm = 10.f;
			const float LatA = (CentreWidthCm[A] * 0.5f) + InnerLatCm
				+ (EdgeRampWidthCm * 0.5f) - OverlapCm;
			const float LatB = (CentreWidthCm[B] * 0.5f) + InnerLatCm
				+ (EdgeRampWidthCm * 0.5f) - OverlapCm;

			for (int32 SideIdx = 0; SideIdx < 2; ++SideIdx)
			{
				const float Side = (SideIdx == 0) ? 1.f : -1.f;   // +1 is the road's right

				USplineMeshComponent* Ramp = LayLane(A, B, n, n + 1,
					(RightA * (Side * LatA)) + (UpA * RaiseCm),
					(RightB * (Side * LatB)) + (UpB * RaiseCm),
					EdgeRampWidthCm, EdgeRampWidthCm, EdgeRampThicknessCm,
					ColourGravel);

				// LayLane sets the road's own roll; add the fall on top of it. Positive
				// roll drops the right-hand side (see RolledFrame), so the sign is the
				// side itself and each fillet falls away from the road rather than one of
				// them climbing.
				if (Ramp != nullptr)
				{
					Ramp->SetStartRoll(
						FMath::DegreesToRadians(CentreRollDeg[A]) + (Side * ExtraRollRad), false);
					Ramp->SetEndRoll(
						FMath::DegreesToRadians(CentreRollDeg[B]) + (Side * ExtraRollRad), false);
					Ramp->UpdateMesh();
				}
			}
		}

		// KERBS, in the same frame and with the same roll, so they lie flush with the
		// road edge instead of beside it. Corners only - a kerb marks the line and is
		// something to put a wheel over. Gravel gets none: a kerb on a rally stage is
		// a rock.
		if (bKerbHere)
		{
			const float RaiseCm = KerbRaiseCm;

			FVector RightA, UpA, RightB, UpB;
			RolledFrame(A, RightA, UpA);
			RolledFrame(B, RightB, UpB);

			const float LatA = (CentreWidthCm[A] * 0.5f) + (KerbWidthCm * 0.5f);
			const float LatB = (CentreWidthCm[B] * 0.5f) + (KerbWidthCm * 0.5f);

			// Alternating red and white in runs of a couple of components, so the
			// pattern reads at speed rather than strobing.
			const FLinearColor KerbColour = ((n / 2) % 2 == 0) ? ColourKerbA : ColourKerbB;

			LayLane(A, B, n, n + 1,
				(RightA * LatA) + (UpA * RaiseCm), (RightB * LatB) + (UpB * RaiseCm),
				KerbWidthCm, KerbWidthCm, KerbThickCm, KerbColour);

			LayLane(A, B, n, n + 1,
				(RightA * -LatA) + (UpA * RaiseCm), (RightB * -LatB) + (UpB * RaiseCm),
				KerbWidthCm, KerbWidthCm, KerbThickCm, KerbColour);
		}
	}

	UE_LOG(LogTestCircuit, Log,
		TEXT("  ribbon: %d components over %d centreline steps, %.1f m average span "
			 "(chord tolerance %.1f cm, cap %.1f m)."),
		RibbonPieces.Num(), Count,
		(RibbonPieces.Num() > 0) ? (CentreLengthCm * 0.01f / RibbonPieces.Num()) : 0.f,
		TolCm, RibbonMaxSpanCm * 0.01f);

	// The two chains, printed side by side. They disagreed silently through three
	// rounds of "the track is in the wrong place", so from now on it is checked out
	// loud: the ribbon's first spline point resolved into world, against the same
	// centreline point put through GetActorTransform - which is what the slabs and the
	// carve both use.
	if (RibbonPieces.Num() > 0 && RibbonPieces[0] != nullptr)
	{
		const USplineMeshComponent* First = RibbonPieces[0];
		const FVector RibbonWS = First->GetComponentTransform().TransformPosition(First->SplineParams.StartPos);
		const FVector ExpectWS = GetActorTransform().TransformPosition(CentrePos[0]);
		const float DriftCm = (RibbonWS - ExpectWS).Size();

		if (DriftCm > 1.f)
		{
			UE_LOG(LogTestCircuit, Error,
				TEXT("  RIBBON IS NOT WHERE THE ROAD IS: start resolves to (%.0f, %.0f, %.0f) but the "
					 "centreline puts it at (%.0f, %.0f, %.0f) - %.2f m of drift. The component is "
					 "not inheriting the actor transform."),
				RibbonWS.X, RibbonWS.Y, RibbonWS.Z, ExpectWS.X, ExpectWS.Y, ExpectWS.Z, DriftCm * 0.01f);
		}
		else
		{
			UE_LOG(LogTestCircuit, Log,
				TEXT("  ribbon start agrees with the centreline to %.1f cm."), DriftCm);
		}
	}
}

FString AVehicleTestCircuit::LayoutSignature() const
{
	// Enough to notice a change that invalidates the carve: the venue, the actor's
	// place in the world, and the layout itself. Cheap, and it does not have to be
	// cryptographic - it only has to differ when the carve would.
	FString Sig = FString::Printf(TEXT("v%d|%s|"), int32(Venue), *GetActorTransform().ToString());
	for (const FCircuitSegment& Seg : Layout)
	{
		Sig += FString::Printf(TEXT("%.0f,%.0f,%.0f,%.1f,%.0f,%d;"),
			Seg.LengthCm, Seg.RadiusCm, Seg.RiseCm, Seg.BankDeg, Seg.WidthCm, Seg.bGravel ? 1 : 0);
	}
	return Sig;
}

void AVehicleTestCircuit::SculptAndBuild()
{
#if WITH_EDITOR
	SculptLandscape();
#endif
	Build();
}

void AVehicleTestCircuit::BuildStations()
{
	if (CentreSegment.Num() != CentrePos.Num())
	{
		UE_LOG(LogTestCircuit, Warning,
			TEXT("  stations skipped: %d segment tags for %d centreline samples."),
			CentreSegment.Num(), CentrePos.Num());
		return;
	}

	// COUNTED AND REPORTED. "I do not see any obstacles" has two causes that look
	// identical from the viewport - the venue does not set any stations, or it does and
	// they built nothing - and only the log can tell them apart.
	int32 StationCount = 0;
	int32 StationPieces = 0;
	const int32 PiecesBefore = Spawned.Num();

	FRandomStream Stream(ScenerySeed + 977);

	auto RightAt = [this](int32 At) -> FVector
	{
		const float HeadingRad = FMath::DegreesToRadians(CentreHeadingDeg[At]);
		return FVector(-FMath::Sin(HeadingRad), FMath::Cos(HeadingRad), 0.f);
	};

	// Sample ranges per segment, so a station knows how much trail it owns.
	for (int32 SegIndex = 0; SegIndex < Layout.Num(); ++SegIndex)
	{
		const ECircuitStation Station = Layout[SegIndex].Station;
		if (Station == ECircuitStation::None)
		{
			continue;
		}
		++StationCount;

		int32 First = INDEX_NONE, Last = INDEX_NONE;
		for (int32 i = 0; i < CentreSegment.Num(); ++i)
		{
			if (CentreSegment[i] == SegIndex)
			{
				if (First == INDEX_NONE) { First = i; }
				Last = i;
			}
		}
		if (First == INDEX_NONE)
		{
			continue;
		}

#if WITH_EDITOR
		PieceFolder = FName(*FString::Printf(TEXT("%s/Stations"), *GetName()));
#endif

		const FVector FirstRight = RightAt(First);
		const float HalfWidth = CentreWidthCm[First] * 0.5f;

		switch (Station)
		{
		case ECircuitStation::RockGarden:
		{
			// 20-60 cm, BURIED TO A QUARTER of its height, which is the doc's
			// specification and not a detail: a rock resting on the surface presents a
			// corner to the tyre and either stops it or launches it, where one sunk into
			// the ground presents a sloped face the tyre can climb. The whole point of
			// the station is the contact normal moving independently under each wheel,
			// and that only happens if the wheel stays on the rock.
			// SIZED AGAINST THE WHEEL. The specification said 20 to 60 cm, and buried to
			// a quarter that is 15 to 45 cm of exposed face against a 33 cm wheel -
			// under half a radius at the small end, which a car drives over without
			// noticing. That is why the course looked empty: the rocks were there and
			// they were scenery.
			const int32 RockN = FMath::Max(FMath::RoundToInt((Last - First + 1) * 3.0f), 14);
			for (int32 r = 0; r < RockN; ++r)
			{
				const int32 At = Stream.RandRange(First, Last);
				const float Size = Stream.FRandRange(45.f, 110.f);
				const float Lat = Stream.FRandRange(-0.85f, 0.85f) * (CentreWidthCm[At] * 0.5f);
				const FVector Base = CentrePos[At] + (RightAt(At) * Lat)
					- FVector(0.f, 0.f, Size * 0.25f);

				SpawnPiece(CubeMesh, Base, FRotator(
					Stream.FRandRange(-25.f, 25.f),
					Stream.FRandRange(0.f, 360.f),
					Stream.FRandRange(-25.f, 25.f)),
					FVector(Size, Size * Stream.FRandRange(0.7f, 1.4f), Size), ColourRock);
			}
			break;
		}

		case ECircuitStation::LedgeStep:
		{
			// One step across the full width at the top of the climb. Approach angle,
			// and whether the front tyre climbs rather than pushes.
			const int32 At = Last;
			SpawnPiece(CubeMesh, CentrePos[At] + FVector(0.f, 0.f, 12.f),
				FRotator(0.f, CentreHeadingDeg[At], 0.f),
				FVector(60.f, CentreWidthCm[At], 25.f), ColourRock);
			break;
		}

		case ECircuitStation::GradeTest:
		{
			// Ramps of increasing pitch laid end to end along the trail. Each is a thin
			// slab tilted about its pitch axis and lifted by half its length times the
			// sine, which puts the trailing edge on the ground and the leading edge up -
			// the same construction the proving ground's jump and RTI ramps use.
			//
			// 15 to 35 degrees in 5 degree steps. The top of that range is deliberately
			// AT the traction ceiling: atan(0.70) for the offroad tyre is 35 degrees, so
			// the last ramp is the one no amount of gearing gets up.
			const float FirstDeg = 15.f;
			const float LastDeg = 35.f;
			const float StepDeg = 5.f;
			const float RunCm = 900.f;      // per ramp, about three wheelbases

			const int32 Steps = FMath::Max(FMath::RoundToInt((LastDeg - FirstDeg) / StepDeg) + 1, 1);
			const float SpanCm = Steps * RunCm;

			// Centred in the segment, so there is run-up before and run-out after.
			const float SegLenCm = FMath::Max(Layout[SegIndex].LengthCm, SpanCm + 4000.f);
			const float StartFrac = FMath::Clamp(0.5f - (SpanCm * 0.5f) / SegLenCm, 0.f, 1.f);

			for (int32 k = 0; k < Steps; ++k)
			{
				const float Deg = FirstDeg + StepDeg * k;
				const float Frac = StartFrac + ((k + 0.5f) * RunCm) / SegLenCm;
				const int32 At = FMath::Clamp(
					First + FMath::RoundToInt((Last - First) * Frac), First, Last);

				const float LiftCm = (RunCm * 0.5f) * FMath::Sin(FMath::DegreesToRadians(Deg));
				SpawnPiece(CubeMesh, CentrePos[At] + FVector(0.f, 0.f, LiftCm),
					FRotator(Deg, CentreHeadingDeg[At], 0.f),
					FVector(RunCm, CentreWidthCm[At], 12.f), ColourRock);
			}

			UE_LOG(LogTestCircuit, Log,
				TEXT("  grade test on segment %d: %d ramps, %.0f to %.0f deg in %.0f deg steps. "
					 "Four driven wheels on a %.2f mu tyre top out near atan(mu) = %.0f deg, so "
					 "stopping well short of that is a differential or load-transfer problem "
					 "rather than a grip one."),
				SegIndex, Steps, FirstDeg, LastDeg, StepDeg, 0.70f,
				FMath::RadiansToDegrees(FMath::Atan(0.70f)));
			break;
		}

		case ECircuitStation::BreakoverRidge:
		{
			// Symmetric ridges: two slabs meeting at a crest, each tilted by the ramp
			// angle, so the pair reads as a roof. The published breakover figure is the
			// full included angle at the apex, which is twice the ramp angle.
			//
			// 8 to 20 degrees brackets the offroad car's 11.3 deg limit (30 cm of
			// clearance over a 300 cm wheelbase, atan(2h/L)), so the first two should be
			// crested and the last two should ground the frame. That is the measurement:
			// the angle at which it stops clearing, read against the number the hulls
			// were built for.
			const float FirstDeg = 8.f;
			const float LastDeg = 20.f;
			const float StepDeg = 4.f;
			const float FaceCm = 320.f;     // each face, so a crest about 6.4 m across
			const float GapCm = 1800.f;     // room to settle between ridges

			const int32 Ridges = FMath::Max(FMath::RoundToInt((LastDeg - FirstDeg) / StepDeg) + 1, 1);
			const float PitchCm = (FaceCm * 2.f) + GapCm;
			const float SpanCm = Ridges * PitchCm;
			const float SegLenCm = FMath::Max(Layout[SegIndex].LengthCm, SpanCm);
			const float StartFrac = FMath::Clamp(0.5f - (SpanCm * 0.5f) / SegLenCm, 0.f, 1.f);

			for (int32 k = 0; k < Ridges; ++k)
			{
				const float Deg = FirstDeg + StepDeg * k;
				const float SinD = FMath::Sin(FMath::DegreesToRadians(Deg));
				const float CrestCm = (FaceCm * 0.5f) * SinD;   // apex height above the trail

				// Up-face then down-face, each half the crest's length, their inner ends
				// meeting at the apex. Offsetting by a quarter of the pitch puts the two
				// slabs either side of the sample point.
				for (int32 Side = 0; Side < 2; ++Side)
				{
					const float Along = (Side == 0 ? -0.25f : 0.25f) * (FaceCm * 2.f);
					const float Frac = StartFrac + ((k + 0.5f) * PitchCm + Along) / SegLenCm;
					const int32 At = FMath::Clamp(
						First + FMath::RoundToInt((Last - First) * Frac), First, Last);

					SpawnPiece(CubeMesh, CentrePos[At] + FVector(0.f, 0.f, CrestCm * 0.5f),
						FRotator(Side == 0 ? Deg : -Deg, CentreHeadingDeg[At], 0.f),
						FVector(FaceCm, CentreWidthCm[At], 12.f), ColourRock);
				}
			}

			UE_LOG(LogTestCircuit, Log,
				TEXT("  breakover ridges on segment %d: %d crests, ramp %.0f to %.0f deg "
					 "(published breakover %.0f to %.0f deg). A 30 cm clearance on a 300 cm "
					 "wheelbase clears a ramp up to atan(2h/L) = %.1f deg, so it should crest "
					 "the first two and hang the frame on the last."),
				SegIndex, Ridges, FirstDeg, LastDeg, FirstDeg * 2.f, LastDeg * 2.f,
				FMath::RadiansToDegrees(FMath::Atan(2.f * 30.f / 300.f)));
			break;
		}

		case ECircuitStation::LogCrossing:
		{
			// Two 40 cm logs, 3 m apart: single-wheel obstacles in sequence, so the
			// second arrives while the car is still settling from the first.
			for (int32 L = 0; L < 2; ++L)
			{
				const int32 At = FMath::Clamp(First + FMath::RoundToInt((Last - First) * (L ? 0.62f : 0.28f)), First, Last);
				SpawnPiece(CylinderMesh, CentrePos[At] + FVector(0.f, 0.f, 14.f),
					FRotator(0.f, CentreHeadingDeg[At], 90.f),
					FVector(40.f, 40.f, CentreWidthCm[At] * 1.05f), ColourWood);
			}
			break;
		}

		case ECircuitStation::RutSection:
		{
			// THREE RIDGES, LEAVING TWO CHANNELS at track width. A rut is a depression
			// and everything here is additive, so the ruts are built as the ground
			// BETWEEN raised ridges - geometrically the same thing to a wheel, and it
			// avoids needing a subtractive carve for one station.
			const float TrackHalf = 75.f;
			for (int32 i = First; i <= Last; ++i)
			{
				const FVector R = RightAt(i);
				for (int32 Ridge = -1; Ridge <= 1; ++Ridge)
				{
					const float Lat = (Ridge == 0) ? 0.f : (Ridge * (TrackHalf * 2.f));
					SpawnPiece(CubeMesh, CentrePos[i] + (R * Lat) + FVector(0.f, 0.f, 10.f),
						FRotator(CentreGradeDeg[i], CentreHeadingDeg[i], 0.f),
						FVector(CentreStepCm[i] * 1.1f, 55.f, 20.f), ColourRock);
				}
			}
			break;
		}

		case ECircuitStation::RockCrawl:
		{
			// SHELVES, then BOULDERS, and the arrangement is the obstacle rather than
			// the pieces. A field of rock scattered at random averages out: every wheel
			// meets roughly the same thing and the car drives across it flat.
			//
			// So the shelves alternate which side is higher, which forces diagonal
			// articulation - one front and the opposite rear taking load while the other
			// two go light. That is the case an open differential cannot answer and the
			// case a raycast contact model has to get right, which is the whole reason
			// this station exists on a climb.
			const int32 Shelves = FMath::Clamp((Last - First) / 3, 3, 7);
			for (int32 sh = 0; sh < Shelves; ++sh)
			{
				const float Frac = (sh + 0.5f) / Shelves;
				const int32 At = FMath::Clamp(First + FMath::RoundToInt((Last - First) * Frac), First, Last);
				const float HighSide = ((sh % 2) == 0) ? 1.f : -1.f;
				const float Wid = CentreWidthCm[At];

				// Two half-width slabs at different heights, so the shelf itself is
				// off-camber rather than a step the car meets square.
				for (int32 Halfs = 0; Halfs < 2; ++Halfs)
				{
					const float Side = (Halfs == 0) ? HighSide : -HighSide;
					const float Rise = (Halfs == 0) ? Stream.FRandRange(45.f, 75.f)
													: Stream.FRandRange(18.f, 34.f);

					SpawnPiece(CubeMesh,
						CentrePos[At] + (RightAt(At) * (Side * Wid * 0.25f))
							+ FVector(0.f, 0.f, Rise * 0.35f),
						FRotator(CentreGradeDeg[At], CentreHeadingDeg[At],
							Stream.FRandRange(-6.f, 6.f)),
						FVector(Stream.FRandRange(120.f, 200.f), Wid * 0.5f, Rise), ColourRock);
				}
			}

			// Boulders in the gaps, big enough to have to be climbed or driven round.
			const int32 Boulders = FMath::Max((Last - First), 10);
			for (int32 b = 0; b < Boulders; ++b)
			{
				const int32 At = Stream.RandRange(First, Last);
				const float Size = Stream.FRandRange(65.f, 140.f);
				const float Lat = Stream.FRandRange(-0.7f, 0.7f) * (CentreWidthCm[At] * 0.5f);

				SpawnPiece(CubeMesh,
					CentrePos[At] + (RightAt(At) * Lat) - FVector(0.f, 0.f, Size * 0.28f),
					FRotator(Stream.FRandRange(-32.f, 32.f), Stream.FRandRange(0.f, 360.f),
						Stream.FRandRange(-32.f, 32.f)),
					FVector(Size, Size * Stream.FRandRange(0.65f, 1.5f), Size * Stream.FRandRange(0.7f, 1.1f)),
					ColourRock);
			}
			break;
		}

		case ECircuitStation::WhoopsRun:
		{
			// Six humps at 5 m spacing. Large-amplitude damping at a speed where the
			// wheels should be working and the body should not.
			const float SpacingCm = 500.f;
			const float RunCm = FMath::Max((Last - First) * FMath::Max(CentreStepCm[First], 1.f), SpacingCm);
			const int32 Humps = FMath::Clamp(FMath::FloorToInt(RunCm / SpacingCm), 3, 10);
			for (int32 Hump = 0; Hump < Humps; ++Hump)
			{
				const float Frac = (Hump + 0.5f) / Humps;
				const int32 At = FMath::Clamp(First + FMath::RoundToInt((Last - First) * Frac), First, Last);
				SpawnPiece(CylinderMesh, CentrePos[At] + FVector(0.f, 0.f, -6.f),
					FRotator(0.f, CentreHeadingDeg[At], 90.f),
					FVector(46.f, 46.f, CentreWidthCm[At]), ColourRock);
			}
			break;
		}

		default:
			break;
		}
	}

	StationPieces = Spawned.Num() - PiecesBefore;

	if (StationCount == 0)
	{
		UE_LOG(LogTestCircuit, Log,
			TEXT("  no stations on this venue - only the offroad course sets any."));
	}
	else
	{
		UE_LOG(LogTestCircuit, Log,
			TEXT("  stations: %d segments carrying geometry, %d pieces placed."),
			StationCount, StationPieces);
	}

#if WITH_EDITOR
	PieceFolder = NAME_None;
#endif
}

void AVehicleTestCircuit::Build()
{
	if (GetWorld() == nullptr || !LoadAssets())
	{
		return;
	}

	Clear();
	BuildCentreline();
	SmoothCentrelineHeight();

	if (CentrePos.Num() == 0)
	{
		return;
	}

	// The road is laid as flat slabs stepped along the centreline, so a corner is a
	// polygon rather than a curve. Each slab is 15% longer than its step so
	// consecutive ones overlap: without that, a rising or turning joint opens a gap,
	// and a raycast suspension reads a gap as a pothole.
	const float Overlap = 1.15f;
	int32 Aborted = 0;

	if (bRoadRibbon)
	{
		BuildRoadRibbon();
	}

#if WITH_EDITOR
	PieceFolder = FName(*FString::Printf(TEXT("%s/Road"), *GetName()));
#endif

	for (int32 i = 0; i < CentrePos.Num(); ++i)
	{
		if (Spawned.Num() >= MaxPieces)
		{
			++Aborted;
			continue;
		}

		const FRotator Rot(CentreGradeDeg[i], CentreHeadingDeg[i], CentreRollDeg[i]);

		if (!bRoadRibbon)
		{
			SpawnPiece(CubeMesh, CentrePos[i], Rot,
				FVector(CentreStepCm[i] * Overlap, CentreWidthCm[i], 20.f),
				CentreGravel[i] ? ColourGravel : ColourTarmac);
		}

		// Kerbs on corners only - they mark the line and they are something to put a
		// wheel over and feel. Gravel gets none: a kerb on a rally stage is a rock.
		// Slab kerbs belong to the slab road: the ribbon lays its own, in the rolled
		// frame, which is the whole point of it.
		if (!bRoadRibbon && bKerbs && !CentreGravel[i] && !FMath::IsNearlyZero(CentreRollDeg[i], 0.01f))
		{
			const float HeadingRad = FMath::DegreesToRadians(CentreHeadingDeg[i]);
			const FVector Right(-FMath::Sin(HeadingRad), FMath::Cos(HeadingRad), 0.f);
			const float Side = (CentreWidthCm[i] * 0.5f) + 40.f;
			const FLinearColor KerbColour = ((i / 2) % 2 == 0) ? ColourKerbA : ColourKerbB;

			SpawnPiece(CubeMesh, CentrePos[i] + (Right * Side) + FVector(0.f, 0.f, 6.f), Rot,
				FVector(CentreStepCm[i] * Overlap, 80.f, 14.f), KerbColour);
			SpawnPiece(CubeMesh, CentrePos[i] - (Right * Side) + FVector(0.f, 0.f, 6.f), Rot,
				FVector(CentreStepCm[i] * Overlap, 80.f, 14.f), KerbColour);
		}
	}

#if WITH_EDITOR
	PieceFolder = FName(*FString::Printf(TEXT("%s/Markers"), *GetName()));
#endif
	SpawnPiece(CubeMesh, CentrePos[0] + FVector(0.f, 0.f, 14.f), FRotator::ZeroRotator,
		FVector(120.f, CentreWidthCm[0], 6.f), ColourStart, false);

	BuildScenery();
	BuildStations();

	// STALE SCULPT, said out loud. This is the failure that keeps reading as the track
	// being in the wrong place: the road goes exactly where the actor puts it and the
	// terrain stays exactly where it was carved, so when those disagree nothing looks
	// broken - there is simply a road over here and a corridor over there.
	if (TargetLandscape != nullptr)
	{
		const FString Now = LayoutSignature();
		if (SculptedSignature.IsEmpty())
		{
			UE_LOG(LogTestCircuit, Warning,
				TEXT("  the landscape has NEVER been sculpted for this layout. Press Sculpt And "
					 "Build, or Sculpt Landscape and then Build."));
		}
		else if (SculptedSignature != Now)
		{
			UE_LOG(LogTestCircuit, Warning,
				TEXT("  the landscape was sculpted for a DIFFERENT layout, venue or actor "
					 "position. The road is where this actor puts it and the carve is where it "
					 "was left. Press Sculpt And Build."));
		}
	}

	UE_LOG(LogTestCircuit, Log,
		TEXT("Circuit: %.3f km over %d segments, %d pieces. Estimated lap %.0f s at %.2f g."),
		CentreLengthCm * 0.00001f, Layout.Num(), Spawned.Num(), CentreLapSeconds, LapEstimateGrip);

	UE_LOG(LogTestCircuit, Log,
		TEXT("  closes to %.2f m horizontally, %.2f m in height."),
		CentreCloseErrorCm * 0.01f, CentreCloseHeightCm * 0.01f);

	// Footprint RELATIVE TO THIS ACTOR, because the venues occupy different places and
	// nothing said so. The combined lap ran to -1092 m in X; the circuit runs to +652.
	// An actor left where the old layout wanted it puts the new one somewhere the
	// landscape was never sculpted, which reads as "the track is in the wrong place".
	{
		FVector Min(BIG_NUMBER), Max(-BIG_NUMBER);
		for (const FVector& P : CentrePos)
		{
			Min = Min.ComponentMin(P);
			Max = Max.ComponentMax(P);
		}
		UE_LOG(LogTestCircuit, Log,
			TEXT("  footprint from this actor: X %+.0f..%+.0f m, Y %+.0f..%+.0f m (%.0f x %.0f m). "
				 "Sculpt Landscape must be re-run whenever this moves."),
			Min.X * 0.01f, Max.X * 0.01f, Min.Y * 0.01f, Max.Y * 0.01f,
			(Max.X - Min.X) * 0.01f, (Max.Y - Min.Y) * 0.01f);
	}

	// Elevation range, because there was no way to tell from the level whether what
	// you were looking at was this build or a stale one baked into the map. The slabs
	// are saved actors and the sculpt is saved heightmap, so opening a level shows
	// whatever was built last time, however old. This is the number that changed.
	{
		float MinZ = BIG_NUMBER, MaxZ = -BIG_NUMBER;
		for (const FVector& P : CentrePos)
		{
			MinZ = FMath::Min(MinZ, P.Z);
			MaxZ = FMath::Max(MaxZ, P.Z);
		}
		if (CentrePos.Num() > 0)
		{
			const float RangeM = (MaxZ - MinZ) * 0.01f;
			const float PerKmM = (CentreLengthCm > 0.f) ? (RangeM / (CentreLengthCm * 0.00001f)) : 0.f;
			UE_LOG(LogTestCircuit, Log,
				TEXT("  elevation %.1f m of range (%.1f m per km), from %.1f to %.1f m."),
				RangeM, PerKmM, MinZ * 0.01f, MaxZ * 0.01f);
		}
	}

	if (Aborted > 0)
	{
		UE_LOG(LogTestCircuit, Warning,
			TEXT("  hit the %d piece cap with %d slabs still to lay - the lap is INCOMPLETE. "
				 "Raise MaxPieces, or raise SlabLengthCm, which costs smoothness rather than track."),
			MaxPieces, Aborted);
	}
	if (TargetLandscape == nullptr)
	{
		UE_LOG(LogTestCircuit, Warning,
			TEXT("  no TargetLandscape set, so the road is floating on whatever is underneath it. "
				 "Assign one and press Sculpt Landscape."));
	}
}

void AVehicleTestCircuit::BuildScenery()
{
	if (CentrePos.Num() == 0)
	{
		return;
	}

	// Seeded, so a rebuild puts everything back where it was. Scenery that moves
	// between builds makes two recordings of the same lap look like two laps.
	FRandomStream Stream(ScenerySeed);

	auto RightOf = [this](int32 At)
	{
		const float H = FMath::DegreesToRadians(CentreHeadingDeg[At]);
		return FVector(-FMath::Sin(H), FMath::Cos(H), 0.f);
	};

#if WITH_EDITOR
	PieceFolder = FName(*FString::Printf(TEXT("%s/Buildings"), *GetName()));
#endif
	if (bBuildings)
	{
		for (int32 i = 0; i < BuildingCount && Spawned.Num() < MaxPieces; ++i)
		{
			const int32 At = Stream.RandRange(0, CentrePos.Num() - 1);
			if (CentreGravel[At] != 0) { continue; }   // the rally loop is countryside

			const float Side = Stream.FRandRange(0.f, 1.f) < 0.5f ? -1.f : 1.f;
			const float Offset = (CentreWidthCm[At] * 0.5f) + Stream.FRandRange(2500.f, 6000.f);
			const float Height = Stream.FRandRange(600.f, 3000.f);
			const float Foot = Stream.FRandRange(700.f, 1800.f);

			SpawnPiece(CubeMesh, CentrePos[At] + (RightOf(At) * Side * Offset) + FVector(0.f, 0.f, Height * 0.5f),
				FRotator(0.f, CentreHeadingDeg[At] + Stream.FRandRange(-25.f, 25.f), 0.f),
				FVector(Foot, Foot * Stream.FRandRange(0.8f, 1.6f), Height), ColourBuilding);
		}
	}

	// Trees line the gravel, which is what makes a stage feel narrow even where it
	// is not. Two primitives each - a trunk and a cone - and no collision on the
	// canopy, so brushing one costs a scare rather than the run.
#if WITH_EDITOR
	PieceFolder = FName(*FString::Printf(TEXT("%s/Trees"), *GetName()));
#endif
	if (bTrees)
	{
		for (int32 i = 0; i < TreeCount && Spawned.Num() < MaxPieces - 1; ++i)
		{
			const int32 At = Stream.RandRange(0, CentrePos.Num() - 1);
			// Trees on BOTH surfaces now. Gating them to gravel is what left the sealed
			// circuit standing in an office park: buildings on the tarmac, forest on the
			// stage, and nothing tying the two together. A hillside circuit is trees.
			(void)0;

			const float Side = Stream.FRandRange(0.f, 1.f) < 0.5f ? -1.f : 1.f;
			const float Offset = (CentreWidthCm[At] * 0.5f) + Stream.FRandRange(300.f, 2500.f);
			// Smaller. At 4 to 9 m of trunk with a canopy up to 1.8x on top of that,
			// these stood taller than the road is wide and read as scenery the car was
			// driving through rather than past.
			const float Trunk = Stream.FRandRange(220.f, 520.f);
			const float Canopy = Trunk * Stream.FRandRange(1.0f, 1.5f);
			const FVector Base = CentrePos[At] + (RightOf(At) * Side * Offset);

			SpawnPiece(CylinderMesh, Base + FVector(0.f, 0.f, Trunk * 0.5f), FRotator::ZeroRotator,
				FVector(28.f, 28.f, Trunk), ColourWood);
			SpawnPiece(ConeMesh ? ConeMesh : CubeMesh,
				Base + FVector(0.f, 0.f, Trunk + (Canopy * 0.4f)), FRotator::ZeroRotator,
				FVector(Canopy * 0.62f, Canopy * 0.62f, Canopy), ColourFoliage, false);
		}
	}

	// Logs laid ALONG the edge rather than across it. Across, they are a jump; along,
	// they are the consequence of running wide, which is the more useful thing.
#if WITH_EDITOR
	PieceFolder = FName(*FString::Printf(TEXT("%s/Logs"), *GetName()));
#endif
	if (bLogs)
	{
		for (int32 i = 0; i < LogCount && Spawned.Num() < MaxPieces; ++i)
		{
			const int32 At = Stream.RandRange(0, CentrePos.Num() - 1);
			if (CentreGravel[At] == 0) { continue; }

			const float Side = Stream.FRandRange(0.f, 1.f) < 0.5f ? -1.f : 1.f;
			const float Offset = (CentreWidthCm[At] * 0.5f) + Stream.FRandRange(60.f, 400.f);
			const float Len = Stream.FRandRange(300.f, 700.f);
			const float Dia = Stream.FRandRange(35.f, 60.f);

			// Rolled onto its side: the cylinder's length is its LOCAL Z, so a roll of
			// 90 degrees lays it flat, and the yaw then points it down the road.
			SpawnPiece(CylinderMesh,
				CentrePos[At] + (RightOf(At) * Side * Offset) + FVector(0.f, 0.f, Dia * 0.5f),
				FRotator(0.f, CentreHeadingDeg[At] + Stream.FRandRange(-12.f, 12.f), 90.f),
				FVector(Dia, Dia, Len), ColourWood);
		}
	}

#if WITH_EDITOR
	PieceFolder = FName(*FString::Printf(TEXT("%s/Rocks"), *GetName()));
#endif
	if (bGravelRocks)
	{
		for (int32 i = 0; i < RockCount && Spawned.Num() < MaxPieces; ++i)
		{
			const int32 At = Stream.RandRange(0, CentrePos.Num() - 1);
			if (CentreGravel[At] == 0) { continue; }

			// Just off the racing line rather than on it. A rock in the middle of the
			// road is an obstacle; one at the edge is a consequence, which is the more
			// useful thing for a surface meant to punish running wide.
			const float Side = Stream.FRandRange(0.f, 1.f) < 0.5f ? -1.f : 1.f;
			const float Offset = (CentreWidthCm[At] * 0.45f) + Stream.FRandRange(0.f, 500.f);
			const float Size = Stream.FRandRange(40.f, 120.f);
			const float RockH = Stream.FRandRange(20.f, 60.f);

			SpawnPiece(CubeMesh,
				CentrePos[At] + (RightOf(At) * Side * Offset) + FVector(0.f, 0.f, RockH * 0.2f),
				FRotator(Stream.FRandRange(-15.f, 15.f), Stream.FRandRange(0.f, 90.f),
					Stream.FRandRange(-15.f, 15.f)),
				FVector(Size, Size, RockH), ColourRock);
		}
	}
}

void AVehicleTestCircuit::Clear()
{
	// The ribbon is components on this actor rather than spawned actors, so it needs
	// its own teardown - without this, Build would stack a second road on the first.
	for (USplineMeshComponent* Piece : RibbonPieces)
	{
		if (Piece != nullptr)
		{
			Piece->DestroyComponent();
		}
	}
	RibbonPieces.Reset();

	for (AActor* A : Spawned)
	{
		if (A)
		{
			A->Destroy();
		}
	}
	Spawned.Reset();

	// Then sweep for ORPHANS - tagged pieces this actor no longer tracks. Without
	// this, every earlier build stays in the level forever: the reported symptom was
	// 1720 actors and a forest following a path the current road does not take.
	//
	// Only pieces owned by nobody, or by this actor, are taken. Another circuit actor's
	// live build is left alone, which is what makes three venues in one level workable.
	int32 Orphans = 0;
	if (UWorld* W = GetWorld())
	{
		for (TActorIterator<AActor> It(W); It; ++It)
		{
			AActor* A = *It;
			if (A == nullptr || A == this || !A->Tags.Contains(CircuitPieceTag()))
			{
				continue;
			}

			const AActor* PieceOwner = A->GetOwner();
			if (PieceOwner == nullptr || PieceOwner == this)
			{
				A->Destroy();
				++Orphans;
			}
		}
	}

	if (Orphans > 0)
	{
		UE_LOG(LogTestCircuit, Warning,
			TEXT("Cleared %d ORPHANED pieces left by an earlier build whose actor no longer "
				 "tracks them. That is what a track in the wrong place usually is."), Orphans);
	}
}

// ---------------------------------------------------------------------------
// Landscape
// ---------------------------------------------------------------------------

#if WITH_EDITOR

void AVehicleTestCircuit::SculptLandscape()
{
	if (TargetLandscape == nullptr)
	{
		UE_LOG(LogTestCircuit, Error,
			TEXT("No TargetLandscape assigned. Create one (Landscape mode > New), then set it here."));
		return;
	}

	BuildCentreline();
	if (CentrePos.Num() == 0)
	{
		UE_LOG(LogTestCircuit, Error, TEXT("Empty layout - nothing to carve."));
		return;
	}

	// ULandscapeInfo is declared NO_API - LandscapeInfo.generated.h has
	// DECLARE_CLASS(ULandscapeInfo, ..., NO_API) - so StaticClass() and Cast<> do not
	// link from another module. Reach it only through these two accessors, and hold a
	// raw pointer.
	ULandscapeInfo* Info = TargetLandscape->GetLandscapeInfo();
	if (Info == nullptr)
	{
		Info = TargetLandscape->CreateLandscapeInfo();
	}
	if (Info == nullptr)
	{
		UE_LOG(LogTestCircuit, Error, TEXT("The landscape has no ULandscapeInfo - is it registered in this world?"));
		return;
	}

	int32 MinX = 0, MinY = 0, MaxX = 0, MaxY = 0;
	if (!Info->GetLandscapeExtent(MinX, MinY, MaxX, MaxY))
	{
		UE_LOG(LogTestCircuit, Error, TEXT("The landscape reports no extent - it has no registered components."));
		return;
	}

	const int32 NumX = MaxX - MinX + 1;   // the bounds are INCLUSIVE vertex coords
	const int32 NumY = MaxY - MinY + 1;

	// Landscape space is vertex indices in X and Y, and 1/128ths in Z, all mapped to
	// the world by the actor transform. Verified against LandscapeDataAccess.h:13-14
	// and 24-31: LocalZ = (H - 32768) / 128, WorldZ = ActorZ + LocalZ * ScaleZ. So at
	// the default Z scale of 100, one height unit is 0.78 cm and the range is +/-256 m.
	const FTransform LandscapeToWorld = TargetLandscape->GetActorTransform();
	const float ScaleZ = TargetLandscape->GetActorScale3D().Z;
	if (FMath::IsNearlyZero(ScaleZ))
	{
		UE_LOG(LogTestCircuit, Error, TEXT("The landscape has a zero Z scale - nothing can be written to it."));
		return;
	}

	// Centreline into world space once, since the circuit actor has its own transform.
	const FTransform& CircuitToWorld = GetActorTransform();
	const int32 N = CentrePos.Num();
	TArray<FVector> WorldCentre;
	WorldCentre.SetNumUninitialized(N);
	for (int32 i = 0; i < N; ++i)
	{
		WorldCentre[i] = CircuitToWorld.TransformPosition(CentrePos[i]);
	}

	float MaxHalfWidth = 0.f;
	for (float W : CentreWidthCm) { MaxHalfWidth = FMath::Max(MaxHalfWidth, W * 0.5f); }

	// Past this, a vertex is pure terrain and the centreline cannot reach it.
	const float Reach = MaxHalfWidth + ShoulderCm + FalloffCm;

	// Uniform bucket grid over the centreline. Naive would be NumX*NumY*N - for a
	// 1778x1524 landscape and a 10000 point centreline that is 2.7e10 distance tests,
	// which is minutes. Bucketing at the reach radius means each vertex only examines
	// the 3x3 cells that could possibly hold a sample within reach.
	FVector2D BoundsMin(BIG_NUMBER, BIG_NUMBER);
	FVector2D BoundsMax(-BIG_NUMBER, -BIG_NUMBER);
	for (const FVector& P : WorldCentre)
	{
		BoundsMin.X = FMath::Min(BoundsMin.X, P.X); BoundsMin.Y = FMath::Min(BoundsMin.Y, P.Y);
		BoundsMax.X = FMath::Max(BoundsMax.X, P.X); BoundsMax.Y = FMath::Max(BoundsMax.Y, P.Y);
	}
	const float Cell = FMath::Max(Reach, 500.f);
	const int32 CellsX = FMath::Max(FMath::CeilToInt((BoundsMax.X - BoundsMin.X) / Cell) + 1, 1);
	const int32 CellsY = FMath::Max(FMath::CeilToInt((BoundsMax.Y - BoundsMin.Y) / Cell) + 1, 1);

	TArray<TArray<int32>> Buckets;
	Buckets.SetNum(CellsX * CellsY);
	for (int32 i = 0; i < N; ++i)
	{
		const int32 cx = FMath::Clamp(FMath::FloorToInt((WorldCentre[i].X - BoundsMin.X) / Cell), 0, CellsX - 1);
		const int32 cy = FMath::Clamp(FMath::FloorToInt((WorldCentre[i].Y - BoundsMin.Y) / Cell), 0, CellsY - 1);
		Buckets[(cy * CellsX) + cx].Add(i);
	}

	TArray<uint16> Heights;
	Heights.SetNumUninitialized(NumX * NumY);

	int32 Carved = 0;
	int32 Clipped = 0;

#if VP_HAS_LANDSCAPE_EDIT_LAYERS
	// AN EDIT LAYER HAS TO BE SELECTED BEFORE THE WRITE, and this is the whole UE5
	// difference.
	//
	// In 4.27 a landscape had no edit layers unless someone deliberately turned them on,
	// so FHeightmapAccessor wrote the final heightmap and that was the end of it. UE5
	// made layers the normal path - ALandscape::bCanHaveLayersContent is
	// _DEPRECATED now - and editor-created landscapes have them. Writing through the
	// accessor with layers present but NO editing layer selected edits the merged
	// result, which TickLayers then recomputes from the layers and overwrites. The carve
	// applies and is immediately undone, which from the viewport is indistinguishable
	// from the sculpt never running.
	//
	// So: point the landscape at a real layer, write into that, and put the previous
	// selection back. Restoring matters - leaving another layer selected changes what
	// the user's next brush stroke in Landscape mode edits.
	FGuid PreviousEditingLayer;
	bool bEditingLayerSet = false;
	if (TargetLandscape->HasLayersContent())
	{
		PreviousEditingLayer = TargetLandscape->GetEditingLayer();

		FGuid TargetLayer;
		if (const ULandscapeEditLayerBase* Layer = TargetLandscape->GetEditLayerConst(0))
		{
			TargetLayer = Layer->GetGuid();
		}
		else
		{
			// Layers content but no layer - possible on a landscape mid-migration.
			TargetLandscape->CreateDefaultLayer();
			if (const ULandscapeEditLayerBase* Made = TargetLandscape->GetEditLayerConst(0))
			{
				TargetLayer = Made->GetGuid();
			}
		}

		if (TargetLayer.IsValid())
		{
			TargetLandscape->SetEditingLayer(TargetLayer);
			bEditingLayerSet = true;
			UE_LOG(LogTestCircuit, Log,
				TEXT("  edit layers present - writing into layer 0 ('%s'). Without selecting one "
					 "the write lands on the merged heightmap and TickLayers discards it."),
				TargetLandscape->GetEditLayerConst(0)
					? *TargetLandscape->GetEditLayerConst(0)->GetName().ToString()
					: TEXT("?"));
		}
		else
		{
			UE_LOG(LogTestCircuit, Error,
				TEXT("  this landscape reports layers content but exposes no edit layer, and one "
					 "could not be created. The carve below will be discarded by TickLayers - "
					 "add a layer in Landscape mode, or turn edit layers off on the landscape."));
		}
	}
#endif

	{
		FHeightmapAccessor<false> Accessor(Info);

		// The height path transacts the heightmap TEXTURE but not the component state,
		// so mark the components dirty for a clean undo.
		{
			TSet<ULandscapeComponent*> Touched;
			Info->GetComponentsInRegion(MinX, MinY, MaxX, MaxY, Touched);
			for (ULandscapeComponent* C : Touched)
			{
				if (C) { C->Modify(); }
			}
		}

		Accessor.GetDataFast(MinX, MinY, MaxX, MaxY, Heights.GetData());

		for (int32 vy = 0; vy < NumY; ++vy)
		{
			for (int32 vx = 0; vx < NumX; ++vx)
			{
				const FVector VertWorld = LandscapeToWorld.TransformPosition(
					FVector(static_cast<float>(MinX + vx), static_cast<float>(MinY + vy), 0.f));

				// --- nearest centreline sample, over the 3x3 neighbourhood ----------
				int32 Best = INDEX_NONE;
				float BestDistSq = Reach * Reach;
				const int32 cx = FMath::Clamp(FMath::FloorToInt((VertWorld.X - BoundsMin.X) / Cell), 0, CellsX - 1);
				const int32 cy = FMath::Clamp(FMath::FloorToInt((VertWorld.Y - BoundsMin.Y) / Cell), 0, CellsY - 1);

				for (int32 oy = -1; oy <= 1; ++oy)
				{
					const int32 by = cy + oy;
					if (by < 0 || by >= CellsY) { continue; }
					for (int32 ox = -1; ox <= 1; ++ox)
					{
						const int32 bx = cx + ox;
						if (bx < 0 || bx >= CellsX) { continue; }
						for (int32 Index : Buckets[(by * CellsX) + bx])
						{
							const float dx = WorldCentre[Index].X - VertWorld.X;
							const float dy = WorldCentre[Index].Y - VertWorld.Y;
							const float D2 = (dx * dx) + (dy * dy);
							if (D2 < BestDistSq) { BestDistSq = D2; Best = Index; }
						}
					}
				}

				if (Best == INDEX_NONE)
				{
					continue;   // out of reach: leave whatever the terrain already was
				}

				// --- cross-section, INTERPOLATED ALONG THE CENTRELINE ---------------
				//
				// This used to snap the vertex to the nearest centreline sample and apply
				// that sample's plane whole. Samples are SlabLengthCm apart - 4 m by
				// default - so at a quarter-metre landscape resolution sixteen vertices
				// in a row shared one plane and the carve came out in 4 m terraces. That
				// is the stepping, and it is this code rather than the landscape.
				//
				// It is also why the road still surfaced through: a terrace edge steps by
				// the span times the gradient, which is 39 cm on a 9.8% section, and no
				// clearance of 18 cm survives that.
				//
				// Projecting onto the SPAN between samples instead makes the carve
				// piecewise-linear along the road rather than piecewise-constant, so the
				// terraces collapse to the sample spacing's second derivative - which is
				// invisible.
				const int32 SampleCount = WorldCentre.Num();
				const bool bLoop = (Venue != EVehicleTestVenue::RallyStage);
				const int32 PrevIdx = bLoop ? ((Best - 1 + SampleCount) % SampleCount) : FMath::Max(Best - 1, 0);
				const int32 NextIdx = bLoop ? ((Best + 1) % SampleCount) : FMath::Min(Best + 1, SampleCount - 1);

				const FVector2D VertFlat(VertWorld.X, VertWorld.Y);

				int32 I0 = Best;
				int32 I1 = Best;
				float AlongT = 0.f;
				float PerpSq = BestDistSq;
				FVector2D OnCentre(WorldCentre[Best].X, WorldCentre[Best].Y);

				auto Consider = [&](int32 A, int32 B)
				{
					if (A == B)
					{
						return;
					}
					const FVector2D PA(WorldCentre[A].X, WorldCentre[A].Y);
					const FVector2D PB(WorldCentre[B].X, WorldCentre[B].Y);
					const FVector2D AB = PB - PA;
					const float LenSq = AB.SizeSquared();
					if (LenSq <= KINDA_SMALL_NUMBER)
					{
						return;
					}
					const float T = FMath::Clamp(FVector2D::DotProduct(VertFlat - PA, AB) / LenSq, 0.f, 1.f);
					const FVector2D On = PA + (AB * T);
					const float D2 = (VertFlat - On).SizeSquared();
					if (D2 < PerpSq)
					{
						PerpSq = D2;
						I0 = A;
						I1 = B;
						AlongT = T;
						OnCentre = On;
					}
				};
				Consider(PrevIdx, Best);
				Consider(Best, NextIdx);

				// Heading is interpolated as a DIRECTION rather than as an angle, so a
				// span that crosses the 360 degree wrap does not swing the whole way
				// round and invert the road's camber for one vertex.
				const float HeadA = FMath::DegreesToRadians(CentreHeadingDeg[I0]);
				const float HeadB = FMath::DegreesToRadians(CentreHeadingDeg[I1]);
				const FVector2D RightRaw(
					FMath::Lerp(-FMath::Sin(HeadA), -FMath::Sin(HeadB), AlongT),
					FMath::Lerp(FMath::Cos(HeadA), FMath::Cos(HeadB), AlongT));

				// GetSafeNormal, not Normalize: FVector2D::Normalize returns void in 4.27
				// where FVector's returns bool, so there is nothing to test.
				const FVector2D Right = RightRaw.IsNearlyZero()
					? FVector2D(-FMath::Sin(HeadA), FMath::Cos(HeadA))
					: RightRaw.GetSafeNormal();

				const float Signed = FVector2D::DotProduct(VertFlat - OnCentre, Right); // +ve is the road's right
				const float Dist = FMath::Sqrt(PerpSq);

				const float HalfWidth = FMath::Lerp(CentreWidthCm[I0], CentreWidthCm[I1], AlongT) * 0.5f;
				const float CentreZ = FMath::Lerp(WorldCentre[I0].Z, WorldCentre[I1].Z, AlongT);
				const float RollHere = FMath::Lerp(CentreRollDeg[I0], CentreRollDeg[I1], AlongT);

				// Banked plane, clamped at the edge so the bank does not keep climbing
				// out into the verge. Negative roll raises +Y, hence the minus.
				const float ClampedLat = FMath::Clamp(Signed, -HalfWidth, HalfWidth);
				// DROPPED BY THE CLEARANCE, so the terrain cannot surface through the
				// road. Carving the corridor to the centreline height leaves the road's
				// top only its own half-thickness above the ground, and the landscape
				// only has a vertex every metre or two: between them it interpolates a
				// CHORD while the road follows the arc, so on the outside of a corner
				// the ground climbs above the road, and on a slope the vertex grid
				// steps through it. Both were reported, and both are this.
				const float RoadZ = CentreZ
					- (ClampedLat * FMath::Tan(FMath::DegreesToRadians(RollHere)))
					- RoadClearanceCm;

				// Three zones. Road, then a verge that levels the bank out at road
				// height, then terrain relief blending in. Smoothstep rather than a
				// linear ramp at both joints: linear is C0, so it leaves a visible
				// crease along the whole length of the road and a ridge the car feels.
				float TargetZ = RoadZ;
				if (Dist > HalfWidth)
				{
					const float T1 = SmoothStep01((Dist - HalfWidth) / FMath::Max(ShoulderCm, 1.f));
					TargetZ = FMath::Lerp(RoadZ, CentreZ, T1);

					if (Dist > HalfWidth + ShoulderCm)
					{
						// Relief is measured RELATIVE to the road, never absolute. Absolute
						// terrain would cut the road in half wherever the lap climbs 45 m.
						const float Relief =
							(TerrainAmplitudeCm * FMath::Sin(VertWorld.X / FMath::Max(TerrainWavelengthCm, 1.f))
								* FMath::Cos(VertWorld.Y / FMath::Max(TerrainWavelengthCm, 1.f)))
							+ (TerrainAmplitudeCm * 0.35f
								* FMath::Sin(VertWorld.X / FMath::Max(TerrainWavelengthCm * 0.31f, 1.f) + 1.7f)
								* FMath::Cos(VertWorld.Y / FMath::Max(TerrainWavelengthCm * 0.27f, 1.f) - 0.9f));

						const float T2 = SmoothStep01(
							(Dist - HalfWidth - ShoulderCm) / FMath::Max(FalloffCm, 1.f));
						TargetZ = FMath::Lerp(TargetZ, CentreZ + Relief, T2);
					}
				}

				// --- world Z to a height unit ---------------------------------------
				const FVector LocalTarget = LandscapeToWorld.InverseTransformPosition(
					FVector(VertWorld.X, VertWorld.Y, TargetZ));
				const int32 Unit = FMath::RoundToInt(
					(LocalTarget.Z * LANDSCAPE_INV_ZSCALE) + LandscapeDataAccess::MidValue);

				if (Unit < 0 || Unit > LandscapeDataAccess::MaxValue) { ++Clipped; }
				Heights[(vy * NumX) + vx] = static_cast<uint16>(
					FMath::Clamp(Unit, 0, LandscapeDataAccess::MaxValue));
				++Carved;
			}
		}

		Accessor.SetData(MinX, MinY, MaxX, MaxY, Heights.GetData());
		// Silently does nothing if any touched component's level is locked or hidden,
		// so a run that reports carved vertices and changes nothing means check that.
	}
	// ~FHeightmapAccessor runs HERE, and it is what makes the car able to drive on it:
	// Flush, then per component UpdateCachedBounds, UpdateComponentToWorld,
	// CollisionComponent->RecreateCollision and the navigation update.

#if VP_HAS_LANDSCAPE_EDIT_LAYERS
	if (bEditingLayerSet)
	{
		TargetLandscape->SetEditingLayer(PreviousEditingLayer);
	}
#endif

	if (Carved == 0)
	{
		UE_LOG(LogTestCircuit, Error,
			TEXT("NOTHING WAS WRITTEN. Every landscape vertex was further than %.1f m from the "
				 "centreline, so the corridor never reached the terrain. Either the landscape is "
				 "somewhere else in the world than this actor, or the reach is too small - it is "
				 "half the widest segment plus the shoulder plus the falloff."),
			Reach * 0.01f);
	}

	if (TargetLandscape->HasLayersContent())
	{
		// A layered landscape defers everything to ALandscape::TickLayers, so nothing
		// is drivable until a tick has elapsed.
		TargetLandscape->RequestLayersContentUpdateForceAll(ELandscapeLayerUpdateMode::Update_All);
		UE_LOG(LogTestCircuit, Warning,
			TEXT("  this landscape has edit layers, so collision updates on the next tick, not now."));
	}

	SculptedSignature = LayoutSignature();

	UE_LOG(LogTestCircuit, Log,
		TEXT("Sculpted %d x %d vertices (%d written) around %.3f km of centreline. "
			 "Corridor %.1f m wide, %.1f m shoulder, %.1f m falloff."),
		NumX, NumY, Carved, CentreLengthCm * 0.00001f,
		MaxHalfWidth * 0.02f, ShoulderCm * 0.01f, FalloffCm * 0.01f);

	if (Clipped > 0)
	{
		UE_LOG(LogTestCircuit, Warning,
			TEXT("  %d vertices clipped against the landscape's height range. Raise the landscape's "
				 "Z scale, or move the actor down - the lap needs %.1f m of vertical range."),
			Clipped, (CentreLengthCm > 0.f) ? 70.f : 0.f);
	}
}

#endif // WITH_EDITOR
