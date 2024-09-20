// Copyright (c) 2024 Obiwan Medeiros.

#include "ErrorBoxWidget.h"
#include "ErrorTextWidget.h"

#include <Components/ScrollBox.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(ErrorBoxWidget)

void UErrorBoxWidget::AddErrorMessage(const FString& ErrorMessage)
{
	if (!ErrorMessageEntryClass)
	{
		return;
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UErrorTextWidget* errorText = CreateWidget<UErrorTextWidget>(GetOwningPlayer(), ErrorMessageEntryClass);
	ErrorList->AddChild(errorText);
	errorText->Update(ErrorMessage, MessageDuration, ErrorList->GetChildrenCount() % 2 == 0);

	TimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::OnTimeEnd), MessageDuration, false);
}

void UErrorBoxWidget::OnTimeEnd()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
