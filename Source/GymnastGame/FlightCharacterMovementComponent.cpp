// Fill out your copyright notice in the Description page of Project Settings.

#include "FlightCharacterMovementComponent.h"

void UFlightCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Yellow, TEXT("Custom Physics Happening"));
}