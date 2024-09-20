// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "Modes/BigMoxiInteractable.h"

#include <GameplayTagContainer.h>

#include "BigMoxiFlagBase.generated.h"

class USphereComponent;

UCLASS()
class ABigMoxiFlagBase : public ABigMoxiInteractable
{
	GENERATED_BODY()
	
public:	
	/** Sets default values for this actor's properties. */
	ABigMoxiFlagBase();

protected:
	//~ Begin AActor interface.
	void BeginPlay() override;
	//~ End AActor interface.

	//~ Begin ABigMoxiInteractable interface.
	void Interact(ABigMoxiCharacter* InteractedCharacter) override;
	//~ End ABigMoxiInteractable interface.

	UFUNCTION(BlueprintImplementableEvent)
	void SetTeamColor(FLinearColor Color);

private:
	/**
	 * Identifier of which team this flag base is part of.
	 * 0 means no team has been assigned to this flag base.
	 */
	UPROPERTY(EditAnywhere, Category = BigMoxi)
	uint8 TeamId;

	/**
	 * Identifier of which team this flag base is part of.
	 * 0 means no team has been assigned to this flag base.
	 */
	UPROPERTY(EditAnywhere, Category = BigMoxi)
	FGameplayTag HasFlagTag;
};
