// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "Input/BigMoxiTaggedInputFunction.h"

#include <Components/ActorComponent.h>

#include "BigMoxiWeaponComponent.generated.h"

class UBigMoxiAbilitySet;
class UBigMoxiAbilitySystemComponent;
class ABigMoxiCharacter;
class UBigMoxiPlatformInputMap;
class UBigMoxiTaggedInputMap;
class ABigMoxiProjectile;

UCLASS(Blueprintable, BlueprintType, Meta = (BlueprintSpawnableComponent))
class UBigMoxiWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties. */
	UBigMoxiWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeapon(ABigMoxiCharacter* TargetCharacter);

	/** Detach the actor from a character if attach. */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool DetachWeapon();

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
	
	//~ Begin UActor interface.
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End UActor interface.

	void RegisterInputs(APlayerController* PlayerController);
	void RegisterAbilitySystem(APlayerController* PlayerController);
	void UpdateAbilityActions(UBigMoxiAbilitySystemComponent* AbilitySystem, APlayerController* PlayerController);
	void UnregisterInputsAndAbilitySet();

private:
	/** The Character holding this weapon. */
	ABigMoxiCharacter* Character;
	APlayerController* CharacterController;

	/** Returns valid functions to bind to inputs. */
	UFUNCTION()
	const TArray<FName> GetFunctionOptions();

	/** Sample functions to compare other functions to check if they match these signatures. */
	UFUNCTION()	void SampleUFunction1(const FInputActionValue& InputValue) {}
	UFUNCTION()	void SampleUFunction2(const FInputActionInstance& InputInstance) {}
	UFUNCTION()	void SampleUFunction3(FInputActionValue& ActionValue, float ElapsedTime, float TriggeredTime, const UInputAction* SourceAction) {}
};
