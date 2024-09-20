// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "Input/BigMoxiTaggedInputFunction.h"

#include <Components/ActorComponent.h>
#include <GameplayTagContainer.h>
#include <InputAction.h>

#include "BigMoxiPawnComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UBigMoxiAbilitySet;
class UBigMoxiAbilitySystemComponent;
class UBigMoxiInputComponent;
class UBigMoxiPlatformInputMap;
class UBigMoxiTaggedInputMap;

/**
 * A component to be added to a APawn that eases the setup of gameplay related tasks.
 */
UCLASS(MinimalAPI, Meta = (BlueprintSpawnableComponent))
class UBigMoxiPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties. */
	UBigMoxiPawnComponent();

	//~ Begin UActorComponent interface.
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End UActorComponent interface.

	//~ Begin UObject interface.
#if WITH_EDITOR
	EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
	//~ End UObject interface.

protected:
	/** Input Mapping to be added to the owner pawn. */
	UPROPERTY(EditDefaultsOnly, Category = "BigMoxi|Input")
	TObjectPtr<const UBigMoxiPlatformInputMap> PlatformInputMap;

	/** Input Config to be added to the owner pawn. */
	UPROPERTY(EditDefaultsOnly, Category = "BigMoxi|Input")
	TObjectPtr<const UBigMoxiTaggedInputMap> TaggedInputMap;

	/** Maps InputTags to UFUNCTIONs. */
	UPROPERTY(EditDefaultsOnly, Category = "BigMoxi|Input", Meta = (TitleProperty = "InputTag", NoElementDuplicate))
	TArray<FTaggedInputFunction> TaggedFunctionMap;

	/** AbilitySet to grant to pawn. */
	UPROPERTY(EditDefaultsOnly, Category = "BigMoxi|Abilities")
	TObjectPtr<UBigMoxiAbilitySet> AbilitySet;

	//~ Begin of UActorComponent interface.
	void OnRegister() override;
	//~ End of UActorComponent interface.

	/**
	 * Binded to ReceiveControllerChangedDelegate on the owning Pawn and it is called when the owning Pawn's controller changes.
	 * @param pawn The Pawn whose Controller changed, this should be the same as the owning Pawn.
	 * @param oldController The Controller previously possessing the owning Pawn.
	 * @param oldController The new Controller possessing the owning Pawn.
	 */
	UFUNCTION()
	void OnOwnerControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

	/**
	 * Updates Player Input by registering inputs to a new controller and unregistering from a old controller.
	 * @param oldPlayerController The Player Controller that will have its inputs unregistered.
	 * @param newPlayerController The Player Controller that will have its inputs registered.
	 */
	void UpdatePlayerInput(APlayerController* OldPlayerController, APlayerController* NewPlayerController);

	/**
	 * Updates Player Input by registering inputs to a new controller and unregistering from a old controller.
	 * @param oldLocalPlayerSubsystem The Player Controller that will have its inputs unregistered.
	 * @param newLocalPlayerSubsystem The Player Controller that will have its inputs registered.
	 */
	void RegisterMappingContexts(UEnhancedInputLocalPlayerSubsystem* OldLocalPlayerSubsystem, UEnhancedInputLocalPlayerSubsystem* NewLocalPlayerSubsystem) const;

	/**
	 * Update InputAction bindings for the controllers.
	 * @param oldPlayerController The Player Controller that will have its InputActions unbinded.
	 * @param newPlayerController The Player Controller that will have its InputActions binded.
	 */
	void UpdateInputActions(APlayerController* OldPlayerController, APlayerController* NewPlayerController);

	/**
	 * Update Register Ability related data to the ASC.
	 * @param PlayerController The PlayerController that will have its Abilities binded.
	 */
	UFUNCTION()
	void RegisterAbilitySystem();

	/**
	 * Update Abilities bindings for the controller.
	 * @param AbilitySystem The AbilitySystemComponent to Activate abilities from.
	 * @param PlayerController The PlayerController that will have its Abilities binded.
	 */
	void UpdateAbilityActions(UBigMoxiAbilitySystemComponent* AbilitySystem, APlayerController* PlayerController);

private:
	/** The Pawn that owns this component. */
	TObjectPtr<APawn> OwningPawn;

	/** The Controller of the owning Pawn. */
	TWeakObjectPtr<AController> OwningController;

	/** Returns valid functions to bind to inputs. */
	UFUNCTION()
	const TArray<FName> GetFunctionOptions();

	/** Sample functions to compare other functions to check if they match these signatures. */
	UFUNCTION()	void SampleUFunction1(const FInputActionValue& InputValue) {}
	UFUNCTION()	void SampleUFunction2(const FInputActionInstance& InputInstance) {}
	UFUNCTION()	void SampleUFunction3(FInputActionValue& ActionValue, float ElapsedTime, float TriggeredTime, const UInputAction* SourceAction) {}
};
