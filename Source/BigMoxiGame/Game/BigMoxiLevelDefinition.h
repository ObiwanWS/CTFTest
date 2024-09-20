// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <Engine/DataAsset.h>

#include "BigMoxiLevelDefinition.generated.h"

USTRUCT()
struct FLevelDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText LevelName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxPlayers;

	FLevelDefinition()
	{
		Level = nullptr;
		Thumbnail = nullptr;
		MaxPlayers = 4;
	}
};

UCLASS()
class UBigMoxiLevelDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FLevelDefinition& GetLevelDefinition()
	{
		return LevelDefinition;
	}

protected:
	UPROPERTY(EditDefaultsOnly)
	FLevelDefinition LevelDefinition;
};
