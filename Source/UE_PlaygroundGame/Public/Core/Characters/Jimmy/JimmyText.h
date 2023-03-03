// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JimmyText.generated.h"


UCLASS()
class UE_PLAYGROUNDGAME_API UJimmyText : public UUserWidget
{
    GENERATED_BODY()

public:
    // For use in BP to set text
    UFUNCTION(BlueprintImplementableEvent)
        void SetJimmyText(const FText& NewText);
};
