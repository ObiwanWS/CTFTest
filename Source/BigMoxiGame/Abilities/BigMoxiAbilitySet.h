// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "BigMoxiAbilitySet.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
struct FGameplayAbilitySpecHandle;

USTRUCT()
struct FAbilitySet_Ability
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Ability.Input"))
	FGameplayTag InputTag;

	FAbilitySet_Ability()
	{
		Ability = nullptr;
		AbilityLevel = 1;
		InputTag = FGameplayTag();
	}
};

USTRUCT()
struct FAbilitySet_Effect
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel;

	FAbilitySet_Effect()
	{
		GameplayEffect = nullptr;
		EffectLevel = 1.0f;
	}
};

UCLASS()
class UBigMoxiAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/**  */
	UBigMoxiAbilitySet();

	/**  */
	void GiveToAbilitySystem(UAbilitySystemComponent* abilityComponent);

	/**  */
	void RemoveFromAbilitySystem(UAbilitySystemComponent* abilityComponent);

protected:
	/** Gameplay abilities to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta = (TitleProperty = Ability))
	TArray<FAbilitySet_Ability> GameplayAbilities;

	/** Gameplay effects to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Effects", meta = (TitleProperty = GameplayEffect))
	TArray<FAbilitySet_Effect> GameplayEffects;

	/** Attribute sets to grant when this ability set is granted. */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes", meta = (TitleProperty = AttributeSet))
	TArray<TSubclassOf<UAttributeSet>> Attributes;
};
