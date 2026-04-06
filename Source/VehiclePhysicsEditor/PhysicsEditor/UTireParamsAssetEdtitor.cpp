#include "UTireParamsAssetEdtitor.h"

#include "VehiclePhysics/Vehicles/STireGraphWidget.h"

void UTireParamsAssetEdtitor::InitTireParamsEditor(const EToolkitMode::Type Mode,
                                                   const TSharedPtr<class IToolkitHost>& InitToolkitHost, UTireParamsDataAsset* Asset)
{
	EditingAsset = Asset;

	FPropertyEditorModule& PropertyEditorModule = 
		FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(EditingAsset);
	
	//EditorSettings = NewObject<UTireEditorSettings>();
	//EditorSettings = NewObject<UTireEditorSettings>(EditingAsset);
	//EditorSettings = NewObject<UTireEditorSettings>(EditingAsset, UTireEditorSettings::StaticClass(), NAME_None, RF_Transactional | RF_Transient);
	//EditorSettings->AddToRoot(); // optional, to prevent garbage collection while editor is open
	//TArray<UObject*> ObjectsToEdit;
	//ObjectsToEdit.Add(EditingAsset);
	//ObjectsToEdit.Add(EditorSettings);

	//DetailsView->SetObjects(ObjectsToEdit, true);
	//DetailsView->SetObjects({EditingAsset, EditorSettings.Get()});

	GraphWidget = SNew(STireGraphWidget).Asset(EditingAsset).bIsInteractive(true);

	// Splitter layout
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("TireParamsEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->AddTab("DetailsTab", ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewStack()
				->AddTab("GraphTab", ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
		);

	InitAssetEditor(Mode, InitToolkitHost, FName("TireParamsEditorApp"),
	Layout, true, true, Asset);
	
	// Slate ticker for updating values each frame
	float a = 0.f;
	FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float DeltaTime)
	{
		if (GraphWidget.IsValid())
		{
			// Pass editor-only values
			GraphWidget->CurrentSlipPoint = FVector(EditingAsset->SlipRatio, 
				EditingAsset->SlipAngle,
				EditingAsset->Camber);
		}
		return true; // continue ticking
	}));
}

void UTireParamsAssetEdtitor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	InTabManager->RegisterTabSpawner("DetailsTab", FOnSpawnTab::CreateLambda([DetailsViewPtr = DetailsView](const FSpawnTabArgs&)
	{
		if (DetailsViewPtr.IsValid())
		{
			return SNew(SDockTab).Label(FText::FromString("Details"))[ DetailsViewPtr.ToSharedRef() ];
		}
		return SNew(SDockTab).Label(FText::FromString("Details"));
		/*return SNew(SDockTab).Label(FText::FromString("Details"))
		[
			DetailsView.ToSharedRef()
		];*/
	}));

	InTabManager->RegisterTabSpawner("GraphTab", FOnSpawnTab::CreateLambda([GraphWidgetPtr = GraphWidget](const FSpawnTabArgs&)
	{
		if (GraphWidgetPtr.IsValid())
		{
			return SNew(SDockTab).Label(FText::FromString("Graph"))[ GraphWidgetPtr.ToSharedRef() ];
		}
		return SNew(SDockTab).Label(FText::FromString("Graph"));
		/*return SNew(SDockTab).Label(FText::FromString("Graph"))
		[
			GraphWidget.ToSharedRef()
		];*/
	}));
}
