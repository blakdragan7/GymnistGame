// Fill out your copyright notice in the Description page of Project Settings.

#include "SwingSpawner.h"
#include "SwingActorBase.h"
#include "Engine.h"

// Sets default values
ASwingSpawner::ASwingSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NearDistance = 500;
	FarDistance = 3000;
	LastSwing = 0;
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

ASwingActorBase* ASwingSpawner::SpawnNextSwing(TSubclassOf<ASwingActorBase> SwingClass)
{
	FVector newLocation;
	
	int32 newPoints = 0;
	if (LastSwing)
	{
		FVector lastLocation = LastSwing->GetActorLocation();
		int32 lastPoints = LastSwing->PointsWorth;

		newPoints = lastPoints * 1.2;
		if (newPoints == 0) newPoints = 100;
		newLocation = lastLocation + FVector(FMath::RandRange(NearDistance, FarDistance),0, 0);
	}

	FTransform transform;
	transform.SetLocation(newLocation);
	transform.SetRotation(FQuat());

	ASwingActorBase* actorBase = GetWorld()->SpawnActor<ASwingActorBase>(SwingClass, transform);
	actorBase->PointsWorth = newPoints;
	actorBase->Spawner = this;

	LastSwing = actorBase;

	AllSwings.Add(actorBase);

	if (AllSwings.Num() > 6)
	{
		ASwingActorBase* FirstSwing = AllSwings[0];
		AllSwings.Remove(FirstSwing);
		GetWorld()->DestroyActor(FirstSwing);
	}

	return actorBase;
}