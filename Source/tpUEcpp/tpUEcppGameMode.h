// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "tpUEcppGameMode.generated.h"

UCLASS(minimalapi)
class AtpUEcppGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AtpUEcppGameMode();

	UFUNCTION()
	void OnPlayerDeath(AController* PlayerController);
};



