#include "TireParamsEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "VehiclePhysics/Vehicles/Physics/TireParamsDataAsset.h"

TSharedPtr<FSlateStyleSet> FTireParamsEditorStyle::StyleInstance = nullptr;

void FTireParamsEditorStyle::Initialize()
{
	if (StyleInstance.IsValid())
	{
		return;
	}

	StyleInstance = MakeShared<FSlateStyleSet>(GetStyleSetName());
	StyleInstance->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	StyleInstance->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	FString PluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("VehiclePhysics"))->GetBaseDir();
	FString ResourcesDir = PluginBaseDir / TEXT("Resources");
	// Point the style set to use your plugin’s Resources folder
	StyleInstance->SetContentRoot(ResourcesDir);
	FString IconFile = ResourcesDir / TEXT("TireIcon.png");

	if (FPaths::FileExists(IconFile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Tire icon found at: %s"), *IconFile);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Tire icon NOT found at: %s"), *IconFile);
	}
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);
	StyleInstance->Set("ClassIcon.TireParamsDataAsset",
	new FSlateImageBrush(StyleInstance->RootToContentDir(TEXT("TireIcon.png")), Icon16x16));
	StyleInstance->Set("ClassThumbnail.TireParamsDataAsset",
	new FSlateImageBrush(StyleInstance->RootToContentDir(TEXT("TireIcon.png")), Icon64x64));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
}

void FTireParamsEditorStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();
	}
}

const ISlateStyle& FTireParamsEditorStyle::Get()
{
	return *StyleInstance;
}

FName FTireParamsEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("TireParamsEditorStyle"));
	return StyleSetName;
}
