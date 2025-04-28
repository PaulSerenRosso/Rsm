// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ProjectEscapeProto : ModuleRules
{
	public ProjectEscapeProto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "VivoxCore", "PerfCounters", "AdvancedSessions", "NavigationSystem", "AIModule" });
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ApplicationCore", "UMG", "EnhancedInput", "AIModule", "GameplayTasks" });
		//CppStandard = CppStandardVersion.Cpp17; // Ou Cpp14 si besoin


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
