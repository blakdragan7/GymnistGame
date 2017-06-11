// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SwingMovementComponenet.generated.h"

/**
 * 
 */
UCLASS()
class GYMNASTGAME_API USwingMovementComponenet : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
    virtual void SwingPhys(float deltaTime,int32 Iterations);
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Swing Physics")
    AActor* swingCenter;
};
