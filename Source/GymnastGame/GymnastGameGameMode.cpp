// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GymnastGameGameMode.h"
#include "GymnastGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"

AGymnastGameGameMode::AGymnastGameGameMode()
{
	GameScore = 0;
	MaxLaunchVelocity = 500.0f;
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	{
		//DefaultPawnClass = PlayerPawnBPClass.Class;
#if PLATFORM_IOS
		FPlatformMisc::ControlScreensaver(FPlatformMisc::EScreenSaverAction::Disable);
#endif
	}
}

void AGymnastGameGameMode::AddScore(AActor * actor, int32 score)
{
	GameScore += score;
	GEngine->AddOnScreenDebugMessage(10, 0.5f, FColor::Red, FString::Printf(TEXT("New Score %f"), GameScore));
}

void AGymnastGameGameMode::SetScore(AActor * actor, int32 score)
{
	GameScore = score;
	GEngine->AddOnScreenDebugMessage(10, 0.5f, FColor::Red, FString::Printf(TEXT("New Score %f"), GameScore));
}
