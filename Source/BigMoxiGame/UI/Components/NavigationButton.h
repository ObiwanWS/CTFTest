// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Components/Button.h>

#include "NavigationButton.generated.h"

UCLASS()
class UNavigationButton : public UButton
{
	GENERATED_BODY()
	
protected:
	// Hard reference so it is included in the initial async load.
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TSubclassOf<UUserWidget> SubWidgetClass;

	//~ Begin UWidget interface.
	void OnWidgetRebuilt() override;
	//~ End UWidget interface.

	// Called once this button gets clicked.
	UFUNCTION()
	void OnNavigationClicked();

private:
	/** 
	 * Subwidget class instanced. 
	 * Marked as UPROPERTY to be managed by the garbage collector.
	 */
	UPROPERTY()
	TObjectPtr<UUserWidget> SubWidget;
};
