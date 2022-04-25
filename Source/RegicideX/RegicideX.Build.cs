// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RegicideX : ModuleRules
{
	public RegicideX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
		"GameplayTags", "AIModule", "GameplayTasks"});


		PublicDependencyModuleNames.Add("GameplayAbilities");
		PublicDependencyModuleNames.Add("MCVGASFramework");
	}
}