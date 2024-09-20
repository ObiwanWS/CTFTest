// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <GameFramework/HUD.h>

#include "BigMoxiHUD.generated.h"

UCLASS()
class ABigMoxiHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Default constructor.
	ABigMoxiHUD();

protected:
	// Widget that will always be added when this HUD class is created.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TSubclassOf<UUserWidget>> WidgetsToAdd;

	//~ Begin of AActor interface.
	void BeginPlay() override;
	//~ End of AActor interface.
};
