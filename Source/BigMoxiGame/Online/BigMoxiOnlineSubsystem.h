// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Interfaces/OnlineSessionDelegates.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <OnlineSessionSettings.h>
#include <Subsystems/GameInstanceSubsystem.h>

#include "BigMoxiOnlineSubsystem.generated.h"

DECLARE_DELEGATE_OneParam(FOnSessionDelegate, bool);
DECLARE_DELEGATE_TwoParams(FOnSessionSearchResultDelegate, bool, TArray<FOnlineSessionSearchResult>&);

USTRUCT(BlueprintType)
struct FSimpleSessionSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite)
	bool bLan;

	UPROPERTY(BlueprintReadWrite)
	FString LevelPath;

	FSimpleSessionSettings()
	{
		MaxPlayers = 0;
		bLan = true;
		LevelPath = TEXT("");
	}

	bool IsValid() const
	{
		if (MaxPlayers < 2)
		{
			return false;
		}

		return true;
	}
};

UCLASS()
class UBigMoxiOnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void CreateSession(const FSimpleSessionSettings SessionSettings);
	FOnSessionDelegate OnCreateSessionCompleted;

	void StartSession();
	FOnSessionDelegate OnStartSessionCompleted;

	void UpdateSession(const FSimpleSessionSettings SessionSettings);
	FOnSessionDelegate OnUpdateSessionCompleted;

	void FindSessions(int32 MaxSearchResults, bool bLan);
	FOnSessionSearchResultDelegate OnSessionsFound;

	void CancelFindSessions();
	FOnSessionDelegate OnCancelFindSessionCompleted;

	void JoinSession(const FOnlineSessionSearchResult& Session);
	FOnSessionDelegate OnJoinSessionCompleted;

	void DestroySession();
	FOnSessionDelegate OnDestroySessionCompleted;

	bool IsValid(const FOnlineSessionSearchResult& Session);

	FString GetMapName(const FOnlineSessionSearchResult& Session);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	int32 GetMaxConnections() const
	{
		return LastSessionSettings->NumPublicConnections + LastSessionSettings->NumPrivateConnections;
	}

protected:
	void OnSessionCreated(FName SessionName, bool bSuccessful);

	void OnSessionStarted(FName SessionName, bool bSuccessful);

	void OnSessionUpdated(FName SessionName, bool bSuccessful);

	void OnFindSessions(bool bSuccessful, TSharedRef<FOnlineSessionSearch> sessionSearch);

	void OnCancelFindSessions(bool bSuccessful);

	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnSessionDestroyed(FName SessionName, bool bSuccessful);

private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FDelegateHandle OnCreateSessionCompleteHandle;
	FDelegateHandle OnStartSessionCompleteHandle;
	FDelegateHandle OnUpdateSessionCompleteHandle;
	FDelegateHandle OnFindSessionsCompleteHandle;
	FDelegateHandle OnCancelFindSessionsCompleteHandle;
	FDelegateHandle OnJoinSessionsCompleteHandle;
	FDelegateHandle OnDestroySessionCompleteHandle;
};
