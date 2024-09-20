// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiWeapon.h"
#include "Character/BigMoxiCharacter.h"
#include "BigMoxiWeaponComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiWeapon)

ABigMoxiWeapon::ABigMoxiWeapon()
{
	// Avoid ticking.
	PrimaryActorTick.bCanEverTick = false;

	// CreateWeaponComponent
	WeaponComponent = CreateDefaultSubobject<UBigMoxiWeaponComponent>(TEXT("WeaponComp"));

	// Set members.
	GetMesh()->bSelfShadowOnly = true;
}

void ABigMoxiWeapon::Interact(ABigMoxiCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	Super::Interact(InteractedCharacter);

	SetOwner(OwnerCharacter);

	WeaponComponent->AttachWeapon(OwnerCharacter);
}

void ABigMoxiWeapon::Release(bool bReset)
{
	WeaponComponent->DetachWeapon();

	Super::Release(bReset);
}
