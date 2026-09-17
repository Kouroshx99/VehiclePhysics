// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleProvingGround.h"

#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogProvingGround, Log, All);

namespace
{
	/**
	 * Desired size in cm -> the scale that achieves it, from the mesh's ACTUAL bounds.
	 *
	 * Not a hardcoded 100 cm. The engine primitives are not all that size, and
	 * assuming they were is what turned the first build's markers into four-metre
	 * spikes. Sizes are given in the MESH's own axes, so a caller states the shape it
	 * wants and any rotation is applied on top of it.
	 */
	FVector ScaleForSize(const UStaticMesh* Mesh, const FVector& SizeCm)
	{
		if (Mesh == nullptr) { return FVector::OneVector; }
		const FVector Bounds = Mesh->GetBoundingBox().GetSize();
		return FVector(
			Bounds.X > KINDA_SMALL_NUMBER ? SizeCm.X / Bounds.X : 1.f,
			Bounds.Y > KINDA_SMALL_NUMBER ? SizeCm.Y / Bounds.Y : 1.f,
			Bounds.Z > KINDA_SMALL_NUMBER ? SizeCm.Z / Bounds.Z : 1.f);
	}

	const FLinearColor ColourBump(0.95f, 0.55f, 0.10f);
	const FLinearColor ColourSplit(0.90f, 0.20f, 0.20f);
	const FLinearColor ColourMarker(0.15f, 0.65f, 0.95f);
	const FLinearColor ColourCone(0.95f, 0.85f, 0.10f);
	const FLinearColor ColourRamp(0.55f, 0.55f, 0.60f);
	const FLinearColor ColourKerb(0.90f, 0.90f, 0.90f);
}

AVehicleProvingGround::AVehicleProvingGround()
{
	PrimaryActorTick.bCanEverTick = false;

	// A root the pieces are placed against. They are spawned as separate actors
	// rather than components so they can be selected, moved and deleted in the
	// editor like any other level geometry once built.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AVehicleProvingGround::BeginPlay()
{
	Super::BeginPlay();

	if (bBuildOnBeginPlay)
	{
		Build();
	}
}

bool AVehicleProvingGround::LoadAssets()
{
	CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
	CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	ConeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone"));
	BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

	if (CubeMesh == nullptr || CylinderMesh == nullptr)
	{
		UE_LOG(LogProvingGround, Error,
			TEXT("Engine basic shapes are missing. Enable 'Show Engine Content' in the Content Browser "
				 "view options, or the proving ground has nothing to build from."));
		return false;
	}
	// A missing cone is survivable - slalom markers fall back to cubes.
	return true;
}

// ---------------------------------------------------------------------------
// Building blocks
// ---------------------------------------------------------------------------

AActor* AVehicleProvingGround::SpawnPiece(UStaticMesh* Mesh, const FVector& LocalLocation,
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

	Piece->SetMobility(EComponentMobility::Movable);
	if (UStaticMeshComponent* Comp = Piece->GetStaticMeshComponent())
	{
		Comp->SetStaticMesh(Mesh);
		// Relative, not world: the size is expressed in the mesh's own axes, and a
		// world scale on a rotated component would apply it along the wrong ones.
		Comp->SetRelativeScale3D(ScaleForSize(Mesh, SizeCm));

		if (bBlocking)
		{
			Comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Comp->SetCollisionProfileName(TEXT("BlockAll"));
		}
		else
		{
			// Markers are scenery. Left blocking, a cone would knock the car off line
			// and the run it was there to measure would be the run it ruined.
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

void AVehicleProvingGround::SpawnBump(float ForwardCm, float LateralCm, float DiameterCm,
	float WidthCm, const FLinearColor& Colour)
{
	// The cylinder's axis is its LOCAL Z, so the length belongs on Z and the diameter
	// on X and Y; the roll then lays it across the path. Sunk to half depth, because
	// a whole cylinder proud of the surface is a wall to hit, not a bump to ride.
	const FVector Size(DiameterCm, DiameterCm, WidthCm);
	SpawnPiece(CylinderMesh, FVector(ForwardCm, LateralCm, 0.f), FRotator(0.f, 0.f, 90.f), Size, Colour);
}

// ---------------------------------------------------------------------------
// Sections
// ---------------------------------------------------------------------------

void AVehicleProvingGround::SpawnSign(float ForwardCm, const FString& Title, const FString& Instruction)
{
	// Every section gets one. A proving ground you have to read the source to
	// understand is just a pile of cones - the value is in knowing what to drive at
	// and what to watch for while you do it.
	const FTransform& Base = GetActorTransform();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = this;

	ATextRenderActor* Sign = GetWorld()->SpawnActor<ATextRenderActor>(
		Base.TransformPosition(FVector(ForwardCm, -900.f, 300.f)),
		(Base.GetRotation() * FRotator(0.f, 90.f, 0.f).Quaternion()).Rotator(), Params);
	if (Sign == nullptr) { return; }

	if (UTextRenderComponent* T = Sign->GetTextRender())
	{
		T->SetText(FText::FromString(Title + LINE_TERMINATOR
			+ Instruction.Replace(TEXT("|"), LINE_TERMINATOR)));
		T->SetWorldSize(80.f);
		T->SetTextRenderColor(FColor(255, 225, 110));
		T->SetHorizontalAlignment(EHTA_Center);
	}
	Spawned.Add(Sign);
}

void AVehicleProvingGround::BuildWashboard(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("WASHBOARD   (damping)"), TEXT("sweep slowly from 20 to 70 km/h|watch for: a speed where it BUILDS UP"));

	for (int32 i = 0; i < WashboardCount; ++i)
	{
		SpawnBump(Cursor + (i * WashboardSpacingCm), 0.f, WashboardDiameterCm, 600.f, ColourBump);
	}
	Cursor += (WashboardCount * WashboardSpacingCm) + 2000.f;

	UE_LOG(LogProvingGround, Log,
		TEXT("  washboard: %d bumps at %.0f cm. Excites ~1 Hz at %.0f km/h, ~2 Hz at %.0f km/h - "
			 "drive a range and watch for the speed where it starts to build."),
		WashboardCount, WashboardSpacingCm,
		WashboardSpacingCm * 0.036f, WashboardSpacingCm * 0.072f);
}

void AVehicleProvingGround::BuildSingleBumps(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("SINGLE BUMPS"), TEXT("one clean hit each, any speed|watch for: 1 bounce and settled, not 3"));

	for (int32 i = 0; i < 3; ++i)
	{
		SpawnBump(Cursor, 0.f, SingleBumpSmallestDiameterCm * (1.f + i), 600.f, ColourBump);
		Cursor += 2500.f;
	}
	Cursor += 1000.f;
}

void AVehicleProvingGround::BuildSplitBumps(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("SPLIT BUMPS   (anti-roll bar)"), TEXT("left side only, about 20 km/h|run twice: bars fitted, then rate = 0"));

	// Offset to one side only. The offset is half the strip width, so the strip
	// spans the centreline outwards and a normally-placed car takes it on one side.
	for (int32 i = 0; i < 4; ++i)
	{
		SpawnBump(Cursor + (i * 400.f), 150.f, SplitBumpDiameterCm, 300.f, ColourSplit);
	}
	Cursor += 2600.f;
}

void AVehicleProvingGround::BuildKerb(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("KERB"), TEXT("put two wheels over it"));

	SpawnPiece(CubeMesh, FVector(Cursor + 600.f, 180.f, 6.f), FRotator(0.f, 0.f, 0.f),
		FVector(1200.f, 250.f, 12.f), ColourKerb);
	Cursor += 2600.f;
}

void AVehicleProvingGround::BuildRtiRamp(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("RTI RAMP   (articulation)"), TEXT("creep ONE side up until a wheel lifts|further up = more articulation"));

	// A long wedge under one side. Modelled as a thin slab tilted about its forward
	// axis, its low edge on the ground so the car can drive onto it.
	const float LengthCm = 1400.f;
	const float WidthCm = 260.f;
	const float Roll = RtiRampAngleDeg;
	const float LiftCm = (WidthCm * 0.5f) * FMath::Sin(FMath::DegreesToRadians(Roll));

	SpawnPiece(CubeMesh, FVector(Cursor + (LengthCm * 0.5f), WidthCm * 0.5f, LiftCm),
		FRotator(0.f, 0.f, -Roll), FVector(LengthCm, WidthCm, 10.f), ColourRamp);

	Cursor += LengthCm + 2000.f;

	UE_LOG(LogProvingGround, Log,
		TEXT("  RTI ramp at %.0f deg. Creep up it with ONE side until a diagonal wheel lifts, "
			 "then compare bars-fitted against bars-at-zero."), RtiRampAngleDeg);
}

void AVehicleProvingGround::BuildWhoops(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("WHOOPS   (airborne + landing)"),
		TEXT("build up from 30 km/h until it goes light|watch for: landing settled, not pogoing"));

	for (int32 i = 0; i < WhoopCount; ++i)
	{
		SpawnBump(Cursor + (i * WhoopSpacingCm), 0.f, WhoopDiameterCm, 700.f, ColourBump);
	}
	Cursor += (WhoopCount * WhoopSpacingCm) + 2500.f;

	UE_LOG(LogProvingGround, Log,
		TEXT("  whoops: %d at %.0f cm, %.0f cm across. Crest to crest is %.1f Hz at 50 km/h - "
			 "well above the body and near the wheel, so the wheels should work and the body should not."),
		WhoopCount, WhoopSpacingCm, WhoopDiameterCm, (50.f / 3.6f) / (WhoopSpacingCm * 0.01f));
}

void AVehicleProvingGround::BuildJumpRamp(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("JUMP"),
		TEXT("40-60 km/h, straight, no steering in the air|watch for: lands flat, one compression, done"));

	// A slab tilted about its pitch axis with the low end on the ground, the same
	// construction the RTI ramp uses about roll. The centre lifts by half the length
	// times the sine, which puts the trailing edge at zero.
	const float Angle = JumpRampAngleDeg;
	const float LiftCm = (JumpRampLengthCm * 0.5f) * FMath::Sin(FMath::DegreesToRadians(Angle));

	SpawnPiece(CubeMesh, FVector(Cursor + (JumpRampLengthCm * 0.5f), 0.f, LiftCm),
		FRotator(Angle, 0.f, 0.f), FVector(JumpRampLengthCm, 700.f, 12.f), ColourRamp);

	// Room to land AND to stop being airborne before anything else is asked of it.
	Cursor += JumpRampLengthCm + 4000.f;

	UE_LOG(LogProvingGround, Log,
		TEXT("  jump: %.0f cm at %.0f deg, lip %.0f cm up. Leaving at 50 km/h gives roughly %.1f m "
			 "of air - approach straight, because yaw picked up in flight has nothing to remove it."),
		JumpRampLengthCm, Angle, LiftCm * 2.f,
		FMath::Square((50.f / 3.6f) * FMath::Sin(FMath::DegreesToRadians(Angle))) / (2.f * 9.81f));
}

void AVehicleProvingGround::BuildOffCamber(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("OFF-CAMBER   (steady side load)"),
		TEXT("drive ALONG it at 30 km/h, hands still|watch for: does it hold line or slide down"));

	const float WidthCm = 900.f;
	const float Roll = OffCamberAngleDeg;
	const float LiftCm = (WidthCm * 0.5f) * FMath::Sin(FMath::DegreesToRadians(Roll));

	SpawnPiece(CubeMesh, FVector(Cursor + (OffCamberLengthCm * 0.5f), 0.f, LiftCm),
		FRotator(0.f, 0.f, -Roll), FVector(OffCamberLengthCm, WidthCm, 12.f), ColourRamp);

	Cursor += OffCamberLengthCm + 2500.f;

	UE_LOG(LogProvingGround, Log,
		TEXT("  off-camber: %.0f m at %.0f deg. That is a standing %.2f g of lateral demand from "
			 "gravity alone against about 0.85 g of grip, so it should hold, and hold with the "
			 "steering roughly centred."),
		OffCamberLengthCm * 0.01f, Roll, FMath::Sin(FMath::DegreesToRadians(Roll)));
}

void AVehicleProvingGround::BuildRockField(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("ROCK FIELD   (contact normals)"),
		TEXT("30-40 km/h, hold the wheel STRAIGHT|watch for: does it track straight or wander"));

	// Seeded, so a rebuild lays out the same field. A test surface that changes
	// between builds cannot be used to compare two runs, which is most of what a
	// test surface is for.
	FRandomStream Stream(RockFieldSeed);

	for (int32 i = 0; i < RockCount; ++i)
	{
		const float X = Cursor + Stream.FRandRange(0.f, RockFieldLengthCm);
		const float Y = Stream.FRandRange(-350.f, 350.f);
		const float H = Stream.FRandRange(RockMaxHeightCm * 0.35f, RockMaxHeightCm);
		const float Size = Stream.FRandRange(35.f, 90.f);

		// Sunk to a quarter of their height and tilted, so what a wheel meets is a
		// sloped face rather than a box corner - the normal varies smoothly and the
		// wheel is never asked to climb a step.
		SpawnPiece(CubeMesh, FVector(X, Y, H * 0.25f),
			FRotator(Stream.FRandRange(-12.f, 12.f), Stream.FRandRange(0.f, 90.f),
				Stream.FRandRange(-12.f, 12.f)),
			FVector(Size, Size, H), ColourRamp);
	}
	Cursor += RockFieldLengthCm + 2500.f;

	UE_LOG(LogProvingGround, Log,
		TEXT("  rock field: %d pieces over %.0f m, up to %.0f cm, seed %d. The only section where "
			 "the contact normal moves independently under each wheel - drive it straight-handed, "
			 "and any wander is the normal rather than the suspension."),
		RockCount, RockFieldLengthCm * 0.01f, RockMaxHeightCm, RockFieldSeed);
}

void AVehicleProvingGround::BuildSkidpad(float ForwardCm)
{
	// Two rings. The inner is the line to hold; the outer shows how far wide the
	// car is running, which is understeer made visible without any instrumentation.
	const float Centre = ForwardCm + SkidpadRadiusCm + 1500.f;
	SpawnSign(ForwardCm + 300.f, TEXT("SKIDPAD   (grip + understeer)"),
		FString::Printf(TEXT("hold the INNER ring at a steady speed|r=%.0f m:   40 km/h = %.2f g    60 km/h = %.2f g|drifting out to the outer ring = understeer"),
			SkidpadRadiusCm * 0.01f,
			FMath::Square(40.f / 3.6f) / (SkidpadRadiusCm * 0.01f) / 9.81f,
			FMath::Square(60.f / 3.6f) / (SkidpadRadiusCm * 0.01f) / 9.81f));
	for (int32 Ring = 0; Ring < 2; ++Ring)
	{
		const float R = SkidpadRadiusCm + (Ring * 300.f);
		for (int32 i = 0; i < SkidpadMarkerCount; ++i)
		{
			const float Angle = (2.f * PI * i) / SkidpadMarkerCount;
			SpawnPiece(ConeMesh ? ConeMesh : CubeMesh,
				FVector(Centre + (R * FMath::Cos(Angle)), R * FMath::Sin(Angle), 20.f),
				FRotator::ZeroRotator, FVector(25.f, 25.f, 40.f),
				Ring == 0 ? ColourMarker : ColourCone, /*bBlocking*/ false);
		}
	}

	UE_LOG(LogProvingGround, Log,
		TEXT("  skidpad radius %.1f m. Lateral g is v^2/r, so holding the inner ring at "
			 "%.0f km/h is %.2f g and %.0f km/h is %.2f g - no instruments needed."),
		SkidpadRadiusCm * 0.01f,
		40.f, FMath::Square(40.f / 3.6f) / (SkidpadRadiusCm * 0.01f) / 9.81f,
		60.f, FMath::Square(60.f / 3.6f) / (SkidpadRadiusCm * 0.01f) / 9.81f);
}

void AVehicleProvingGround::BuildSlalom(float& Cursor)
{
	SpawnSign(Cursor - 700.f, TEXT("SLALOM   (transient response)"), TEXT("weave at a steady speed|watch for: does the rear follow, or lag"));

	for (int32 i = 0; i < SlalomCount; ++i)
	{
		SpawnPiece(ConeMesh ? ConeMesh : CubeMesh,
			FVector(Cursor + (i * SlalomSpacingCm), 0.f, 25.f), FRotator::ZeroRotator,
			FVector(30.f, 30.f, 50.f), ColourCone, /*bBlocking*/ false);
	}
	Cursor += (SlalomCount * SlalomSpacingCm) + 2000.f;
}

void AVehicleProvingGround::BuildDistanceMarkers()
{
	for (int32 i = 1; i <= DistanceMarkerCount; ++i)
	{
		const float X = i * 1000.f;      // every 10 m
		const bool bMajor = (i % 5) == 0;
		SpawnPiece(CubeMesh, FVector(X, -700.f, 15.f), FRotator::ZeroRotator,
			FVector(20.f, 60.f, bMajor ? 90.f : 40.f), ColourMarker, /*bBlocking*/ false);
	}
}

// ---------------------------------------------------------------------------
// Build / clear
// ---------------------------------------------------------------------------

void AVehicleProvingGround::Build()
{
	if (GetWorld() == nullptr || !LoadAssets())
	{
		return;
	}

	Clear();

	UE_LOG(LogProvingGround, Log, TEXT("Building proving ground at %s"), *GetActorLocation().ToCompactString());

	// Sections are laid out nose to tail down +X, each advancing the cursor past
	// itself plus room to settle - a car arriving at the next section still bouncing
	// from the last one is measuring both at once.
	float Cursor = 3000.f;

	if (bSingleBumps) { BuildSingleBumps(Cursor); }
	if (bWashboard)   { BuildWashboard(Cursor); }
	if (bSplitBumps)  { BuildSplitBumps(Cursor); }
	if (bKerb)        { BuildKerb(Cursor); }
	if (bRtiRamp)     { BuildRtiRamp(Cursor); }
	if (bSlalom)      { BuildSlalom(Cursor); }
	if (bWhoops)      { BuildWhoops(Cursor); }
	if (bJumpRamp)    { BuildJumpRamp(Cursor); }
	if (bOffCamber)   { BuildOffCamber(Cursor); }
	if (bRockField)   { BuildRockField(Cursor); }
	if (bSkidpad)     { BuildSkidpad(Cursor); }
	if (bDistanceMarkers) { BuildDistanceMarkers(); }

	UE_LOG(LogProvingGround, Log, TEXT("  %d pieces. Straight runs about %.0f m before the skidpad."),
		Spawned.Num(), Cursor * 0.01f);
}

void AVehicleProvingGround::Clear()
{
	for (AActor* A : Spawned)
	{
		if (IsValid(A))
		{
			A->Destroy();
		}
	}
	Spawned.Reset();
}
