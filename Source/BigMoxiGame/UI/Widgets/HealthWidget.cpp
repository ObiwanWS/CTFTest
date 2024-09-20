// Copyright (c) 2024 Obiwan Medeiros.

#include "HealthWidget.h"
#include "Character/BigMoxiCharacter.h"
#include "Player/BigMoxiPlayerState.h"

#include <Components/ProgressBar.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(HealthWidget)

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bool bFoundPlayerState;
	BindPlayerState(bFoundPlayerState);

	if (!bFoundPlayerState)
	{
		if (ABigMoxiCharacter* character = GetOwningPlayerPawn<ABigMoxiCharacter>())
		{
			character->OnPlayerStateRepDelegate.AddUniqueDynamic(this, &ThisClass::OnRepPlayerState);
		}
	}
}

void UHealthWidget::OnHealthChanged(float NewValue, float OldValue)
{
	UpdateHealthBar(NewValue);
}

void UHealthWidget::UpdateHealthBar(float Health)
{
	if (PlayerState.IsValid())
	{
		float newPercent = Health / PlayerState->GetCurrentMaxHealth();
		HealthBar->SetPercent(newPercent);
	}
}

void UHealthWidget::BindPlayerState(bool& bOutSuccess)
{
	PlayerState = GetOwningPlayerState<ABigMoxiPlayerState>();
	if (PlayerState.IsValid())
	{
		PlayerState->OnHealthChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnHealthChanged);
		UpdateHealthBar(PlayerState->GetCurrentHealth());

		bOutSuccess = true;
	}
	
	bOutSuccess = false;
}

void UHealthWidget::OnRepPlayerState(APlayerState* RepPlayerState)
{
	bool bFoundPlayerState;
	BindPlayerState(bFoundPlayerState);
}
