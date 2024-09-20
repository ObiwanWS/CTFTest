// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiPlatformInputMap.h"
#include "Platform/BigMoxiPlatformTraitsSettings.h"

#include <InputMappingContext.h>
#if WITH_EDITOR
#include <Misc/DataValidation.h>
#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiPlatformInputMap)

bool FBigMoxiPlatformInputContext::IsValid() const
{
	return InputMappingContext->IsValidLowLevelFast();
}

UBigMoxiPlatformInputMap::UBigMoxiPlatformInputMap()
{
	InputMapContexts.Empty();
}

#if WITH_EDITOR
EDataValidationResult UBigMoxiPlatformInputMap::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult result = Super::IsDataValid(Context);

	if (InputMapContexts.IsEmpty())
	{
		Context.AddError(INVTEXT("Mapping Contexts cannot be empty!"));
		result = CombineDataValidationResults(result, EDataValidationResult::Invalid);
	}

	for (int32 i = 0; i < InputMapContexts.Num(); i++)
	{
		if (!InputMapContexts[i].IsValid())
		{
			FFormatNamedArguments args;
			args.Add(TEXT("AssetPath"), FText::FromString(GetPathName()));
			args.Add(TEXT("Index"), FText::AsNumber(i));

			if (!InputMapContexts[i].InputMappingContext)
			{
				const FText errorMessage = FText::Format(INVTEXT("Index '{Index}' has invalid InputMappingContext!"), args);
				Context.AddError(errorMessage);
				result = CombineDataValidationResults(result, EDataValidationResult::Invalid);
			}

			if (!InputMapContexts[i].PlatformAllowTraits.IsValid())
			{
				const FText errorMessage = FText::Format(INVTEXT("Index '{Index}' has no PlatformAllowTraits!"), args);
				Context.AddError(errorMessage);
				result = CombineDataValidationResults(result, EDataValidationResult::Invalid);
			}
		}
	}
	return CombineDataValidationResults(result, EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

TArray<const UInputMappingContext*> UBigMoxiPlatformInputMap::GetSupportedInputMappingContexts() const
{
	TArray<const UInputMappingContext*> foundInputMappingContexts;

	for (int32 i = 0; i < InputMapContexts.Num(); i++)
	{
		if (InputMapContexts[i].IsValid())
		{
			foundInputMappingContexts.Add(InputMapContexts[i].InputMappingContext);
		}
	}
	return foundInputMappingContexts;
}

const TArray<FBigMoxiPlatformInputContext> UBigMoxiPlatformInputMap::GetSupportedInputMappings() const
{
	return FindMatchingInputMappings(UPerPlatformTraitsSettings::Get()->GetPlatformTraits());
}

TArray<FBigMoxiPlatformInputContext> UBigMoxiPlatformInputMap::FindMatchingInputMappings(const FGameplayTagContainer& PlatformTraits) const
{
	TArray<FBigMoxiPlatformInputContext> foundInputMappingContexts;

	if (PlatformTraits.IsValid())
	{
		for (int32 i = 0; i < InputMapContexts.Num(); i++)
		{
			if (InputMapContexts[i].IsSupported(PlatformTraits))
			{
				foundInputMappingContexts.Add(InputMapContexts[i]);
			}
		}
	}
	return foundInputMappingContexts;
}
