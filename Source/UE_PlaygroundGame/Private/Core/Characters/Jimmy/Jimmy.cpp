// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Characters/Jimmy/Jimmy.h"

// Sets default values
AJimmy::AJimmy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	JimmySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("JimmySkeletalMesh"));
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	// Attach components
	JimmySkeletalMesh->SetupAttachment(RootComponent);
	WidgetComponent->SetupAttachment(JimmySkeletalMesh);
}

// Called when the game starts or when spawned
void AJimmy::BeginPlay()
{
	Super::BeginPlay();

	// Update JimmyText
	UpdateJimmyTextWidget();
}

#if WITH_EDITOR
void AJimmy::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	// Update JimmyText
	UpdateJimmyTextWidget();
}
#endif

// Cast to widget and if not null try and set the text
void AJimmy::UpdateJimmyTextWidget()
{
	UJimmyText* JimmyTextWidget = Cast<UJimmyText>(WidgetComponent->GetWidget());
	if (JimmyTextWidget)
	{
		JimmyTextWidget->SetJimmyText(JimmyText);
	}
}



