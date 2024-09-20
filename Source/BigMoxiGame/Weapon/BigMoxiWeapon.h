// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "Modes/BigMoxiInteractable.h"

#include "BigMoxiWeapon.generated.h"

class UBigMoxiWeaponComponent;

UCLASS()
class ABigMoxiWeapon : public ABigMoxiInteractable
{
	GENERATED_BODY()
	
public:	
	/** Sets default values for this actor's properties. */
	ABigMoxiWeapon();

protected:
	//~ Begin ABigMoxiInteractable interface.
	void Interact(ABigMoxiCharacter* InteractedCharacter) override;
	void Release(bool bReset = false) override;
	//~ End ABigMoxiInteractable interface.

private:
	/**  */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBigMoxiWeaponComponent> WeaponComponent;
};
