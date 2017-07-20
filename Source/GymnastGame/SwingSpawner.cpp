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

ASwingActorBase* ASwingSpawner::SpawnNextSwing(TSubclassOf<ASwingActorBase> SwingClass,ASwingActorBase* LastSwing)
{
	FVector newLocation;
	FRotator rotation(0,0,0);
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
	transform.SetRotation(rotation.Quaternion());

	ASwingActorBase* actorBase = GetWorld()->SpawnActor<ASwingActorBase>(SwingClass, transform);
	actorBase->PointsWorth = newPoints;
	actorBase->Spawner = this;
	return actorBase;
}