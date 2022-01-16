// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintProjectile.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


// Sets default values
APaintProjectile::APaintProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	//set the sphere component
	ProjectileRadius = 5.0f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(ProjectileRadius);

	RootComponent = SphereComponent;

	//set the projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 2500.0f;
	ProjectileMovementComponent->MaxSpeed = 2500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.1f;

	//set the sphere mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	if (SphereMeshAsset.Succeeded())
	{
		ProjectileMeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	}

	//sphere Material
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/ThirdPersonCPP/Materials/PaintProjectileMaterial'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);

	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	//collisions
	FCollisionResponseContainer RespContainer;
	RespContainer.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannels(RespContainer);

	//Material for decal
	UMaterial* SelectionMaterial;
	static ConstructorHelpers::FObjectFinder<UMaterial>MaterialInstanceObj(TEXT("Material'/Game/ThirdPersonCPP/Decal/PaintSplash.PaintSplash'"));

	if (MaterialInstanceObj.Succeeded())
	{
		SelectionMaterial = MaterialInstanceObj.Object;
		MaterialInstanceForDecal = UMaterialInstanceDynamic::Create(SelectionMaterial, NULL);
	}
}

// Called when the game starts or when spawned
void APaintProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentHit.AddDynamic(this, &APaintProjectile::OnHit);
}

// Called every frame
void APaintProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APaintProjectile::ShootProjectile(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void APaintProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	FRotator DecalRotation = Hit.Location.Rotation();
	FVector DecalLocation = Hit.Location;

	ADecalActor* PaintSplashDecal = GetWorld()->SpawnActor<ADecalActor>(DecalLocation, DecalRotation);
	PaintSplashDecal->SetActorRotation(DecalRotation);

	//set decal material and size
	FVector DecalSize(100.f, 100.f, 100.f);
	PaintSplashDecal->SetDecalMaterial(MaterialInstanceForDecal);
	PaintSplashDecal->GetDecal()->DecalSize = DecalSize;

	//set decal life span and fade out. life span is time before starting fade out + fade out duration
	PaintSplashDecal->GetDecal()->FadeStartDelay = 2.5f;
	PaintSplashDecal->GetDecal()->FadeDuration = 2.f;
	PaintSplashDecal->SetLifeSpan(PaintSplashDecal->GetDecal()->FadeStartDelay + PaintSplashDecal->GetDecal()->FadeDuration);

	//destroy the PaintProjectile Actor
	Destroy();
}

