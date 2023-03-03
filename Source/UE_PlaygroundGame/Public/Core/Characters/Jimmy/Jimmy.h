// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"

#include "Core/Characters/Jimmy/JimmyText.h"

#include "Jimmy.generated.h"

UCLASS()
class UE_PLAYGROUNDGAME_API AJimmy : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AJimmy();



protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // To change the text dynamically
#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif

private:
    void UpdateJimmyTextWidget();

public:
    // Components
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        USkeletalMeshComponent* JimmySkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        UWidgetComponent* WidgetComponent;

    // Config
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
        FText JimmyText;
};
