// Fill out your copyright notice in the Description page of Project Settings.
#include "SwingActorBase.h"
#include "GymnastGameCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GymnastGameGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SwingMovementComponent.h"
#include "SwingSpawner.h"


// Sets default values
ASwingActorBase::ASwingActorBase()
{
	AlreadyHadActor = false;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Componenets
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneComponent;

	FrontFacingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FrontFacingArrow"));
	FrontFacingArrow->SetupAttachment(RootComponent);
	HelperForceCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HelperForceCollisionBox"));
	HelperForceCollisionBox->SetupAttachment(RootComponent);
	MainSwing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainSwing"));
	MainSwing->SetupAttachment(RootComponent);

	ActorBindLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ActorBindLocation"));
	ActorBindLocation->SetupAttachment(MainSwing);
	PhysicsHitLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PhysicsHitLocation"));
	PhysicsHitLocation->SetupAttachment(MainSwing);
	AttachmentCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttachmentCollisionBox"));
	AttachmentCollisionBox->SetupAttachment(MainSwing);

	SwingMovement = CreateDefaultSubobject<USwingMovementComponent>(TEXT("SwingMovement"));
	/* Setup Events */
	AttachmentCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASwingActorBase::HitBoxOverlapBegin);
	HelperForceCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASwingActorBase::HelperForceOverlapBegin);
	HelperForceCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASwingActorBase::HelperForceOverlapEnd);

	SwingMovement->SetPhysicsComponent(MainSwing);

	// Init All Variables
	PointsWorth = 0;
	LaunchVelocityMultiplier = 1.0;
	SwingingPlayer = 0;
	bCanSetNeedsStartingLocation = true;
	Spawner = 0;

	SwingSpawnClass = StaticClass();
}

void ASwingActorBase::HitBoxOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (AGymnastGameCharacter* player = Cast<AGymnastGameCharacter>(OtherActor))
	{
		StartingPlayerRotation = player->GetActorRotation();
		SwingMovement->SetSwingRadiusComponent(ActorBindLocation);
		SwingMovement->SetEffectedActor(player,PhysicsHitLocation->GetComponentLocation());
		player->GetCharacterMovement()->SetMovementMode(MOVE_None);
		player->SetSwinging(this);
		SwingingPlayer = player;
		if (!AlreadyHadActor)
		{
			if (AGymnastGameGameMode* GameMode = GetWorld()->GetAuthGameMode<AGymnastGameGameMode>())
			{
				GameMode->AddScore(this, PointsWorth);
			}
			if (IsValid(Spawner))
			{
				Spawner->SpawnNextSwing(SwingSpawnClass);
			}

			AlreadyHadActor = true;
		}
	}
}

void ASwingActorBase::HelperForceOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void ASwingActorBase::HelperForceOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void ASwingActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwingActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SwingMovement->bIsZeroVelocity && SwingingPlayer)
	{
		if (bCanSetNeedsStartingLocation)
		{
			bCanSetNeedsStartingLocation = false;
			SwingingPlayer->bNeedsNewStartingLocation = true;
		}
	}
	else
	{
		bCanSetNeedsStartingLocation = true;
	}
}

void ASwingActorBase::LaunchActor()
{
	AGymnastGameCharacter* tempCharecter = SwingingPlayer;
	SwingingPlayer = 0;
	SwingMovement->RemoveEffectedActor();
	tempCharecter->GetCharacterMovement()->SetMovementMode(MOVE_Custom);
	
	FVector LaunchVelocity;

	if (SwingMovement->GetLaunchVelocity(LaunchVelocity,LaunchVelocityMultiplier))
	{
		tempCharecter->GetCharacterMovement()->Velocity = LaunchVelocity;
	}
}

