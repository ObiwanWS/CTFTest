// Copyright (c) 2024 Obiwan Medeiros.

#include "SessionWidget.h"
#include "BigMoxiGameDebug.h"
#include "Online/BigMoxiOnlineSubsystem.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(SessionWidget)

void USessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SessionButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnJoinSession);
}

void USessionWidget::UpdateText()
{
	if (SessionSearchResult.IsValid())
	{
		// Host name limited to 20 characters.
		HostText->SetText(FText::FromString(SessionSearchResult.Session.OwningUserName.Left(20)));

		FString levelName;
		SessionSearchResult.Session.SessionSettings.Get(FName(TEXT("SETTING_MAPNAME")), levelName);

		// Cut full path from level name and only leave the actual asset name.
		int32 barIndex = levelName.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		LevelText->SetText(FText::FromString(levelName.RightChop(barIndex + 1)));

		int32 currentPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections - SessionSearchResult.Session.NumOpenPublicConnections;
		CurrentPlayersText->SetText(FText::AsNumber(currentPlayers));

		int32 maxPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections;
		MaxPlayersText->SetText(FText::AsNumber(maxPlayers));
	}
}

void USessionWidget::SetSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (SessionResult.IsValid())
	{
		SessionSearchResult = MoveTemp(const_cast<FOnlineSessionSearchResult&>(SessionResult));
		UpdateText();
	}
}

void USessionWidget::OnJoinSession()
{
	if (SessionSearchResult.IsValid())
	{
		UBigMoxiOnlineSubsystem* onlineSubsystem = GetGameInstance()->GetSubsystem<UBigMoxiOnlineSubsystem>();
		if (onlineSubsystem)
		{
			onlineSubsystem->OnJoinSessionCompleted.BindLambda([this](bool bSuccess)
			{
				if (!bSuccess)
				{
					UE_LOG(LogBigMoxiGame, Warning, TEXT("%s: Failed to Join Session."), ANSI_TO_TCHAR(__FUNCTION__));
				}
			});

			onlineSubsystem->JoinSession(SessionSearchResult);
		}
	}
}
