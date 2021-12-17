// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaZone.h"
#include "DrawDebugHelpers.h"
#include "tpUEcppCharacter.h"

ALavaZone::ALavaZone()
{
	PrimaryActorTick.bCanEverTick = true;

	//Charcater entering the zone
	CharacterInZone = nullptr;
}

void ALavaZone::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ALavaZone::OnOverlapBegin);

	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
}

void ALavaZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ALavaZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		/*GLog->Log("Overlap Start");*/
		AtpUEcppCharacter* CharacterOverlapping = Cast<AtpUEcppCharacter>(OtherActor);

		if (CharacterOverlapping != nullptr)
		{
			CharacterInZone = CharacterOverlapping;
			ALavaZone::ApplyLifeChange();
		}
	}
}

void ALavaZone::ApplyLifeChange()
{
	//Apply a life change
	if (CharacterInZone != nullptr)
	{
		//CharacterInZone est de class AtpUEcppCharacter est possède donc une fonction ApplyLifeChange
		CharacterInZone->ApplyLifeChange(-CharacterInZone->MaxLife);
	}
}