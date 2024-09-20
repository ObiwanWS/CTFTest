// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Blueprint/UserWidget.h>

#include "ErrorBoxWidget.generated.h"

class UErrorTextWidget;
class UScrollBox;

UCLASS()
class UErrorBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**  */
	void AddErrorMessage(const FString& ErrorMessage);

protected:
	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UScrollBox> ErrorList;

	/**  */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UErrorTextWidget> ErrorMessageEntryClass;

	/**  */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MessageDuration = 5.0f;

	void OnTimeEnd();

private:
	FTimerHandle TimerHandle = FTimerHandle();
};
