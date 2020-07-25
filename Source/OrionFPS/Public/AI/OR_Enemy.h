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

////// Movement Enum States //////
/*
UENUM(BlueprintType)
enum class ERangeStatus :uint8
{
	EMS_RunRange UMETA(DisplayName = "RunRange"),
	EMS_FireHipsRange UMETA(DisplayName = "FireHipsRange"),
	EMS_IronSightRange UMETA(DisplayName = "IronSightRange"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")

};
*/

UCLASS()
class ORIONFPS_API AOR_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOR_Enemy();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




	/** Anim Instance */
	UPROPERTY(BlueprintReadOnly, Category = "Main|References")
		class UAnimInstance* EnemyAnimInstance;

	FORCEINLINE void SetMyPitch(float value) { MyPitch = value; }
	FORCEINLINE void SetMyYaw(float value) { MyYaw = value; }

	FORCEINLINE bool GetIsHipsFiring() { return bIsHipsFiring; }

	FORCEINLINE bool GetIsIronSightFiring() { return bIsIronSightFiring; }
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main|Enums")
		ERangeStatus RangeStatus;

	/** Set the New Movement Status
	FORCEINLINE void SetRangeStatus(ERangeStatus Status) { RangeStatus= Status; }

	/** Get Current Movement Status
	FORCEINLINE ERangeStatus GetRangeStatus()  const { return RangeStatus; } */



protected:

	/** Anim Instance */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|References")
		UAnimInstance* OwnAnimInstance;

	FTimerHandle ShootHipsHandle;
	FTimerHandle ShootIronHandle;

	/** Hip Shoot Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montages")
		UAnimMontage* ShootHipsMontage;

	/** IronSight Shoot Montage*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montages")
		UAnimMontage* ShootIronSightMontage;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bIsHipsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = " Enemy|Variables")
		bool bIsIronSightFiring;





protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
		void StartHipsFire();

	UFUNCTION(BlueprintCallable)
		void StartIroSightFire();
	
	void Firing();

	UFUNCTION(BlueprintCallable)
	    void StopHipsFire();

	UFUNCTION(BlueprintCallable)
		void StopIronFire();

	UFUNCTION(BlueprintCallable)
	FVector GetMovementDirection(float PlayerDistance);






protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Character Class
	//
	////////////////////////////////////////////////////////////////////

		/** Weapon Skeletal Mesh for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main|CharacterComponents")
		USkeletalMeshComponent* Weapon;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint6;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* LocoPoint7;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		TArray <FVector> LocomotionPoints;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		float MyPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		float MyYaw;

};
