// Fill out your copyright notice in the Description page of Project Settings.
#include "SwingMovementComponent.h"
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
			double DotProduct = FVector::DotProduct(upVector,DirectionVector);
			FVector CrossProduct = FVector::CrossProduct(upVector, DirectionVector);
			FRotator rotator = UKismetMathLibrary::RotatorFromAxisAndAngle(CrossProduct, FMath::Acos(DotProduct));
			AEffectedActor->SetActorRotation(AEffectedActor->GetActorRotation()+rotator);
			//UKismetMathLibrary::FindLookAtRotation
		}
		else
		{
			// TODO: Setup Calculations to use a set radius for Actor positioning
		}
		if (LastActorLocationIsValid)
		{
			Velocity = (CurrentActorLocation-LastActorLocation) / DeltaTime;
		}
		else
		{
			LastActorLocationIsValid = true;
		}

		LastActorLocation = CurrentActorLocation;
	}

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
