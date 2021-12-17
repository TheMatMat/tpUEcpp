// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstActor.h"


// Sets default values
//constructeur
AMyFirstActor::AMyFirstActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	LetterCount = 10;
}

// Called when the game starts or when spawned
void AMyFirstActor::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent->OnComponentHit.AddDynamic(this, &AMyFirstActor::OnHit);

}

// Called every frame
void AMyFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GLog->Log("Tick");
}

void AMyFirstActor::PrintLetterCount() 
{
	GLog->Log("LetterCount: " + FString::FromInt(LetterCount));
}

void AMyFirstActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GLog->Log("Hit");

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);

	if (OtherCharacter != nullptr)
	{
		OtherCharacter->LaunchCharacter(FVector(0, 0, 1000), false, true);
	}
}

