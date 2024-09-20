// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "Game/BigMoxiGameState.h"

#include "BigMoxiGameState_CTF.generated.h"

class ABigMoxiFlag;

UCLASS()
class ABigMoxiGameState_CTF : public ABigMoxiGameState
{
	GENERATED_BODY()
	
public:
	/**  */
	ABigMoxiGameState_CTF();

	/**  */
	ABigMoxiFlag* GetFlagForTeamId(uint8 TeamId, bool bAccepFlagWithoutTeam = false) const;

	/**  */
	ABigMoxiFlag* GetFlagForPlayerState(ABigMoxiPlayerState* PlayerState, bool bAccepFlagWithoutTeam = false) const;

	/**  */
	bool AddFlagForTeam(ABigMoxiFlag* Flag, uint8 TeamId);

private:
	//~ Flag as key instead of team to support multiple flags per team.
	TMap<ABigMoxiFlag*, uint8> FlagMap;
};
