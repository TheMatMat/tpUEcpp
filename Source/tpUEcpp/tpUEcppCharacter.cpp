// Copyright Epic Games, Inc. All Rights Reserved.

#include "tpUEcppCharacter.h"
#include "tpUEcppGameMode.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AtpUEcppCharacter

AtpUEcppCharacter::AtpUEcppCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//grabbed object handler
	GrabbedObjectHandler = CreateDefaultSubobject<USceneComponent>(TEXT("GrabbedObjectHandler"));
	GrabbedObjectHandler->SetupAttachment(RootComponent);

	LineTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("LineTraceStart"));
	LineTraceStart->SetupAttachment(RootComponent);

	//LifeManagement
	MaxLife = 100.f;
	MinLife = 0.f;
	Life = 100.f;

	//Speed
	WalkSpeed = 200.f;
	RunSpeed = 900.f;

	//PickUp
	PickedUpComponent = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AtpUEcppCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AtpUEcppCharacter::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AtpUEcppCharacter::OnStopRun);

	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AtpUEcppCharacter::PickUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AtpUEcppCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AtpUEcppCharacter::MoveRight);

	

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AtpUEcppCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AtpUEcppCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AtpUEcppCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AtpUEcppCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AtpUEcppCharacter::OnResetVR);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AtpUEcppCharacter::FireProjectile);
}


void AtpUEcppCharacter::OnResetVR()
{
	// If tpUEcpp is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in tpUEcpp.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AtpUEcppCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AtpUEcppCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AtpUEcppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AtpUEcppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AtpUEcppCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AtpUEcppCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AtpUEcppCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Jump Max Count
	JumpMaxCount = 2;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AtpUEcppCharacter::OnStartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AtpUEcppCharacter::OnStopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AtpUEcppCharacter::ApplyLifeChange(float LifeAmount)
{
	//Apply Life change and clamp between MaxLife:0 and MinLife:100
	Life = FMath::Clamp(Life += LifeAmount, MinLife, MaxLife);

	GLog->Log("Life: " + FString::FromInt(Life));

	if (Life <= 0)
	{
		//MakePlayerDie - Ragdolize
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll", true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AController* PlayerController = Cast<APlayerController>(GetController());
		AtpUEcppGameMode* GameMode = Cast<AtpUEcppGameMode>(GetWorld()->GetAuthGameMode());

		if (PlayerController != nullptr && GameMode != nullptr)
		{
			PlayerController->UnPossess();
			GameMode->OnPlayerDeath(PlayerController);
		}
		
	}
}

void AtpUEcppCharacter::PickUp()
{
	//Result of Hit
	FHitResult HitResult;
	bool HasHit = false;

	//Attachment Transform Rules - Start Hold
	const FAttachmentTransformRules MyRulesStartHold{ EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true};
	//Detachment Transform Rules - End Hold
	const FDetachmentTransformRules MyRulesEndHold{ EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, true };

	//Collision Response Container - Start Hold
	FCollisionResponseContainer RespContainerStartHold;
	RespContainerStartHold.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//Collision Response Container - Start Hold
	FCollisionResponseContainer RespContainerEndHold;
	RespContainerEndHold.SetResponse(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);

	//Trace Line
	FVector StartPoint = LineTraceStart->GetComponentLocation();
	FVector EndPoint = StartPoint + FollowCamera->GetForwardVector() * 500;

	FCollisionQueryParams TraceParams;

	//Hold or not
	if (PickedUpComponent == nullptr)
	{
		HasHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, TraceParams);

		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 3.0f);
	}
	else
	{
		PickedUpComponent->GetOwner()->DetachFromActor(MyRulesEndHold);
		PickedUpComponent->SetSimulatePhysics(true);
		PickedUpComponent->SetCollisionResponseToChannels(RespContainerStartHold);
		PickedUpComponent = nullptr;
	}
	
	//Hit or not
	if (HasHit && HitResult.GetActor()->IsRootComponentMovable())
	{
		HitResult.GetActor()->AttachToComponent(GrabbedObjectHandler, MyRulesStartHold);
		PickedUpComponent = HitResult.GetComponent();

		PickedUpComponent->SetSimulatePhysics(false);
		PickedUpComponent->SetCollisionResponseToChannels(RespContainerEndHold);
	}
}

void AtpUEcppCharacter::FireProjectile()
{
	// Camera position and rotation for aiming.
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	// Set Spawn in front of the camera
	PaintProjectileSpawnerOffset.Set(100.0f, 0.0f, 0.0f);
	FVector PaintProjectileSpawner = CameraLocation + FTransform(CameraRotation).TransformVector(PaintProjectileSpawnerOffset);

	// Skew the aim to be slightly upwards.
	FRotator PaintProjectileRotation = CameraRotation;
	PaintProjectileRotation.Pitch += 10.0f;

	// Spawn the projectile in the World at the Spawner.
	UWorld* World = GetWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();


	APaintProjectile* Projectile = World->SpawnActor<APaintProjectile>(APaintProjectile::StaticClass(), PaintProjectileSpawner, PaintProjectileRotation, SpawnParams);

}

