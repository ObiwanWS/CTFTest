// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include "UI/Widgets/BigMoxiWidgetInterface.h"

#include <Blueprint/UserWidget.h>

#include "HostWidget.generated.h"

class UAssetManager;
class UBackButton;
class UButton;
class UImage;
class UBigMoxiLevelDefinition;
class UTextBlock;
class UWidgetSwitcher;
struct FStreamableHandle;

UCLASS(Abstract)
class UHostWidget : public UUserWidget, public IBigMoxiWidgetInterface
{
	GENERATED_BODY()
	
public:
	/**  */
	void Activate() override;

protected:
	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelName;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> LevelImage;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> NextButton;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> PreviousButton;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> HostButton;

	/**  */
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UBackButton> BackButton;

	//~ Begin UUserWidget interface.
	void NativeConstruct() override;
	//~ End UUserWidget interface.

	/**  */
	void LoadDefinition(int32 Index);

	/**  */
	UFUNCTION()
	void OnDefinitionLoaded(FPrimaryAssetId LoadedId);

	/**  */
	UFUNCTION()
	void OnNextDefinition();

	/**  */
	UFUNCTION()
	void OnPreviousDefinition();

	/**  */
	UFUNCTION()
	void OnHostGame();

	/**  */
	UFUNCTION()
	void OnBack();

private:
	/**  */
	UAssetManager* AssetManager = nullptr;

	/**  */
	TArray<FPrimaryAssetId> LevelDefinitions;

	/**  */
	int32 LevelIndex = 0;

	/**  */
	TSharedPtr<FStreamableHandle> LoadHandle;

	/**  */
	UBigMoxiLevelDefinition* LastDefinition;
};
