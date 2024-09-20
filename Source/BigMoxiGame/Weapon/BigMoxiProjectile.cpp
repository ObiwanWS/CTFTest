// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiProjectile.h"

#include <AbilitySystemInterface.h>
#include <AbilitySystemComponent.h>
#include <GameFramework/Character.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <GameplayEffect.h>
#include <Components/SphereComponent.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiProjectile)

ABigMoxiProjectile::ABigMoxiProjectile() 
{
	// Actor defaults
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	InitialLifeSpan = 3.0f;

	// Network defaults
	bReplicates = true;
	SetReplicateMovement(false);
	bAlwaysRelevant = true;
	NetDormancy = ENetDormancy::DORM_Never;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapOnlyPawn");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetCanEverAffectNavigation(false);
	CollisionComp->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bSimulationEnabled = true;
	ProjectileMovement->bSimulationUseScopedMovement = true;
	ProjectileMovement->bForceSubStepping = true;
	ProjectileMovement->bInterpMovement = true;
	ProjectileMovement->bThrottleInterpolation = true;
	ProjectileMovement->bInterpolationUseScopedMovement = true;

	// Create mesh component.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetGenerateOverlapEvents(false);
	MeshComp->SetNotifyRigidBodyCollision(false);
	MeshComp->BodyInstance.SetCollisionProfileName("NoCollision");
	MeshComp->CanCharacterStepUpOn = ECB_No;
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetupAttachment(RootComponent);

	DamageEffect = nullptr;
	bDamageInstigator = false;

	// Bind delegates.
	CollisionComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
}

void ABigMoxiProjectile::PostNetInit()
{
	Super::PostNetInit();

	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->SetInterpolatedComponent(MeshComp);
}

void ABigMoxiProjectile::PostNetReceiveLocationAndRotation()
{
	Super::PostNetReceiveLocationAndRotation();

	ProjectileMovement->MoveInterpolationTarget(GetActorLocation(), GetActorRotation());
}

void ABigMoxiProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics and are authority
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics() && HasAuthority())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

void ABigMoxiProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* instigator = GetInstigator();

	if (!DamageEffect || !instigator)
	{
		return;
	}

	if (!bDamageInstigator && OtherActor == instigator)
	{
		return;
	}

	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		if (const IAbilitySystemInterface* otherAbilityInterface = Cast<IAbilitySystemInterface>(OtherActor))
		{
			if (const IAbilitySystemInterface* abilityInterface = Cast<IAbilitySystemInterface>(instigator))
			{
				if (UAbilitySystemComponent* abilitySystem = abilityInterface->GetAbilitySystemComponent())
				{
					if (abilitySystem->IsOwnerActorAuthoritative())
					{
						FGameplayEffectContextHandle context = abilitySystem->MakeEffectContext();
						UGameplayEffect* effect = DamageEffect->GetDefaultObject<UGameplayEffect>();
						abilitySystem->ApplyGameplayEffectToTarget(effect, otherAbilityInterface->GetAbilitySystemComponent(), 1.0f, context);
					}
				}
			}
		}

		Destroy();
	}
}
