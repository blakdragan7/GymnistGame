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
	/* Force ment for input such as tilting phone and moving in that direction */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight Physics", Meta = (AllowPrivateAccess = "true"))
	FVector FSteerForce;

	FVector InstantaneousCustomForce;
	bool customForceToggle;
	bool HasReachedPeekHeight;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

public:
	UFlightCharacterMovementComponent();

public:
	/** Controls Forward and Vertical motion through pitch*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
	float PitchSteer;
	/** Scale for Gravity After Peek Height is hit. Used to simulate Opening umbrella*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Physics")
	float GravityFallLimit;
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

	UFUNCTION()
	void SetSteerVelocity(FVector steerVelocity);

	
};
