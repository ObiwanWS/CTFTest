// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <GameFramework/Actor.h>
#include <GameplayTagContainer.h>

#include "BigMoxiProjectile.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ABigMoxiProjectile : public AActor
{
	GENERATED_BODY()

public:
	/** Default constructor for setting defaults. */
	ABigMoxiProjectile();

	//~ Begin AActor interface;
	void PostNetInit() override;
	void PostNetReceiveLocationAndRotation() override;
	//~ End AActor interface;

	/** Returns CollisionComp subobject. */
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject. */
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	/** Called when projectile hits something. */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Called when projectile overlaps something. */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	/** Sphere collision component. */
	UPROPERTY(VisibleDefaultsOnly, Category = BigMoxi)
	TObjectPtr<USphereComponent> CollisionComp;

	/** Projectile movement component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BigMoxi, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Sphere collision component. */
	UPROPERTY(VisibleDefaultsOnly,Category = BigMoxi)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	/** Gameplay Effect to apply to hit characters. */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	TSubclassOf<UGameplayEffect> DamageEffect;

	/** Gameplay Cue to execute when a character is hit. */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	uint8 bDamageInstigator : 1;
};

