// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiErrorMessageSubsystem.h"
#include "BigMoxiGameDebug.h"
#include "UObject/ConstructorHelpers.h"

#include "UI/Widgets/ErrorBoxWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiErrorMessageSubsystem)

UBigMoxiErrorMessageSubsystem::UBigMoxiErrorMessageSubsystem()
{
	ErrorWidget = nullptr;
	
	static ConstructorHelpers::FClassFinder<UUserWidget> widgetClassFinder(TEXT("/Game/BigMoxi/UI/ErrorBox/W_ErrorBox"));
	ErrorWidgetClass = widgetClassFinder.Class;
}

void UBigMoxiErrorMessageSubsystem::AddErrorMessage(const FString& ErrorMessage)
{
	if (!ErrorMessage.IsEmpty())
	{
		if (!ErrorWidget)
		{
			if (!FindErrorWidget())
			{
				// Use the first Player to account for local multiplayer scenarios (just because since this is a test without local multiplayer).
				if (APlayerController* firstPlayerController = GetWorld()->GetGameInstance()->GetFirstGamePlayer()->GetPlayerController(GetWorld()))
				{
					if (ErrorWidgetClass)
					{
						ErrorWidget = CreateWidget<UErrorBoxWidget>(firstPlayerController, ErrorWidgetClass);
						ErrorWidget->AddToViewport();
					}
				}
				else
				{
					UE_LOG(LogBigMoxiGame, Error, TEXT("%s: Invalid first local player."), ANSI_TO_TCHAR(__FUNCTION__));
					return;
				}
			}
		}

		ErrorWidget->AddErrorMessage(ErrorMessage);
	}
}

bool UBigMoxiErrorMessageSubsystem::FindErrorWidget()
{
	for (TObjectIterator<UErrorBoxWidget> Itr; Itr; ++Itr)
	{
		UErrorBoxWidget* widget = *Itr;

		// Skip any widget that's not in the current world context or that is not a child of the class specified.
		if (widget->GetWorld() != GetWorld() || !widget->GetClass()->IsChildOf(UErrorBoxWidget::StaticClass()))
		{
			continue;
		}

		if (widget->IsInViewport())
		{
			ErrorWidget = widget;
			return true;
		}
	}

	return false;
}
