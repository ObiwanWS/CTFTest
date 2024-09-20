// Copyright (c) 2024 Obiwan Medeiros.

#include "BigMoxiGameInstance.h"
#include "Online/BigMoxiOnlineSubsystem.h"
#include "UI/BigMoxiErrorMessageSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BigMoxiGameInstance)

void UBigMoxiGameInstance::Init()
{
	Super::Init();

	GEngine->OnNetworkFailure().AddUObject(this, &ThisClass::OnNetworkFailure);
	GEngine->OnTravelFailure().AddUObject(this, &ThisClass::OnTravelFailure);
}

void UBigMoxiGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage)
{
	HandleNetError(ErrorMessage);
}

void UBigMoxiGameInstance::OnTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorMessage)
{
	HandleNetError(ErrorMessage);
}

void UBigMoxiGameInstance::HandleNetError(const FString& ErrorMessage)
{
	UBigMoxiOnlineSubsystem* onlineSubsystem = GetSubsystem<UBigMoxiOnlineSubsystem>();
	if (onlineSubsystem)
	{
		onlineSubsystem->DestroySession();
	}

	ReturnToMainMenu();

	GetWorld()->GetSubsystem<UBigMoxiErrorMessageSubsystem>()->AddErrorMessage(ErrorMessage);
}
