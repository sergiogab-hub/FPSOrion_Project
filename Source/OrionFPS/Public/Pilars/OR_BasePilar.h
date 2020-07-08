// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_BasePilar.generated.h"


class USphereComponent;
class UBoxComponent;
class UStaticMeshComponent;
class AMainCharacter;
class AOR_Enemy;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class ORIONFPS_API AOR_BasePilar : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOR_BasePilar();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = " BasePilar| Collisions")
		UBoxComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BasePilar | Mesh")
		UStaticMeshComponent* MeshSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BasePilar | Mesh")
		UStaticMeshComponent* MeshCilinder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BasePilar | Mesh")
		UStaticMeshComponent* MeshCone;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = " BasePilar | Reference")
		AMainCharacter* MainCharacterReference;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar| Rate")
		float PilarDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar | Rate")
		float PilarAddRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar| Rate")
		float PilarTickRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar | Movement")
		float MinPilarInterpMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar | Movement")
		float CurrentPilarInterpMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar | Movement")
		float MaxInterpMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar | Movement")
		FRotator IdleRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar | Movement")
		bool bIsOnUltimate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BasePilar | Enemy")
		int32 CurrentIndexEnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BasePilar | Enemy")
		TArray<AOR_Enemy*> EnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BasePilar | Enemy")
		AOR_Enemy* CurrentEnemyTarget;

	FTimerHandle MovemenetTorretHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BasePilar | Shoot")
		UParticleSystem* ShootTorretParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BasePilar | Shoot")
		UParticleSystemComponent* ShootParticleSystemComponent;

	/** Projectile Rocket to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "BasePilar |Shoot")
		TSubclassOf<AActor> Collider;

	FTimerHandle ShootHandle;

	FTimerHandle ColliderSpawnHandle;

	FTimerHandle CameraHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool GetNewTargetEnemy();

	void StartMovement();

	void StopMovement();

	void MoveToEnemy();

	void MoveToIdle();

	void StartShoot();

	void StopShoot();

	void SpawnCollider();



public:	

	UFUNCTION()
	void StartUltimate();

	UFUNCTION()
	void StopUltimate();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_CameraShake();

	
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
