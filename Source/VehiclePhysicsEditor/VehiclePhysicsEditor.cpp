#include "VehiclePhysicsEditor.h"

#include "AssetToolsModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "PhysicsEditor/UTireParamsDataAssetActions.h"
#include "PhysicsEditor/WheelColliderVisualizer.h"
#include "PhysicsEditor/WishboneSuspensionVisualizer.h"
#include "VehiclePhysics/Vehicles/Physics/WheelCollider.h"
#include "VehiclePhysics/Vehicles/Physics/WishboneSuspension.h"
#include "UnrealEd.h"
#include "PhysicsEditor/TireParamsEditorStyle.h"


IMPLEMENT_GAME_MODULE(FVehiclePhysicsEditorModule, TehranEditor);
DEFINE_LOG_CATEGORY(VehiclePhysicsEditor)

#define LOCTEXT_NAMESPACE "VehcilePhysicsEditor"
void FVehiclePhysicsEditorModule::StartupModule()
{
	IModuleInterface::StartupModule();
	UE_LOG(LogTemp, Log, TEXT("VehiclePhysicsEditor StartupModule"));
	TSharedPtr<FWheelColliderVisualizer> WheelColliderVisualizer = MakeShared<FWheelColliderVisualizer>();
	
	TSharedPtr<FWishboneSuspensionVisualizer> WishboneVisualizer = MakeShared<FWishboneSuspensionVisualizer>();

	if(GUnrealEd)
	{
		UE_LOG(LogTemp, Log, TEXT("VehiclePhysicsEditor registering"));

		// The name is recorded alongside each visualizer because that is what
		// unregistering takes - see RegisteredVisualizerClassNames.
		auto Register = [this](FName ClassName, TSharedPtr<FComponentVisualizer> Visualizer)
		{
			Visualizer->OnRegister();
			GUnrealEd->RegisterComponentVisualizer(ClassName, Visualizer);
			RegisteredVisualizers.Add(Visualizer);
			RegisteredVisualizerClassNames.Add(ClassName);
		};

		Register(UWheelCollider::StaticClass()->GetFName(), WheelColliderVisualizer);
		Register(UWishboneSuspension::StaticClass()->GetFName(), WishboneVisualizer);
	}

	IAssetTools* AssetTools = &FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	TSharedPtr<IAssetTypeActions> Action = MakeShareable(new UTireParamsDataAssetActions);
	AssetTools->RegisterAssetTypeActions(Action.ToSharedRef());
	RegisteredAssetActions.Add(Action);

	FTireParamsEditorStyle::Initialize();
}

void FVehiclePhysicsEditorModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
	UE_LOG(LogTemp, Log, TEXT("VehiclePhysicsEditor ShutdownModule"));
	
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

		for (auto& Action : RegisteredAssetActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}
	RegisteredAssetActions.Empty();

	FTireParamsEditorStyle::Shutdown();

	// By recorded name, one per registration. The previous loop named UWheelCollider
	// on every iteration, which unregistered it repeatedly and left anything else
	// registered against unloaded code.
	if (GUnrealEd)
	{
		for (const FName& ClassName : RegisteredVisualizerClassNames)
		{
			GUnrealEd->UnregisterComponentVisualizer(ClassName);
		}
	}
	RegisteredVisualizerClassNames.Empty();
	RegisteredVisualizers.Empty();
}
#undef LOCTEXT_NAMESPACE