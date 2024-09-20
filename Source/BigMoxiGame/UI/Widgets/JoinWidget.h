// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "UI/Widgets/BigMoxiWidgetInterface.h"

#include <Blueprint/UserWidget.h>

#include "JoinWidget.generated.h"

class UBackButton;
class UButton;
class UScrollBox;
class FOnlineSessionSearchResult;
class USessionWidget;
class UWidgetSwitcher;

UCLASS(Abstract)
class UJoinWidget : public UUserWidget, public IBigMoxiWidgetInterface
{
	GENERATED_BODY()
	
protected:
	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UScrollBox> SessionsList;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> SearchButton;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UBackButton> BackButton;

	/**  */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<USessionWidget> SessionListEntryClass;

	//~ Begin UUserWidget interface.
	void NativeConstruct() override;
	//~ End UUserWidget interface.

	/**  */
	UFUNCTION()
	void OnSearch();

	/**  */
	UFUNCTION()
	void OnBack();

	/**  */
	UFUNCTION()
	void OnJoinSession();

	/**  */
	void CreateSessionButtons(TArray<FOnlineSessionSearchResult>& Sessions);
};
