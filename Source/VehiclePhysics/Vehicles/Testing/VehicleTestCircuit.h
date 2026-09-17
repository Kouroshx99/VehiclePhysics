// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VehicleTestCircuit.generated.h"

class UStaticMesh;
class UMaterialInterface;
class ALandscape;

/**
 * Which venue the actor builds. Three places, three cars - which is what the design
 * document specifies and what a single combined lap could not give.
 */
UENUM(BlueprintType)
enum class EVehicleTestVenue : uint8
{
	/** Sealed closed circuit, 2.31 km, eight corners. The default. */
	Circuit UMETA(DisplayName = "Circuit"),

	/** Gravel special stage, 1.35 km, point to point. Does not close, by design. */
	RallyStage UMETA(DisplayName = "Rally Stage"),

	/** Slow technical loop, 770 m. Suspension stations rather than corners. */
	OffroadCourse UMETA(DisplayName = "Offroad Course"),

	/**
	 * The 4 km lap with the gravel loop spliced inside its late chicane. Kept because
	 * it closes to 0.63 cm and carries a solved 61 m landform, but it is not what the
	 * document asks for - the three venues above are.
	 */
	CombinedLap UMETA(DisplayName = "Combined Lap (legacy)")
};

/**
 * An obstacle station on the offroad course.
 *
 * The layout names its stations in comments - Rock Garden, Log Crossing, Whoops Run -
 * and until now that is all they were: a name beside a gradient. The course built as a
 * narrow road with some slopes on it, which is why it drove as one.
 *
 * This is how a segment says what actually stands on it. Only the offroad course sets
 * these; the rally stage does not need them, and would be worse for them - a stage is
 * about carrying speed over terrain that moves under you, not about placing a wheel on
 * a specific rock.
 */
UENUM(BlueprintType)
enum class ECircuitStation : uint8
{
	None UMETA(DisplayName = "None"),

	/** Scattered rock, 20-60 cm, each buried to a quarter of its height. */
	RockGarden UMETA(DisplayName = "Rock Garden"),

	/** A single step up across the width, at the top of a climb. */
	LedgeStep UMETA(DisplayName = "Ledge Step"),

	/** Two logs across the trail, a few metres apart. */
	LogCrossing UMETA(DisplayName = "Log Crossing"),

	/** Parallel ridges leaving two channels at track width. */
	RutSection UMETA(DisplayName = "Rut Section"),

	/** A run of humps at fixed spacing. */
	WhoopsRun UMETA(DisplayName = "Whoops Run"),

	/**
	 * Rock crawling. Stepped shelves across the trail with boulders between them,
	 * offset side to side so opposite wheels are lifted in turn.
	 *
	 * Sized against the WHEEL rather than by eye, which is what separates this from
	 * the rock garden. A 33 cm wheel meets a rock buried to a quarter of its height
	 * as an exposed face of three quarters of it: 20 cm of rock is 15 cm of face,
	 * under half a wheel radius, and a car drives over that without noticing. 60 cm
	 * and up is 1.4 radii and more, which has to be climbed - the tyre has to find
	 * the sloped face, take load on one corner, and let the suspension articulate.
	 *
	 * That is the difference between scenery and an obstacle.
	 */
	RockCrawl UMETA(DisplayName = "Rock Crawl"),

	/**
	 * A PROGRESSIVE CLIMB, to find the gradient the vehicle actually runs out on.
	 *
	 * The offroad course already climbs - segment 8 at +19.1% and descends at -18.4% -
	 * but a fixed gradient only ever answers "yes it managed that one". Gradeability is
	 * a limit, so the feature has to bracket it: a run of ramps that steepen in steps,
	 * driven until the car stops going up.
	 *
	 * Where the limit should be is arithmetic, and worth knowing before driving it. On
	 * four driven wheels with no weight-transfer losses the ceiling is atan(mu): the
	 * offroad tyre's peak is 0.70, so about 35 degrees. Below that it is a power and
	 * gearing question; at it, the tyres simply let go. A car that stops well short of
	 * 35 has a differential or a load-transfer problem, not a traction one, and that
	 * distinction is the reason to sweep the angle rather than pick one.
	 */
	GradeTest UMETA(DisplayName = "Grade Test (progressive climb)"),

	/**
	 * A RUN OF RIDGES that steepen, to find where the belly grounds out.
	 *
	 * This is the one geometric limit the course never tested, and the offroad car was
	 * built for it - the approach and departure ramps in its collision hulls exist
	 * precisely so a crest catches the frame before the tyres.
	 *
	 * Also the additive answer to a problem the layout notes as unsolvable. Segment 6
	 * is the Ditch Crossing and carries no station, because a 1.5 m V is a HOLE and
	 * everything here is built by adding pieces on top of the trail. A ridge is that
	 * ditch turned inside out: it asks the same question about belly clearance from
	 * above the road instead of below it, and it can actually be built.
	 *
	 * Sized off the vehicle. A symmetric ridge of ramp angle theta is crested without
	 * the belly touching while tan(theta) <= 2h/L, for clearance h at the midpoint and
	 * wheelbase L. The offroad car is 30 cm over 300 cm, so theta_max is atan(0.2) =
	 * 11.3 deg - which is the 22.6 deg figure a spec sheet would quote, since the
	 * published breakover angle is the full included angle, 2*theta. The ridges
	 * therefore run either side of 11 degrees, and the car should crest the shallow
	 * ones and hang up on the steep ones.
	 */
	BreakoverRidge UMETA(DisplayName = "Breakover Ridge (progressive)")
};

/**
 * One piece of a circuit, described the way a track actually is: a length, a
 * curvature, a change in height and a bank.
 *
 * Deliberately NOT a list of points. Points are easy to generate and miserable to
 * edit - move one and the two either side no longer meet it smoothly, and there is
 * no way to say "make this corner tighter" without moving all of them. A radius
 * and an arc length say that in one number each, join continuously by
 * construction, and are how every real circuit is described on paper.
 */
USTRUCT(BlueprintType)
struct FCircuitSegment
{
	GENERATED_BODY()

	/** Distance travelled along this piece, cm. For a corner, the arc length. */
	UPROPERTY(EditAnywhere, Category = "Segment", meta = (ClampMin = "100.0", Units = "cm"))
	float LengthCm = 10000.f;

	/**
	 * Corner radius, cm. ZERO is a straight.
	 *
	 * Positive turns right, negative left. Tighter radius means a slower corner, and
	 * the lap estimate reads it directly: v = sqrt(mu * g * R).
	 */
	UPROPERTY(EditAnywhere, Category = "Segment", meta = (Units = "cm"))
	float RadiusCm = 0.f;

	/** Height gained over the piece, cm. Negative descends. */
	UPROPERTY(EditAnywhere, Category = "Segment", meta = (Units = "cm"))
	float RiseCm = 0.f;

	/**
	 * Banking, degrees. Positive leans into the corner, which is what a real one does.
	 *
	 * Bank does real work: it converts part of the cornering demand into vertical
	 * load rather than asking the tyres for it sideways, so a banked corner can be
	 * taken faster than its radius alone allows.
	 */
	UPROPERTY(EditAnywhere, Category = "Segment", meta = (ClampMin = "-25.0", ClampMax = "25.0"))
	float BankDeg = 0.f;

	/** Road width, cm. Widen for the rally section, narrow for a technical one. */
	UPROPERTY(EditAnywhere, Category = "Segment", meta = (ClampMin = "300.0", Units = "cm"))
	float WidthCm = 900.f;

	/**
	 * Loose surface: colours it, widens the shoulder, adds an undulation and scatters
	 * rocks alongside.
	 *
	 * The surface itself is NOT made more slippery - friction lives in the tyre
	 * asset, not the geometry, and quietly changing mu underneath the car would make
	 * every measurement taken here incomparable with every measurement taken on the
	 * proving ground. What this changes is what the SUSPENSION is asked, which is
	 * the half a circuit can actually contribute.
	 */
	UPROPERTY(EditAnywhere, Category = "Segment")
	bool bGravel = false;

	/**
	 * What stands on this segment, if anything. Geometry, not gradient.
	 *
	 * Set after construction rather than through the constructor, so the layout tables
	 * stay readable as six columns of numbers.
	 */
	UPROPERTY(EditAnywhere, Category = "Segment")
	ECircuitStation Station = ECircuitStation::None;

	FCircuitSegment() = default;
	FCircuitSegment(float InLength, float InRadius, float InRise, float InBank, float InWidth, bool bInGravel)
		: LengthCm(InLength), RadiusCm(InRadius), RiseCm(InRise), BankDeg(InBank)
		, WidthCm(InWidth), bGravel(bInGravel) {}
};

/**
 * A closed circuit built from primitives, long enough to actually drive.
 *
 * The proving ground answers questions one at a time - here is a bump, here is a
 * slalom, drive it and watch one thing. This is the other kind of test: several
 * minutes of continuous driving where nothing is isolated, corners arrive while
 * the car is still settling from the last one, and the gearbox has to make its own
 * decisions. Faults that a single manoeuvre hides tend to surface here, and it is
 * also the thing you can put in front of someone who wants to see it drive rather
 * than read what it measured.
 *
 * The layout is a list of FCircuitSegments, editable in the details panel. The
 * default is a roughly 3 km lap with a fast half, a technical half, a climb and a
 * gravel loop - about three minutes at road-car pace.
 *
 * Drop it in an empty level, point +X where the start straight should run, and
 * press Build. It spawns ordinary actors, so anything it makes can be selected,
 * moved and saved like hand-placed geometry.
 */
UCLASS()
class VEHICLEPHYSICS_API AVehicleTestCircuit : public AActor
{
	GENERATED_BODY()

public:
	AVehicleTestCircuit();

	/** Lays out the circuit. Clears any previous build first. */
	/**
	 * Which venue to build. Changing this and pressing Build replaces the layout.
	 *
	 * Place one actor per venue on different parts of the landscape and set each one,
	 * rather than switching a single actor back and forth - Clear only removes what
	 * that actor made, so they coexist happily.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit")
	EVehicleTestVenue Venue = EVehicleTestVenue::Circuit;

	/**
	 * Lay the road as a continuous deformed ribbon instead of stepped slabs.
	 *
	 * The slabs are what made every joint abrupt. Each one is a rigid flat box at its
	 * own step's grade, heading and roll, overlapping the next by 15% so no gap opens
	 * - and an overlap at a grade change is a STEP, one slab's end standing proud
	 * through the next one's start. A raycast reads a step as an instant change in
	 * height, which is an infinite gradient, which is a wheel force spike. Smoothing
	 * the layout cannot fix it because the layout was never the problem.
	 *
	 * A USplineMeshComponent deforms its mesh along a curve instead, so position,
	 * tangent and roll are continuous across every joint by construction.
	 *
	 * Collision is real and it is worth knowing exactly what it is. The engine turns
	 * the source box into an eight-vertex CONVEX HULL and pushes each corner through
	 * CalcSliceTransform individually, so the hull follows the spline at both ends.
	 * What it cannot do is bulge in the middle, so the driving surface is a chord
	 * across each component, short by L^2/8R.
	 *
	 * Which axis that error lands on decides whether it matters:
	 *
	 *   VERTICALLY it is the one that bumps the car, and it is tiny. Grade changes
	 *   happen over segments 60 m and longer, so the vertical radius is 400 m upward
	 *   and a 4 m component is short by 0.1 to 0.5 cm - under a tyre's own deflection.
	 *
	 *   HORIZONTALLY it only scallops the inside edge: 9 cm on the offroad hairpin at
	 *   4 m components, 20 cm on the circuit hairpin at 8 m. Cosmetic, but visible,
	 *   which is why the spacing below follows the radius.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit")
	bool bRoadRibbon = true;

	/**
	 * Chord tolerance for ribbon spacing, cm. Components are shortened on tight
	 * corners until the chord is within this of the true arc - L = sqrt(8*R*tol).
	 *
	 * 2 cm puts a 22 m hairpin on 1.9 m components and a 250 m sweep on 6.3 m ones,
	 * which is the point: detail where the curvature asks for it and nowhere else.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit",
		meta = (ClampMin = "0.25", ClampMax = "50.0", Units = "cm", EditCondition = "bRoadRibbon"))
	float RibbonChordToleranceCm = 2.f;

	/** Longest a ribbon component may be, cm. Caps the spacing on straights. */
	UPROPERTY(EditAnywhere, Category = "Circuit",
		meta = (ClampMin = "100.0", Units = "cm", EditCondition = "bRoadRibbon"))
	float RibbonMaxSpanCm = 800.f;

	/**
	 * Distance over which a change of gradient is blended, cm. Zero disables it.
	 *
	 * This is the design document's first rule, finally enforced by something. The
	 * layout describes each segment's rise as a total, so the gradient CHANGES in one
	 * step at every segment boundary, and a step in gradient is a ramp. The ribbon
	 * smooths across one component, which is up to 8 m - nowhere near enough.
	 *
	 * How much is needed follows from the vertical acceleration it produces:
	 *
	 *     L  >=  v^2 * dGrade / (0.3 * 9.81)
	 *
	 * The circuit's worst joint is 18.2% of gradient change at Start/Finish, which at
	 * 114 km/h wants 62 m. Getting 8 m instead is the jump on the first rise.
	 *
	 * Applied as a moving average on the centreline's height, wrapped for a closed
	 * venue and clamped for the point-to-point stage, so closure survives - a
	 * symmetric average over a periodic sequence stays periodic. It costs a little
	 * off the sharpest crests, which is real and is also what a road does.
	 *
	 * The offroad course needs far less than this figure suggests, because the
	 * requirement scales with the SQUARE of speed: its 20.3% step wants 69 m at
	 * 114 km/h and 4.7 m at 30 km/h, and nothing there is driven at 114.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit", meta = (ClampMin = "0.0", Units = "cm"))
	float GradeBlendLengthCm = 6000.f;

	/**
	 * How far BELOW the road surface the corridor is carved, cm.
	 *
	 * Carving to the centreline height leaves the road's top only its own
	 * half-thickness clear of the ground, and that is not enough. A landscape has a
	 * vertex every metre or two and interpolates a straight chord between them, while
	 * the road follows an arc - so on the outside of a corner the terrain climbs above
	 * the road, and across a slope the vertex grid steps through it. Both were
	 * reported as the terrain poking through and the road going under.
	 *
	 * The cost is a lip at the road edge, which is honest: a road IS a built surface
	 * sitting on graded ground, and running wide should have a consequence. The verge
	 * then rises back to grade across ShoulderCm.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Landscape", meta = (ClampMin = "0.0", Units = "cm"))
	float RoadClearanceCm = 18.f;

#if WITH_EDITOR
	/** Repopulates Layout when Venue changes - see the implementation for why that is required. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Circuit")
	void Build();

	/** Removes everything Build made and leaves the rest of the level alone. */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Circuit")
	void Clear();

	/**
	 * Sculpt the landscape and lay the circuit, in that order, in one press.
	 *
	 * The two-button version has a failure mode that keeps happening and is invisible
	 * when it does: press Build without Sculpt, or after moving the actor or switching
	 * venue, and the road is laid somewhere the ground was never carved. Nothing looks
	 * broken - the road is exactly where the actor says and the terrain is exactly
	 * where it was left - so it reads as the track being in the wrong place.
	 *
	 * One button cannot be pressed in the wrong order.
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Circuit")
	void SculptAndBuild();

#if WITH_EDITOR
	/**
	 * Carves the circuit into TargetLandscape so the road has ground under it and a
	 * car that runs wide can get back on.
	 *
	 * Editor only, and not because of a policy: FLandscapeEditDataInterface and
	 * FHeightmapAccessor live inside an #if WITH_EDITOR that spans LandscapeEdit.h
	 * from line 33 to 617, so there is nothing to call in a packaged build.
	 *
	 * Run it BEFORE Build, or run Build again after - the slabs sit at the centreline
	 * height either way, and this is what moves the ground to meet them.
	 */
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Circuit|Landscape")
	void SculptLandscape();
#endif

	/**
	 * The lap, piece by piece. Edit freely - it is integrated, not validated, so a
	 * layout that does not return to its start simply will not close, and the log
	 * says by how far.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit")
	TArray<FCircuitSegment> Layout;

	/**
	 * How long each road slab is, cm.
	 *
	 * The only quality-against-cost dial here. The road is laid as overlapping
	 * slabs, each flat, so a corner is a polygon rather than a curve and every joint
	 * is a small step. Shorter slabs mean a smoother road and more actors; 400 cm
	 * puts the step under a centimetre on everything in the default layout, which is
	 * below what the suspension notices.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit", meta = (ClampMin = "100.0", ClampMax = "2000.0", Units = "cm"))
	float SlabLengthCm = 400.f;

	/** Refuses to build past this many actors, rather than locking the editor up. */
	UPROPERTY(EditAnywhere, Category = "Circuit", meta = (ClampMin = "100"))
	int32 MaxPieces = 4000;

	/**
	 * The landscape to carve the circuit into. Leave null to lay road on whatever is
	 * already there.
	 *
	 * Create it yourself first - Landscape mode, New - because building one from code
	 * means getting component counts, subsection sizes and GUIDs right, and five
	 * clicks are cheaper than that risk. What the tool wants:
	 *
	 *     Section Size            127 x 127 Quads
	 *     Sections Per Component  2 x 2
	 *     Number of Components    12 x 10
	 *     Scale                   50, 50, 100
	 *
	 * That is 1524 x 1270 m at HALF a metre per quad, containing the default lap's
	 * 1357 x 1038 m footprint. 120 components, 7.7 M vertices, about 30 MB of
	 * heightmap.
	 *
	 * Why half a metre and not one, given the road is slabs and the tyres never touch
	 * the landscape on-track: vertex density does not change how the car drives here
	 * at all. It shows up at EDGES, and the carve creates one - the shoulder where
	 * slab road meets terrain. At one metre that edge is quantised to a metre
	 * laterally and reads as stepped.
	 *
	 * Why not a quarter of a metre: 456 components and 29 M vertices to sharpen an
	 * edge that is being viewed as untextured grey. On smooth terrain perceived
	 * detail comes from the material, not the mesh. Worth revisiting if the landscape
	 * ever gets painted layers, and not before.
	 *
	 * The Z scale is independent of X and Y. 100 gives 0.78 cm of height resolution
	 * over a +/-256 m range, and the lap needs about 70 m of that.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Landscape")
	ALandscape* TargetLandscape = nullptr;

	/**
	 * Distance over which the banked road levels out into flat verge, cm.
	 *
	 * The bank is clamped at the road edge and then unwound across this, so a corner
	 * banked at 5 degrees does not keep climbing out into the countryside.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Landscape", meta = (ClampMin = "100.0", Units = "cm"))
	float ShoulderCm = 1200.f;

	/**
	 * Distance over which the verge blends into open terrain, cm.
	 *
	 * Generous on purpose. This is what turns a 14 m climb into an embankment rather
	 * than a wall, and it is the parameter to raise if the terrain beside the road
	 * looks like it was cut with a knife.
	 *
	 * It also sets how far the carve REACHES. Past half-width + shoulder + falloff a
	 * vertex is out of range of every centreline sample and is left exactly as it
	 * was, so with the defaults the landscape is shaped within about 80 m of the road
	 * and whatever it already was beyond that. Raise this to shape more of it, at the
	 * cost of a wider search and a slower sculpt.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Landscape", meta = (ClampMin = "100.0", Units = "cm"))
	float FalloffCm = 6000.f;

	/**
	 * Height of the rolling relief outside the corridor, cm.
	 *
	 * Applied RELATIVE to the nearby road height, never as an absolute elevation -
	 * absolute terrain would cut straight through the road wherever the lap climbs.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Landscape", meta = (ClampMin = "0.0", Units = "cm"))
	float TerrainAmplitudeCm = 500.f;

	/** Wavelength of that relief, cm. Long is calmer and reads as landform. */
	UPROPERTY(EditAnywhere, Category = "Circuit|Landscape", meta = (ClampMin = "100.0", Units = "cm"))
	float TerrainWavelengthCm = 18000.f;

	/** Kerbs on the inside and outside of every corner. Free, and they read the line. */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery")
	bool bKerbs = true;

	/**
	 * A driveable fillet down both road edges, so going off is recoverable.
	 *
	 * The road's top surface stands 28 cm above the ground beside it - the ribbon is
	 * 20 cm thick and sits centred on the centreline, while SculptLandscape carves the
	 * terrain a further RoadClearanceCm below to stop it surfacing through the road on
	 * the outside of corners. That is most of a 33 cm wheel radius presented as a
	 * vertical wall, running the entire length of the circuit on both sides: a car that
	 * drops a wheel off cannot climb back and has to be reset.
	 *
	 * The clearance itself cannot simply go - it is what keeps the terrain from poking
	 * through the road between landscape vertices. So the step is ramped instead.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery")
	bool bEdgeRamps = true;

	/**
	 * How far out the fillet reaches, cm. Wider is gentler.
	 *
	 * 180 puts the 28 cm rise at about 9 degrees, which is climbable at any speed and
	 * at any angle of approach. Under about 100 it starts to be something the car
	 * bounces off rather than drives up.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery", meta = (ClampMin = "40.0", Units = "cm"))
	float EdgeRampWidthCm = 300.f;

	/**
	 * How thick the fillet slab is, cm. This is the one that copes with bad terrain.
	 *
	 * The driveable surface is the slab's TOP; the thickness is what hangs below it. Where
	 * the landscape does not actually meet the road - a hole under the edge, a vertex that
	 * carved low - a thin fillet floats over the gap and just moves the drop outboard. A
	 * thick one still has body down there and bridges it.
	 *
	 * So this can be raised freely without making the ramp any steeper, which is why it
	 * is separate from the width. 60 cm spans most of what the carve leaves behind.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery", meta = (ClampMin = "5.0", Units = "cm"))
	float EdgeRampThicknessCm = 60.f;

	/** Blocks placed back from the road on the sealed sections, for scale and reference. */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery")
	bool bBuildings = false;

	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery", meta = (ClampMin = "0"))
	int32 BuildingCount = 60;

	/** Rocks scattered along the gravel sections. */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery")
	bool bGravelRocks = true;

	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery", meta = (ClampMin = "0"))
	int32 RockCount = 220;

	/**
	 * Trees along the gravel. A trunk and a cone each, and no collision on the canopy -
	 * brushing one should cost a scare rather than the run.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery")
	bool bTrees = true;

	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery", meta = (ClampMin = "0"))
	int32 TreeCount = 700;

	/**
	 * Logs at the gravel edges, laid ALONG the road rather than across it.
	 *
	 * Across, a log is a jump. Along, it is the consequence of running wide, which is
	 * the more useful thing for an edge to be.
	 */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery")
	bool bLogs = true;

	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery", meta = (ClampMin = "0"))
	int32 LogCount = 60;

	/** Seeded, so a rebuild puts every rock and building back where it was. */
	UPROPERTY(EditAnywhere, Category = "Circuit|Scenery")
	int32 ScenerySeed = 20260904;

	/** Grip assumed when estimating a lap time. Only affects the printed estimate. */
	UPROPERTY(EditAnywhere, Category = "Circuit", meta = (ClampMin = "0.3", ClampMax = "2.0"))
	float LapEstimateGrip = 0.85f;

	/** Top speed assumed when estimating a lap time, km/h. Estimate only. */
	UPROPERTY(EditAnywhere, Category = "Circuit", meta = (ClampMin = "20.0"))
	float LapEstimateTopSpeedKph = 160.f;

protected:
	virtual void BeginPlay() override;

private:
	bool LoadAssets();

	AActor* SpawnPiece(UStaticMesh* Mesh, const FVector& LocalLocation, const FRotator& LocalRotation,
		const FVector& SizeCm, const FLinearColor& Colour, bool bBlocking = true);

	void BuildDefaultLayout();
	void BuildCircuitLayout();
	void BuildStageLayout();
	void BuildOffroadLayout();
	void BuildCombinedLayout();

	/**
	 * Integrates the layout into a dense polyline, once, for everything downstream.
	 *
	 * The road, the scenery and the landscape carve all need the same answer to
	 * "where does the track go", and integrating it three times invites the three
	 * copies to disagree.
	 */
	void BuildCentreline();
	void BuildScenery();

	/** Places the geometry each station names. See ECircuitStation. */
	void BuildStations();

	/** Which layout segment each centreline sample came from. */
	UPROPERTY(Transient)
	TArray<int32> CentreSegment;

#if WITH_EDITOR
	/** Outliner folder the next SpawnPiece calls drop into. */
	FName PieceFolder;
#endif

	/** The road as continuous spline-mesh ribbon. See bRoadRibbon. */
	void BuildRoadRibbon();

	/** Blends gradient changes along the centreline. See GradeBlendLengthCm. */
	void SmoothCentrelineHeight();

	/** Identifies the layout, venue and actor transform a carve belongs to. */
	FString LayoutSignature() const;

	/** Ribbon components, tracked so Clear can take them down again. */
	UPROPERTY()
	TArray<class USplineMeshComponent*> RibbonPieces;

	/** The integrated centreline, in this actor's local space. */
	TArray<FVector> CentrePos;
	TArray<float> CentreHeadingDeg;
	TArray<float> CentreGradeDeg;
	TArray<float> CentreRollDeg;
	TArray<float> CentreWidthCm;
	TArray<float> CentreStepCm;
	TArray<uint8> CentreGravel;

	float CentreLengthCm = 0.f;
	float CentreLapSeconds = 0.f;
	float CentreCloseErrorCm = 0.f;
	float CentreCloseHeightCm = 0.f;

	UPROPERTY()
	TArray<AActor*> Spawned;

	/**
	 * The layout the landscape was last carved for, and the actor transform it was
	 * carved at. Build compares against these and warns when they no longer match,
	 * because a stale sculpt is silent otherwise.
	 */
	UPROPERTY()
	FString SculptedSignature;

	UStaticMesh* CubeMesh = nullptr;
	UStaticMesh* CylinderMesh = nullptr;
	UStaticMesh* ConeMesh = nullptr;
	UMaterialInterface* BaseMaterial = nullptr;
};
