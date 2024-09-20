// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Blueprint/UserWidget.h>
#include <Blueprint/IUserObjectListEntry.h>
#include <OnlineSessionSettings.h>

#include "SessionWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS(Abstract)
class USessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**  */
	void SetSession(const FOnlineSessionSearchResult& SessionResult);

	/**  */
	UButton* GetSessionButton()
	{
		return SessionButton;
	}

	void UpdateText();

protected:
	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> SessionButton;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> HostText;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentPlayersText;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxPlayersText;

	//~ Begin UUserWidget interface.
	void NativeConstruct() override;
	//~ End UUserWidget interface.

	/**  */
	UFUNCTION()
	void OnJoinSession();

private:
	FOnlineSessionSearchResult SessionSearchResult;
};
