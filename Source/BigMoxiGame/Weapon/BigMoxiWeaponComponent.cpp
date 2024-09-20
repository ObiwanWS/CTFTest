// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiWeaponComponent.h"
#include "Abilities/BigMoxiAbilitySystemComponent.h"
#include "Abilities/BigMoxiAbilitySet.h"
#include "Character/BigMoxiCharacter.h"
#include "BigMoxiProjectile.h"
#include "Input/BigMoxiInputComponent.h"
#include "Input/BigMoxiPlatformInputMap.h"

#include <GameFramework/PlayerController.h>
#include <EnhancedInputSubsystems.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiWeaponComponent)

UBigMoxiWeaponComponent::UBigMoxiWeaponComponent()
{
	PlatformInputMap = nullptr;
	TaggedInputMap = nullptr;
	AbilitySet = nullptr;
}

bool UBigMoxiWeaponComponent::AttachWeapon(ABigMoxiCharacter* TargetCharacter)
{
	// Check that the character is valid, and has no weapon component yet
	if (TargetCharacter == nullptr || TargetCharacter->GetInstanceComponents().FindItemByClass<UBigMoxiWeaponComponent>())
	{
		return false;
	}

	Character = TargetCharacter;

	// Attach to character.
	FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
	GetOwner()->AttachToComponent(Character->GetMesh1P(), attachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings. This will only happen
	CharacterController = Cast<APlayerController>(Character->GetController());
	if (CharacterController)
	{
		RegisterInputs(CharacterController);
		RegisterAbilitySystem(CharacterController);
	}

	return true;
}

bool UBigMoxiWeaponComponent::DetachWeapon()
{
	if (Character)
	{
		FDetachmentTransformRules attachmentRules(EDetachmentRule::KeepWorld, true);
		GetOwner()->DetachFromActor(attachmentRules);

		Character->RemoveInstanceComponent(this);
		UnregisterInputsAndAbilitySet();
		return true;
	}
	
	return false;
}

void UBigMoxiWeaponComponent::RegisterInputs(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}

	if (PlayerController->IsLocalPlayerController())
	{
		if (PlatformInputMap)
		{
			if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				for (const FBigMoxiPlatformInputContext& mappingContext : PlatformInputMap->GetSupportedInputMappings())
				{
					inputSubsystem->AddMappingContext(mappingContext.InputMappingContext, mappingContext.Priority);
				}
			}
		}

		if (TaggedInputMap)
		{
			if (UBigMoxiInputComponent* inputComponent = Cast<UBigMoxiInputComponent>(PlayerController->InputComponent))
			{
				for (int32 i = 0; i < TaggedFunctionMap.Num(); i++)
				{
					inputComponent->BindActionByTag(TaggedInputMap, TaggedFunctionMap[i].InputTag, TaggedFunctionMap[i].InputTrigger, this, TaggedFunctionMap[i].FunctionName);
				}
			}
		}
	}
}

void UBigMoxiWeaponComponent::RegisterAbilitySystem(APlayerController* PlayerController)
{
	if (IAbilitySystemInterface* abilityInterface = Cast<IAbilitySystemInterface>(Character))
	{
		if (UBigMoxiAbilitySystemComponent* abilitySystem = Cast<UBigMoxiAbilitySystemComponent>(abilityInterface->GetAbilitySystemComponent()))
		{
			if (AbilitySet)
			{
				AbilitySet->GiveToAbilitySystem(abilitySystem);
				UpdateAbilityActions(abilitySystem, PlayerController);
			}
		}
	}
}

void UBigMoxiWeaponComponent::UpdateAbilityActions(UBigMoxiAbilitySystemComponent* AbilitySystem, APlayerController* PlayerController)
{
	if (PlayerController && PlayerController->IsLocalPlayerController())
	{
		UBigMoxiInputComponent* inputComponent = Cast<UBigMoxiInputComponent>(PlayerController->InputComponent);
		if (inputComponent && AbilitySystem)
		{
			inputComponent->BindAllAbilitisByTags(TaggedInputMap, AbilitySystem, &UBigMoxiAbilitySystemComponent::TriggerAbilityInputTag);
		}
	}
}

void UBigMoxiWeaponComponent::UnregisterInputsAndAbilitySet()
{
	if (!CharacterController)
	{
		return;
	}

	if (CharacterController->IsLocalPlayerController())
	{
		if (PlatformInputMap)
		{
			if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CharacterController->GetLocalPlayer()))
			{
				for (const FBigMoxiPlatformInputContext& mappingContext : PlatformInputMap->GetSupportedInputMappings())
				{
					inputSubsystem->RemoveMappingContext(mappingContext.InputMappingContext);
				}
			}
		}

		if (TaggedInputMap)
		{
			if (UBigMoxiInputComponent* inputComponent = Cast<UBigMoxiInputComponent>(CharacterController->InputComponent))
			{
				inputComponent->UnbindAllActions(TaggedInputMap);
			}
		}
	}

	if (AbilitySet && Character->IsValidLowLevelFast())
	{
		if (IAbilitySystemInterface* abilityInterface = Cast<IAbilitySystemInterface>(Character))
		{
			if (UBigMoxiAbilitySystemComponent* abilitySystem = Cast<UBigMoxiAbilitySystemComponent>(abilityInterface->GetAbilitySystemComponent()))
			{
				AbilitySet->RemoveFromAbilitySystem(abilitySystem);
			}
		}
	}
}

void UBigMoxiWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInputsAndAbilitySet();

	Super::EndPlay(EndPlayReason);
}

const TArray<FName> UBigMoxiWeaponComponent::GetFunctionOptions()
{
	TArray<FName> names;
	if (UBlueprintGeneratedClass* outer = Cast<UBlueprintGeneratedClass>(GetOuter()))
	{
		UFunction* sampleUFunction1 = this->GetClass()->FindFunctionByName(TEXT("SampleUFunction1"));
		UFunction* sampleUFunction2 = this->GetClass()->FindFunctionByName(TEXT("SampleUFunction2"));
		UFunction* sampleUFunction3 = this->GetClass()->FindFunctionByName(TEXT("SampleUFunction3"));

		// Find all functions in the owner that match one of the sample signatures.
		for (TFieldIterator<UFunction> iterator(outer->GetDefaultObject()->GetClass(), EFieldIteratorFlags::IncludeSuper); iterator; ++iterator) {

			UFunction* function = *iterator;
			if (function->IsSignatureCompatibleWith(sampleUFunction1) || function->IsSignatureCompatibleWith(sampleUFunction2) || function->IsSignatureCompatibleWith(sampleUFunction3))
			{
				names.Add(*function->GetName());
			}
		}

		// Remove functions that are already bound.
		if (names.Num() > 0 && TaggedFunctionMap.Num() > 0)
		{
			for (int32 i = 0; i < TaggedFunctionMap.Num(); i++)
			{
				if (names.Contains(TaggedFunctionMap[i].FunctionName))
				{
					names.RemoveSwap(TaggedFunctionMap[i].FunctionName);;
				}
			}
		}
	}
	return names;
}
