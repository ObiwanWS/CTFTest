// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiFlagBase.h"
#include "BigMoxiGameState_CTF.h"
#include "BigMoxiFlag.h"
#include "Character/BigMoxiCharacter.h"
#include "Player/BigMoxiPlayerState.h"

#include <AbilitySystemComponent.h>
#include <Components/SphereComponent.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiFlagBase)

ABigMoxiFlagBase::ABigMoxiFlagBase()
{
 	// Disable tick.
	PrimaryActorTick.bCanEverTick = false;
	bReleaseOnEndOverlap = true;

	// Init members.
	TeamId = 0U;
}

void ABigMoxiFlagBase::BeginPlay()
{
	Super::BeginPlay();

	if (ABigMoxiGameState* gameState = GetWorld()->GetGameState<ABigMoxiGameState>())
	{
		FLinearColor teamColor;
		if (gameState->GetTeamColorForTeamId(TeamId, teamColor))
		{
			SetTeamColor(teamColor);
		};
	}
}

void ABigMoxiFlagBase::Interact(ABigMoxiCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	OwnerCharacter = InteractedCharacter;
	OnInteract.Broadcast(OwnerCharacter);

	if (HasAuthority())
	{
		ApplyGameplayEffect(OwnerCharacter, InteractEffect);

		if (OwnerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(HasFlagTag))
		{
			if (ABigMoxiPlayerState* playerState = InteractedCharacter->GetPlayerState<ABigMoxiPlayerState>())
			{
				if (playerState->GetTeamId() == TeamId)
				{
					if (ABigMoxiGameState_CTF* gameState = GetWorld()->GetGameState<ABigMoxiGameState_CTF>())
					{
						if (ABigMoxiFlag* flag = gameState->GetFlagForTeamId(TeamId, true))
						{
							flag->Release_Server(true);

							// Add score to player to track how many points each player scored.
							playerState->SetScore(playerState->GetScore() + 1.0f);
							gameState->AddScoreToTeam(TeamId, 1.0f);
						}
					}
				}
			}
		}
	}
}
