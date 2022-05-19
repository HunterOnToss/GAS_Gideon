// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_GideonGameMode.h"
#include "GAS_GideonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_GideonGameMode::AGAS_GideonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
