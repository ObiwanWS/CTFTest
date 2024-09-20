// Copyright (c) 2024 Obiwan Medeiros.

#include "JoinWidget.h"
#include "Online/BigMoxiOnlineSubsystem.h"
#include "SessionWidget.h"
#include "UI/BigMoxiErrorMessageSubsystem.h"
#include "UI/Components/BackButton.h"

#include <Components/Button.h>
#include <Components/ScrollBox.h>
#include <Components/WidgetSwitcher.h>
#include <OnlineSessionSettings.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(JoinWidget)

void UJoinWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SearchButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSearch);
	BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBack);
}

void UJoinWidget::OnSearch()
{
	Switcher->SetActiveWidgetIndex(1);
	SessionsList->ClearChildren();

	UBigMoxiOnlineSubsystem* onlineSubsystem = GetGameInstance()->GetSubsystem<UBigMoxiOnlineSubsystem>();
	if (onlineSubsystem)
	{
		onlineSubsystem->OnSessionsFound.BindLambda([this](bool bSuccess, TArray<FOnlineSessionSearchResult>& sessions)
		{
			if (bSuccess)
			{
				CreateSessionButtons(sessions);
			}
			else
			{
				GetWorld()->GetSubsystem<UBigMoxiErrorMessageSubsystem>()->AddErrorMessage(TEXT("No Sessions found."));
			}

			Switcher->SetActiveWidgetIndex(0);
		});

		onlineSubsystem->FindSessions(10, true); // Lan should be a param or a button for this test I'll leave it hardcoded.
	}
}

void UJoinWidget::OnBack()
{
	UBigMoxiOnlineSubsystem* onlineSubsystem = GetGameInstance()->GetSubsystem<UBigMoxiOnlineSubsystem>();
	if (onlineSubsystem)
	{
		onlineSubsystem->OnCancelFindSessionCompleted.BindLambda([this](bool bSuccess)
		{
			if (bSuccess)
			{
				SessionsList->ClearChildren();
				Switcher->SetActiveWidgetIndex(0);

				GetWorld()->GetSubsystem<UBigMoxiErrorMessageSubsystem>()->AddErrorMessage(TEXT("Session search canceled."));
			}
		});

		onlineSubsystem->CancelFindSessions();
	}
}

void UJoinWidget::OnJoinSession()
{
	Switcher->SetActiveWidgetIndex(1);
}

void UJoinWidget::CreateSessionButtons(TArray<FOnlineSessionSearchResult>& Sessions)
{
	if (!SessionListEntryClass)
	{
		return;
	}

	UBigMoxiOnlineSubsystem* onlineSubsystem = GetGameInstance()->GetSubsystem<UBigMoxiOnlineSubsystem>();
	if (onlineSubsystem)
	{
		for (const FOnlineSessionSearchResult& sessionResult : Sessions)
		{
			if (sessionResult.IsValid() && sessionResult.Session.NumOpenPublicConnections > 0)
			{
				USessionWidget* sessionButton = CreateWidget<USessionWidget>(GetOwningPlayer(), SessionListEntryClass);
				
				sessionButton->GetSessionButton()->OnClicked.AddUniqueDynamic(this, &ThisClass::OnJoinSession);

				onlineSubsystem->OnJoinSessionCompleted.BindLambda([this, sessionButton](bool bSuccess)
				{
					if (!bSuccess)
					{
						Switcher->SetActiveWidgetIndex(0);
						SessionsList->RemoveChild(sessionButton);

						GetWorld()->GetSubsystem<UBigMoxiErrorMessageSubsystem>()->AddErrorMessage(TEXT("Failed to Join Session."));
					}
				});

				SessionsList->AddChild(sessionButton);
				sessionButton->SetSession(sessionResult);
				
			}
		}
	}
}
