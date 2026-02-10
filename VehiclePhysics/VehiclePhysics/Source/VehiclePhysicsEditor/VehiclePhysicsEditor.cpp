#include "VehiclePhysicsEditor.h"

#include "AssetToolsModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "PhysicsEditor/UTireParamsDataAssetActions.h"
#include "PhysicsEditor/WheelColliderVisualizer.h"
#include "VehiclePhysics/Vehicles/Physics/WheelCollider.h"
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
	
	if(GUnrealEd)
	{
		WheelColliderVisualizer->OnRegister();
		GUnrealEd->RegisterComponentVisualizer(UWheelCollider::StaticClass()->GetFName(), WheelColliderVisualizer);
		UE_LOG(LogTemp, Log, TEXT("VehiclePhysicsEditor registering"));
		RegisteredVisualizers.Add(WheelColliderVisualizer);
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
	for (auto& Vis : RegisteredVisualizers)
	{
		if (GUnrealEd)
			GUnrealEd->UnregisterComponentVisualizer(UWheelCollider::StaticClass()->GetFName());
	}

	//if(GUnrealEd)
	//	GUnrealEd->UnregisterComponentVisualizer(UWheelCollider::StaticClass()->GetFName());
}
#undef LOCTEXT_NAMESPACE