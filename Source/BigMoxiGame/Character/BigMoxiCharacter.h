// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <AbilitySystemInterface.h>
#include <GameFramework/Character.h>

#include "BigMoxiCharacter.generated.h"

class UBigMoxiAbilitySystemComponent;
class UBigMoxiCharacterMovement;
class UCameraComponent;
struct FInputActionValue;

/**
 * Compressed representation of acceleration.
 */
USTRUCT()
struct FReplicatedAcceleration
{
	GENERATED_BODY()

	/** Direction of XY accel component, quantized to represent[0, 2 * pi]. */
	UPROPERTY()
	uint8 AccelXYRadians;

	/** Accel rate of XY component, quantized to represent [0, MaxAcceleration]. */
	UPROPERTY()
	uint8 AccelXYMagnitude;

	/** Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]. */
	UPROPERTY()
	int8 AccelZ;

	FReplicatedAcceleration()
	{
		AccelXYRadians = 0U;
		AccelXYMagnitude = 0U;
		AccelZ = 0U;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRepPlayerState, APlayerState*, PlayerState);

UCLASS()
class ABigMoxiCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	/** Whether to replicate acceleration to simulated proxies. */
	UPROPERTY(Category = BigMoxi, EditAnywhere, BlueprintReadOnly)
	uint8 bReplicateAcceleration : 1;

	/** Constructor used to allow overrides of base classes. */
	ABigMoxiCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin UObject interface.
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End UObject interface.

	//~ Begin AActor interface.
	void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	//~ End AActor interface.

	//~ Begin APawn interface.
	void PossessedBy(AController* NewController) override;
	void OnRep_PlayerState() override;
	void OnRep_Controller() override;
	//~ End APawn interface.

	/** Returns FirstPersonMesh subobject. **/
	UFUNCTION(BlueprintCallable, Category = "BigMoxi|Character")
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const 
	{ 
		return FirstPersonMesh; 
	}

	/** Returns CameraComponent subobject. **/
	UFUNCTION(BlueprintCallable, Category = "BigMoxi|Character")
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const 
	{ 
		return CameraComponent; 
	}

	/** Returns the UBigMoxiCharacterMovement subobject. */
	UFUNCTION(BlueprintCallable, Category = "BigMoxi|Character")
	FORCEINLINE UBigMoxiCharacterMovement* GetBigMoxiCharacterMovementComponent() const
	{
		return BigMoxiCharacterMovementComponent;
	}

	/** Returns this character's ASC. */
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**  */
	void Death();

	/**  */
	UPROPERTY(BlueprintAssignable)
	FOnRepPlayerState OnPlayerStateRepDelegate;

protected:
	/** Compressed acceleration that will be replicated on PreReplication. */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedAcceleration)
	FReplicatedAcceleration ReplicatedAcceleration;

	//~ Begin AActor interface.
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	//~ End AActor interface.

	/** Called before the character gets corrected on the client by the server. */
	void OnClientLocationCorrected();

	/** This will check if the character is in movement mode MODE_Walking and has JumpCurrentCount > 0, which should never happen, then fix it. */
	void CheckImpossibleJumpState();

	/** Called for movement input */
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION()
	void Look(const FInputActionValue& Value);

private:
	/** ExtendedCharacterMovement component used in this character. */
	TObjectPtr<UBigMoxiCharacterMovement> BigMoxiCharacterMovementComponent;

	/** Ability system weak pointer from the player state. */
	TWeakObjectPtr<UBigMoxiAbilitySystemComponent> AbilitySystem;

	/** Pawn mesh's first person view (arms; seen only by self). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BigMoxi, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BigMoxi, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	/** Decompresses the acceleration on simulated proxies and sends the data to the CharacterMovementComponent. */
	UFUNCTION()
	void OnRep_ReplicatedAcceleration();
};
