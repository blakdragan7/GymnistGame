// Fill out your copyright notice in the Description page of Project Settings.
#include "SwingMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

void USwingMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UMovementComponent::TickComponent(DeltaTime,TickType,ThisTickFunction);
	if (IsValid(AEffectedActor))
	{
		FVector CurrentActorLocation = AEffectedActor->GetActorLocation();
		if (bUseSwingComponent)
		{
			AEffectedActor->SetActorLocation(USwingRadiusComponent->GetComponentLocation());
			FVector upVector = AEffectedActor->GetActorUpVector();
			FVector DirectionVector = (GetOwner()->GetActorLocation() - CurrentActorLocation).GetSafeNormal();
			fCurrentDotProduct = FVector::DotProduct(upVector,DirectionVector);
			FVector CrossProduct = FVector::CrossProduct(upVector, DirectionVector);
			fCurrnetAngleFromRestingPoint = FMath::Acos(fCurrentDotProduct);
			FRotator rotator = UKismetMathLibrary::RotatorFromAxisAndAngle(CrossProduct, fCurrnetAngleFromRestingPoint);
			AEffectedActor->SetActorRotation(AEffectedActor->GetActorRotation()+rotator);
			// TODO: Implement Better Rotation To Center point and Reset Rotatiation after Launch
			//UKismetMathLibrary::FindLookAtRotation
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
		FVector AngVel(TangentForce, TangentForce, TangentForce);
		PhysicsComponent->SetPhysicsAngularVelocity(AngVel,true);
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

void USwingMovementComponent::SetEffectedActor(AActor* EffectedActor)
{
	check(EffectedActor);
	this->AEffectedActor = EffectedActor;
	LastActorLocationIsValid = false;
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
