// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightCharacterMovementComponent.h"
#include "GymnastGameCharacter.h"
#include "Engine.h"
void UFlightCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	AGymnastGameCharacter* character = Cast<AGymnastGameCharacter>(GetCharacterOwner());
	FVector totalForces;
	FVector CurrentGravity = CustomGravity;
	if (Velocity.Z <= 0)
	{
		if (!HasReachedPeekHeight)
		{
			HasReachedPeekHeight = true;
			if(character)character->ReachedPeakHeight();
			//Velocity.X *= GravityFallScale;
		}
	}
	else
	{
		HasReachedPeekHeight = false;
	}
	// Simluate Simple Gravity With Drag
	totalForces += CurrentGravity;
	// Apply Custom Forces
	totalForces += ConstCustomForce;
	if (customForceToggle)
	{
		totalForces += InstantaneousCustomForce;
		InstantaneousCustomForce = FVector();
		customForceToggle = false;
	}

	totalForces.X *= PitchSteer;
	totalForces.Z *= PitchSteer;

	Velocity += totalForces * deltaTime;

	// Apply Drag
	Velocity *= CustomDrag;
	if(HasReachedPeekHeight)Velocity.Z = FMath::Min(Velocity.Z, GravityFallLimit);
	// Move Charecter
	FHitResult Hit;
	FVector CurrentActorLocation = GetActorLocation();
	bool success = SafeMoveUpdatedComponent((Velocity+FSteerForce)*deltaTime, UpdatedComponent->GetComponentRotation(), true, Hit);
	if (!success)
	{
		HasReachedPeekHeight = false;
		SetMovementMode(MOVE_Walking);
		if (character)
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
	HasReachedPeekHeight = false;
	GravityFallLimit = -200;
	PitchSteer = 1.0;
}

void UFlightCharacterMovementComponent::ApplyInstantaneousForce(const FVector newCustomFoce)
{
	InstantaneousCustomForce = newCustomFoce;
	customForceToggle = true;
}

void UFlightCharacterMovementComponent::SetSteerVelocity(FVector steerVelocity)
{
	FSteerForce = steerVelocity;
}
