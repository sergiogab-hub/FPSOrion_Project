// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_Trap.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS(ABSTRACT)
class ORIONFPS_API AOR_Trap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOR_Trap();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trap / Collisions")
		USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trap/ Collisions")
		USphereComponent* HitVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trap / Mesh")
	    UStaticMeshComponent* Mesh;



	                 //////////// Damage//////////////

    /** Damage Type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap / Property")
		TSubclassOf<UDamageType> DamageType;

	/** Bullet Damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap / Property")
		float BaseDamage;

	/** Bullet Damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap / Property")
		float RadiusDamage;

	/** EmptyArray RadialDamageExeptions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap / Property")
		TArray<AActor*> TrapExceptions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Hit Functions */
	UFUNCTION()
	virtual	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnEmitter();

};
