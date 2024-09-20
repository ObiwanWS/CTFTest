// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "Modes/BigMoxiInteractable.h"

#include "BigMoxiFlag.generated.h"

class USphereComponent;

UCLASS()
class ABigMoxiFlag : public ABigMoxiInteractable
{
	GENERATED_BODY()
	
public:	
	/** Sets default values for this actor's properties. */
	ABigMoxiFlag();

protected:
	//~ Begin AActor interface.
	void BeginPlay() override;
	//~ End AActor interface.

	//~ Begin ABigMoxiInteractable interface.
	void Interact(ABigMoxiCharacter* InteractedCharacter) override;
	void Release(bool bReset = false) override;
	//~ End ABigMoxiInteractable interface.

private:
	/**  */
	FTransform StartTransform;
};
