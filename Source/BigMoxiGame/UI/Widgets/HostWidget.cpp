// Copyright (c) 2024 Obiwan Medeiros.

#include "HostWidget.h"
#include "BigMoxiGameDebug.h"
#include "Game/BigMoxiLevelDefinition.h"
#include "Online/BigMoxiOnlineSubsystem.h"
#include "UI/BigMoxiErrorMessageSubsystem.h"
#include "UI/Components/BackButton.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/WidgetSwitcher.h>
#include <Engine/AssetManager.h>
#include <Engine/StreamableManager.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(HostWidget)

void UHostWidget::Activate()
{
	if (LevelDefinitions.Num() > 0)
	{
		LoadDefinition(0);
	}
}

void UHostWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AssetManager = UAssetManager::GetIfInitialized();
	if (AssetManager)
	{
		FPrimaryAssetType definitionType = UBigMoxiLevelDefinition::StaticClass()->GetFName();
		
		AssetManager->GetPrimaryAssetIdList(definitionType, LevelDefinitions);
		if (LevelDefinitions.Num() > 0)
		{
			if (LevelDefinitions.Num() == 1)
			{
				NextButton->SetVisibility(ESlateVisibility::Collapsed);
				PreviousButton->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				// Bind buttons.
				NextButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnNextDefinition);
				PreviousButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnPreviousDefinition);
			}

			HostButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnHostGame);
			BackButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBack);
		}
		else
		{
			UE_LOG(LogBigMoxiGame, Error, TEXT("%s: Failed to find any Level Definitions."), ANSI_TO_TCHAR(__FUNCTION__));
		}
	}
}

void UHostWidget::LoadDefinition(int32 Index)
{
	Switcher->SetActiveWidgetIndex(0);
	LevelImage->SetVisibility(ESlateVisibility::Hidden);
	LoadHandle = AssetManager->LoadPrimaryAsset(LevelDefinitions[Index], TArray<FName>(), FStreamableDelegate::CreateUObject(this, &ThisClass::OnDefinitionLoaded, LevelDefinitions[Index]));
}

void UHostWidget::OnDefinitionLoaded(FPrimaryAssetId LoadedId)
{
	Switcher->SetActiveWidgetIndex(1);

	LastDefinition = Cast<UBigMoxiLevelDefinition>(AssetManager->GetPrimaryAssetObject(LoadedId));
	if (LastDefinition)
	{
		LevelName->SetText(LastDefinition->GetLevelDefinition().LevelName);
		LevelImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		LevelImage->SetBrushFromSoftTexture(LastDefinition->GetLevelDefinition().Thumbnail);
	}
}

void UHostWidget::OnNextDefinition()
{
	if (LevelIndex == LevelDefinitions.Num() - 1)
	{
		LevelIndex = 0;
	}
	else
	{
		LevelIndex++;
	}

	LoadDefinition(LevelIndex);
}

void UHostWidget::OnPreviousDefinition()
{
	if (LevelIndex == 0)
	{
		LevelIndex = LevelDefinitions.Num() - 1;
	}
	else
	{
		LevelIndex--;
	}

	LoadDefinition(LevelIndex);
}

void UHostWidget::OnHostGame()
{
	UBigMoxiOnlineSubsystem* onlineSubsystem = GetGameInstance()->GetSubsystem<UBigMoxiOnlineSubsystem>();
	if (onlineSubsystem && LastDefinition)
	{
		Switcher->SetActiveWidgetIndex(0);
		BackButton->SetVisibility(ESlateVisibility::Collapsed);

		FSimpleSessionSettings* sessionSettings = new FSimpleSessionSettings();
		sessionSettings->MaxPlayers = LastDefinition->GetLevelDefinition().MaxPlayers;
		sessionSettings->bLan = true; // This should be a parameter or a button, but for this test I'll leave it hardcoded.
		sessionSettings->LevelPath = LastDefinition->GetLevelDefinition().Level.GetLongPackageName();

		onlineSubsystem->OnCreateSessionCompleted.BindLambda([this](bool bSuccess)
		{
			if (bSuccess)
			{
				UBigMoxiOnlineSubsystem* onlineSubsystem = GetGameInstance()->GetSubsystem<UBigMoxiOnlineSubsystem>();
				if (onlineSubsystem)
				{
					onlineSubsystem->StartSession();
				}
			}
			else
			{
				Switcher->SetActiveWidgetIndex(1);
				BackButton->SetVisibility(ESlateVisibility::Visible);

				GetWorld()->GetSubsystem<UBigMoxiErrorMessageSubsystem>()->AddErrorMessage(TEXT("Failed to Create Session."));
				UE_LOG(LogBigMoxiGame, Warning, TEXT("%s: Failed to Create Session."), ANSI_TO_TCHAR(__FUNCTION__));
			}
		});

		onlineSubsystem->CreateSession(*sessionSettings);
	}
}

void UHostWidget::OnBack()
{
	if (LoadHandle.IsValid() && LoadHandle->IsActive())
	{
		LoadHandle->CancelHandle();
	}
}
