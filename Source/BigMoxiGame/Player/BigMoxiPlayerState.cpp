// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiPlayerState.h"
#include "Abilities/BigMoxiAbilitySystemComponent.h"
#include "Abilities/BigMoxiAttributeSet.h"
#include "Character/BigMoxiCharacter.h"
#include "Game/BigMoxiGameMode.h"
#include "Game/BigMoxiGameState.h"

#include <Blueprint/UserWidget.h>
#include <Net/Core/PushModel/PushModel.h>
#include <Net/UnrealNetwork.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiPlayerState)

ABigMoxiPlayerState::ABigMoxiPlayerState()
{
	// Don't replicate ping by default to save a little bandwidth.
	SetShouldUpdateReplicatedPing(false);
	//NetUpdateFrequency = 100.0f; // This is not a very fast game, no need to increase this value.

	// Create ASC.
	AbilitySystem = CreateDefaultSubobject<UBigMoxiAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Initialize values.
	TeamId = 0U;
	RespawnWidgetClass = nullptr;
}

UAbilitySystemComponent* ABigMoxiPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ABigMoxiPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams sharedParams;
	sharedParams.bIsPushBased = true;
	sharedParams.Condition = COND_None;
	sharedParams.RepNotifyCondition = REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, TeamId, sharedParams);
}

float ABigMoxiPlayerState::GetCurrentHealth()
{
	bool bFound;
	return AbilitySystem->GetGameplayAttributeValue(UBigMoxiAttributeSet::GetHealthAttribute(), bFound);
}

float ABigMoxiPlayerState::GetCurrentMaxHealth()
{
	bool bFound;
	return AbilitySystem->GetGameplayAttributeValue(UBigMoxiAttributeSet::GetMaxHealthAttribute(), bFound);
}

void ABigMoxiPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Bind on health changed for the server.
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UBigMoxiAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ABigMoxiPlayerState::SetTeamId_Implementation(uint8 NewTeamId)
{
	if (HasAuthority())
	{
		if (NewTeamId != TeamId)
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, TeamId, this);
			TeamId = NewTeamId;
			OnTeamChangedDelegate.Broadcast(TeamId);
		}
	}
}

void ABigMoxiPlayerState::OnRep_TeamId()
{
	OnTeamChangedDelegate.Broadcast(TeamId);
}

void ABigMoxiPlayerState::OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	OnHealthChangedDelegate.Broadcast(AttributeChangeData.NewValue, AttributeChangeData.OldValue);

	if (AttributeChangeData.NewValue <= 0.0f)
	{
		if (ABigMoxiCharacter* character = GetPawn<ABigMoxiCharacter>())
		{
			if (HasAuthority())
			{
				SetupForRespawn();
				character->Death();
			}
		}
	}
}

void ABigMoxiPlayerState::SetupForRespawn()
{
	HandleRespawn_Client();

	const float respawnTime = GetWorld()->GetGameState()->GetPlayerRespawnDelay(GetOwningController());

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::RequestRespawn), respawnTime, false);
}

void ABigMoxiPlayerState::RequestRespawn()
{
	if (ABigMoxiGameMode* gameMode = Cast<ABigMoxiGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gameMode->RestartPlayer(GetOwningController());
	}
}

void ABigMoxiPlayerState::HandleRespawn_Client_Implementation()
{
	if (GetOwningController()->IsLocalPlayerController() && RespawnWidgetClass)
	{
		UUserWidget* widget = CreateWidget<UUserWidget>(Cast<APlayerController>(GetOwningController()), RespawnWidgetClass);
		widget->AddToViewport();
	}
}
