// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFramework/Actor.h"
#include "SwingMovementComponent.h"

void USwingMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	UMovementComponent::TickComponent(DeltaTime,TickType,ThisTickFunction);
	if (IsValid(AEffectedActor))
	{
		if (bUseSwingComponent)
		{
			AEffectedActor->SetActorLocation(USwingRadiusComponent->GetComponentLocation());
		}
		else
		{
			// TODO: Setup Calculations to use a set radius for Actor positioning
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

void USwingMovementComponent::SetEffectedActor(AActor* EffectedActor)
{
	check(EffectedActor);
	this->AEffectedActor = EffectedActor;
}