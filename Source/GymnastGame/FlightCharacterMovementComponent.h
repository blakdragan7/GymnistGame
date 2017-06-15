// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FlightCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GYMNASTGAME_API UFlightCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	
	
};
