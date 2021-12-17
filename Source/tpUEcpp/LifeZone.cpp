// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeZone.h"
#include "DrawDebugHelpers.h"
#include "tpUEcppCharacter.h"


ALifeZone::ALifeZone()
{
	PrimaryActorTick.bCanEverTick = true;

	//Charcater entering the zone
	CharacterInZone = nullptr;

	//LifeAmount given by the zone. Can be positive or negative
	LifeAmount = 10.0f;
}

void ALifeZone::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ALifeZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ALifeZone::OnOverlapEnd);
	
	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}

void ALifeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ALifeZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		/*GLog->Log("Overlap Start");*/
		AtpUEcppCharacter* CharacterOverlapping = Cast<AtpUEcppCharacter>(OtherActor);

		if (CharacterOverlapping != nullptr)
		{
			CharacterInZone = CharacterOverlapping;
			GetWorldTimerManager().SetTimer(ZoneTimer, this, &ALifeZone::ApplyLifeChange, 0.5f, true, -0.5f);
		}
	}
}

void ALifeZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		/*GLog->Log("Overlap End");*/

		AtpUEcppCharacter* CharacterOverlapping = Cast<AtpUEcppCharacter>(OtherActor);

		if (CharacterOverlapping != nullptr)
		{
			GetWorldTimerManager().ClearTimer(ZoneTimer);
		}
	}
}

void ALifeZone::ApplyLifeChange()
{
	//Apply a life change
	if (CharacterInZone != nullptr)
	{	
		//CharacterInZone est de class AtpUEcppCharacter est possède donc une fonction ApplyLifeChange
		CharacterInZone->ApplyLifeChange(LifeAmount);
	}
}
