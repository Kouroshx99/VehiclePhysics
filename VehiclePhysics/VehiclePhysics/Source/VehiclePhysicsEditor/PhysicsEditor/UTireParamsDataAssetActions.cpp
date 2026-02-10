#include "UTireParamsDataAssetActions.h"

#include "UTireParamsAssetEdtitor.h"
#include "VehiclePhysics/Vehicles/Physics/TireParamsDataAsset.h"

UClass* UTireParamsDataAssetActions::GetSupportedClass() const
{
	return UTireParamsDataAsset::StaticClass();
}

void UTireParamsDataAssetActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
	for (auto Obj : InObjects)
	{
		UTireParamsDataAsset* Data = Cast<UTireParamsDataAsset>(Obj);
		if (Data)
		{
			TSharedRef<UTireParamsAssetEdtitor> Editor = MakeShared<UTireParamsAssetEdtitor>();
			Editor->InitTireParamsEditor(EToolkitMode::Standalone, EditWithinLevelEditor, Data);
		}
	}
}
