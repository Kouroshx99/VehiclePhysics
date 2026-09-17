// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VehiclePhysicsEditor : ModuleRules
{
	public VehiclePhysicsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Unity off, matching the runtime module - see the longer note there. The editor
		// module had its own instance of the same problem: UTireParamsAssetEdtitor.cpp
		// called FTicker::GetCoreTicker, which UE5 renamed, and it went unnoticed because
		// the file was never compiled by itself.
		//
		// Per-module, so a consuming project keeps unity builds for its own code.
		bUseUnity = false;

		
		PublicIncludePaths.Add(ModuleDirectory);
		PrivateIncludePaths.Add(ModuleDirectory);
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core",
			"CoreUObject",
			"Engine",
			"VehiclePhysics"
		});
		
		PrivateDependencyModuleNames.AddRange(
			new string[] 
			{
				"Slate",
				"SlateCore",
				"EditorStyle",
				"UnrealEd",
				"Projects",      // For IPluginManager
				"AssetTools",    // For AssetTypeActions
				"PropertyEditor",
				"RenderCore",
				"InputCore",
				"LevelEditor"
			});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
