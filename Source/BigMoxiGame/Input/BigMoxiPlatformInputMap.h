// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "BigMoxiPlatformInputMap.generated.h"

class UInputMappingContext;

/**
 * A extended version of a InputMappingContext.
 * Will be added only on platforms that pass the GameplayTag query.
 */
USTRUCT()
struct FBigMoxiPlatformInputContext
{
	GENERATED_BODY()

public:
	/** The InputMappingContext to be added on supported platforms. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputMappingContext> InputMappingContext;

	/** The priority of this InputMappingContext. */
	UPROPERTY(EditDefaultsOnly)
	int32 Priority;

	/** The traits the platform needs to have all of to be considered a supported platform. */
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Platform.Trait"))
	FGameplayTagContainer PlatformAllowTraits;

	/** The traits the platform can't have any of to be considered a supported platform. */
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Platform.Trait"))
	FGameplayTagContainer PlatformDenyTraits;

	/** Set default values for this struct. */
	FBigMoxiPlatformInputContext()
	{
		InputMappingContext = nullptr;
		Priority = 0;
		PlatformAllowTraits = FGameplayTagContainer::EmptyContainer;
		PlatformDenyTraits = FGameplayTagContainer::EmptyContainer;
	}

	/** Returns if this ExtendedInputMappingContext has all of its data valid. */
	bool IsValid() const;

	/**
	 * Returns if this ExtendedInputMappingContext is supported on by having all the given traits.
	 * @param Traits The GameplayTagContainer to compare if this InputMappingContext fits that trait criteria.
	 */
	bool IsSupported(const FGameplayTagContainer Traits) const
	{
		if (Traits.IsValid())
		{
			return Traits.HasAll(PlatformAllowTraits) && !Traits.HasAny(PlatformDenyTraits);
		}
		return false;
	}
};

/**
 * Container for defining several InputMappingContext that are platform filtered.
 */
UCLASS(NotBlueprintable, Const, MinimalAPI)
class UBigMoxiPlatformInputMap : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/** Set default values for this class. */
	UBigMoxiPlatformInputMap();

	//~ Begin UObject interface.
#if WITH_EDITOR
	EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
	//~ End UObject interface.

	/** Returns all the InputMappingContext that are supported by the current running platform. */
	TArray<const UInputMappingContext*> GetSupportedInputMappingContexts() const;

	/** Returns all the ExtendedInputMappingContext that are supported by the current running platform. */
	const TArray<FBigMoxiPlatformInputContext> GetSupportedInputMappings() const;

	/**
	 * Returns the supported ExtendedInputMappingContext that pass the given filter in PlatformTraits.
	 * @param PlatformTraits The tag container to check for compatible Input Mappings.
	 */
	TArray<FBigMoxiPlatformInputContext> FindMatchingInputMappings(const FGameplayTagContainer& PlatformTraits) const;

private:
	/** The InputMappingContexts to be defined with a platform traits filter.  */
	UPROPERTY(EditDefaultsOnly, Category = "Input|Mappings", Meta = (TitleProperty = "InputMappingContext"))
	TArray<FBigMoxiPlatformInputContext> InputMapContexts;
};
