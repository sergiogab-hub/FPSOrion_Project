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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Collisions")
		UBoxComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshCilinder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshCone;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Reference")
		AMainCharacter* MainCharacterReference;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Rate")
		float PilarDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Rate")
		float PilarAddRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Rate")
		float PilarTickRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Movement")
		float MinPilarInterpMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Movement")
		float CurrentPilarInterpMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Movement")
		float MaxInterpMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Movement")
		FRotator IdleRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Movement")
		bool bIsOnUltimate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Enemy")
		int32 CurrentIndexEnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Enemy")
		TArray<AOR_Enemy*> EnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Enemy")
		AOR_Enemy* CurrentEnemyTarget;

	FTimerHandle MovemenetTorretHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pilar/Shoot")
		UParticleSystem* ShootTorretParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Shoot")
		UParticleSystemComponent* ShootParticleSystemComponent;

	/** Projectile Rocket to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Pilar/Shoot")
		TSubclassOf<AActor> Collider;

	FTimerHandle ShootHandle;

	FTimerHandle ColliderSpawnHandle;

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

	
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
