// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_RocketProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USphereComponent;
class UDamageType;

class AMainCharacter;

UCLASS()
class ORIONFPS_API AOR_RocketProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOR_RocketProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		AMainCharacter* MainCharacter;

	void SetMain(AMainCharacter* Main) { MainCharacter = Main; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	               //////////// Components //////////////

	/** Collision Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		USphereComponent* CollisionComp;

	/** Projectile Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		UStaticMeshComponent* Mesh;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		UProjectileMovementComponent* Projectile;

	             //////////// Particle System //////////////

   /** Impact Particle System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		UParticleSystem* ImpactParticles;

	/** Impact Particle System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		UParticleSystem* Trail;

	               //////////// Damage//////////////

   /** Damage Type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		TSubclassOf<UDamageType> DamageType;

	/** Bullet Damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		float Damage;
	/** EmptyArray Radial
	DamageExeptions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		TArray<AActor*> ExplotionExceptions;

	               //////////// Functions //////////////

    /** Hit Functions */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
