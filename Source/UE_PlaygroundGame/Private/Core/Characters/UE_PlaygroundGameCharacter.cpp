// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/Characters/UE_PlaygroundGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AUE_PlaygroundGameCharacter

AUE_PlaygroundGameCharacter::AUE_PlaygroundGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a Glider mesh
	GliderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GliderMesh"));
	GliderMesh->SetVisibility(false, true);
	GliderMesh->SetupAttachment(RootComponent);

	// Create a AudioComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	// Create PhysicsHandleComponent
	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
}

void AUE_PlaygroundGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Backup Variables
	BackupMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// Set CheckpointLocation
	CheckpointLocation = GetActorLocation();

	// Bind Events
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AUE_PlaygroundGameCharacter::OnCasuleStartOverlap);
}

void AUE_PlaygroundGameCharacter::Tick(float DeltaSeconds)
{
	// Call the base class  
	Super::Tick(DeltaSeconds);

	// Updates grab target
	UpdateGrab();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUE_PlaygroundGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUE_PlaygroundGameCharacter::DoJumpAction);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE_PlaygroundGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE_PlaygroundGameCharacter::Look);

		//Looking
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AUE_PlaygroundGameCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUE_PlaygroundGameCharacter::StopSprinting);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AUE_PlaygroundGameCharacter::Grab);
	}

}

void AUE_PlaygroundGameCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	// Call the base class  
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// Stop gliding on landed
	if (PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		// Only stop if was gliding
		if (IsGliding)
		{
			StopGliding();
		}

		// Play SFX
		AudioComponent->SetTriggerParameter("Land");
	}
}

void AUE_PlaygroundGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUE_PlaygroundGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUE_PlaygroundGameCharacter::Sprint(const FInputActionValue& Value)
{
	// Set MaxSpeed
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	IsSprinting = true;
}

void AUE_PlaygroundGameCharacter::StopSprinting(const FInputActionValue& Value)
{
	// Set MaxSpeed
	GetCharacterMovement()->MaxWalkSpeed = BackupMaxWalkSpeed;

	IsSprinting = false;
}

void AUE_PlaygroundGameCharacter::DoJumpAction()
{
	// If gliding stop it
	if (IsGliding) 
	{
		StopGliding();
		return;
	}

	// Character height check
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = true;
	GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() - GetActorUpVector() * 350, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(30));

	if (HitResult.bBlockingHit) // If not in air, Jump
	{
		Jump();

		// Play SFX
		AudioComponent->SetTriggerParameter("Jump");
	}
	else
	{
		Glide();
	}
}

void AUE_PlaygroundGameCharacter::Glide()
{
	// Show Glider
	GliderMesh->SetVisibility(true, true);

	// Change movement
	GetCharacterMovement()->GravityScale = 0.3;
	GetCharacterMovement()->MaxAcceleration = 1000;
	GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity / 5;

	// Play SFX
	AudioComponent->SetTriggerParameter("OpenGlider");

	IsGliding = true;
}

void AUE_PlaygroundGameCharacter::StopGliding()
{
	// Hide Glider
	GliderMesh->SetVisibility(false, true);

	// Change movement
	GetCharacterMovement()->GravityScale = 1.75;
	GetCharacterMovement()->MaxAcceleration = 2048;

	// Play SFX
	AudioComponent->SetTriggerParameter("CloseGlider");

	IsGliding = false;
}

void AUE_PlaygroundGameCharacter::Grab()
{
	// Release if grabbing
	if (IsGrabbing)
	{
		StopGrabbing();
		return;
	}

	// Query for grabbable objects
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = true;
	GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation() - GetActorUpVector() * 10, GetActorLocation() + GetActorForwardVector() * 150, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(10));

	// Nothing found
	if (!HitResult.bBlockingHit || !HitResult.GetComponent())
	{
		return;
	}

	// Only detect static meshes
	AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(HitResult.GetActor());
	if (StaticMeshActor)
	{
		if (StaticMeshActor->GetStaticMeshComponent() && StaticMeshActor->GetStaticMeshComponent()->IsSimulatingPhysics())
		{
			// Grab and contrain rotation
			PhysicsHandleComponent->GrabComponentAtLocationWithRotation(StaticMeshActor->GetStaticMeshComponent(), NAME_None, 
				StaticMeshActor->GetActorLocation(), StaticMeshActor->GetActorRotation());

			IsGrabbing = true;
		}
	}
}

void AUE_PlaygroundGameCharacter::StopGrabbing()
{
	PhysicsHandleComponent->ReleaseComponent();

	IsGrabbing = false;
}

// If grabbing, the grab target will be located in front of the player and the rotation is from the Control Rotation
void AUE_PlaygroundGameCharacter::UpdateGrab()
{
	if (IsGrabbing)
	{
		FVector TargetLocation = GetActorLocation() + (GetActorUpVector() * 200) + (UKismetMathLibrary::GetForwardVector(Controller->GetControlRotation()) * 250);
		PhysicsHandleComponent->SetTargetLocationAndRotation(TargetLocation, FRotator(0, Controller->GetControlRotation().Yaw, 0));
	}
}

// Game System
void AUE_PlaygroundGameCharacter::PlayerDeath()
{
	// If already dead dont do anything
	if (IsDead)
	{
		return;
	}

    // Detach camera from player
	FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	// Wait a bit before respawning
	GetWorld()->GetTimerManager().SetTimer(TH_PlayerRespawn, this, &AUE_PlaygroundGameCharacter::PlayerRespawn, 1, false);

	// Play SFX
	AudioComponent->SetTriggerParameter("Death");

	IsDead = true;
}

void AUE_PlaygroundGameCharacter::PlayerRespawn()
{
	// Stop all movement then set location
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->ClearAccumulatedForces();
	SetActorLocation(CheckpointLocation);

	// Reattach camera
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	IsDead = false;
}

void AUE_PlaygroundGameCharacter::OnCasuleStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Death") && !IsDead)
	{
		PlayerDeath();
	}
}


