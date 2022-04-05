// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGPROTGameMode.h"
#include "RPGPROTCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARPGPROTGameMode::ARPGPROTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
