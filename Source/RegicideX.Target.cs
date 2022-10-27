// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RegicideXTarget : TargetRules
{
	public RegicideXTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("RegicideX");
		bUseLoggingInShipping = true;
		bOverrideBuildEnvironment = true;
	}
}
