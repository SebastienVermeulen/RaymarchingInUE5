// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EditorCustomShaderNode : ModuleRules
{
	public EditorCustomShaderNode(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {});
				
		PrivateIncludePaths.AddRange(new string[] {"/Engine/Source/Runtime/Engine/Classes/",});
		
		PublicDependencyModuleNames.AddRange(new string[]{"Core",});
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"RenderCore",
				//"ShaderCore",
				"AssetTools",
			});
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[] { "DetailCustomizations", "PropertyEditor", "EditorStyle", });
			PublicDependencyModuleNames.AddRange(
				new string[] {	"UnrealEd",	});
		}
		
		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}
