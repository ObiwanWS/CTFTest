// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiFlag.h"
#include "BigMoxiGameState_CTF.h"
#include "Character/BigMoxiCharacter.h"

#include <Components/SphereComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiFlag)

ABigMoxiFlag::ABigMoxiFlag()
{
	// Disable tick.
	PrimaryActorTick.bCanEverTick = false;

	// Init members.
	StartTransform = FTransform::Identity;
}

void ABigMoxiFlag::BeginPlay()
{
	Super::BeginPlay();

	StartTransform = GetActorTransform();

	if (ABigMoxiGameState_CTF* gameState = GetWorld()->GetGameState<ABigMoxiGameState_CTF>())
	{
		gameState->AddFlagForTeam(this, 0U);
	}
}

void ABigMoxiFlag::Interact(ABigMoxiCharacter* InteractedCharacter)
{
	if (!InteractedCharacter)
	{
		return;
	}

	Super::Interact(InteractedCharacter);

	// Attach to character.
	FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(OwnerCharacter->GetMesh1P(), attachmentRules, FName(TEXT("Flag")));
}

void ABigMoxiFlag::Release(bool bReset)
{
	if (!OwnerCharacter)
	{
		return;
	}

	if (IsAttachedTo(OwnerCharacter))
	{
		FDetachmentTransformRules attachmentRules(EDetachmentRule::KeepWorld, true);
		DetachFromActor(attachmentRules);

		if (bReset)
		{
			SetActorLocationAndRotation(StartTransform.GetLocation(), StartTransform.GetRotation(), false, nullptr, ETeleportType::ResetPhysics);
			bReset = false;
		}
		else
		{
			const FVector traceStart = OwnerCharacter->GetActorLocation();
			const FVector traceEnd = OwnerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, -1000.0f);

			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, traceStart, traceEnd, ECollisionChannel::ECC_Visibility))
			{
				const FVector newLocation = Hit.ImpactPoint + FVector(0.0f, 0.0f, 100.0f);
				SetActorLocationAndRotation(newLocation, StartTransform.GetRotation());
			}
			else
			{
				SetActorLocation(traceStart);
			}
		}
	}

	Super::Release(bReset);
}
