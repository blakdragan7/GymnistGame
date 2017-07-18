// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GymnastGameGameMode.generated.h"

UCLASS(minimalapi)
class AGymnastGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGymnastGameGameMode();
	/* Max Launch Velocity allowed for Level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Settings")
	float MaxLaunchVelocity;
	/* Score Value for Current Level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Global")
	int32 GameScore;
	/* Used to add Score to GameScore */
	UFUNCTION(BlueprintCallable, Category = "Global")
	void AddScore(AActor* actor, int32 score);
	/* Used to Set Score to GameScore */
	UFUNCTION(BlueprintCallable, Category = "Global")
	void SetScore(AActor* actor, int32 score);
};



