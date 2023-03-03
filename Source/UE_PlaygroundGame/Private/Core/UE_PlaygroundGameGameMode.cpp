// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/UE_PlaygroundGameGameMode.h"
#include "Core/Characters/UE_PlaygroundGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_PlaygroundGameGameMode::AUE_PlaygroundGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/UE_PlaygroundGame/Core/Characters/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
