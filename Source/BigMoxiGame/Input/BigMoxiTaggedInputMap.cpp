// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiTaggedInputMap.h"

#include <InputAction.h>

#if WITH_EDITOR
#include <Misc/DataValidation.h>
#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiTaggedInputMap)

bool FBigMoxiTaggedInput::IsValid() const
{
	return InputAction->IsValidLowLevelFast() && InputTag.IsValid();
}

UBigMoxiTaggedInputMap::UBigMoxiTaggedInputMap()
{
	TaggedInputs = TArray<FBigMoxiTaggedInput>();
}

#if WITH_EDITOR
EDataValidationResult UBigMoxiTaggedInputMap::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult result = Super::IsDataValid(Context);

	for (int32 i = 0; i < TaggedInputs.Num(); i++)
	{
		if (!TaggedInputs[i].IsValid())
		{
			FFormatNamedArguments args;
			args.Add(TEXT("AssetPath"), FText::FromString(GetPathName()));
			args.Add(TEXT("Index"), FText::AsNumber(i));

			const FText errorMessage = FText::Format(INVTEXT("TaggedInputs Index '{Index}' cannot have invalid data!"), args);
			Context.AddError(errorMessage);
			result = CombineDataValidationResults(result, EDataValidationResult::Invalid);
		}
	}

	for (int32 i = 0; i < AbilityTaggedInputs.Num(); i++)
	{
		if (!AbilityTaggedInputs[i].IsValid())
		{
			FFormatNamedArguments args;
			args.Add(TEXT("AssetPath"), FText::FromString(GetPathName()));
			args.Add(TEXT("Index"), FText::AsNumber(i));

			const FText noInputConfigError = FText::Format(INVTEXT("AbilityTaggedInput Index '{Index}' cannot have invalid data!"), args);
			Context.AddError(noInputConfigError);
			result = CombineDataValidationResults(result, EDataValidationResult::Invalid);
		}
	}

	return CombineDataValidationResults(result, EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

const UInputAction* UBigMoxiTaggedInputMap::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (int32 i = 0; i < TaggedInputs.Num(); i++)
	{
		if (TaggedInputs[i].InputAction && TaggedInputs[i].InputTag == InputTag)
		{
			return TaggedInputs[i].InputAction;
		}
	}
	return nullptr;
}

TArray<const UInputAction*> UBigMoxiTaggedInputMap::GetAllInputActions() const
{
	TArray<const UInputAction*> inputActions;

	for (int32 i = 0; i < TaggedInputs.Num(); i++)
	{
		inputActions.Add(TaggedInputs[i].InputAction);
	}

	for (int32 i = 0; i < AbilityTaggedInputs.Num(); i++)
	{
		inputActions.Add(AbilityTaggedInputs[i].InputAction);
	}

	return inputActions;
}

const UInputAction* UBigMoxiTaggedInputMap::FindInputActionForAbilityTaggedInput(const FGameplayTag& AbilityTag) const
{
	for (int32 i = 0; i < AbilityTaggedInputs.Num(); i++)
	{
		if (AbilityTaggedInputs[i].InputAction && AbilityTaggedInputs[i].InputTag == AbilityTag)
		{
			return AbilityTaggedInputs[i].InputAction;
		}
	}
	return nullptr;
}
