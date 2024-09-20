// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Blueprint/UserWidget.h>

#include "HealthWidget.generated.h"

class ABigMoxiPlayerState;
class UProgressBar;

UCLASS()
class UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	//~ Begin UUserWidget interface.
	void NativeConstruct() override;
	//~ End UUserWidget interface.

	/**  */
	UFUNCTION()
	void OnHealthChanged(float NewValue, float OldValue);

	/**  */
	void UpdateHealthBar(float Health);

private:
	/**  */
	TWeakObjectPtr<ABigMoxiPlayerState> PlayerState = nullptr;

	/**  */
	void BindPlayerState(bool& bOutSuccess);

	/**  */
	UFUNCTION()
	void OnRepPlayerState(APlayerState* RepPlayerState);
};
