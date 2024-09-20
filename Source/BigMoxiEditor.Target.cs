// Copyright (c) 2024 Obiwan Medeiros.

using UnrealBuildTool;

public class BigMoxiEditorTarget : TargetRules
{
	public BigMoxiEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        
        ExtraModuleNames.AddRange(
            new string[]
            {
                "BigMoxiGame",
                "BigMoxiEditor"
            }
        );

        BigMoxiGameTarget.ApplySharedTargetSettings(this);
    }
}
