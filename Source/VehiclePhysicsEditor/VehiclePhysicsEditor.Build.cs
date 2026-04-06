// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VehiclePhysicsEditor : ModuleRules
{
	public VehiclePhysicsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
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
