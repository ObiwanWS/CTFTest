// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "BigMoxiTaggedInputMap.h"

#include <EnhancedInputComponent.h>

#include "BigMoxiInputComponent.generated.h"

/**
 * InputComponent that handles binding user inputs to functions that are mapped with GameplayTags.
 */
UCLASS(Transient)
class UBigMoxiInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	/** Bind the InputAction mapped to InputTag to the given function. */
	template<class UserClass, typename FuncType>
	void BindActionByTag(const UBigMoxiTaggedInputMap* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Function);

	/** Bind the InputAction mapped to InputTag to the given function. */
	template<class UserClass, typename FuncType>
	void BindAbilityByTag(const UBigMoxiTaggedInputMap* InputConfig, const FGameplayTag& AbilityTag, UserClass* object, FuncType Function);

	/** Binds all the InputActions mapped to an InputTag inside InputConfig to the given function. */
	template<class UserClass, typename FuncType>
	void BindAllAbilitisByTags(const UBigMoxiTaggedInputMap* InputConfig, UserClass* object, FuncType Function);

	/**
	 * Remove input bindings for the given InputAction.
	 * @return Returns if the unbinding was successful.
	 */
	bool UnbindAction(const UInputAction* InputAction);

	/**
	 *  Remove input bindings for the InputAction mapped to the given InputTag.
	 * @return Returns if the unbinding was successful.
	 */
	bool UnbindActionByTag(const UBigMoxiTaggedInputMap* InputConfig, const FGameplayTag& InputTag);

	/** Remove input bindings for InputAction inside of the given InputConfig. */
	void UnbindAllActions(const UBigMoxiTaggedInputMap* InputConfig);
};

template<class UserClass, typename FuncType>
void UBigMoxiInputComponent::BindActionByTag(const UBigMoxiTaggedInputMap* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Function)
{
	if (InputConfig && InputTag.IsValid())
	{
		if (const UInputAction* inputAction = InputConfig->FindInputActionForTag(InputTag))
		{
			BindAction(inputAction, TriggerEvent, Object, Function);
		}
	}
}

template<class UserClass, typename FuncType>
inline void UBigMoxiInputComponent::BindAbilityByTag(const UBigMoxiTaggedInputMap* InputConfig, const FGameplayTag& AbilityTag, UserClass* Object, FuncType Function)
{
	if (InputConfig && AbilityTag.IsValid())
	{
		if (const UInputAction* inputAction = InputConfig->FindInputActionForAbilityTaggedInput(AbilityTag))
		{
			// Bind on TriggeredEvent to activate the GameplayAbility on button press.
			BindAction(inputAction, ETriggerEvent::Triggered, Object, Function, AbilityTag);

			// Bind on CompletedEvent to end the GameplayAbility on button release.
			BindAction(inputAction, ETriggerEvent::Completed, Object, Function, AbilityTag);
		}
	}
}

template<class UserClass, typename FuncType>
inline void UBigMoxiInputComponent::BindAllAbilitisByTags(const UBigMoxiTaggedInputMap* InputConfig, UserClass* Object, FuncType Function)
{
	if (InputConfig)
	{
		for (const FBigMoxiTaggedInput& taggetInput : InputConfig->GetAllAbilityTaggedInputs())
		{
			if (taggetInput.IsValid())
			{
				// Bind on TriggeredEvent to activate the GameplayAbility on button press.
				BindAction(taggetInput.InputAction, ETriggerEvent::Triggered, Object, Function, taggetInput.InputTag);

				// Bind on CompletedEvent to end the GameplayAbility on button release.
				BindAction(taggetInput.InputAction, ETriggerEvent::Completed, Object, Function, taggetInput.InputTag);
			}
		}
	}
}
