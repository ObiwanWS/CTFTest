// Copyright (c) 2024 Obiwan Medeiros.

using UnrealBuildTool;

public class BigMoxiClientTarget : TargetRules
{
	public BigMoxiClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
        
        ExtraModuleNames.Add("BigMoxiGame");

        BigMoxiGameTarget.ApplySharedTargetSettings(this);
    }
}
