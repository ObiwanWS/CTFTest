// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Components/Button.h>

#include "BackButton.generated.h"

UCLASS()
class UBackButton : public UButton
{
	GENERATED_BODY()
	
	//~ Begin UWidget interface.
	void OnWidgetRebuilt() override;
	//~ End UWidget interface.

	// Called once this button gets clicked.
	UFUNCTION()
	void OnBackClicked();
};
