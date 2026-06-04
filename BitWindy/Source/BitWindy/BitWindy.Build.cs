using UnrealBuildTool;

public class BitWindy : ModuleRules
{
    public BitWindy(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
 {
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "EnhancedInput",
    "OnlineSubsystem",
    "OnlineSubsystemUtils",
    "AdvancedSessions",
    "AdvancedSteamSessions"
 });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
    "OnlineSubsystem",
    "OnlineSubsystemUtils",
    "AdvancedSessions",
    "AdvancedSteamSessions"
        });
    }
}
