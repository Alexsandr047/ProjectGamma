// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectGammaTarget : TargetRules
{
	public ProjectGammaTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("ProjectGamma");
		ExtraModuleNames.Add("CoverSystem"); 
		ExtraModuleNames.Add("CharacterInitializationSystem");
	}
}
