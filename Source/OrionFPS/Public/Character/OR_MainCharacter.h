// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OR_MainCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UAnimMontage;
class UAnimInstance;
class AProjectile;
class AOR_LauncherProjectile;
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
	EMS_Reload UMETA(DisplayName = "Reload"),
	EMS_Melee UMETA(DisplayName = "Melee"),
	EMS_Grenade UMETA (DisplayName="Grenade"),
	
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

/////// Combat Enum States /////////
UENUM(BlueprintType)
enum class ECurrentWeapon :uint8
{
	EMS_Rifle UMETA(DisplayName = "Rifle"),
	EMS_Rocket UMETA(DisplayName = "Rocket"),


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
	void EndShootByOther();

	/**  Star Reload Left Mouse */
	void StarReload();
	//EndReload() -> BP Function

	/**  Star Melee Attack */
	void StarMeleeAtaack();
	//EndMeleeAttack() -> BP Function

	/**  Star Grenade Launcher */
	void StarGrenadeLauncher();
	//EndMeleeAttack() -> BP Function


	

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

	/** Main Camera RelativeRotationVariable*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		FRotator CameraRotationWithPawn;

	/** Arms Skeletal Mesh for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		USkeletalMeshComponent* Arms;

	/** Weapon Skeletal Mesh for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		USkeletalMeshComponent* Weapon;

	/** Weapon Skeletal Mesh for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		USkeletalMeshComponent* Rocket;

	/** Spring Arm (Camera-Arms) for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		USpringArmComponent* SpringArm;

	/** Spring Arm (Camera-Arms) for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		UCapsuleComponent* MeleeDetector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		FName WeaponSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		FName MeleeCapsuleSocketName;



	                 /////////// Combat Character ///////////

	/////Montages

	/** Under Shoot Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|Montage")
	    UAnimMontage* ShootMontage;

	/** Pointed Shoot Montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|Montage")
		UAnimMontage* PointedShoot_Montage;

	/** Reload Montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|Montage")
		UAnimMontage* ReloadMontage;

	/** Mele Montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|Montage")
		UAnimMontage* MeleMontage;

	/** Mele Montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|Montage")
		UAnimMontage* GrenadeMontage;

	/** Anim Instance */
	UPROPERTY(BlueprintReadOnly, Category = "Main|Montage")
		UAnimInstance* MainAnimInstance;


	/////ParticleSystem

	/** Muzzle Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* MuzzleShoot1;

	/** Muzzle Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* MuzzleShoot2;

	/** Muzzle Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* SmokeMuzzle;

	/** Shell Eject Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* ShellEject;

	/** Shell Eject Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* SmokeShell;

	/** SmokeReload ParticleSystem */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem")
		UParticleSystem* SmokeReload;

	/** Ultimate Particle System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|ParticleSystem") //Temporal Not Yet
		UParticleSystem* FinalGun;


	                   ///////// Projectile Class /////////

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<AProjectile> BulletClass;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<AOR_LauncherProjectile> LauncherClass;


protected:
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

	/**Tarea */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|CharacterMovement") ////////Temporal Tarea
		int32 Count = 0;


	           ////////////////////Combat Character/////////////////////

	/** Control Variable Reload State*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsReload;

	/** Control Variable Reload State*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterMovement")
		bool bIsMeleeAttack;

	/** Shooting Control Variable*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsShooting;

	/** Shooting Control Variable*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsGrenadeLauncher;

	/** Shooting Control Variable*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsRuningControlDelay;

	/** Shooting Control Variable*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsKeyShootPressed;

	/** Open/Close BP_UnderShoot Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsUnderShootCalled;

	/** Open/Close BP_PointedShoot Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsPointedShootCalled;

	/** Open/Close BP_UnderShoot Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsReloadCalled;

	/** Open/Close BP_PointedShoot Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsGrenadeLauncherCalled;

	/** Open/Close BP_PointedShoot Function*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		bool bIsMeleeAttackCalled;

	/** Is ultimate Active */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main|CharacterCombat") //Temporal Not Yet
		bool bIsUltimate;

	/** CountAmmo*/
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
	    int32 WeaponAmmo;

	/** CountAmmo*/
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Main|CharacterCombat")
		int32 GrenadeAmmo;

	


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
	FORCEINLINE void SetMovementStatus(EMovementStatus Status) { MovementStatus = Status; } 
	/** Get Current Movement Status*/
	FORCEINLINE EMovementStatus GetMovementStatus()  const { return MovementStatus; } 


	                           /////// Combat Status //////

	/** Player Combat Status Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|Enums")
		ECombatStatus CombatStatus;

	/** Set the New Combat Status*/
	FORCEINLINE void SetCombatStatus(ECombatStatus Status) { CombatStatus = Status; } 
	/** Get Current Combat Status*/
	FORCEINLINE ECombatStatus GetCombatStatus() const { return CombatStatus; } 

	                         /////// Weapon Status //////

    /** Player Current Weapon Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|Enums")
		ECurrentWeapon CurrentWeaponStatus;

	/** Set the New Combat Status*/
	FORCEINLINE void SetCurrentWeaponStatus (ECurrentWeapon Status) { CurrentWeaponStatus = Status; }
	/** Get Current Combat Status*/
	FORCEINLINE ECurrentWeapon  GetCurrentWeaponStatus() const { return CurrentWeaponStatus; }



	                         /////// Character Variables //////

	FORCEINLINE bool GetKeyBoolValue() const { return Key; }
	FORCEINLINE bool GetIsRuning() const { return bIsRuning; }
	FORCEINLINE bool GetIsPointed() const { return bIsPointed; }

	void SetEnumMeleeCollision(ECollisionEnabled::Type CollisionState);

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

	/** Star Grenade Launcher Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_StarGrenadeLauncher();

	/** End Grenade Launcher Camera Logi*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_EndGrenadeLauncher();

	/** Star Grenade Launcher Camera Logic*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_StarMeleeAttack();

	/** End Grenade Launcher Camera Logi*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_EndMeleeAttack();

	/** Star Reload BP Function*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_StarReload();

	/** End Reload BP Function*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Main|BP_CombatEvents")
		void BP_EndReload();




public:
	////////////////////////////////////////////////////////////////////
	//
	//  Character Functions
	//
	////////////////////////////////////////////////////////////////////

     ///////////Movement Functions/////////////////////////////

	    /** Update Player Properies()*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterMovement")
		void UpdatePlayerProperties();



    ////////////Combat Functions ////////////////////////////////

	     /** Shoot Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterCombat")
		void Shoot();

	     /** Shoot Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterCombat") //Temporal Not Yet
		void UltimateShoot();
   
	     /*** End Reload Function*/
	UFUNCTION(BlueprintCallable , Category= "Main|CharacterCombat")
	    void EndReload();

		/*** End MeleeAttack Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterCombat")
		void EndMeleeAttack();

	    /*** Make Mele Damage*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterCombat")
	void MakeMeleeDamage( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		/*** End Grenade Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterCombat")
		void EndGrenadeLauncher();

	/*** End Reload Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|CharacterCombat")
		void SpawnGrenadeLauncher();

	/** FTimerHablde Shoot Function*/
	FTimerHandle ShootHandle;
	FTimerHandle StillPressed; //Not yet



	////////////////// Montage Functions ///////////////////////

		/*** Play Montage Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|Montage")
		void PlayMyMontage(UAnimMontage* MontageToPlay, float Ratio, FName Section);

	  /*** Stop Montage Function*/
	UFUNCTION(BlueprintCallable, Category = "Main|Montage")
		void StopMyMontage(float RatioStop);

	  

};
