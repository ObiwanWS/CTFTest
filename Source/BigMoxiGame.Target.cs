// Copyright (c) 2024 Obiwan Medeiros.

using UnrealBuildTool;

public class BigMoxiGameTarget : TargetRules
{
	public BigMoxiGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		
		ExtraModuleNames.Add("BigMoxiGame");

        ApplySharedTargetSettings(this);
    }

    internal static void ApplySharedTargetSettings(TargetRules Target)
    {
        // Configure build settings for all targets.
        Target.DefaultBuildSettings = BuildSettingsVersion.Latest;
        Target.IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        if (Target.Type == UnrealBuildTool.TargetType.Editor)
        {
            // Raw pointers should be replaced with TObjectPtr.
            Target.NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
        }

        if (Target.BuildEnvironment == TargetBuildEnvironment.Unique)
        {
            // Disable needless features.
            Target.bBuildDeveloperTools = false;
            Target.bCompileSpeedTree = false;
            Target.bCompileCEF3 = false;
            Target.bUseXGEController = false;
            Target.bCompilePython = false;
            Target.bUseChaosMemoryTracking = false;
            Target.bCompileRecast = false;
            Target.bCompileNavmeshSegmentLinks = false;
            Target.bCompileNavmeshClusterLinks = false;
            Target.bCompileICU = false;
            Target.bUseGameplayDebugger = false;
            Target.bUseGameplayDebuggerCore = false;
            Target.bCompileWithAccessibilitySupport = false;
            Target.bUsesSlate = false;

            // Make compilation more strict.
            Target.bWarningsAsErrors = true;
            Target.bUndefinedIdentifierErrors = true;
            Target.ShadowVariableWarningLevel = WarningLevel.Error;

            // Enable push model for networking.
            Target.bWithPushModel = true;

            if (Target.Configuration == UnrealTargetConfiguration.Shipping || Target.Configuration == UnrealTargetConfiguration.Test)
            {
                if (Target.Type == UnrealBuildTool.TargetType.Server)
                {
                    // Enable checks for dedicated servers even on shipping builds.
                    Target.bUseChecksInShipping = true;
                }
                else
                {
                    // Make sure that we validate certificates for HTTPS traffic.
                    Target.bDisableUnverifiedCertificates = true;
                }

                // Block ini access in shipping.
                Target.bAllowGeneratedIniWhenCooked = false;
                Target.bAllowNonUFSIniWhenCooked = false;

                // Lock down command line.
                Target.GlobalDefinitions.Add("UE_COMMAND_LINE_USES_ALLOW_LIST=1");
                Target.GlobalDefinitions.Add("UE_OVERRIDE_COMMAND_LINE_ALLOW_LIST=\"-windowed -fullscreen\"");
            }
        }
    }
}
