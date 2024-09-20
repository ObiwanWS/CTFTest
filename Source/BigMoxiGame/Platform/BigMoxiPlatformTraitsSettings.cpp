// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiPlatformTraitsSettings.h"
#include "BigMoxiGameDebug.h"
#include "BigMoxiTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiPlatformTraitsSettings)

UPlatformTraitsSettings::UPlatformTraitsSettings()
{
	PlatformTraits.Initialize(UPerPlatformTraitsSettings::StaticClass());
}

FGameplayTagContainer UPlatformTraitsSettings::GetCurrentPlatformTraits()
{
	if (const UPerPlatformTraitsSettings* settings = UPlatformSettingsManager::Get().GetSettingsForPlatform<UPerPlatformTraitsSettings>())
	{
		return settings->GetPlatformTraits();
	}
	else
	{
		return FGameplayTagContainer::EmptyContainer;
	}
}

bool UPlatformTraitsSettings::AddTraitToCurrentPlatform(FGameplayTag Trait)
{
	if (Trait.IsValid())
	{
		if (Trait.MatchesTag(Platform_Trait))
		{
			if (UPerPlatformTraitsSettings* settings = UPlatformSettingsManager::Get().GetSettingsForPlatform<UPerPlatformTraitsSettings>())
			{
				settings->GetPlatformTraits().AddTag(Trait);
				return true;
			}
		}
		else
		{
			UE_LOG(LogBigMoxiGame, Error, TEXT("%s: Attempted to add a GameplayTag (%s) that doesn't start with '%s'."),
				ANSI_TO_TCHAR(__FUNCTION__), *Trait.GetTagName().ToString(), *Platform_Trait.GetTag().GetTagName().ToString())
		}
	}
	else
	{
		UE_LOG(LogBigMoxiGame, Error, TEXT("%s: Attempted to add an invalid GameplayTag (%s)."), ANSI_TO_TCHAR(__FUNCTION__),
			*Trait.GetTagName().ToString())
	}
	return false;
}

UPerPlatformTraitsSettings::UPerPlatformTraitsSettings()
{
	Traits = FGameplayTagContainer::EmptyContainer;
}

void UPerPlatformTraitsSettings::InitializePlatformDefaults()
{
	const FDataDrivenPlatformInfo& platformInfo = FDataDrivenPlatformInfoRegistry::GetPlatformInfo(GetPlatformIniName());

	if (platformInfo.bHasDedicatedGamepad)
	{
		Traits.AddTag(Platform_Trait_DedicatedGamepad);
	}

	if (platformInfo.bSupportsGamepad)
	{
		Traits.AddTag(Platform_Trait_Gamepad);
	}

	if (platformInfo.bSupportsMouseAndKeyboard)
	{
		Traits.AddTag(Platform_Trait_MouseAndKeyboard);
	}

	if (platformInfo.bSupportsTouch)
	{
		Traits.AddTag(Platform_Trait_Touch);
	}
}
