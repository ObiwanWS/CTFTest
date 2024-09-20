// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiGameState_CTF.h"
#include "Player/BigMoxiPlayerState.h"
#include "BigMoxiFlag.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiGameState_CTF)

ABigMoxiGameState_CTF::ABigMoxiGameState_CTF()
{
	FlagMap.Empty();
}

ABigMoxiFlag* ABigMoxiGameState_CTF::GetFlagForTeamId(uint8 TeamId, bool bAccepFlagWithoutTeam) const
{
	ABigMoxiFlag* const* flag = FlagMap.FindKey(TeamId);
	if (flag)
	{
		return *flag;
	}
	else if (bAccepFlagWithoutTeam)
	{
		flag = FlagMap.FindKey(0U);
		if (flag)
		{
			return *flag;
		}
	}
	return nullptr;
}

ABigMoxiFlag* ABigMoxiGameState_CTF::GetFlagForPlayerState(ABigMoxiPlayerState* PlayerState, bool bAccepFlagWithoutTeam) const
{
	return GetFlagForTeamId(PlayerState->GetTeamId(), bAccepFlagWithoutTeam);
}

bool ABigMoxiGameState_CTF::AddFlagForTeam(ABigMoxiFlag* Flag, uint8 TeamId)
{
	if (!FlagMap.Contains(Flag))
	{
		FlagMap.Add(Flag, TeamId);
		return true;
	}

	return false;
}
