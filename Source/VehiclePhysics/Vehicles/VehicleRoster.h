#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VehicleRoster.generated.h"

class ACar;
class SCarRosterWidget;

/** One switchable car: what to spawn, and what to call it on screen. */
USTRUCT()
struct FVehicleRosterEntry
{
	GENERATED_BODY()

	/** Shown in the on-screen list. Left empty, the asset name is used. */
	UPROPERTY(EditAnywhere, Category = "Roster")
	FString DisplayName;

	/**
	 * SOFT on purpose. Listing a car here costs nothing until somebody switches to it -
	 * a hard reference would pull every car in the roster into memory the moment the
	 * level loaded, meshes and all.
	 */
	UPROPERTY(EditAnywhere, Category = "Roster", meta = (AllowAbstract = "false"))
	TSoftClassPtr<ACar> CarClass;
};

/**
 * Place one of these in a level to turn on car switching there.
 *
 * IT IS AN ACTOR, AND THAT IS THE DESIGN. This used to be a UDeveloperSettings page plus
 * a world subsystem, which meant the plugin reached into every level of every project
 * that installed it: someone using one car in one level got the switch button live in
 * their main menu, configured from a Project Settings page they had never seen. A plugin
 * should not do that.
 *
 * As an actor the feature is opt-in per level by construction. No roster in the level,
 * no switching - silently, because nothing has gone wrong; the level simply does not use
 * it. The roster is configured on the instance, in the level, next to the thing it
 * affects, and it is discoverable in the Place Actors panel rather than hidden in
 * settings.
 *
 * It also still outlives the pawn, which was the one thing the subsystem was for. The
 * swap destroys the car, not this.
 *
 * Comes preloaded with the four cars that ship with the plugin, so dropping it in works
 * immediately; edit Cars to reorder it, trim it, or point it at your own Blueprints.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Vehicle Roster"))
class VEHICLEPHYSICS_API AVehicleRoster : public AActor
{
	GENERATED_BODY()

public:
	AVehicleRoster();

	/**
	 * The cars the switch button cycles through, in order.
	 *
	 * These want to be BLUEPRINTS. The C++ car classes carry the simulation but assign no
	 * skeletal mesh, and a car with no mesh has no physics body - ACar::BeginPlay reports
	 * that and refuses to simulate rather than crashing, but it still will not drive.
	 */
	UPROPERTY(EditAnywhere, Category = "Roster")
	TArray<FVehicleRosterEntry> Cars;

	/** How long the roster stays on screen after a switch. Zero hides it entirely. */
	UPROPERTY(EditAnywhere, Category = "Roster", meta = (ClampMin = "0.0", Units = "s"))
	float RosterDisplaySeconds = 2.5f;

	/**
	 * Lift applied when the replacement is placed, cm.
	 *
	 * The new car inherits the old one's transform but not its ride height - a saloon and
	 * an offroad car sit differently on the same ground - so placing one exactly where the
	 * other sat can start it inside the terrain. Keep it small: this is a settle, not a
	 * drop, and dropping a car from height is what makes the suspension read a ride height
	 * that is not there yet.
	 */
	UPROPERTY(EditAnywhere, Category = "Roster", meta = (ClampMin = "0.0", Units = "cm"))
	float SwapLiftCm = 25.f;

	/** Cycles forward, or back with a negative step. Wraps. */
	void CycleCar(int32 Delta);

	/** Switches to a specific slot. Out-of-range indices are ignored. */
	void SwitchToCar(int32 Index);

	/**
	 * The roster governing this world, or null when the level has none.
	 *
	 * Null is the ordinary case for a level that does not use car switching, so callers
	 * should treat it as "feature off" rather than as an error.
	 */
	static AVehicleRoster* Find(const UWorld* World);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** Works out which slot the current pawn corresponds to, so cycling starts from it. */
	int32 FindIndexForPawn() const;

	void ShowRoster();

	/**
	 * The control line for the overlay, built from the project's ACTUAL bindings for
	 * NextCar and PrevCar rather than from the keys this plugin happens to ship.
	 *
	 * A hardcoded hint is worse than none the moment somebody rebinds: it then states
	 * something confidently and wrongly. Reading the mappings costs nothing and cannot
	 * go stale.
	 */
	FText BuildControlHint() const;

	/** Runs the deferred swap - see the note on the deferral in SwitchToCar. */
	void PerformPendingSwap();

	int32 CurrentIndex = 0;

	/** Slot waiting to be swapped in, or INDEX_NONE when nothing is queued. */
	int32 PendingIndex = INDEX_NONE;

	TSharedPtr<SCarRosterWidget> RosterWidget;

	/** The wrapper actually handed to the viewport, kept so it can be removed again. */
	TSharedPtr<SWidget> RosterContainer;
};
