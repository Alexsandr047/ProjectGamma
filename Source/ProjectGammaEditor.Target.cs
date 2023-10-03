// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectGammaEditorTarget : TargetRules
{
	public ProjectGammaEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("ProjectGamma");
		ExtraModuleNames.Add("CoverSystem"); 
		ExtraModuleNames.Add("CharacterInitializationSystem");
	}
}
