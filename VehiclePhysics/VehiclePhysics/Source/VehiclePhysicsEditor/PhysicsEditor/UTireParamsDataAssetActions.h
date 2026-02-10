#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class UTireParamsDataAssetActions : public FAssetTypeActions_Base
{
public:
	// Which class this action applies to
	virtual UClass* GetSupportedClass() const override;
	// How it should be displayed in the menu
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "FTireParams", "Tire Params"); }
	virtual FColor GetTypeColor() const override { return FColor(255, 200, 50); }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Physics; }
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
};
