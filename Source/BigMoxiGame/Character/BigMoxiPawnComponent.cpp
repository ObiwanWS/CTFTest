// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiPawnComponent.h"
#include "Abilities/BigMoxiAbilitySystemComponent.h"
#include "Abilities/BigMoxiAbilitySet.h"
#include "BigMoxiGameDebug.h"
#include "Input/BigMoxiInputComponent.h"
#include "Input/BigMoxiTaggedInputMap.h"
#include "Input/BigMoxiPlatformInputMap.h"

#include <AbilitySystemInterface.h>
#include <EnhancedInputSubsystems.h>
#if WITH_EDITOR
#include <Misc/DataValidation.h>
#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiPawnComponent)

UBigMoxiPawnComponent::UBigMoxiPawnComponent()
{
	// Disable tick for this components.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Default member values.
	PlatformInputMap = nullptr;
	TaggedInputMap = nullptr;
	TaggedFunctionMap.Empty();
	OwningPawn = nullptr;
	OwningController = nullptr;
}

#if WITH_EDITOR
EDataValidationResult UBigMoxiPawnComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult result = Super::IsDataValid(Context);

	UObject* outer = GetOuter();

	if (const UBlueprintGeneratedClass* outerBlueprint = Cast<UBlueprintGeneratedClass>(outer))
	{
		outer = outerBlueprint->GetDefaultObject();
	}

	if (!outer->IsA(APawn::StaticClass()))
	{
		Context.AddError(INVTEXT("Extended Pawn Component should only be added to a Pawn!"));
		result = CombineDataValidationResults(result, EDataValidationResult::Invalid);
	}

	return CombineDataValidationResults(result, EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UBigMoxiPawnComponent::OnRegister()
{
	Super::OnRegister();

	if (OwningPawn = Cast<APawn>(GetOuter()))
	{
		// Listen to when the pawn changes controller to update inputs.
		OwningPawn->ReceiveControllerChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnOwnerControllerChanged);

		// In the case of this component being added in runtime, at a point the pawn was already possessed.
		// We'll call the function imediately since the delegate already triggered.
		if (AController* controller = OwningPawn->GetController())
		{
			OnOwnerControllerChanged(OwningPawn, controller, nullptr);
		}

		if (IAbilitySystemInterface* abilityInterface = Cast<IAbilitySystemInterface>(OwningPawn))
		{
			RegisterAbilitySystem();
		}
	}
	else
	{
		UE_LOG(LogBigMoxiGame, Error, TEXT("%s: %s failed to register because owner (%s) is not a Pawn."), ANSI_TO_TCHAR(__FUNCTION__), *this->GetName(), *GetOuter()->GetName());
	}
}

void UBigMoxiPawnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBigMoxiPawnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed || EndPlayReason == EEndPlayReason::RemovedFromWorld)
	{
		if (OwningPawn)
		{
			if (IAbilitySystemInterface* abilityInterface = Cast<IAbilitySystemInterface>(OwningPawn))
			{
				if (UBigMoxiAbilitySystemComponent* abilitySystem = Cast<UBigMoxiAbilitySystemComponent>(abilityInterface->GetAbilitySystemComponent()))
				{
					AbilitySet->RemoveFromAbilitySystem(abilitySystem);
				}
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UBigMoxiPawnComponent::OnOwnerControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	bool bNeedsInputUpdate = false;
	if (OldController)
	{
		bNeedsInputUpdate = OldController->IsLocalPlayerController();
	}

	if (NewController)
	{
		OwningController = NewController;
		bNeedsInputUpdate = NewController->IsLocalPlayerController() || bNeedsInputUpdate;
	}
	else if (OwningController.IsValid())
	{
		OwningController.Reset();
	}

	if (bNeedsInputUpdate)
	{
		UpdatePlayerInput(Cast<APlayerController>(OldController), Cast<APlayerController>(NewController));
	}
}

void UBigMoxiPawnComponent::UpdatePlayerInput(APlayerController* OldPlayerController, APlayerController* NewPlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* oldLocalPlayerSubsystem = nullptr;
	UEnhancedInputLocalPlayerSubsystem* newLocalPlayerSubsystem = nullptr;

	if (OldPlayerController)
	{
		oldLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OldPlayerController->GetLocalPlayer());
	}

	if (NewPlayerController)
	{
		newLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayerController->GetLocalPlayer());
	}

	RegisterMappingContexts(oldLocalPlayerSubsystem, newLocalPlayerSubsystem);
	UpdateInputActions(OldPlayerController, NewPlayerController);
}

void UBigMoxiPawnComponent::RegisterMappingContexts(UEnhancedInputLocalPlayerSubsystem* OldLocalPlayerSubsystem, UEnhancedInputLocalPlayerSubsystem* NewLocalPlayerSubsystem) const
{
	if (PlatformInputMap)
	{
		for (const FBigMoxiPlatformInputContext& mappingContext : PlatformInputMap->GetSupportedInputMappings())
		{
			if (OldLocalPlayerSubsystem)
			{
				OldLocalPlayerSubsystem->RemoveMappingContext(mappingContext.InputMappingContext);
			}

			if (NewLocalPlayerSubsystem)
			{
				NewLocalPlayerSubsystem->AddMappingContext(mappingContext.InputMappingContext, mappingContext.Priority);
			}
		}
	}
}

void UBigMoxiPawnComponent::UpdateInputActions(APlayerController* OldPlayerController, APlayerController* NewPlayerController)
{
	if (OldPlayerController)
	{
		UBigMoxiInputComponent* inputComponent = Cast<UBigMoxiInputComponent>(OldPlayerController->InputComponent);
		if (TaggedInputMap && inputComponent)
		{
			inputComponent->UnbindAllActions(TaggedInputMap);
		}
	}

	if (NewPlayerController)
	{
		UBigMoxiInputComponent* inputComponent = Cast<UBigMoxiInputComponent>(NewPlayerController->InputComponent);
		if (OwningPawn && TaggedInputMap && inputComponent)
		{
			for (int32 i = 0; i < TaggedFunctionMap.Num(); i++)
			{
				inputComponent->BindActionByTag(TaggedInputMap, TaggedFunctionMap[i].InputTag, TaggedFunctionMap[i].InputTrigger, OwningPawn.Get(), TaggedFunctionMap[i].FunctionName);
			}
		}
	}
}

void UBigMoxiPawnComponent::RegisterAbilitySystem()
{
	bool bFoundAbilitySystem = false;
	if (IAbilitySystemInterface* abilityInterface = Cast<IAbilitySystemInterface>(OwningPawn))
	{
		if (UBigMoxiAbilitySystemComponent* abilitySystem = Cast<UBigMoxiAbilitySystemComponent>(abilityInterface->GetAbilitySystemComponent()))
		{
			bFoundAbilitySystem = true;

			if (AbilitySet)
			{
				AbilitySet->GiveToAbilitySystem(abilitySystem);
				UpdateAbilityActions(abilitySystem, Cast<APlayerController>(OwningPawn->GetController()));
			}
		}
	}

	if (!bFoundAbilitySystem)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::RegisterAbilitySystem));
	}
}

void UBigMoxiPawnComponent::UpdateAbilityActions(UBigMoxiAbilitySystemComponent* AbilitySystem, APlayerController* PlayerController)
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

const TArray<FName> UBigMoxiPawnComponent::GetFunctionOptions()
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
