// Copyright (c) 2024 Obiwan Medeiros.

using UnrealBuildTool;

public class BigMoxiServerTarget : TargetRules
{
	public BigMoxiServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

        ExtraModuleNames.Add("BigMoxiGame");

        BigMoxiGameTarget.ApplySharedTargetSettings(this);
    }
}
