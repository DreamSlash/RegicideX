// Copyright Epic Games, Inc. All Rights Reserved.

#include "RegicideXGameMode.h"
#include "RegicideXCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARegicideXGameMode::ARegicideXGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
