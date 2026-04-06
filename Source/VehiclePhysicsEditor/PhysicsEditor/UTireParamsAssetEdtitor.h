#pragma once
#include "TireSettingsEditor.h"

//class UTireEditorSettings;
class STireGraphWidget;
class UTireParamsDataAsset;

class UTireParamsAssetEdtitor : public FAssetEditorToolkit
{
public:
	void InitTireParamsEditor(const EToolkitMode::Type Mode, 
							  const TSharedPtr<class IToolkitHost>& InitToolkitHost, 
							  UTireParamsDataAsset* Asset);

	// Required overrides
	virtual FName GetToolkitFName() const override { return FName("TireParamsEditor"); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString("Tire Params Editor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("TireParams"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White; }
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
protected:
	UTireEditorSettings* EditorSettings;
	UTireParamsDataAsset* EditingAsset;

	TSharedPtr<IDetailsView> DetailsView;
	TSharedPtr<STireGraphWidget> GraphWidget;
};
