// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiAbilitySet.h"
#include "BigMoxiGameDebug.h"

#include <AbilitySystemComponent.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiAbilitySet)

UBigMoxiAbilitySet::UBigMoxiAbilitySet()
{
	GameplayAbilities.Empty();
	GameplayEffects.Empty();
	Attributes.Empty();
}

void UBigMoxiAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* abilityComponent)
{
	// Must be authoritative to give or take ability sets.
	if (abilityComponent && abilityComponent->IsOwnerActorAuthoritative())
	{
		// Grant the attribute sets
		for (int32 i = 0; i < Attributes.Num(); ++i)
		{
			if (!IsValid(Attributes[i]))
			{
				UE_LOG(LogBigMoxiGame, Error, TEXT("Attributes[%d] on ability set [%s] is not valid"), i, *GetNameSafe(this));
				continue;
			}

			UAttributeSet* newSet = NewObject<UAttributeSet>(abilityComponent->GetOwner(), Attributes[i]);
			abilityComponent->AddAttributeSetSubobject(newSet);
		}

		// Grant the gameplay effects.
		for (int32 i = 0; i < GameplayEffects.Num(); i++)
		{
			if (!IsValid(GameplayEffects[i].GameplayEffect))
			{
				UE_LOG(LogBigMoxiGame, Error, TEXT("GameplayEffects[%d] on ability set [%s] is not valid"), i, *GetNameSafe(this));
				continue;
			}

			const UGameplayEffect* gameplayEffect = GameplayEffects[i].GameplayEffect->GetDefaultObject<UGameplayEffect>();
			const FActiveGameplayEffectHandle gameplayEffectHandle = abilityComponent->ApplyGameplayEffectToSelf(gameplayEffect, GameplayEffects[i].EffectLevel, abilityComponent->MakeEffectContext());
		}

		// Grant the gameplay abilities.
		for (int32 i = 0; i < GameplayAbilities.Num(); i++)
		{
			if (!IsValid(GameplayAbilities[i].Ability))
			{
				UE_LOG(LogBigMoxiGame, Error, TEXT("GameplayAbilities[%d] on ability set [%s] is not valid."), i, *GetNameSafe(this));
				continue;
			}

			UGameplayAbility* abilityCDO = GameplayAbilities[i].Ability->GetDefaultObject<UGameplayAbility>();
			const FGameplayAbilitySpec abilitySpec = FGameplayAbilitySpec(abilityCDO, GameplayAbilities[i].AbilityLevel);
			abilityComponent->GiveAbility(abilitySpec);
		}
	}
}

void UBigMoxiAbilitySet::RemoveFromAbilitySystem(UAbilitySystemComponent* abilityComponent)
{
	// Must be authoritative to give or take ability sets.
	if (abilityComponent && abilityComponent->IsOwnerActorAuthoritative())
	{
		// Remove the attribute sets
		for (int32 i = 0; i < Attributes.Num(); ++i)
		{
			if (!IsValid(Attributes[i]))
			{
				UE_LOG(LogBigMoxiGame, Error, TEXT("Attributes[%d] on ability set [%s] is not valid"), i, *GetNameSafe(this));
				continue;
			}

			abilityComponent->RemoveSpawnedAttribute(const_cast<UAttributeSet*>(abilityComponent->GetAttributeSet(Attributes[i])));
		}

		// Remove the gameplay effects.
		for (int32 i = 0; i < GameplayEffects.Num(); i++)
		{
			if (!IsValid(GameplayEffects[i].GameplayEffect))
			{
				UE_LOG(LogBigMoxiGame, Error, TEXT("GameplayEffects[%d] on ability set [%s] is not valid"), i, *GetNameSafe(this));
				continue;
			}

			abilityComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffects[i].GameplayEffect, abilityComponent);
		}

		// Remove the gameplay abilities.
		for (int32 i = 0; i < GameplayAbilities.Num(); i++)
		{
			if (!IsValid(GameplayAbilities[i].Ability))
			{
				UE_LOG(LogBigMoxiGame, Error, TEXT("GivenAbilities[%d] on ability set [%s] is not valid."), i, *GetNameSafe(this));
				continue;
			}

			if (FGameplayAbilitySpec* spec = abilityComponent->FindAbilitySpecFromClass(GameplayAbilities[i].Ability))
			{
				abilityComponent->MarkAbilitySpecDirty(*spec);
				abilityComponent->ClearAbility(spec->Handle);
			}
		}
	}
}
