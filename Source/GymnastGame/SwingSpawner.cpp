// Fill out your copyright notice in the Description page of Project Settings.

#include "SwingSpawner.h"


// Sets default values
ASwingSpawner::ASwingSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASwingSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwingSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor * ASwingSpawner::SpawnNextSwing(TSubclassOf<AActor> SwingClass, AActor * LastSwing)
{
	FVector newLocation;

	return SpawnSwing(newLocation,SwingClass);
}