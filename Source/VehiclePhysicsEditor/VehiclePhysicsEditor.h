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

	/**
	 * The component class each visualizer was registered against, same order.
	 *
	 * GUnrealEd keys visualizers by class NAME, so unregistering needs the name
	 * rather than the visualizer - and with more than one registered, naming a
	 * single class in the shutdown loop unregisters that one repeatedly and leaks
	 * the rest. A leaked registration outlives a hot reload still pointing at
	 * unloaded code.
	 */
	TArray<FName> RegisteredVisualizerClassNames;
	TArray<TSharedPtr<IAssetTypeActions>> RegisteredAssetActions;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
