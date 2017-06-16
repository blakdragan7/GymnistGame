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
private:
	FVector InstantaneousCustomForce;
	bool customForceToggle;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

public:
	UFlightCharacterMovementComponent();

public:
	/* Custom Gravity Vector That is Applied Every Tick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
	FVector CustomGravity;
	/* Custom Drag float, Simply Multiplied by Velocity every frame */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
	float CustomDrag;
	/* Custom Force Vector for simulateing wind Or Something Similiar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics", meta = (AllowPrivateAcces = "true"))
	FVector ConstCustomForce;
	/* Applies a custom force, param newCustomForce, that is instantaneous, it's similiar to Add Impulse*/
	UFUNCTION(BlueprintCallable, Category = "Flight Physics")
	void ApplyInstantaneousForce(const FVector newCustomFoce);
};
