// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;


////// Temporal Enum States //////

UENUM(BlueprintType)
enum class EMovementStatus :uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Pointing UMETA(DisplayName = "Pointing"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")

};

///////// Main Class /////////

UCLASS()
class ORIONFPS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	////////////////////////////////////////////////////////////////////
	//
	//  Player Input
	//
	////////////////////////////////////////////////////////////////////

	/**  Forward/Backward control input */
	void MoveForward(float value);

	/**  Right/Left control input */
	void MoveRight(float value);

	/**  Up/Down control mouse input */
	void RotatePitch(float value);

	/**  Right/Left control mouse input */
	void RotateYaw(float value);

	/**  Jump Space Bar input */
	void StarJump();
	void EndJump();


	/**  Sprint Left Shift Input */
	void StarSprint();
	void StopSprint();

	/**  Pointed Weapon Right Mouse Input */
	void StarGunPoint();
	void EndGunPoint();


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Character Movement 
	//
	////////////////////////////////////////////////////////////////////

				   //////// Movement Character Components///////

	/** Main Camera Component for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		UCameraComponent* Camera;

	/** Arms Skeletal Mesh for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		USkeletalMeshComponent* Arms;

	/** Weapon Skeletal Mesh for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		USkeletalMeshComponent* Weapon;

	/** Spring Arm (Camera-Arms) for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		USpringArmComponent* SpringArm;

public:
	////////////////////////////////////////////////////////////////////
	//
	//   Character Movement
	//
	////////////////////////////////////////////////////////////////////


		  ////////////////Movement Character Variables//////////////

	/** Rotation Turn Speed Pitch/Yaw*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|CharacterMovement")
		float RotationSpeed;

	/** Control Variable Run State*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsRuning;

	/** Control Variable Jump State*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsJumping;

	/** Control Variable Moving/Idle State*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsMoving;

	/** Control Variable Pointed Weapon State*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsPointed;

	/** Open/Close BP_Pointed Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsPointedCalled;

	/** Open/Close BP_Sprint Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsSprintCalled;

	/** Open/Close Jump Camera Logic*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsJumpCalled;

	/** Jump Camera Lag Delay*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|CharacterMovement")
		bool bIsDelay;

	/** Pawn Current Velocity  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		float CurrentVelocity;

	//////// Launcher Pad Variable Key //////////
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|CharacterMovement") ////////Temporal
		bool Key = false;

public:
	////////////////////////////////////////////////////////////////////
	//
	//   Getter /Setter Enum State/
	//
	////////////////////////////////////////////////////////////////////

		 /** Player Movement Status Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|Enums")
		EMovementStatus MovementStatus;

	/** Set the New Movement Status*/
	FORCEINLINE void SetMovementStatus(EMovementStatus Status) { MovementStatus = Status; } //Temporaly No Const

	/** Get Current Movement Status*/
	FORCEINLINE EMovementStatus GetMovementStatus()  { return MovementStatus; } //Temporaly No Const


public:
	////////////////////////////////////////////////////////////////////
	//
	//   BP_Functions
	//
	////////////////////////////////////////////////////////////////////


	/** Star Camera Pointed BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_MovementEvents")
		void BP_StarCameraPointed();

	/** End Camera Pointed BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_MovementEvents")
		void BP_EndCameraPointed();

	/** Star Camera Sprint BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_MovementEventss")
		void BP_StarCameraSprint();

	/** End Camera Sprint BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_MovementEvents")
		void BP_EndCameraSprint();

	/** Star Camera Jump BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_MovementEvents")
		void BP_StarCameraJump();

	/** End Camera Jump BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_MovementEvents")
		void BP_EndCameraJump();

public:
	////////////////////////////////////////////////////////////////////
	//
	//  Character Movement Functions
	//
	////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "Main|MovementInternal")
		void CheckCurrentVariables();

	UFUNCTION(BlueprintCallable, Category = "Main|MovementInternal")
		void SetCurrentMovementStatus();

	UFUNCTION(BlueprintCallable, Category = "Main|MovementInternal")
		void SetCameraMovement();


	


};
