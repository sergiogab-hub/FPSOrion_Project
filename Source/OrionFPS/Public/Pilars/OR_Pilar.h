// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_Pilar.generated.h"


class USphereComponent;
class UBoxComponent;
class UStaticMeshComponent;
class AMainCharacter;
class AOR_Enemy;

UENUM(BlueprintType)
enum class AOR_PilarType:uint8
{
	PilarType_Attack UMETA(DisplayName = "Attack"),
	PilarType_Defence UMETA(DisplayName = "Defence"),
	PilarType_Movility UMETA(DisplayName = "Movility"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class ORIONFPS_API AOR_Pilar : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOR_Pilar();


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Collisions")
		UBoxComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshCilinder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshCone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Enum")
		AOR_PilarType PylarType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar / Main")
		AMainCharacter* MainCharacter;

	FTimerHandle AddTorretRateHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Enemy")
		TArray<AOR_Enemy*> EnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Enemy")
		AOR_Enemy* CurrentEnemyTarget;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Enemy")
		int32 CurretRandArrayEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pilar/Enemy")
		FRotator RotationTorret;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Movement")
		FRotator IdlePosition;

	UPROPERTY(BlueprintReadOnly, Category = "Pilar/Movement")
		bool bHasMovementEnemy;

	UPROPERTY(BlueprintReadOnly, Category = "Pilar/Movement")
		bool bHasBackIdle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pilar/Movement")
		float PilarInterpolationSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Movement")
		bool bIsInAttack;


	////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Rate")
		float AttackPilarRateState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Rate")
		float DefencePilarRateState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Rate")
		float MovilityPilarRateState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pilar/Rate")
		float MaxPilarRateState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pilar/Rate")
		float PilarDrainRate;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SendTorretType();

	void ActivateTimer();

	void SetCharacterOverlapState(bool state);

	bool GetNewTargertEnemy();

	void StartMovementFunction();
	void StopMovement();

	void StartFire();
	void StopFire();

	

	FTimerHandle PilarMovementHandle;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	   void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void ActivateUltimate();

	UFUNCTION()
		void DeactivateUltimate();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void BP_StartFire();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_StoptFire();



	FORCEINLINE	AOR_PilarType GetPylarType() { return PylarType; }

	

};
