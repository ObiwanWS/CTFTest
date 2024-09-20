// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiAbilitySystemComponent)

void UBigMoxiAbilitySystemComponent::TriggerAbilityInputTag(const FInputActionValue& InputValue, FGameplayTag AbilityTag)
{
	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& abilitySpec : ActivatableAbilities.Items)
	{
		FScopedServerAbilityRPCBatcher abilityRPCBatcher(this, abilitySpec.Handle);

		if (abilitySpec.Ability && (abilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag)))
		{
			const bool bWasPressed = abilitySpec.InputPressed;
			
			bool bInputVal = InputValue.Get<bool>();

			abilitySpec.InputPressed = bInputVal;

			bool bTryActivation = false;
			
			bTryActivation = !abilitySpec.IsActive() && bInputVal;

			if (bTryActivation)
			{
				TryActivateAbility(abilitySpec.Handle);
			}
			else if (bWasPressed != abilitySpec.InputPressed)
			{
				const bool bSendServerEvents = (abilitySpec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false);
				if (abilitySpec.InputPressed)
				{
					if (bSendServerEvents)
					{
						ServerSetInputPressed(abilitySpec.Handle);
					}

					AbilitySpecInputPressed(abilitySpec);
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, abilitySpec.Handle, abilitySpec.ActivationInfo.GetActivationPredictionKey());
				}
				else
				{
					if (bSendServerEvents)
					{
						ServerSetInputReleased(abilitySpec.Handle);
					}

					AbilitySpecInputReleased(abilitySpec);
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, abilitySpec.Handle, abilitySpec.ActivationInfo.GetActivationPredictionKey());
				}
			}
		}
	}
}
