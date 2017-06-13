// Fill out your copyright notice in the Description page of Project Settings.

#include "SwingMovementComponenet.h"

USwingMovementComponenet::USwingMovementComponenet()
{
	bHasInput = false;
	swingDrag = 0;
}

void USwingMovementComponenet::PhysCustom(float deltaTime, int32 Iterations)
{
	SwingPhys(deltaTime, Iterations);
}

void USwingMovementComponenet::SwingPhys(float deltaTime,int32 Iterations)
{
	
	if (IsValid(swingCenter))
	{
		FVector myLocation = GetOwner()->GetActorLocation();
		FVector ActorCurrentUp = GetOwner()->GetActorUpVector();
		// Face Actor Up Vector To Swing Center
		FVector SwingCenterLocation = swingCenter->GetActorLocation();
		FVector radius = SwingCenterLocation - myLocation;

		FRotator newRotation = FRotationMatrix::MakeFromXZ(radius, ActorCurrentUp).Rotator();
		GetOwner()->SetActorRotation(newRotation);

		FVector ActorCurrentTangentVector = (RestingLocation - myLocation).GetSafeNormal();

		// Calculate Swing Velocity and Update Actor Position
		float dotProduct = FVector::DotProduct(StartingRadius.GetSafeNormal(), radius.GetSafeNormal());
		float angle = 0;
		if (dotProduct == 1.0)angle = 0;
		else angle = acos(dotProduct);
		angle = FMath::IsNearlyZero(angle,.001f) ? 0 : angle;
		float Tension = (Velocity.SizeSquared() / RadiusLength) + (980 * cos(angle));
		Velocity += radius.SafeNormal() * Tension * deltaTime;
		Velocity += (myLocation - SwingCenterLocation).GetSafeNormal() * 980 * cos(angle)*deltaTime;
		Velocity += ActorCurrentTangentVector * 980 * sin(angle)*deltaTime;

		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Yellow, ActorCurrentUp.ToString());
		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Yellow, ActorCurrentTangentVector.ToString());
		GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Yellow, FString::Printf(TEXT("%f"), dotProduct));
		GEngine->AddOnScreenDebugMessage(3, 0.5f, FColor::Yellow, FString::Printf(TEXT("%f"), angle));

		Velocity *= 0.99;
		if (bHasInput)
		{
			bHasInput = false;
			Velocity += currentSwingVelocity * deltaTime;
			currentSwingVelocity = FVector();
		}

		FVector newLocation = myLocation + Velocity;
		GetOwner()->SetActorLocation(newLocation);
	}
}

void USwingMovementComponenet::AddSwingImpusle(const FVector velocity)
{
	currentSwingVelocity += velocity;
	bHasInput = true;
}

void USwingMovementComponenet::SetSwingCenterActor(const AActor * SwingActor)
{
	check(SwingActor);
	swingCenter = SwingActor;

	FVector centerLocation = swingCenter->GetActorLocation();
	FVector myLocation = GetOwner()->GetActorLocation();

	StartingRadius = (centerLocation - myLocation);
	RadiusLength = StartingRadius.Size();
	// Face Actor Up Vector To Swing Center
	FRotator newRotation = FRotationMatrix::MakeFromX(StartingRadius).Rotator();
	GetOwner()->SetActorRotation(newRotation);
	
	StartingActorUpVector = GetOwner()->GetActorUpVector();
	RestingLocation = myLocation;
}
