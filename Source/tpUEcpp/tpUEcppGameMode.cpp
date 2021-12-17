// Copyright Epic Games, Inc. All Rights Reserved.

#include "tpUEcppGameMode.h"
#include "tpUEcppCharacter.h"
#include "UObject/ConstructorHelpers.h"

AtpUEcppGameMode::AtpUEcppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AtpUEcppGameMode::OnPlayerDeath(AController* PlayerController)
{
	PlayerControllerForRespawn = PlayerController;

	if (PlayerControllerForRespawn != nullptr)
	{
		PlayerControllerForRespawn->UnPossess();
		GetWorld()->GetTimerManager().SetTimer(RespawnWaitTimeHandle, this, &AtpUEcppGameMode::OnRespawnTimerEnd, 3.f, false);
	}
}

void AtpUEcppGameMode::OnRespawnTimerEnd()
{
	RestartPlayerAtPlayerStart(PlayerControllerForRespawn, ChoosePlayerStart(PlayerControllerForRespawn));
	PlayerControllerForRespawn = nullptr;
}
