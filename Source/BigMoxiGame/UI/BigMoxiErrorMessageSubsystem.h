// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Subsystems/LocalPlayerSubsystem.h>

#include "BigMoxiErrorMessageSubsystem.generated.h"

class UErrorBoxWidget;

UCLASS()
class UBigMoxiErrorMessageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/**  */
	UBigMoxiErrorMessageSubsystem();

	/**  */
	void AddErrorMessage(const FString& ErrorMessage);

private:
	/**  */
	UErrorBoxWidget* ErrorWidget;
	TSubclassOf<UUserWidget> ErrorWidgetClass;

	/**  */
	bool FindErrorWidget();
};
