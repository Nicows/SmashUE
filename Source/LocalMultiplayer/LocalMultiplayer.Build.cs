using UnrealBuildTool;

public class LocalMultiplayer : ModuleRules
{
    public LocalMultiplayer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "DeveloperSettings",
                "InputCore",
                "EnhancedInput"
            }
        );
        

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}