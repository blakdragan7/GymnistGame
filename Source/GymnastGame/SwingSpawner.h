// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingSpawner.generated.h"

UCLASS()
class GYMNASTGAME_API ASwingSpawner : public AActor
{
	GENERATED_BODY()
private:
	class ASwingActorBase* LastSwing;
	TArray<class ASwingActorBase*> AllSwings;
	
public:	
	// Sets default values for this actor's properties
	ASwingSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Closest Distance the next Swing will be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	float NearDistance;
	/* Farthest Distance the next Swing will be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	float FarDistance;
	/* Spawn Next Swing Actor Based on LastSwing */
	UFUNCTION(BlueprintCallable, Category = "Swing Spawn")
	class ASwingActorBase* SpawnNextSwing(TSubclassOf<class ASwingActorBase> SwingClass);
};
