// Fill out your copyright notice in the Description page of Project Settings.
#include "SwingMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Actor.h"
#include "GymnastGameGameMode.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine.h"
USwingMovementComponent::USwingMovementComponent()
{
	EnertiaMultiplier = 25;
}

void USwingMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UMovementComponent::TickComponent(DeltaTime,TickType,ThisTickFunction);
	if (IsValid(AEffectedActor))
	{
		FVector CurrentActorLocation = AEffectedActor->GetActorLocation();
		if (bUseSwingComponent)
		{
			AEffectedActor->SetActorLocation(USwingRadiusComponent->GetComponentLocation());
			AEffectedActor->SetActorRotation(USwingRadiusComponent->GetComponentRotation());
		}
		else
		{
			// TODO: Setup Calculations to use a set radius for Actor positioning
		}
		if (LastActorLocationIsValid)
		{
			Velocity = (CurrentActorLocation-LastActorLocation) / DeltaTime;
			DrawDebugDirectionalArrow(GetWorld(), CurrentActorLocation, CurrentActorLocation + (Velocity.GetSafeNormal() * 100.0),
				100.f, FColor::Red, false, -1.f, (uint8)'\000', 10.f);
		}
		else
		{
			LastActorLocationIsValid = true;
		}

		LastActorLocation = CurrentActorLocation;
	}
	// Apply Tanget Gravity force for Pendulum Motion
	if (IsValid(PhysicsComponent))
	{
		FRotator rotation = PhysicsComponent->GetComponentRotation();
		float TangentForce = FMath::Sin(FMath::DegreesToRadians(rotation.Pitch)) * 980 * DeltaTime;
		GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Red, FString::Printf(TEXT("TangentForce %f !"), TangentForce));

		if (TangentForce)
		{
			FVector AngVel(TangentForce, TangentForce, TangentForce);
			PhysicsComponent->SetPhysicsAngularVelocity(AngVel, true);

			if (PhysicsComponent->GetComponentVelocity().IsNearlyZero(5))bIsZeroVelocity = true;
			else bIsZeroVelocity = false;
		}
	}
}

void USwingMovementComponent::SetSwingRadius(float newRadius)
{
	bUseSwingComponent = false;
	fSwingRadius = newRadius;
}

void USwingMovementComponent::SetSwingRadiusComponent(USceneComponent* newRadius)
{
	check(newRadius);
	bUseSwingComponent = true;
	USwingRadiusComponent = newRadius;
}

void USwingMovementComponent::SetEffectedActor(AActor* EffectedActor,FVector ImpuleLocation)
{
	check(EffectedActor);
	this->AEffectedActor = EffectedActor;
	LastActorLocationIsValid = false;
	if (PhysicsComponent)
	{
		PhysicsComponent->AddImpulseAtLocation(EffectedActor->GetVelocity()*EnertiaMultiplier, ImpuleLocation);
	}
}

void USwingMovementComponent::SetPhysicsComponent(UPrimitiveComponent * newPhysicsComponent)
{
	PhysicsComponent = newPhysicsComponent;
}

void USwingMovementComponent::RemoveEffectedActor()
{
	AEffectedActor = 0;
	LastActorLocationIsValid = false;
}

bool USwingMovementComponent::GetAbsolutePitch(float& OutPitch)
{
	static const FVector ZUpVector = FVector(0, 0, 1);
	static const FVector YUpVector = FVector(1, 0, 0);
	if (IsValid(PhysicsComponent))
	{
		FVector CurrentUp = PhysicsComponent->GetUpVector();
		FRotator rotation = PhysicsComponent->GetComponentRotation();
		FVector ZCross = FVector::CrossProduct(CurrentUp, ZUpVector);
		FVector YCross = FVector::CrossProduct(CurrentUp, YUpVector);
		if (ZCross.Y > 0 && YCross.Y < 0)OutPitch = (90 - rotation.Pitch) + 90;
		else if (ZCross.Y < 0 && YCross.Y < 0)OutPitch = (-90 - rotation.Pitch) - 90;
		else OutPitch = rotation.Pitch;
		return true;
	}

	return false;
}

bool USwingMovementComponent::GetCurrentLaunchVelocity(FVector & launchVelocity)
{
	if (IsValid(AEffectedActor))
	{
		launchVelocity = Velocity;
		return true;
	}
	return false;
}

bool USwingMovementComponent::GetCurrentLaunchStrength(float& strength)
{
	if (IsValid(AEffectedActor))
	{
		strength = Velocity.Size();
		return true;
	}
	return false;
}

bool USwingMovementComponent::GetLaunchVelocity(FVector& launchVelocity,float scale)
{
	//if (IsValid(AEffectedActor))
	{
		AGymnastGameGameMode * GameMode = GetWorld()->GetAuthGameMode<AGymnastGameGameMode>();
		float maxLaunch = GameMode->MaxLaunchVelocity;
		float LaunchStrength = (Velocity * scale).Size();
		if (LaunchStrength > maxLaunch)
			launchVelocity = Velocity.GetSafeNormal()*maxLaunch;
		else
			launchVelocity = Velocity * scale;
		return true;
	}
	return false;
}
