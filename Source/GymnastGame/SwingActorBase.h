// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingActorBase.generated.h"

class USwingMovementComponent;
class AGymnastGameCharacter;
class UArrowComponent;
class UBoxComponent;
class ASwingSpawner;

UCLASS()
class GYMNASTGAME_API ASwingActorBase : public AActor
{
	GENERATED_BODY()
	
private:
	/* Components */
	UPROPERTY(Category = Swing, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USwingMovementComponent* SwingMovement;
	/* Location Actor is held and rotate with */
	UPROPERTY(Category = Swing, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ActorBindLocation;
	/* Location Force Is Applied to simulate swinging */
	UPROPERTY(Category = Swing, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* PhysicsHitLocation;
	/* Main Swing Mesh */
	UPROPERTY(Category = Swing, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MainSwing;
	/* Arrow that Points towards the front */
	UPROPERTY(Category = Swing, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* FrontFacingArrow;
	/* Collision Box Used for Attaching the Player to the Component. Should be resized in Blueprints to match the static mesh */
	UPROPERTY(Category = Swing, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttachmentCollisionBox;
	/* Collision Box used to determine When the helper force should be applied */
	UPROPERTY(Category = Swing, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HelperForceCollisionBox;

public:	
	// Sets default values for this actor's properties
	ASwingActorBase();
	
	/* Overlap Attachment Collision Box began */
	UFUNCTION()
	void HitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/* Overlap Helper Force Collision Box began */
	UFUNCTION()
	void HelperForceOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/* Called When Overlap Ended */
	UFUNCTION()
	void HelperForceOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/* Class to spawn of next Swing if using a SwingSpawner */
	UPROPERTY(Category = "SwingSpawning", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASwingActorBase> SwingSpawnClass;
	/* Spawner that Spawned this Swing or NULL if not spawned by a SwingSpawner */
	UPROPERTY(Category = "SwingSpawning", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ASwingSpawner* Spawner;
	/* Player Starting Rotation When Attached to Swing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Settings")
	FRotator StartingPlayerRotation;
	/* Player that is current attached to this swing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Settings")
	AGymnastGameCharacter* SwingingPlayer;
	/* How many points the player earns from landing on this swing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 PointsWorth;
	/* Wether or not  needs new starting velocity can be set*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Settings")
	bool bCanSetNeedsStartingLocation;
	/* Amount o Multiply Player Velocity By When Launched */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch Settings")
	float LaunchVelocityMultiplier;

	UFUNCTION(BlueprintCallable, Category = "Launch Actions")
	void LaunchActor();
};
