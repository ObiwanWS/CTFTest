// Copyright (c) 2024 Obiwan Medeiros.

#include "MainMenuWidget.h"

#include <Components/Button.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(MainMenuWidget)

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind buttons.
	QuitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::QuitGame);

	// Setting for menu navigation.
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetWidgetToFocus(TakeWidget());
	GetOwningPlayer()->SetInputMode(InputMode);
	GetOwningPlayer()->FlushPressedKeys();
	GetOwningPlayer()->bShowMouseCursor = true;
}

void UMainMenuWidget::QuitGame()
{
	// Same as what KismetSystemLibrary::QuitGame does, but without all that happens in between.
	GetOwningPlayer()->ConsoleCommand("quit");
}
