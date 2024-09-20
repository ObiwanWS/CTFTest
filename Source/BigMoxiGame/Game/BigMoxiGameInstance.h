// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Engine/GameInstance.h>

#include "BigMoxiGameInstance.generated.h"

UCLASS()
class UBigMoxiGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//~ Begin UGameInstance interface.
	void Init() override;
	//~ End UGameInstance interface.

protected:
	/**  */
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage);

	/**  */
	void OnTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorMessage);

	/**  */
	void HandleNetError(const FString& ErrorMessage);
};
