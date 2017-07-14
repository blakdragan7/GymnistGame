// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GymnastGameCharacter.generated.h"

class UFlightCharacterMovementComponent;
UCLASS(config=Game)
class AGymnastGameCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	APlayerController* currentController;


	float StartingSteerX;
	float StartingSteerY;
	float StartingSteerZ;

	bool CanAddLowerImpulse;
	bool CanAddUpperImpulse;

	FRotator StartingLaunchRotation;
	FRotator StartingBoomRotation;
	FRotator StartingActorRotation;

	FVector StartingTilt;
	FVector StartingGravity;
	/* Velocity Used to Launch Player after Swinging. Calculated Via lastPosition - currentPosition */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector LastSwingVelocity;
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
private:
	void ControlSwing(float CurrentAngle);
	void ControlFlight(UFlightCharacterMovementComponent* component,FVector tilt);

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0)override;
public:
	AGymnastGameCharacter(const FObjectInitializer& ObjectInitializer);

	// Actor Overrides
	void Tick(float DeltaTime)override;
	void BeginPlay()override;

	/** Determins The Offset CameraRotationPositionRange uses*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Settings")
	float CameraRotationPositionOffset;
	/** Determins The range for the Z position of the Character for which the camera will rotate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Settings")
	float CameraRotationPositionRange;
	/** The Range of Rotation The Camera Will Take. ex 40 degrees means the camera will pitch + 40 to - 40 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Settings")
	float CameraRotationRotationRange;
	/** Amount to Multiply the X Tilt amount. Affects @TiltAngleTresh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Control")
	float TiltRotateAmount;
	/** Amount Of Tilt needed in degrees for pitch */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Control")
	float PitchSteerTiltLimit;
	/** Amount of rotation applied to pitch from tilt while steering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Control")
	float PitchSteerRotateLimit;
	/** Amount to Steer Actor after being launched from the Y tilt of the mobile device.  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Control")
	float TiltSteerAmount;

	/** Amount to 'Padd' the swing input axis. essentually clamp the -InputAxisPadding - InputAxisPadding to 0  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Control")
	float InputAxisPadding;

	/** Resets Starting Angle and Starting Tilt for Swing and Flight Calcuations. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Swing Control")
	bool bNeedsNewStartingLocation;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/* Angle The Tilt Must Pass To have Add Impulse To Swing Called */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swing Control")
	float TiltAngleTresh;

	/* Simply passes through current tilt values after calculations.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Swing Physics")
	void PassCurrentTiltValues(FVector tilts);

	/* Called when charecter reaches peak height from laucnch. ex. starting to fall instead of going up.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Swing Physics")
	void ReachedPeakHeight();

	/* Called when charecter is launched From a Swing */
	UFUNCTION(BlueprintNativeEvent, Category = "Swing Physics")
	void WasLaunched();
	
	/* Called when a chareter lands on the ground after a launch */
	UFUNCTION(BlueprintNativeEvent, Category = "Swing Physics")
	void HasLanded();

	/* Called when Force should be added to the swing actor to simulate pendulam mechanics  */
	UFUNCTION(BlueprintImplementableEvent, Category = "Swing Control")
	void AddImpulseToSwing(int32 direction);
	/* Called constantly like an input axis. Amount is the strength of current rotation  */
	UFUNCTION(BlueprintImplementableEvent, Category = "Swing Control")
	void SwingMovementAxis(float amount);

protected:
	/* Steers Flight Based On Tilt */
	UFUNCTION(BlueprintCallable, Category = "Swing Physics")
	void SteerFlight(float tilt);

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

