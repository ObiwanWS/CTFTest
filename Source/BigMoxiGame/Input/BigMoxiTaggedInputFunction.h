// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <InputTriggers.h>
#include <GameplayTagContainer.h>

#include "BigMoxiTaggedInputFunction.generated.h"

/**
 * InputTag mapped to a UFUNCTION that will be called once the input is triggered.
 */
USTRUCT()
struct FTaggedInputFunction
{
	GENERATED_BODY()

public:
	/** The InputTag associate to an InputAction inside a InputConfig asset. */
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Input"))
	FGameplayTag InputTag;

	/** The function that will be called once the InputAction associanted with the InputTag is triggered. */
	UPROPERTY(EditDefaultsOnly, Meta = (GetOptions = "GetFunctionOptions"))
	FName FunctionName;

	/** The type of trigger for the input. */
	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent InputTrigger;

	/** Default constructor values. */
	FTaggedInputFunction()
	{
		InputTag = FGameplayTag::EmptyTag;
		InputTrigger = ETriggerEvent::Triggered;
		FunctionName = NAME_None;
	}

	/** Overriden the == operator to allow UPROPERTY specifier with NoElementDuplicate. */
	FORCEINLINE bool operator==(const FTaggedInputFunction& Other)
	{
		return InputTag == Other.InputTag && InputTrigger == Other.InputTrigger && FunctionName.IsEqual(Other.FunctionName);
	}
};
