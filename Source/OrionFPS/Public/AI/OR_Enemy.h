// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OR_Enemy.generated.h"

/*UE CLASS*/

class USkeletalMeshComponent;
class UAnimMontage;
class UAnimInstance;
class UParticleSystem;
class USphereComponent;

/*MyCLASS*/
class AMainCharacter;
class AProjectile;
class AOR_AIBaseEnemyController;
class UEnemyAnimInstance;


UCLASS()
class ORIONFPS_API AOR_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOR_Enemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Character Class
	//
	////////////////////////////////////////////////////////////////////

					   //////// Character Components///////

		/** Weapon Skeletal Mesh for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterComponents")
		USkeletalMeshComponent* Weapon;

	/** Health Component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterComponents")
		UOR_HealthComponent* Health;

	//////// Navigation AI points ///////

/** Locomotion Point 0 -> Front */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint0;

	/** Locomotion Point 1 -> Diagonal Right */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint1;

	/** Locomotion Point 2 -> Right Side*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint2;

	/** Locomotion Point 3 -> Behind*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint3;

	/** Locomotion Point 0 -> Diagonal left*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint4;

	/** Locomotion Point 0 -> Left Side */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint5;

	/** Locomotion Point 0 -> Diagonal*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint6;

	/** Locomotion Point 0 -> Diagonal*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint7;

	/** Navigation Points Array*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		TArray <FVector> LocomotionPoints;

	/** Character Pitch*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		float MyPitch;

	/** Character Yaw*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		float MyYaw;

	//////// Montages ///////

/** Hip Shoot Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montages")
		UAnimMontage* ShootHipsMontage;

	/** IronSight Shoot Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montages")
		UAnimMontage* ShootIronSightMontage;

	/** Reload Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montages")
		UAnimMontage* IronSightReload;

	/** Reload Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montages")
		UAnimMontage* HitReactMontage;

	/** Death Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montages")
		UAnimMontage* DeathMontage;

	///// References //////

   /** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Projectiles")
		TSubclassOf<AProjectile> BulletClass;

	/** Reload Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|References")
		AMainCharacter* MainReference;

	/** AIBaseEnemyController*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|References")
		AOR_AIBaseEnemyController* OwnController;

	/** AnimInstance*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|References")
		UEnemyAnimInstance* EnemyAnimInstaceReference;

public:

	////////////////////////////////////////////////////////////////////
	//
	//   Getter /Setter 
	//
	////////////////////////////////////////////////////////////////////

	/** Pitch - Yaw */
	FORCEINLINE void SetMyPitch(float value) { MyPitch = value; }
	FORCEINLINE void SetMyYaw(float value) { MyYaw = value; }

	/** Fire-ReloadStates */
	FORCEINLINE bool GetIsHipsFiring() { return bIsHipsFiring; }
	FORCEINLINE bool GetIsIronSightFiring() { return bIsIronFiringMoving; }
	FORCEINLINE bool GetbIsReload() { return bIsRealod; }


protected:

	////////////////////////////////////////////////////////////////////
	//
	//    AI  Character Variables
	//
	////////////////////////////////////////////////////////////////////

	/** HIPS Firing State */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bIsHipsFiring;

	/** Iron Firing State Moving */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bIsIronFiringMoving;

	/** Iron Firing State Not Moving */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bIsIronFiringQuiet;

	/** Is Firin State */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bIsFiring;

	/** Has to reload state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bHasNeedReload;

	/** On Reload State */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bIsRealod;

	/** Current Munition */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		int32 CurrentMunition;

	/** Max Munition */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Enemy|Variables")
		int32 MaxMunition;


	/** Initial Min DIntance to Run */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Enemy|Variables")
		float InitialMinDistanceToRun;

	/** Current Min DIntance to Run */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Enemy|Variables")
		float MinDistanceToRun;

	/** Max Distance to Stay */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Enemy|Variables")
		float MaxDistanceToStay;

	/** MinDistancetoStay */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = " Enemy|Variables")
		float MinDistanceToStay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		int32 CurrentIndex; //Temporal Not Use

		/*Handles*/
	FTimerHandle ShootHipsHandle;
	FTimerHandle ShootIronHandle;
	FTimerHandle UpdateNavegationSystemHandle;



protected:

	////////////////////////////////////////////////////////////////////
	//
	//  AI Character Functions
	//
	////////////////////////////////////////////////////////////////////

	/*Start Fire with IronSights*/
	UFUNCTION(BlueprintCallable)
		void StartIroSightFire(bool Moving);

	/*Stop Iron Fire*/
	UFUNCTION(BlueprintCallable)
		void StopIronFire();

	/*Shoot Spawn proyectile called by Anim notificy*/
	UFUNCTION(BlueprintCallable)
		void Shoot();

	/*Shoot BP Function*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_Shoot();

	/*Start Reload*/
	UFUNCTION(BlueprintCallable)
		void StartReload();

	/* Start end Reload */
	UFUNCTION(BlueprintCallable)
		void EndReload();

	/*Hit React */  //Temporal not use
	UFUNCTION(BlueprintCallable)
		void HitReact();

	/* Get Navigation point to move */
	UFUNCTION(BlueprintCallable)
		FVector GetMovementDirection(float PlayerDistance);

	/*Proporcional speed to movement */
	UFUNCTION(BlueprintCallable)
		void GetProportionalSpeed(float distance);

	/* Check valid navigation point */
	FVector CheckNavigationPoint(FVector TargetPosition);

	/* Update points position */
	void UpdateNavegationSystem();

	/*Firing timer function Montage*/
	void Firing();

	/*Stop Firing*/
	void StopFiring();

	void PlayMontageDeathAnimation();

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable , Category = "Enemy|Functions")
	void BP_Death();

	void SetupReferences();

	////////////////// Delegate Component ///////////////////////

	 /** Health Delegate Component */
	UFUNCTION(BlueprintCallable, Category = "Enemy|Component")
		void OnHealthChange(UOR_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);




	




};
