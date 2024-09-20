// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <AbilitySystemComponent.h>
#include <InputActionValue.h>

#include "BigMoxiAbilitySystemComponent.generated.h"

UCLASS()
class UBigMoxiAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	/**  */
	void TriggerAbilityInputTag(const FInputActionValue& InputValue, FGameplayTag AbilityTag);

protected:
	//~ Begin UAbilitySystemComponent interface.
	bool ShouldDoServerAbilityRPCBatch() const override {return true;}
	//~ End UAbilitySystemComponent interface.
};
