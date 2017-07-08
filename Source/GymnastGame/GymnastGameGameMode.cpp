// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GymnastGameGameMode.h"
#include "GymnastGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGymnastGameGameMode::AGymnastGameGameMode()
{
	MaxLaunchVelocity = 500.0f;
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
#if PLATFORM_IOS
		FPlatformMisc::ControlScreensaver(FPlatformMisc::EScreenSaverAction::Disable);
#endif
	}
}
