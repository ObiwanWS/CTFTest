// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <GameFramework/Actor.h>

#include "BigMoxiInteractable.generated.h"

class ABigMoxiCharacter;
class UGameplayEffect;
class USphereComponent;

// Delegate for sending character events.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDelegate, ABigMoxiCharacter*, Character);

UCLASS()
class ABigMoxiInteractable : public AActor
{
	GENERATED_BODY()

public:
	/** Called when an character starts interacting with this actor. */
	UPROPERTY(BlueprintAssignable)
	FCharacterDelegate OnInteract;

	/** Called when an character ends interacting with this actor. */
	UPROPERTY(BlueprintAssignable)
	FCharacterDelegate OnRelease;

	/** Sets default values for this actor's properties. */
	ABigMoxiInteractable();

	/** Called when an character starts interacting with this actor. */
	virtual void Interact(ABigMoxiCharacter* InteractedCharacter);
	UFUNCTION(Server, Reliable)
	void Interact_Server(ABigMoxiCharacter* InteractedCharacter);

	/** Called when an character ends interacting with this actor. */
	virtual void Release(bool bReset = false);
	UFUNCTION(Server, Reliable)
	void Release_Server(bool bReset = false);

	/**  */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	FORCEINLINE UStaticMeshComponent* GetMesh() const
	{
		return Mesh;
	}

	/**  */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BigMoxi)
	FORCEINLINE ABigMoxiCharacter* GetOwnerCharacter() const
	{
		return OwnerCharacter;
	}

protected:
	/** If Interact should be called once this actor overlaps a character. */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	uint8 bInteractOnBeginOverlap : 1;

	/** If Release should be called once this actor ends overlaping a character. */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	uint8 bReleaseOnEndOverlap : 1;

	/** Gameplay Effect to apply when interacted. */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	TSubclassOf<UGameplayEffect> InteractEffect;

	/** Gameplay Effect to apply when released. */
	UPROPERTY(EditDefaultsOnly, Category = BigMoxi)
	TSubclassOf<UGameplayEffect> ReleaseEffect;

	/**  */
	TObjectPtr<ABigMoxiCharacter> OwnerCharacter;

	//~ Begin AActor interface.
	void BeginPlay() override;
	//~ End AActor interface.

	/** Called this actor overlaps something. */
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called this actor ends overlaping something. */
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex);

	/** */
	UFUNCTION(NetMulticast, Reliable)
	void Interact_Multicast(ABigMoxiCharacter* InteractedCharacter);

	/** */
	UFUNCTION(NetMulticast, Reliable)
	void Release_Multicast(bool bReset = false);

	/** Apply a GE. */
	void ApplyGameplayEffect(ABigMoxiCharacter* Target, TSubclassOf<UGameplayEffect> EffectClass);

	/** Remove a GE. */
	void RemoveGameplayEffect(ABigMoxiCharacter* Target, TSubclassOf<UGameplayEffect> EffectClass);

private:
	/** Mesh component of this interactable. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Collision component of this interactable. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Collision;

	/** If this actor is currently being interacted with. */
	uint8 bInteracted : 1;

	/**  */
	UFUNCTION()
	void OnOwnerEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	/**  */
	void LatentBindOvelap();
};
