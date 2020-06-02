// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UAnimMontage;
class AProjectile;
class UParticleSystem;


////// Movement Enum States //////
UENUM(BlueprintType)
enum class EMovementStatus :uint8
{
	EMS_Idle UMETA(DisplayName = "Idle"),
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Pointing UMETA(DisplayName = "Pointing"),
	

	EMS_MAX UMETA(DisplayName = "DefaultMAX")

};


/////// Combat Enum States /////////
UENUM(BlueprintType)
enum class ECombatStatus :uint8
{
	EMS_NoCombat UMETA(DisplayName = "NoCombat"),
	EMS_FireUnder UMETA(DisplayName = "FireUnder"),
	EMS_PointedFire UMETA(DisplayName = "PointedFire"),

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

	/**  Shoot Left Mouse Input */
	void StarShoot();
	void EndShoot();



protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Character Class
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


	                 /////////// Combat Character ///////////

	/** Under Shoot Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|Montage")
	    UAnimMontage* ShootMontage;

	/** Pointed Shoot Montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|Montage")
		UAnimMontage* PointedShoot_Montage;

	/** Muzzle Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* MuzzleShoot1;

	/** Muzzle Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* MuzzleShoot2;

	/** Shell Eject Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* ShellEject;

	/** Shell Eject Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* SmokeMuzzle;

	/** Shell Eject Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* SmokeShell;


	                   ///////// Projectile Class /////////

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<AProjectile> ProjectileClass;



public:
	////////////////////////////////////////////////////////////////////
	//
	//   Character Variables
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


	/** Launcher Pad Variable Key*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|CharacterMovement") ////////Temporal
		bool Key = false;


	           ////////////////////Shoot Character/////////////////////

	/** Shooting Control Variable  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsShooting;

	/** Open/Close BP_UnderShoot Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsUnderShootCalled;

	/** Open/Close BP_PointedShoot Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsPointedShootCalled;




public:
	////////////////////////////////////////////////////////////////////
	//
	//   Getter /Setter Enum State/
	//
	////////////////////////////////////////////////////////////////////

                                /////// Movement Status //////

	 /** Player Movement Status Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|Enums")
		EMovementStatus MovementStatus;

	/** Set the New Movement Status*/
	FORCEINLINE void SetMovementStatus(EMovementStatus Status) { MovementStatus = Status; } //Temporaly No Const

	/** Get Current Movement Status*/
	FORCEINLINE EMovementStatus GetMovementStatus()  const { return MovementStatus; } 


	                           /////// Combat Status //////

	/** Player Combat Status Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|Enums")
		ECombatStatus CombatStatus;

	/** Set the New Movement Status*/
	FORCEINLINE void SetCombatStatus(ECombatStatus Status) { CombatStatus = Status; } //Temporaly No Const

	/** Get Current Movement Status*/
	FORCEINLINE ECombatStatus GetCombatStatus() const { return CombatStatus; } 

public:
	////////////////////////////////////////////////////////////////////
	//
	//  Character BP_Functions
	//
	////////////////////////////////////////////////////////////////////


                     ///////////// Movement BP Functions ///////////////////

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

	                  /////////////Combat BP Functions /////////////////

	/** Star Camera Under Shoot BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_StarUnderShoot();

	/** End Camera Under Shoot BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_EndUnderShoot();

	/** Star Camera Pointed SHoot BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_StarPointedShoot();

	/** End Camera Pointed Shoot BP Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_EndPointedShoot();



public:
	////////////////////////////////////////////////////////////////////
	//
	//  Character Functions
	//
	////////////////////////////////////////////////////////////////////

	////////////////////Movement Functions//////////////////////////

	/** Star Camera Pointed SHoot BP Camera Logic*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterMovement")
		void CheckCurrentVariables();

	/** Star Camera Pointed SHoot BP Camera Logic*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterMovement")
		void SetCurrentStatus();

	/** Star Camera Pointed SHoot BP Camera Logic*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterMovement")
		void SetCameraMovement();

	////////////////////Shoot Functions //////////////////////////

	/** Shoot Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterCombat")
		void Shoot();

	/** FTimerHablde Shoot Function*/
	FTimerHandle ShootHandle;
	


};
