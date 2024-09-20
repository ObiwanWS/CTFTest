// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Blueprint/UserWidget.h>

#include "MainMenuWidget.generated.h"

class UButton;

UCLASS(Abstract)
class UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/** Bound property to the Quit Button on the blueprint widget. */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	//~ Begin UUserWidget interface.
	void NativeConstruct() override;
	//~ End UUserWidget interface.

	// Buton click functions.
	UFUNCTION()
	void QuitGame();
};
