// Copyright (c) 2024 Obiwan Medeiros.

#include "HUDWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HUDWidget)

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Setting for menu navigation.
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->FlushPressedKeys();
	GetOwningPlayer()->bShowMouseCursor = false;
}
