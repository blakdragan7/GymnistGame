// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GymnastGameGameMode.h"
#include "GymnastGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGymnastGameGameMode::AGymnastGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
