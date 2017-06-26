// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightCharacterMovementComponent.h"
#include "GymnastGameCharacter.h"
void UFlightCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	// Simluate Simple Gravity With Drag
	Velocity += CustomGravity * deltaTime;
	// Apply Custom Forces
	Velocity += ConstCustomForce*deltaTime;
	if (customForceToggle)
	{
		Velocity += InstantaneousCustomForce*deltaTime;
		InstantaneousCustomForce = FVector();
		customForceToggle = false;
	}
	// Apply Drag
	Velocity *= CustomDrag;
	Velocity.Z = FSteerForce.Z;
	
	// Move Charecter
	FHitResult Hit;
	bool success = SafeMoveUpdatedComponent(Velocity*deltaTime, UpdatedComponent->GetComponentRotation(), true, Hit);
	if (!success)
	{
		SetMovementMode(MOVE_Walking);
		if (AGymnastGameCharacter* character = Cast<AGymnastGameCharacter>(GetCharacterOwner()))
		{
			character->HasLanded();
		}
	}
}

UFlightCharacterMovementComponent::UFlightCharacterMovementComponent()
{
	CustomGravity = FVector(0,0,-980);
	CustomDrag = 0.99;
	ConstCustomForce = FVector(0,0,0);
	InstantaneousCustomForce = FVector(0,0,0);
	customForceToggle = false;
}

void UFlightCharacterMovementComponent::ApplyInstantaneousForce(const FVector newCustomFoce)
{
	InstantaneousCustomForce = newCustomFoce;
	customForceToggle = true;
}

void UFlightCharacterMovementComponent::SetSteerVelocity(FVector steerVelocity)
{
	FSteerForce = Velocity;
}
