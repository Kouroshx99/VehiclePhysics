// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class VehiclePhysics : ModuleRules
{
	public VehiclePhysics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// UNITY BUILDS OFF FOR THIS MODULE, on purpose, and it applies only to this module.
		//
		// Unreal normally pastes many .cpp files into one translation unit to compile
		// faster. That also lets a file use whatever its neighbours happened to include,
		// so a file missing an include still builds - until the day it is compiled on its
		// own, which is what happens to every file in a freshly copied plugin. Three files
		// here were broken that way for a long time and nobody could have known: they
		// compiled perfectly in the project they were written in and failed immediately
		// for anyone who installed the plugin.
		//
		// Since this plugin is meant to be dropped into other people's projects, the
		// arrangement that catches the bug is the one it should always build under. Every
		// file now has to stand on its own, every time.
		//
		// It costs compile time here and NOTHING anywhere else: bUseUnity is a per-module
		// setting, so a project consuming this plugin keeps unity builds for all of its
		// own code.
		bUseUnity = false;

		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"InputCore", "ChaosVehiclesCore",
				// Landscape sculpting for AVehicleTestCircuit. Both are RUNTIME modules in
				// 4.27 - Runtime/Landscape and Runtime/Foliage - so this stays a runtime
				// module and the sculpt code is #if WITH_EDITOR instead of moving house.
				//
				// Foliage is not a mistake and not optional. LandscapeEdit.h line 18 has an
				// unconditional #include "InstancedFoliageActor.h", and beyond the include,
				// FHeightmapAccessor::SetData calls AInstancedFoliageActor::HasFoliageAttached,
				// which is FOLIAGE_API - a real link edge. Epic's own LandscapeEditor.Build.cs
				// carries "Foliage" for exactly this reason.
				"Landscape", "Foliage"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		

		// Chaos and PhysicsCore back the physics-thread particle read in
		// ACar::ReadBodyState, where the game-thread copy is a frame stale inside an
		// async tick. 4.27 drives the vehicle through PhysX substepping and needs
		// neither, and asking for them there does not link.
		if (Target.Version.MajorVersion >= 5)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Chaos", "PhysicsCore" });
		}

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
	}
}
