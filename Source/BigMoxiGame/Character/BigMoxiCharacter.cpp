// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiCharacter.h"
#include "Abilities/BigMoxiAbilitySystemComponent.h"
#include "BigMoxiCharacterMovement.h"
#include "Player/BigMoxiPlayerState.h"
#include "Weapon/BigMoxiWeaponComponent.h"

#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <InputActionValue.h>
#include <Net/Core/PushModel/PushModel.h>
#include <Net/UnrealNetwork.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiCharacter)

ABigMoxiCharacter::ABigMoxiCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBigMoxiCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Store the ExtendedCharacterMovement.
	BigMoxiCharacterMovementComponent = GetCharacterMovement<UBigMoxiCharacterMovement>();

	// Bind the location correction delegate.
	BigMoxiCharacterMovementComponent->OnClientLocationCorrected.BindUObject(this, &ThisClass::OnClientLocationCorrected);

	// Sets default values for this character's properties.
	bReplicateAcceleration = false;
	ReplicatedAcceleration = FReplicatedAcceleration();
	AbilitySystem = nullptr;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(30.f, 0.f, 60.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(CameraComponent);
	FirstPersonMesh->bSelfShadowOnly = true;
	FirstPersonMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ABigMoxiCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams sharedParams;
	sharedParams.bIsPushBased = true;
	sharedParams.Condition = COND_SimulatedOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ReplicatedAcceleration, sharedParams);
}

void ABigMoxiCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	if (bReplicateAcceleration)
	{
		// Compress Acceleration: XY components as direction + magnitude, Z component as direct value.
		const float maxAccel = BigMoxiCharacterMovementComponent->MaxAcceleration;
		const FVector currentAccel = BigMoxiCharacterMovementComponent->GetCurrentAcceleration();
		double accelXYRadians, accelXYMagnitude;
		FMath::CartesianToPolar(currentAccel.X, currentAccel.Y, accelXYMagnitude, accelXYRadians);

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, ReplicatedAcceleration, this);

		// int64 to int8 is unsafe type cast and could trigger warnings/errors depending on build settings.
		ReplicatedAcceleration.AccelXYRadians = FMath::FloorToInt((accelXYRadians / TWO_PI) * 255.0); // [0, 2PI] -> [0, 255]
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((accelXYMagnitude / maxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
		ReplicatedAcceleration.AccelZ = FMath::FloorToInt((currentAccel.Z / maxAccel) * 127.0); // [-MaxAccel, MaxAccel] -> [-127, 127]
	}

}

void ABigMoxiCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystem == nullptr)
	{
		if (ABigMoxiPlayerState* playerState = GetPlayerState<ABigMoxiPlayerState>())
		{
			AbilitySystem = Cast<UBigMoxiAbilitySystemComponent>(playerState->GetAbilitySystemComponent());
			AbilitySystem->InitAbilityActorInfo(playerState, this);
		}
	}
}

void ABigMoxiCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AbilitySystem == nullptr)
	{
		if (ABigMoxiPlayerState* playerState = GetPlayerState<ABigMoxiPlayerState>())
		{
			AbilitySystem = Cast<UBigMoxiAbilitySystemComponent>(playerState->GetAbilitySystemComponent());
			AbilitySystem->InitAbilityActorInfo(playerState, this);
		}
	}
	else
	{
		AbilitySystem->RefreshAbilityActorInfo();
	}

	OnPlayerStateRepDelegate.Broadcast(GetPlayerState());
}

void ABigMoxiCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (ABigMoxiPlayerState* playerState = GetPlayerState<ABigMoxiPlayerState>())
	{
		playerState->GetAbilitySystemComponent()->RefreshAbilityActorInfo();
	}
}

UAbilitySystemComponent* ABigMoxiCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem.Get();
}

void ABigMoxiCharacter::OnClientLocationCorrected()
{
	// Set timer for next tick since in this tick the client has not been corrected yet.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CheckImpossibleJumpState);
}

void ABigMoxiCharacter::CheckImpossibleJumpState()
{
	// This is a fix for a bug where the character gets corrected to the floor after a jump and the jump state isn't properly handled on the client, making so the client can't jump again.
 	if ((GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking || GetCharacterMovement()->MovementMode == EMovementMode::MOVE_NavWalking) && JumpCurrentCount > 0)
	{
		ResetJumpState();
	}
}

void ABigMoxiCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABigMoxiCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABigMoxiCharacter::OnRep_ReplicatedAcceleration()
{
	// Decompress Acceleration.
	const double accelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * BigMoxiCharacterMovementComponent->MaxAcceleration / 255.0; // [0, 255] -> [0, MaxAccel]
	const double accelXYRadians = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0; // [0, 255] -> [0, 2PI]
	FVector unpackedAcceleration(FVector::ZeroVector);
	FMath::PolarToCartesian(accelXYMagnitude, accelXYRadians, unpackedAcceleration.X, unpackedAcceleration.Y);
	unpackedAcceleration.Z = ReplicatedAcceleration.AccelZ * BigMoxiCharacterMovementComponent->MaxAcceleration / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]

	// Set new decompress acceleration.
	BigMoxiCharacterMovementComponent->SetReplicatedAcceleration(unpackedAcceleration);
}

void ABigMoxiCharacter::Death()
{
	if (HasAuthority())
	{
		Destroy();
	}
}
