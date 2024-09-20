// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <GameFramework/GameModeBase.h>

#include "BigMoxiGameMode.generated.h"

UCLASS()
class ABigMoxiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/**  */
	ABigMoxiGameMode();

	//~ Begin AGameModeBase interface.
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	void RestartPlayer(AController* NewPlayer) override;
	//~ End AGameModeBase interface.

	/**  */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "WorldContextObject"))
	void RestartGame(const UObject* WorldContextObject);
};
