// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LifeZone.generated.h"


UCLASS()
class TPUECPP_API ALifeZone : public ATriggerBox
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float LifeAmount;

	UPROPERTY()
	class AtpUEcppCharacter* CharacterInZone;

	UPROPERTY()
	FTimerHandle ZoneTimer;

	public:

		ALifeZone();

		UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

		UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

		UFUNCTION()
		void ApplyLifeChange();
		

	protected:

		virtual void BeginPlay() override;

	public:

		virtual void Tick(float DeltaTime) override;

};
