// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiGameMode.h"
#include "BigMoxiGameState.h"
#include "BigMoxiGameDebug.h"
#include "Player/BigMoxiPlayerState.h"

#include <GameFramework/GameState.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiGameMode)

ABigMoxiGameMode::ABigMoxiGameMode()
{
	bUseSeamlessTravel = true;
}

void ABigMoxiGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (ABigMoxiPlayerState* playerState = NewPlayer->GetPlayerState<ABigMoxiPlayerState>())
	{
		if (ABigMoxiGameState* gameState = GetGameState<ABigMoxiGameState>())
		{
			gameState->SetNewTeamIdForPlayer(playerState);
		}
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ABigMoxiGameMode::RestartPlayer(AController* NewPlayer)
{
	if (ABigMoxiPlayerState* playerState = NewPlayer->GetPlayerState<ABigMoxiPlayerState>())
	{
		FString startTag = TEXT("Team");
		startTag.AppendInt(playerState->GetTeamId());

		// Same as Super but with a name to find the correct player start.

		if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
		{
			return;
		}

		AActor* StartSpot = FindPlayerStart(NewPlayer, startTag);

		// If a start spot wasn't found,
		if (StartSpot == nullptr)
		{
			// Check for a previously assigned spot
			if (NewPlayer->StartSpot != nullptr)
			{
				StartSpot = NewPlayer->StartSpot.Get();
				UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
			}
		}

		RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
	}
	else
	{
		Super::RestartPlayer(NewPlayer);
	}
}

void ABigMoxiGameMode::RestartGame(const UObject* WorldContextObject)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (world == nullptr)
	{
		return;
	}

	const FWorldContext& context = GEngine->GetWorldContextFromWorldChecked(world);
	FURL lastURL = context.LastURL;

#if WITH_EDITOR
	lastURL.Map = UWorld::StripPIEPrefixFromPackageName(lastURL.Map, context.World()->StreamingLevelsPrefix);
#endif

	lastURL.AddOption(TEXT("SeamlessTravel"));

	FString URL = lastURL.ToString();
	URL.RemoveFromStart(lastURL.GetHostPortString());

	world->ServerTravel(URL, false, false);
}
