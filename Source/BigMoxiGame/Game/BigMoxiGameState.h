// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <GameFramework/GameStateBase.h>

#include "BigMoxiGameState.generated.h"

class ABigMoxiPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedDelegate, APlayerState*, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeamScoresChanged);

USTRUCT()
struct FTeam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, NotReplicated)
	uint8 TeamId;

	UPROPERTY(EditDefaultsOnly, NotReplicated)
	FLinearColor TeamColor;

	UPROPERTY(NotReplicated)
	int32 PlayerNum;

	FTeam()
	{
		TeamId = 0U;
		TeamColor = FLinearColor::Black;
		PlayerNum = 0;
	}
};

UCLASS()
class ABigMoxiGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	/**  */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangedDelegate OnPlayerStateAddedDelegate;

	/**  */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangedDelegate OnPlayerStateRemovedDelegate;

	/**  */
	UPROPERTY(BlueprintAssignable)
	FOnTeamScoresChanged OnTeamScoresChangedDelegate;

	/**  */
	ABigMoxiGameState();

	//~ Begin UObject interface.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	//~ End UObject interface.

	//~ Begin AGameStateBase interface.
	void AddPlayerState(APlayerState* PlayerState) override;
	void RemovePlayerState(APlayerState* PlayerState) override;
	float GetPlayerRespawnDelay(AController* Controller) const override;
	//~ End AGameStateBase interface.

	/**  */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	float GetServerWorldTimeSecondsDelta() const
	{
		return ServerWorldTimeSecondsDelta;
	}

	/**  */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	bool GetTeamColorForPlayerState(ABigMoxiPlayerState* PlayerState, FLinearColor& Color) const;

	/**  */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	bool GetTeamColorForTeamId(uint8 TeamId, FLinearColor& Color) const;

	/**  */
	UFUNCTION(BlueprintCallable, Category = BigMoxi)
	void AddScoreToTeam(uint8 TeamId, float Score);

	/**  */
	UFUNCTION(BlueprintCallable, Category = BigMoxi)
	void AddScoreToPlayerTeam(ABigMoxiPlayerState* PlayerState, float Score);

	/** Returns the current score for a team, -1 if the team wasn't found. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	float GetTeamScore(uint8 TeamId) const
	{
		int32 scoreIndex = FindScoreIndexForTeamId(TeamId);

		if (TeamScores.IsValidIndex(FindScoreIndexForTeamId(TeamId)))
		{
			return TeamScores[scoreIndex];
		}
		
		return -1.0f;
	}

	/** 
	 * Returns the id of the first team that isn't the given id. 
	 * Useful for situation where there are only two teams and you need to know which one is not your team.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	uint8 FindEnemyTeamId(uint8 MyTeamId) const
	{
		for (int32 i = 0; i < Teams.Num(); i++)
		{
			if (Teams[i].TeamId != MyTeamId)
			{
				return Teams[i].TeamId;
			}
		}

		return 0U;
	}

	/**  */
	UFUNCTION(BlueprintCallable)
	virtual void SetNewTeamIdForPlayer(ABigMoxiPlayerState* PlayerState);

protected:
	/** The default respawn time players */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	float RespawnTime;

	/**  */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	TArray<FTeam> Teams;

	/**  */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	float WinningScore;

	//~ Should be a TFastArraySerializer, but for this test, this gets the job done.
	UPROPERTY(ReplicatedUsing = "OnRepTeamScores")
	TArray<float> TeamScores;

	//~ Begin AActor interface.
	void BeginPlay() override;
	//~ End AActor interface.

	/**  */
	UFUNCTION(BlueprintNativeEvent)
	void EndGame();
	virtual void EndGame_Implementation();

private:
	/**  */
	UFUNCTION()
	void OnRepTeamScores();

	/**  */
	bool FindTeamForId(uint8 TeamId, FTeam& Team) const;

	/**  */
	int32 FindScoreIndexForTeamId(uint8 TeamId) const;
};
