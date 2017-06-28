// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GymnastGameCharacter.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "FlightCharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine.h"

//////////////////////////////////////////////////////////////////////////
// AGymnastGameCharacter

AGymnastGameCharacter::AGymnastGameCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UFlightCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	currentController = 0;

	CanAddLowerImpulse = true;
	CanAddUpperImpulse = true;

	bNeedsNewStartingLocation = true;

	TiltRotateAmount = 90;
	TiltAngleTresh = 45.0;
	TiltSteerAmount = 500;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AGymnastGameCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);
	if (currentController)
	{
		FVector CurrentTilt;
		FVector CurrentGravity;
		FVector CurrentAccel;
		FVector CurrentRotationRate;

		currentController->GetInputMotionState(CurrentTilt, CurrentRotationRate, CurrentGravity, CurrentAccel);

		double CurrentAngle = CurrentTilt.Z * TiltRotateAmount;
		double CurrentAngleY = (FMath::Cos((CurrentTilt.X - StartingSteerX)*PI*2) + FMath::Sin((CurrentTilt.X - StartingSteerX)*PI * 2)) * TiltSteerAmount;

		if (bNeedsNewStartingLocation)
		{
			bNeedsNewStartingLocation = false;
			StartingSteerX = CurrentTilt.X;
			StartingSteerY = CurrentTilt.Y;
			StartingAngle = CurrentAngle;
			return;
		}
		
		if (CurrentAngle - StartingAngle < -TiltAngleTresh && CanAddLowerImpulse)
		{
			AddImpulseToSwing(1);
			CanAddLowerImpulse = false;
			CanAddUpperImpulse = true;
		}

		if (CurrentAngle - StartingAngle > TiltAngleTresh && CanAddUpperImpulse)
		{
			AddImpulseToSwing(-1);
			CanAddLowerImpulse = true;
			CanAddUpperImpulse = false;
		}

		FVector CurrentActorLocation = GetActorLocation();
		DrawDebugDirectionalArrow(GetWorld(), CurrentActorLocation, CurrentActorLocation + (FVector(0, -CurrentAngleY,0).GetSafeNormal() * 100.0),
			100.f, FColor::Red, false, -1.f, (uint8)'\000', 10.f);
		SteerFlight(CurrentAngleY);
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, FString::Printf(TEXT("Angle %f %f"), CurrentAngle,CurrentAngleY));
		
	}
}
void AGymnastGameCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	currentController = Cast<APlayerController>(GetController());
}
//////////////////////////////////////////////////////////////////////////
// Input

void AGymnastGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGymnastGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGymnastGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGymnastGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGymnastGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGymnastGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGymnastGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGymnastGameCharacter::OnResetVR);
}


void AGymnastGameCharacter::SteerFlight(float tilt)
{
	UFlightCharacterMovementComponent* fMovement = Cast<UFlightCharacterMovementComponent>(GetMovementComponent());
	fMovement->SetSteerVelocity(FVector(0,-tilt,0));
}

void AGymnastGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGymnastGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGymnastGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGymnastGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGymnastGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGymnastGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGymnastGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
