// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Blueprint/UserWidget.h>

#include "ErrorTextWidget.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class UErrorTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Update(const FString& ErrorMessage, float Duration, bool bEven);

protected:
	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ErrorText;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UBorder> ErrorBorder;

	/**  */
	void OnTimeEnd();
};
