// Copyright (c) 2024 Obiwan Medeiros.

#include "ErrorTextWidget.h"

#include <Components/Border.h>
#include <Components/TextBlock.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(ErrorTextWidget)

void UErrorTextWidget::Update(const FString& ErrorMessage, float Duration, bool bEven)
{
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::OnTimeEnd), Duration, false);

	ErrorText->SetText(FText::FromString(ErrorMessage));

	FLinearColor borderColor = bEven ? FLinearColor(0.15f, 0.15f, 0.15f) : FLinearColor(0.1f, 0.1f, 0.1f);
	ErrorBorder->SetBrushColor(borderColor);
}

void UErrorTextWidget::OnTimeEnd()
{
	RemoveFromParent();
}
