// Copyright (c) 2024 Obiwan Medeiros.

#include "BackButton.h"
#include "UI/Widgets/BigMoxiWidgetInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BackButton)

void UBackButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackClicked);
}

void UBackButton::OnBackClicked()
{
	UUserWidget* owningWidget = GetTypedOuter<UUserWidget>();
	if (IBigMoxiWidgetInterface* widgetInterface = Cast<IBigMoxiWidgetInterface>(owningWidget))
	{
		owningWidget->SetVisibility(ESlateVisibility::Collapsed);
		
		widgetInterface->Deactivate();
		widgetInterface->GetRootWidget()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
