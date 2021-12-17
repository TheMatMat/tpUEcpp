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
	GLog->Log("InGameMode");

	RestartPlayerAtPlayerStart(PlayerController, ChoosePlayerStart(PlayerController));
}
