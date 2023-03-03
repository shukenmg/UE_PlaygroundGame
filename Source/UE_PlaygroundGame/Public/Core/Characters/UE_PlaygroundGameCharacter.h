// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Engine/StaticMeshActor.h"
#include "InputActionValue.h"

#include "UE_PlaygroundGameCharacter.generated.h"


UCLASS(config = Game)
class AUE_PlaygroundGameCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AUE_PlaygroundGameCharacter();

public:
    virtual void Tick(float DeltaSeconds) override;
    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

private:
    // Gameplay
    void DoJumpAction();

    // Called for movement input
    void Move(const FInputActionValue& Value);

    // Called for looking input
    void Look(const FInputActionValue& Value);

    // Sprinting
    void Sprint(const FInputActionValue& Value);
    void StopSprinting(const FInputActionValue& Value);

    // Gliding
    void Glide();
    void StopGliding();

    // Interact/Grab
    void Grab();
    void UpdateGrab();
    void StopGrabbing();

    // Death and (Taxes) Life
    void PlayerDeath();
    void PlayerRespawn();

    // Event to detect if player overlapped a death volume
    UFUNCTION()
        void OnCasuleStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // To add mapping context
    virtual void BeginPlay();

public:
    // Compoenets

    // Camera boom positioning the camera behind the character
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* CameraBoom;

    // Follow camera
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* FollowCamera;

    // Glider mesh
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        class UStaticMeshComponent* GliderMesh;

    // Glider mesh
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        class UAudioComponent* AudioComponent;

    // Physics handle
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        class UPhysicsHandleComponent* PhysicsHandleComponent;

    // Input

    // MappingContext
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputMappingContext* DefaultMappingContext;

    // Jump Input Action
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* JumpAction;

    // Move Input Action
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* MoveAction;

    // Look Input Action
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* LookAction;

    // Sprint Input Action
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* SprintAction;

    // Interact Input Action
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* InteractAction;

    // Gameplay

    //sprint Speed
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
        float SprintSpeed = 1000;

private:
    // Game states
    UPROPERTY()
        bool IsSprinting = false;

    UPROPERTY()
        bool IsGliding = false;

    UPROPERTY()
        bool IsGrabbing = false;

    UPROPERTY()
        bool IsDead = false;

    UPROPERTY()
        float BackupMaxWalkSpeed = 0;

    UPROPERTY()
        FVector CheckpointLocation = FVector::ZeroVector;

    UPROPERTY()
        FTimerHandle TH_PlayerRespawn;
};

