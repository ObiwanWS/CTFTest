// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <AbilitySystemInterface.h>
#include <GameFramework/PlayerState.h>
#include <GameplayEffectTypes.h>

#include "BigMoxiPlayerState.generated.h"

class UBigMoxiAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamChanged, uint8, NewTeamId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeathChanged, float, NewValue, float, OldValue);

/**
 * Extended APlayerState class.
 */
UCLASS()
class ABigMoxiPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	/** Set default values for this class. */
	ABigMoxiPlayerState();

	//~ Begin UObject interface.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	//~ End UObject interface.

	//~ Begin IAbilitySystemInterface implementation.
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface implementation.

	/** Assign a new team to this player. */
	UFUNCTION(Server, Reliable)
	void SetTeamId(uint8 NewTeamId);

	/** Returns the current team of this player. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE uint8 GetTeamId() const
	{
		return TeamId;
	}

	/** Will be called when this player changes teams. */
	UPROPERTY(BlueprintAssignable)
	FOnTeamChanged OnTeamChangedDelegate;

	/** Will be called when this player's health changes. */
	UPROPERTY(BlueprintAssignable)
	FOnHeathChanged OnHealthChangedDelegate;

	/**  */
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	/**  */
	UFUNCTION(BlueprintCallable)
	float GetCurrentMaxHealth();

protected:
	/** The widget that will be added to the screen once this player starts respawning */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	TSubclassOf<UUserWidget> RespawnWidgetClass;

	//~ Begin AActor interface.
	void BeginPlay() override;
	//~ End AActor interface.

	/** Called once the Health attribute changes. */
	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);

	/** Call to start the respawn flow. */
	void SetupForRespawn();

	/** Request the game mode to respawn this player. */
	void RequestRespawn();

	/** Handle respawn logic on the owning client. */
	UFUNCTION(Client, Reliable)
	void HandleRespawn_Client();

private:
	/** The ability system component of this player. */
	TObjectPtr<UBigMoxiAbilitySystemComponent> AbilitySystem;

	/**
	 * Identifier of which team this player is part of.
	 * 0 means no team has been assigned to this player.
	 */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TeamId)
	uint8 TeamId;

	/** Called on the clients when this player's team changes. */
	UFUNCTION()
	void OnRep_TeamId();
};
