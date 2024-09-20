// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <AttributeSet.h>
#include <AbilitySystemComponent.h>
#include <Net/Core/PushModel/PushModel.h>

#include "BigMoxiAttributeSet.generated.h"

#define GAMEPLAYATTRIBUTE_VALUE_SETTER_WITH_MARKDIRTY(PropertyName)							\
	FORCEINLINE void Set##PropertyName(float NewVal)										\
	{																						\
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PropertyName, this);						\
		UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();			\
		if (ensure(AbilityComp))															\
		{																					\
			AbilityComp->SetNumericAttributeBase(Get##PropertyName##Attribute(), NewVal);	\
		};																					\
	}

// Macro to ease on attribute creation with Push Model. (I have no idea why this isn't on the engine by default at this point)
#define ATTRIBUTE_ACCESSORS_WITH_MARKDIRTY(ClassName, PropertyName)         \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)							\
	GAMEPLAYATTRIBUTE_VALUE_SETTER_WITH_MARKDIRTY(PropertyName)             \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UBigMoxiAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	/** Set initial attribute values. */
	UBigMoxiAttributeSet();

	//~ Begin UObject interface.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	//~ End UObject interface.

	//~ Begin UAttributeSet interface.
	void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	//~ End UAttributeSet interface.

	//~ Begin Attribute accessors.
	ATTRIBUTE_ACCESSORS_WITH_MARKDIRTY(UBigMoxiAttributeSet, Health)
	ATTRIBUTE_ACCESSORS_WITH_MARKDIRTY(UBigMoxiAttributeSet, MaxHealth)
	//~ End Attribute accessors.

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

private:
	/** Current health of the owner. */
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	/** Max health that the owner can have. */
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	/** Don't let attributes go out of bounds. */
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};
