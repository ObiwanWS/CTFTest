// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiInteractable.h"
#include "Character/BigMoxiCharacter.h"

#include <AbilitySystemComponent.h>
#include <Components/SphereComponent.h>
#include <GameplayEffect.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiInteractable)

ABigMoxiInteractable::ABigMoxiInteractable()
{
	// Disable tick.
	PrimaryActorTick.bCanEverTick = false;

	// Enable replication.
	bReplicates = true;

	// Create mesh.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetNotifyRigidBodyCollision(false);
	Mesh->BodyInstance.SetCollisionProfileName("NoCollision");
	Mesh->CanCharacterStepUpOn = ECB_No;
	RootComponent = Mesh;

	// Create collision.
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetGenerateOverlapEvents(true);
	Collision->SetNotifyRigidBodyCollision(false);
	Collision->BodyInstance.SetCollisionProfileName("OverlapAll");
	Collision->SetCanEverAffectNavigation(false);
	Collision->InitSphereRadius(5.0f);
	Collision->SetupAttachment(RootComponent);

	// Init members.
	bInteractOnBeginOverlap = true;
	bReleaseOnEndOverlap = false;
	bInteracted = false;
	OwnerCharacter = nullptr;
}

void ABigMoxiInteractable::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// Bind delegates.
		Collision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::BeginOverlap);
		Collision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::EndOverlap);
	}
}

void ABigMoxiInteractable::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bInteractOnBeginOverlap)
	{
		if (ABigMoxiCharacter* character = Cast<ABigMoxiCharacter>(OtherActor))
		{
			Interact_Server(character);
		}
	}
}

void ABigMoxiInteractable::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	if (bReleaseOnEndOverlap)
	{
		Release_Server();
	}
}

void ABigMoxiInteractable::Interact(ABigMoxiCharacter* InteractedCharacter)
{
	if (bInteracted || !InteractedCharacter)
	{
		return;
	}

	bInteracted = true;
	OwnerCharacter = InteractedCharacter;
	OnInteract.Broadcast(OwnerCharacter);

	// Release if the character we're interacting with is destroyed during interaction.
	OwnerCharacter->OnEndPlay.AddUniqueDynamic(this, &ThisClass::OnOwnerEndPlay);

	if (HasAuthority())
	{
		ApplyGameplayEffect(OwnerCharacter, InteractEffect);

		// Unbind delegates.
		Collision->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::BeginOverlap);
	}
}

void ABigMoxiInteractable::Interact_Server_Implementation(ABigMoxiCharacter* InteractedCharacter)
{
	Interact_Multicast(InteractedCharacter);
}

void ABigMoxiInteractable::Interact_Multicast_Implementation(ABigMoxiCharacter* InteractedCharacter)
{
	Interact(InteractedCharacter);
}

void ABigMoxiInteractable::Release(bool bReset)
{
	if (!bInteracted || !OwnerCharacter)
	{
		return;
	}

	bInteracted = false;
	OnRelease.Broadcast(OwnerCharacter);
	OwnerCharacter->OnEndPlay.RemoveDynamic(this, &ThisClass::OnOwnerEndPlay);

	if (HasAuthority())
	{
		RemoveGameplayEffect(OwnerCharacter, InteractEffect);
		ApplyGameplayEffect(OwnerCharacter, ReleaseEffect);

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::LatentBindOvelap);
	}

	OwnerCharacter = nullptr;
}

void ABigMoxiInteractable::Release_Server_Implementation(bool bReset)
{
	Release_Multicast(bReset);
}

void ABigMoxiInteractable::Release_Multicast_Implementation(bool bReset)
{
	Release(bReset);
}

void ABigMoxiInteractable::ApplyGameplayEffect(ABigMoxiCharacter* Target, TSubclassOf<UGameplayEffect> EffectClass)
{
	if (EffectClass)
	{
		if (UAbilitySystemComponent* abilitySystem = Target->GetAbilitySystemComponent())
		{
			if (abilitySystem->IsOwnerActorAuthoritative())
			{
				FGameplayEffectContextHandle context = abilitySystem->MakeEffectContext();
				UGameplayEffect* effect = InteractEffect->GetDefaultObject<UGameplayEffect>();
				abilitySystem->ApplyGameplayEffectToSelf(effect, 1.0f, context);
			}
		}
	}
}

void ABigMoxiInteractable::RemoveGameplayEffect(ABigMoxiCharacter* Target, TSubclassOf<UGameplayEffect> EffectClass)
{
	if (EffectClass)
	{
		if (UAbilitySystemComponent* abilitySystem = Target->GetAbilitySystemComponent())
		{
			if (abilitySystem->IsOwnerActorAuthoritative())
			{
				abilitySystem->RemoveActiveGameplayEffectBySourceEffect(InteractEffect, abilitySystem);
			}
		}
	}
}

void ABigMoxiInteractable::OnOwnerEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	if (Actor && HasAuthority() && (EndPlayReason != EEndPlayReason::EndPlayInEditor && EndPlayReason != EEndPlayReason::Quit))
	{
		Release_Server();
	}
}

void ABigMoxiInteractable::LatentBindOvelap()
{
	// Bind delegates.
	Collision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::BeginOverlap);
}
