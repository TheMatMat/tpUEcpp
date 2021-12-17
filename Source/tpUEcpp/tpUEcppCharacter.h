// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PaintProjectile.h"
#include "tpUEcppCharacter.generated.h"


UCLASS(config=Game)
class AtpUEcppCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AtpUEcppCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** PickUp*/
	UPROPERTY(VisibleAnywhere)
	class UPrimitiveComponent* PickedUpComponent;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* GrabbedObjectHandler;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* LineTraceStart;

	/** Player life. */
	UPROPERTY(EditAnywhere, Category= LifeManagement)
	float Life;

	UPROPERTY(EditAnywhere, Category = LifeManagement)
		float MaxLife;

	UPROPERTY(EditAnywhere, Category = LifeManagement)
		float MinLife;

	/** Player speed. */
	UPROPERTY(EditAnywhere, Category = Speed)
		float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Speed)
		float RunSpeed;

protected:

	void BeginPlay() override;

	UFUNCTION()
	void OnStartRun();
	
	UFUNCTION()
	void OnStopRun();

	UFUNCTION()
	void PickUp();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UPROPERTY(EditDefaultsOnly, Category = FireProjectile)
	TSubclassOf<APaintProjectile> PaintProjectileClass;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void ApplyLifeChange(float LifeAmount);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FireProjectile)
	FVector PaintProjectileSpawnerOffset;

	UFUNCTION()
	void FireProjectile();
};

