// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightCharacterMovementComponent.h"
#include "GymnastGameCharacter.h"
#include "Engine.h"
void UFlightCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	AGymnastGameCharacter* character = Cast<AGymnastGameCharacter>(GetCharacterOwner());
	FVector totalForces;

	if (Velocity.Z <= 0)
	{
		if (!HasReachedPeekHeight)
		{
			HasReachedPeekHeight = true;
			if(character)character->ReachedPeakHeight();
		}
	}
	else
	{
		HasReachedPeekHeight = false;
	}
	// Simluate Simple Gravity With Drag
	totalForces += CustomGravity;
	// Apply Custom Forces
	totalForces += ConstCustomForce;
	if (customForceToggle)
	{
		totalForces += InstantaneousCustomForce;
		InstantaneousCustomForce = FVector();
		customForceToggle = false;
	}

	float pitchZSteerAdjust = FMath::Clamp<float>((PitchSteer * PitchSteerZAmount), -PitchSteerZAmount, 0);

	totalForces.X -= FMath::Clamp<float>((PitchSteer * PitchSteerXAmount), -PitchSteerXAmount, 0);
	totalForces.Z += pitchZSteerAdjust;

	Velocity += totalForces * deltaTime;
	GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, FString::Printf(TEXT("PitchSteer %f"), PitchSteer));
	GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, FString::Printf(TEXT("PitchSteerX %f"), PitchSteer * PitchSteerXAmount));
	GEngine->AddOnScreenDebugMessage(3, 0.5f, FColor::Red, FString::Printf(TEXT("PitchSteerY %f"), PitchSteer * PitchSteerZAmount));

	// Apply Drag
	Velocity *= CustomDrag;
	if(HasReachedPeekHeight)Velocity.Z = FMath::Max(Velocity.Z, (GravityFallLimit + pitchZSteerAdjust));
	
	Velocity.X = FMath::Clamp<float>(Velocity.X,0, TNumericLimits< float >::Max());
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
	PitchSteer = 0.0;
	PitchSteerZAmount = 1000;
	PitchSteerXAmount = 1000;
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
