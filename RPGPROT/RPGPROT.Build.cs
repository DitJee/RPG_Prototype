// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGPROT : ModuleRules
{
	public RPGPROT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay" 
		});

		PublicDependencyModuleNames.AddRange(new string[] { 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks",
			"Slate",
			"SlateCore",
			"UMG"
		});
	}
}
