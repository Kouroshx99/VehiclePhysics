#include "VehicleRoster.h"

#include "SCarRosterWidget.h"
#include "Physics/Car.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputSettings.h"
#include "TimerManager.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"

DEFINE_LOG_CATEGORY_STATIC(LogCarRoster, Log, All);

namespace
{
	/** Class path for one of the plugin's own car Blueprints. */
	FSoftObjectPath PluginCarPath(const TCHAR* Folder, const TCHAR* Asset)
	{
		return FSoftObjectPath(FString::Printf(
			TEXT("/VehiclePhysics/VehiclePhysics/Vehicles/%s/%s.%s_C"), Folder, Asset, Asset));
	}
}

AVehicleRoster::AVehicleRoster()
{
	// Nothing to tick. The swap is driven by input and runs off a next-tick timer.
	PrimaryActorTick.bCanEverTick = false;

	// PRELOADED WITH THE PLUGIN'S OWN CARS so that dropping this into a level works with
	// no configuration at all - which is the whole point of it being an actor rather than
	// a settings page.
	//
	// Soft paths, so listing them costs nothing: none of these load until somebody
	// actually switches to one, and a path that no longer resolves degrades to a logged
	// skip rather than a failure to construct.
	const TCHAR* const Folders[] = { TEXT("Saloon"), TEXT("Sport"),      TEXT("Rally"),  TEXT("Offroad")  };
	const TCHAR* const Assets[]  = { TEXT("BP_Saloon"), TEXT("BP_SportCar"), TEXT("BP_Rally"), TEXT("BP_Offroad") };
	const TCHAR* const Names[]   = { TEXT("Saloon"), TEXT("Sports"),     TEXT("Rally"),  TEXT("Offroad")  };
	static_assert(UE_ARRAY_COUNT(Folders) == UE_ARRAY_COUNT(Assets), "folder/asset mismatch");
	static_assert(UE_ARRAY_COUNT(Folders) == UE_ARRAY_COUNT(Names), "folder/name mismatch");

	for (int32 i = 0; i < UE_ARRAY_COUNT(Folders); ++i)
	{
		FVehicleRosterEntry Entry;
		Entry.DisplayName = Names[i];
		Entry.CarClass = TSoftClassPtr<ACar>(PluginCarPath(Folders[i], Assets[i]));
		Cars.Add(Entry);
	}
}

AVehicleRoster* AVehicleRoster::Find(const UWorld* World)
{
	if (World == nullptr)
	{
		return nullptr;
	}

	AVehicleRoster* First = nullptr;
	int32 Count = 0;
	for (TActorIterator<AVehicleRoster> It(const_cast<UWorld*>(World)); It; ++It)
	{
		if (First == nullptr)
		{
			First = *It;
		}
		++Count;
	}

	if (Count > 1)
	{
		UE_LOG(LogCarRoster, Warning,
			TEXT("%d Vehicle Roster actors in this level; using '%s'. Two rosters cannot both "
				 "govern the switch button, so delete the ones you do not want."),
			Count, *First->GetName());
	}

	return First;
}

void AVehicleRoster::BeginPlay()
{
	Super::BeginPlay();

	// FLASHED ONCE AT LEVEL START, which is the only moment it can actually teach
	// anything. The overlay used to appear only after a switch, so it listed the cars to
	// somebody who had already found the button and told the people who had not found it
	// nothing at all. Shown here it names the key while there is still a use for that,
	// then fades like any other flash and does not come back until a switch.
	ShowRoster();

	// Said once, because a roster with nothing in it is almost certainly an accident -
	// the defaults ship populated, so an empty one means somebody cleared it.
	if (Cars.Num() == 0)
	{
		UE_LOG(LogCarRoster, Warning,
			TEXT("%s has an empty car list, so the switch button will do nothing here. Add "
				 "car Blueprints to its Cars array, or delete the actor if this level is not "
				 "meant to have car switching."),
			*GetName());
	}
}

void AVehicleRoster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (RosterContainer.IsValid() && GEngine != nullptr && GEngine->GameViewport != nullptr)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(RosterContainer.ToSharedRef());
	}
	RosterContainer.Reset();
	RosterWidget.Reset();

	Super::EndPlay(EndPlayReason);
}

int32 AVehicleRoster::FindIndexForPawn() const
{
	const UWorld* World = GetWorld();
	const APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;
	const APawn* Pawn = PC ? PC->GetPawn() : nullptr;
	if (Pawn == nullptr)
	{
		return INDEX_NONE;
	}

	UClass* PawnClass = Pawn->GetClass();

	// Exact match by path first. This deliberately loads nothing - comparing soft paths
	// answers the common case without pulling every car Blueprint into memory.
	const FSoftObjectPath PawnPath(PawnClass);
	for (int32 i = 0; i < Cars.Num(); ++i)
	{
		if (Cars[i].CarClass.ToSoftObjectPath() == PawnPath)
		{
			return i;
		}
	}

	// Then the loose match, among entries already loaded, for a level-placed Blueprint
	// whose PARENT is what the roster names. Most-derived wins, so a roster listing both
	// ASportsCar and a Blueprint of it resolves to the Blueprint.
	int32 Best = INDEX_NONE;
	int32 BestDepth = -1;
	for (int32 i = 0; i < Cars.Num(); ++i)
	{
		UClass* Candidate = Cars[i].CarClass.Get();
		if (Candidate == nullptr || !PawnClass->IsChildOf(Candidate))
		{
			continue;
		}

		int32 Depth = 0;
		for (const UClass* C = Candidate; C != nullptr; C = C->GetSuperClass())
		{
			++Depth;
		}

		if (Depth > BestDepth)
		{
			BestDepth = Depth;
			Best = i;
		}
	}

	return Best;
}

void AVehicleRoster::CycleCar(int32 Delta)
{
	if (Cars.Num() == 0)
	{
		return;   // already reported at BeginPlay; no need to repeat it per keypress
	}

	// Started from what is actually being driven rather than from the last index handed
	// out, so the first press after driving a level-placed car steps off that car instead
	// of jumping to slot 1.
	const int32 Found = FindIndexForPawn();
	const int32 From = (Found != INDEX_NONE) ? Found : CurrentIndex;

	SwitchToCar(((From + Delta) % Cars.Num() + Cars.Num()) % Cars.Num());
}

void AVehicleRoster::SwitchToCar(int32 Index)
{
	if (!Cars.IsValidIndex(Index))
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	// DEFERRED BY A TICK, on purpose. The call arrives from the outgoing car's own input
	// binding, and the swap destroys that car - so the actor, its input component and the
	// delegate currently on the stack all go away underneath the caller. Running it from
	// the timer instead means the input stack has fully unwound first.
	//
	// The guard is what makes a mashed button behave: the last press before the tick wins
	// and the intermediate cars are never built.
	const bool bAlreadyQueued = (PendingIndex != INDEX_NONE);
	PendingIndex = Index;

	if (!bAlreadyQueued)
	{
		World->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateUObject(this, &AVehicleRoster::PerformPendingSwap));
	}
}

void AVehicleRoster::PerformPendingSwap()
{
	const int32 Index = PendingIndex;
	PendingIndex = INDEX_NONE;

	UWorld* World = GetWorld();
	if (World == nullptr || !Cars.IsValidIndex(Index))
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC == nullptr)
	{
		UE_LOG(LogCarRoster, Warning, TEXT("Car switch ignored: no player controller."));
		return;
	}

	UClass* NewClass = Cars[Index].CarClass.LoadSynchronous();
	if (NewClass == nullptr)
	{
		UE_LOG(LogCarRoster, Error,
			TEXT("Roster slot %d (%s) did not load as a car class, so the switch was skipped. "
				 "Check that entry on %s."),
			Index, *Cars[Index].CarClass.ToString(), *GetName());
		return;
	}

	APawn* Old = PC->GetPawn();
	if (Old == nullptr)
	{
		UE_LOG(LogCarRoster, Warning,
			TEXT("Car switch ignored: nothing is possessed, so there is no transform to put "
				 "the replacement at."));
		return;
	}

	// Position and heading are kept so the swap happens where you were standing - that is
	// the point of it, trying the same corner in another car. Pitch and roll are NOT kept:
	// inheriting them puts the replacement in whatever attitude the old car happened to be
	// in, which after a roll is upside down.
	FRotator Rot = Old->GetActorRotation();
	Rot.Pitch = 0.f;
	Rot.Roll = 0.f;
	const FVector Loc = Old->GetActorLocation() + FVector(0.f, 0.f, SwapLiftCm);

	FActorSpawnParameters Params;
	// The outgoing car still occupies this spot for another moment, so the point tests as
	// blocked. It is not - see the same reasoning in the bench -benchspawn path.
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACar* NewCar = World->SpawnActor<ACar>(NewClass, Loc, Rot, Params);
	if (NewCar == nullptr)
	{
		UE_LOG(LogCarRoster, Error,
			TEXT("Roster slot %d failed to spawn; keeping the current car."), Index);
		return;
	}

	// Possess before destroying, so there is never a frame with no pawn - an unpossessed
	// frame is enough for a camera to snap to the origin and back.
	PC->Possess(NewCar);
	Old->Destroy();

	CurrentIndex = Index;

	UE_LOG(LogCarRoster, Log, TEXT("Switched to slot %d (%s) at %s"),
		Index, *NewCar->GetName(), *Loc.ToCompactString());

	ShowRoster();
}

void AVehicleRoster::ShowRoster()
{
	if (RosterDisplaySeconds <= 0.f || GEngine == nullptr || GEngine->GameViewport == nullptr)
	{
		return;
	}

	if (!RosterWidget.IsValid())
	{
		RosterWidget = SNew(SCarRosterWidget);

		RosterContainer =
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			.Padding(30.f, 30.f)
			[
				RosterWidget.ToSharedRef()
			];

		// Above the tyre graphs, which sit at 100 in the bottom-left corner.
		GEngine->GameViewport->AddViewportWidgetContent(RosterContainer.ToSharedRef(), 110);
	}

	TArray<FText> Names;
	for (const FVehicleRosterEntry& Entry : Cars)
	{
		if (!Entry.DisplayName.IsEmpty())
		{
			Names.Add(FText::FromString(Entry.DisplayName));
			continue;
		}

		// Falls back to the asset name, with the Blueprint class suffix trimmed so the
		// list reads BP_SportCar rather than BP_SportCar_C.
		FString Name = Entry.CarClass.GetAssetName();
		Name.RemoveFromEnd(TEXT("_C"));
		Names.Add(FText::FromString(Name));
	}

	// Highlighted by what is actually being driven, not by the last slot handed out.
	// CurrentIndex is still 0 at BeginPlay, so trusting it would mark the first car on a
	// level whose placed vehicle is any of the others. After a swap the two agree, since
	// the new car has just been possessed.
	const int32 Found = FindIndexForPawn();
	RosterWidget->SetRoster(Names, (Found != INDEX_NONE) ? Found : CurrentIndex,
		BuildControlHint());
	RosterWidget->Flash(RosterDisplaySeconds);
}

FText AVehicleRoster::BuildControlHint() const
{
	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (Settings == nullptr)
	{
		return FText::GetEmpty();
	}

	// Every key bound to an action, in the order the project lists them, joined for
	// display. Reading them rather than assuming them is the whole point - see the header.
	auto KeysFor = [Settings](const TCHAR* ActionName) -> FString
	{
		TArray<FInputActionKeyMapping> Mappings;
		Settings->GetActionMappingByName(FName(ActionName), Mappings);

		TArray<FString> KeyNames;
		for (const FInputActionKeyMapping& Mapping : Mappings)
		{
			KeyNames.Add(Mapping.Key.GetDisplayName().ToString());
		}
		return FString::Join(KeyNames, TEXT(" / "));
	};

	const FString Next = KeysFor(TEXT("NextCar"));
	const FString Prev = KeysFor(TEXT("PrevCar"));

	// Nothing bound at all: say nothing rather than print an empty label. This happens if
	// a project strips the plugin's input config, and the silence is the honest answer.
	if (Next.IsEmpty() && Prev.IsEmpty())
	{
		return FText::GetEmpty();
	}

	if (Prev.IsEmpty())
	{
		return FText::FromString(FString::Printf(TEXT("next  %s"), *Next));
	}
	if (Next.IsEmpty())
	{
		return FText::FromString(FString::Printf(TEXT("previous  %s"), *Prev));
	}

	return FText::FromString(FString::Printf(TEXT("next  %s      previous  %s"), *Next, *Prev));
}
