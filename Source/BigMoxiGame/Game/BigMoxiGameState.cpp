// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiGameState.h"
#include "BigMoxiGameMode.h"
#include "Player/BigMoxiPlayerState.h"

#include <Net/Core/PushModel/PushModel.h>
#include <Net/UnrealNetwork.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiGameState)

ABigMoxiGameState::ABigMoxiGameState()
{
	Teams.Empty();
	TeamScores.Empty();
	RespawnTime = 5.0f;
	WinningScore = 3.0f;
}

void ABigMoxiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams sharedParams;
	sharedParams.bIsPushBased = true;
	sharedParams.Condition = COND_None;
	sharedParams.RepNotifyCondition = REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, TeamScores, sharedParams);
}

void ABigMoxiGameState::BeginPlay()
{
	Super::BeginPlay();

	// Create a score for each team.
	TeamScores.Init(0.0f, Teams.Num());
}

void ABigMoxiGameState::AddPlayerState(APlayerState* PlayerState)
{
	const bool bAddedBefore = PlayerArray.Contains(PlayerState);

	Super::AddPlayerState(PlayerState);

	const bool bAdded = !bAddedBefore && PlayerArray.Contains(PlayerState);

	if (bAdded)
	{
		OnPlayerStateAddedDelegate.Broadcast(PlayerState);
	}
}

void ABigMoxiGameState::RemovePlayerState(APlayerState* PlayerState)
{
	if (!PlayerState)
	{
		return;
	}

	const bool bWasPresent = PlayerArray.Contains(PlayerState);

	Super::RemovePlayerState(PlayerState);

	const bool bRemoved = bWasPresent && !PlayerArray.Contains(PlayerState);

	if (bRemoved)
	{
		OnPlayerStateRemovedDelegate.Broadcast(PlayerState);
	}
}

float ABigMoxiGameState::GetPlayerRespawnDelay(AController* Controller) const
{
	return RespawnTime;
}

bool ABigMoxiGameState::GetTeamColorForPlayerState(ABigMoxiPlayerState* PlayerState, FLinearColor& Color) const
{
	if (PlayerState)
	{
		return GetTeamColorForTeamId(PlayerState->GetTeamId(), Color);
	}
	return false;
}

bool ABigMoxiGameState::GetTeamColorForTeamId(uint8 TeamId, FLinearColor& Color) const
{
	FTeam team;
	if (FindTeamForId(TeamId, team))
	{
		Color = team.TeamColor;
		return true;
	}
	else
	{
		return false;
	}
}

void ABigMoxiGameState::AddScoreToTeam(uint8 TeamId, float Score)
{
	FTeam team;
	if (HasAuthority() && FindTeamForId(TeamId, team))
	{
		int32 scoreIndex = FindScoreIndexForTeamId(TeamId);

		if (TeamScores.IsValidIndex(FindScoreIndexForTeamId(TeamId)))
		{
			MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, TeamScores, this);
			TeamScores[scoreIndex] += Score;
			OnRepTeamScores();
		}
	}
}

void ABigMoxiGameState::AddScoreToPlayerTeam(ABigMoxiPlayerState* PlayerState, float Score)
{
	if (HasAuthority() && PlayerState)
	{
		AddScoreToTeam(PlayerState->GetTeamId(), Score);
	}
}

void ABigMoxiGameState::EndGame_Implementation()
{
	if (ABigMoxiGameMode* gameMode = Cast<ABigMoxiGameMode>(AuthorityGameMode))
	{
		gameMode->RestartGame(this);
	}
}

void ABigMoxiGameState::SetNewTeamIdForPlayer(ABigMoxiPlayerState* PlayerState)
{
	int32 lessPlayers = 999;
	FTeam* teamWithLessPlayers = nullptr;
	for (int32 i = 0; i < Teams.Num(); i++)
	{
		if (Teams[i].PlayerNum < lessPlayers)
		{
			lessPlayers = Teams[i].PlayerNum;
			teamWithLessPlayers = &Teams[i];
		}
	}

	teamWithLessPlayers->PlayerNum += 1;
	PlayerState->SetTeamId(teamWithLessPlayers->TeamId);
}

void ABigMoxiGameState::OnRepTeamScores()
{
	OnTeamScoresChangedDelegate.Broadcast();

	for (int32 i = 0; i < TeamScores.Num(); i++)
	{
		if (FMath::IsNearlyEqual(TeamScores[i], WinningScore))
		{
			EndGame();
		}
	}
}

bool ABigMoxiGameState::FindTeamForId(uint8 TeamId, FTeam& Team) const
{
	for (int32 i = 0; i < Teams.Num(); i++)
	{
		if (Teams[i].TeamId == TeamId)
		{
			Team = Teams[i];
			return true;
		}
	}

	return false;
}

int32 ABigMoxiGameState::FindScoreIndexForTeamId(uint8 TeamId) const
{
	for (int32 i = 0; i < Teams.Num(); i++)
	{
		if (Teams[i].TeamId == TeamId)
		{
			return i;
		}
	}
	return -1;
}
