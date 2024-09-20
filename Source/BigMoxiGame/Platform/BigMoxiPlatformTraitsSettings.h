// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Engine/DeveloperSettings.h>
#include <Engine/PlatformSettings.h>
#include <GameplayTagContainer.h>

#include "BigMoxiPlatformTraitsSettings.generated.h"

/**
 * Settings class to manage this module.
 */
UCLASS(MinimalAPI, Config = Game, DefaultConfig, Meta = (DisplayName = "Platform Traits"))
class UPlatformTraitsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	/** Set default values for this class. */
	UPlatformTraitsSettings();

	//~ Begin UDeveloperSettings interface.
	FName GetCategoryName() const override	{ return TEXT("Plugins"); }
	//~ End UDeveloperSettings interface.

	/** Returns all the trait GameplayTags on the current platform. */
	static FGameplayTagContainer GetCurrentPlatformTraits();

	/**
	 * Adds a trait GameplayTag to the current running platform.
	 * @warning The GameplayTag to be added needs to start with "Platform.Trait".
	 * @return If the trait was succesefully added.
	 * @param Trait The tag that will be added to the current platform.
	 */
	static bool AddTraitToCurrentPlatform(FGameplayTag Trait);

private:
	/** Per-platform settings for handling platform unique traits. */
	UPROPERTY(EditAnywhere, Category = "Platform Settings")
	FPerPlatformSettings PlatformTraits;
};

/**
 * Per-platform settings to assign specific traits to specific platforms in the form of a GameplayTag.
 * Can be used for example, to add a "TouchControls" trait to mobile platforms or a "Mouse&Keyboard" to desktop platforms.
 * Then you can query for the traits on the current platform and adjust your code according to what that platform's traits.
 */
UCLASS(MinimalAPI, Config = Game, DefaultConfig, CollapseCategories)
class UPerPlatformTraitsSettings : public UPlatformSettings
{
	GENERATED_BODY()

public:
	/** Set default values for this class. */
	UPerPlatformTraitsSettings();

	/** Default getter for the platform settings. */
	static UPerPlatformTraitsSettings* Get()
	{
		return UPlatformSettingsManager::Get().GetSettingsForPlatform<UPerPlatformTraitsSettings>();
	}

	/** Returns all the trait GameplayTags of this current platform. */
	FGameplayTagContainer GetPlatformTraits() const	{ return Traits; }

protected:
	/** Initialize the platform settings and assign default values. */
	void InitializePlatformDefaults();

private:
	/** This platform's unique traits as a GameplayTag. */
	UPROPERTY(Config, EditAnywhere, Meta = (Categories = "Platform.Trait"))
	FGameplayTagContainer Traits;
};
