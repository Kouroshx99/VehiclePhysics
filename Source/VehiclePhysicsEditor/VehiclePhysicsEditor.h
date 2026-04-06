#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FComponentVisualizer;
class IAssetTypeActions;
DECLARE_LOG_CATEGORY_EXTERN(VehiclePhysicsEditor, All, All)

class FVehiclePhysicsEditorModule: public IModuleInterface
{
public:
	TArray<TSharedPtr<FComponentVisualizer>> RegisteredVisualizers;
	TArray<TSharedPtr<IAssetTypeActions>> RegisteredAssetActions;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
