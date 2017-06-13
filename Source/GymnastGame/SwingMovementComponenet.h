// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SwingMovementComponenet.generated.h"

/**
 * MovementComponent Extension to add Swing physics for Actors
 */
UCLASS()
class GYMNASTGAME_API USwingMovementComponenet : public UCharacterMovementComponent
{
	GENERATED_BODY()
private:
	FVector StartingActorUpVector;
	FVector RestingLocation;
	FVector StartingRadius;
	double swingDrag;
	double RadiusLength;
	FVector currentSwingVelocity;
	int32	bHasInput:1;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
    virtual void SwingPhys(float deltaTime,int32 Iterations);
    
public:
	USwingMovementComponenet();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Swing Physics")
    const AActor* swingCenter;

	UFUNCTION(BlueprintCallable,Category= "Swing")
	void AddSwingImpusle(const FVector velocicy);

	UFUNCTION(BlueprintCallable, Category = "Swing")
	void SetSwingCenterActor(const AActor *SwingActor);
};
