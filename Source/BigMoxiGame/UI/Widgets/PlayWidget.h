// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "UI/Widgets/BigMoxiWidgetInterface.h"

#include <Blueprint/UserWidget.h>

#include "PlayWidget.generated.h"

UCLASS(Abstract)
class UPlayWidget : public UUserWidget, public IBigMoxiWidgetInterface
{
	GENERATED_BODY()
	
};
