// Fill out your copyright notice in the Description page of Project Settings.

#include "SwingMovementComponenet.h"

void USwingMovementComponenet::PhysCustom(float deltaTime, int32 Iterations)
{
    
}

void USwingMovementComponenet::SwingPhys(float deltaTime,int32 Iterations)
{
    FVector centerLocation = swingCenter->GetActorLocation();
    FVector myLocation = GetOwner()->GetActorLocation();
    
    float length = (myLocation - centerLocation).Size();
}
