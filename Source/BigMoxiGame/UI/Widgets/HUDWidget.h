// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Blueprint/UserWidget.h>

#include "HUDWidget.generated.h"

UCLASS(Abstract)
class UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//~ Begin UUserWidget interface.
	void NativeConstruct() override;
	//~ End UUserWidget interface.
};
