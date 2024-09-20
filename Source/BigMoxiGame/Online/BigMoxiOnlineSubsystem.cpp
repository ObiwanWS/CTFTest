// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiOnlineSubsystem.h"

#include <OnlineSubsystemUtils.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiOnlineSubsystem)

void UBigMoxiOnlineSubsystem::CreateSession(const FSimpleSessionSettings SessionSettings)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid() || !SessionSettings.IsValid())
	{
		OnCreateSessionCompleted.ExecuteIfBound(false);
		return;
	}

	LastSessionSettings = MakeShareable<FOnlineSessionSettings>(new FOnlineSessionSettings());
	LastSessionSettings->NumPublicConnections = SessionSettings.MaxPlayers;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bIsLANMatch = SessionSettings.bLan;
	LastSessionSettings->bShouldAdvertise = true;

	LastSessionSettings->Set(FName(TEXT("SETTING_MAPNAME")), SessionSettings.LevelPath, EOnlineDataAdvertisementType::ViaOnlineService);

	OnCreateSessionCompleteHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionCreated));

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	const bool bSessionCreated = sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings);

	if (!bSessionCreated)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteHandle);

		OnCreateSessionCompleted.ExecuteIfBound(false);
	}
}

void UBigMoxiOnlineSubsystem::OnSessionCreated(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteHandle);
	}

	OnCreateSessionCompleted.ExecuteIfBound(bSuccessful);
}

void UBigMoxiOnlineSubsystem::UpdateSession(const FSimpleSessionSettings SessionSettings)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid() || !SessionSettings.IsValid())
	{
		OnUpdateSessionCompleted.ExecuteIfBound(false);
		return;
	}

	TSharedPtr<FOnlineSessionSettings> updatedSessionSettings = MakeShareable<FOnlineSessionSettings>(new FOnlineSessionSettings(*LastSessionSettings));
	updatedSessionSettings->NumPublicConnections = SessionSettings.MaxPlayers;
	updatedSessionSettings->bIsLANMatch = SessionSettings.bLan;
	updatedSessionSettings->Set(FName(TEXT("SETTING_MAPNAME")), SessionSettings.LevelPath, EOnlineDataAdvertisementType::ViaOnlineService);

	OnUpdateSessionCompleteHandle = sessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionUpdated));

	if (sessionInterface->UpdateSession(NAME_GameSession, *updatedSessionSettings))
	{
		LastSessionSettings = updatedSessionSettings;
	}
	else
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteHandle);
		OnUpdateSessionCompleted.ExecuteIfBound(false);
	}
}

void UBigMoxiOnlineSubsystem::OnSessionUpdated(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteHandle);
	}

	OnUpdateSessionCompleted.ExecuteIfBound(bSuccessful);
}

void UBigMoxiOnlineSubsystem::StartSession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnStartSessionCompleted.ExecuteIfBound(false);
		return;
	}

	OnStartSessionCompleteHandle = sessionInterface->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionStarted));

	if (!sessionInterface->StartSession(NAME_GameSession))
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteHandle);
		OnStartSessionCompleted.ExecuteIfBound(false);
	}
}

void UBigMoxiOnlineSubsystem::OnSessionStarted(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteHandle);
	}

	OnStartSessionCompleted.ExecuteIfBound(bSuccessful);

	FString levelName;
	LastSessionSettings.Get()->Get(FName(TEXT("SETTING_MAPNAME")), levelName);

	GetWorld()->ServerTravel(levelName + TEXT("?listen"), true);
}

void UBigMoxiOnlineSubsystem::FindSessions(int32 MaxSearchResults, bool bLan)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		TArray<FOnlineSessionSearchResult> results;
		OnSessionsFound.ExecuteIfBound(false, results);
		return;
	}

	TSharedRef<FOnlineSessionSearch> search = MakeShareable(new FOnlineSessionSearch());
	search->MaxSearchResults = MaxSearchResults;
	search->bIsLanQuery = bLan;
	search->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);

	OnFindSessionsCompleteHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessions, search));

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), search))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteHandle);
		
		TArray<FOnlineSessionSearchResult> results;
		OnSessionsFound.ExecuteIfBound(false, results);
	}
}

void UBigMoxiOnlineSubsystem::CancelFindSessions()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnCancelFindSessionCompleted.ExecuteIfBound(false);
		return;
	}

	OnCancelFindSessionsCompleteHandle = sessionInterface->AddOnCancelFindSessionsCompleteDelegate_Handle(FOnCancelFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnCancelFindSessions));

	if (OnFindSessionsCompleteHandle.IsValid())
	{
		sessionInterface->CancelFindSessions();
	}
	else
	{
		sessionInterface->ClearOnCancelFindSessionsCompleteDelegate_Handle(OnCancelFindSessionsCompleteHandle);
		OnCancelFindSessionCompleted.ExecuteIfBound(false);
	}
}

void UBigMoxiOnlineSubsystem::OnCancelFindSessions(bool bSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnCancelFindSessionsCompleteDelegate_Handle(OnCancelFindSessionsCompleteHandle);
	}

	OnCancelFindSessionCompleted.ExecuteIfBound(bSuccessful);
}

bool UBigMoxiOnlineSubsystem::IsValid(const FOnlineSessionSearchResult& Session)
{
	return Session.IsSessionInfoValid() && Session.Session.NumOpenPublicConnections > 0;
}

FString UBigMoxiOnlineSubsystem::GetMapName(const FOnlineSessionSearchResult& Session)
{
	FString mapName;
	Session.Session.SessionSettings.Get(FName(TEXT("SETTING_MAPNAME")), mapName);
	return mapName;
}

void UBigMoxiOnlineSubsystem::OnFindSessions(bool bSuccessful, TSharedRef<FOnlineSessionSearch> sessionSearch)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteHandle);
	}

	if (!bSuccessful || sessionSearch->SearchResults.Num() < 1)
	{
		TArray<FOnlineSessionSearchResult> results;
		OnSessionsFound.ExecuteIfBound(false, results);
		return;
	}

	OnSessionsFound.ExecuteIfBound(true, sessionSearch->SearchResults);
}

void UBigMoxiOnlineSubsystem::JoinSession(const FOnlineSessionSearchResult& Session)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnJoinSessionCompleted.ExecuteIfBound(false);
		return;
	}

	OnJoinSessionsCompleteHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionJoined));

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Session))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionsCompleteHandle);

		OnJoinSessionCompleted.ExecuteIfBound(false);
	}
}

void UBigMoxiOnlineSubsystem::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionsCompleteHandle);
	}

	OnJoinSessionCompleted.ExecuteIfBound(true);

	FString connectString;

	if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectString))
	{
		return;
	}

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
}

void UBigMoxiOnlineSubsystem::DestroySession()
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (!sessionInterface.IsValid())
	{
		OnDestroySessionCompleted.ExecuteIfBound(false);
		return;
	}

	OnDestroySessionCompleteHandle = sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnSessionDestroyed));

	if (!sessionInterface->DestroySession(NAME_GameSession))
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteHandle);

		OnDestroySessionCompleted.ExecuteIfBound(false);
	}
}

void UBigMoxiOnlineSubsystem::OnSessionDestroyed(FName SessionName, bool bSuccessful)
{
	const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());
	if (sessionInterface)
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteHandle);
	}

	OnDestroySessionCompleted.ExecuteIfBound(bSuccessful);
}
