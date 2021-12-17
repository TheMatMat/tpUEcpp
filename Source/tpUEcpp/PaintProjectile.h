// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaintProjectile.generated.h"

UCLASS()
class TPUECPP_API APaintProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaintProjectile();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere Comp and its radius
	UPROPERTY(EditAnywhere)
		float ProjectileRadius;

	UPROPERTY()
	class USphereComponent* SphereComponent;

	// Projectile Movement Component
	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	// Projectile mesh
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ProjectileMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaintSplash")
	UMaterialInstance* MaterialInstanceForDecal;

	// Function that initializes the projectile's velocity in the shoot direction.
	void ShootProjectile(const FVector& ShootDirection);
};
