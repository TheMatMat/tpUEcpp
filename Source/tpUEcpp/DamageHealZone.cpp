// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageHealZone.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADamageHealZone::ADamageHealZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//OnActorBeginOverlap.AddDynamic(this, &ADamageHealZone::OnOverlapBegin);
	//OnActorEndOverlap.AddDynamic(this, &ADamageHealZone::OnOverlapEnd);
}



// Called when the game starts or when spawned
void ADamageHealZone::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADamageHealZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADamageHealZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ADamageHealZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

