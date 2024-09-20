// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiInputComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiInputComponent)

bool UBigMoxiInputComponent::UnbindAction(const UInputAction* InputAction)
{
	bool bRemoved = false;

	// Copy bindings to remove to a new array since removing binding causes the binding array to shrink.
	TArray<uint32> tempBindings;
	for (int32 i = 0; i < EnhancedActionEventBindings.Num(); i++)
	{
		if (EnhancedActionEventBindings[i]->GetAction() == InputAction)
		{
			tempBindings.Add(EnhancedActionEventBindings[i]->GetHandle());
		}
	}

	// Actually remove the bindings.
	for (int32 i = 0; i < tempBindings.Num(); i++)
	{
		bRemoved = RemoveBindingByHandle(tempBindings[i]);
	}

	return bRemoved;
}

bool UBigMoxiInputComponent::UnbindActionByTag(const UBigMoxiTaggedInputMap* InputConfig, const FGameplayTag& InputTag)
{
	if (InputConfig && InputTag.IsValid())
	{
		if (const UInputAction* inputAction = InputConfig->FindInputActionForTag(InputTag))
		{
			return UnbindAction(inputAction);
		}

		if (const UInputAction* inputAction = InputConfig->FindInputActionForAbilityTaggedInput(InputTag))
		{
			return UnbindAction(inputAction);
		}
	}
	return false;
}

void UBigMoxiInputComponent::UnbindAllActions(const UBigMoxiTaggedInputMap* InputConfig)
{
	if (InputConfig)
	{
		for (const UInputAction* inputAction : InputConfig->GetAllInputActions())
		{
			UnbindAction(inputAction);
		}
	}
}
