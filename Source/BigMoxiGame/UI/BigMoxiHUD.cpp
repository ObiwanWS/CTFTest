// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiHUD.h"

#include <Blueprint/UserWidget.h>
#include <Engine/AssetManager.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiHUD)

ABigMoxiHUD::ABigMoxiHUD()
{
	// Disable tick.
	PrimaryActorTick.bCanEverTick = false;
}

void ABigMoxiHUD::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < WidgetsToAdd.Num(); i++)
	{
		if (WidgetsToAdd[i])
		{
			UUserWidget* widget = CreateWidget<UUserWidget>(PlayerOwner, WidgetsToAdd[i]);
			widget->AddToViewport();
		}
	}
}
