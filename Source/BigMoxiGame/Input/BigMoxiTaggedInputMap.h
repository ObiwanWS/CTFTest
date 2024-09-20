// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "BigMoxiTaggedInputMap.generated.h"

class UInputAction;

/**
 * An InputAction mapped to a GameplayTag.
 */
USTRUCT()
struct FBigMoxiTaggedInput
{
	GENERATED_BODY()

public:
	/** The InputAction that will be mapped to a GameplayTag. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction;

	/** The tag that is mapped to the InputAction. */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	/** Set default values for this struct. */
	FBigMoxiTaggedInput()
	{
		InputAction = nullptr;
		InputTag = FGameplayTag::EmptyTag;
	}

	/** Returns if this TaggedInput has all of its data valid. */
	bool IsValid() const;
};

/**
 * Container for defining several TaggedInputs for later input binding.
 */
UCLASS(NotBlueprintable, Const, MinimalAPI)
class UBigMoxiTaggedInputMap : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Set default values for this class. */
	UBigMoxiTaggedInputMap();

	//~ Begin UObject interface.
#if WITH_EDITOR
	EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
	//~ End UObject interface.

	/**
	 * Return the InputAction that is associated with the given GameplayTag on a TaggedInput definition.
	 * @param InputTag The GameplayTag that is associated with the return InputAction.
	 */
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

	/** Returns all TaggedInputs of this TaggedInputMapping. */
	const TArray<FBigMoxiTaggedInput>& GetAllTaggedInputs() const
	{
		return TaggedInputs;
	}

	/** Returns all InputActions of this InputConfig. */
	TArray<const UInputAction*> GetAllInputActions() const;

	/**
	 * Return the InputAction that is associated with the given GameplayTag on a AbilityTaggedInput definition.
	 * @param AbilityTag The GameplayTag that is associated with the return InputAction.
	 */
	const UInputAction* FindInputActionForAbilityTaggedInput(const FGameplayTag& AbilityTag) const;

	/** Returns all AbilityTaggedInput of this InputConfig. */
	const TArray<FBigMoxiTaggedInput>& GetAllAbilityTaggedInputs() const
	{
		return AbilityTaggedInputs;
	}

private:
	/** List of InputActions to map to tags. */
	UPROPERTY(EditDefaultsOnly, Category = "Input", Meta = (TitleProperty = "InputAction"))
	TArray<FBigMoxiTaggedInput> TaggedInputs;

	/** List of InputActions to map to GameplayAbility Input tags. */
	UPROPERTY(EditDefaultsOnly, Category = "Ability", Meta = (TitleProperty = "InputAction"))
	TArray<FBigMoxiTaggedInput> AbilityTaggedInputs;
};
