// Copyright (c) 2024 Obiwan Medeiros.

#pragma once

#include <GameFramework/CharacterMovementComponent.h>

#include "BigMoxiCharacterMovement.generated.h"

/**
 * Extended UCharacterMovementComponent class.
 */
UCLASS(MinimalAPI, Within = BigMoxiCharacter)
class UBigMoxiCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	/** Called on the client before the character that owns this movement component has its location corrected by the server. */
	FSimpleDelegate OnClientLocationCorrected;

	/** Sets default values for this Movement Component. */
	UBigMoxiCharacterMovement();

	/** Sets the new acceleration value from the replicated value. */
	void SetReplicatedAcceleration(const FVector& ReplicatedAcceleration);

protected:
	//~ Begin UCharacterMovementComponent interface.
	void SimulateMovement(float DeltaTime) override;
	void OnClientCorrectionReceived(class FNetworkPredictionData_Client_Character& ClientData, float TimeStamp, FVector NewLocation, FVector NewVelocity, UPrimitiveComponent* NewBase, FName NewBaseBoneName, bool bHasBase, bool bBaseRelativePosition, uint8 ServerMovementMode, FVector ServerGravityDirection) override;
	//~ End UCharacterMovementComponent interface.

private:
	/** Flag that acceleration has been replicated and needs to be updated. */
	UPROPERTY(Transient)
	uint8 bHasReplicatedAcceleration : 1;
};
