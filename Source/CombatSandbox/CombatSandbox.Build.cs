// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CombatSandbox : ModuleRules
{
	public CombatSandbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"CombatSandbox",
			"CombatSandbox/Variant_Platforming",
			"CombatSandbox/Variant_Platforming/Animation",
			"CombatSandbox/Variant_Combat",
			"CombatSandbox/Variant_Combat/AI",
			"CombatSandbox/Variant_Combat/Animation",
			"CombatSandbox/Variant_Combat/Gameplay",
			"CombatSandbox/Variant_Combat/Interfaces",
			"CombatSandbox/Variant_Combat/UI",
			"CombatSandbox/Variant_SideScrolling",
			"CombatSandbox/Variant_SideScrolling/AI",
			"CombatSandbox/Variant_SideScrolling/Gameplay",
			"CombatSandbox/Variant_SideScrolling/Interfaces",
			"CombatSandbox/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
