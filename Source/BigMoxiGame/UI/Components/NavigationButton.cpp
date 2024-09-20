// Copyright (c) 2024 Obiwan Medeiros.

#include "NavigationButton.h"
#include "UI/Widgets/BigMoxiWidgetInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NavigationButton)

void UNavigationButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	OnClicked.AddUniqueDynamic(this, &ThisClass::OnNavigationClicked);
}

void UNavigationButton::OnNavigationClicked()
{
	if (!SubWidgetClass)
	{
		return;
	}

	GetTypedOuter<UUserWidget>()->SetVisibility(ESlateVisibility::Collapsed);

	if (SubWidget)
	{
		SubWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (IBigMoxiWidgetInterface* widgetInterface = Cast<IBigMoxiWidgetInterface>(SubWidget))
		{
			widgetInterface->Activate();
		}
	}
	else
	{
		SubWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), SubWidgetClass);
		SubWidget->AddToViewport();

		if (IBigMoxiWidgetInterface* widgetInterface = Cast<IBigMoxiWidgetInterface>(SubWidget))
		{
			widgetInterface->SetRootWidget(GetTypedOuter<UUserWidget>());
			widgetInterface->Activate();
		}
	}
}
