// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "SwingMovementComponent.generated.h"

/**
 * Movement Component that Implements Swing Physics to an Actor.
 * This Keeps Another Actor Rotated To and In line with the swing of the Owning Actor 
 * This component Only keeps track of The Last Velocity Given
 * and only does anything with SimulatePhysics Turned on
 */
UCLASS()
class GYMNASTGAME_API USwingMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
private:
	/* The Actor thats location will be set to the offset and rotation of this owners swing result */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	AActor* AEffectedActor;

	/* The radius of the Effected Actor is held At */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	float fSwingRadius;

	/* Same as SwingRadius Except the Components position is used instead of a float radius */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swing Physics", meta = (AllowPrivateAccess = "true"))
	USceneComponent* USwingRadiusComponent;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Swing Settings")
	void SetSwingRadius(float newRadius);

	UFUNCTION(BlueprintCallable, Category = "Swing Settings")
	void SetSwingRadiusComponent(USceneComponent* newSwingRadiusComponent);

	UFUNCTION(BlueprintCallable, Category = "Swing Settings")
	void SetEffectedActor(AActor* newEffectedActor);

	/* If True then Swing Radius Component will be used for updates and if false Swing Radius will be used instead */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Settings")
	uint32 bUseSwingComponent:1;
};
