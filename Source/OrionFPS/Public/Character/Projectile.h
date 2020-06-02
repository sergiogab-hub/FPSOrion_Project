// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USphereComponent;


UCLASS()
class ORIONFPS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();


protected:
	
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

	/** Impact Particle System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	     UParticleSystem* ImpactParticles;

	/** Impact Particle System */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		UParticleSystem* ImpactParticlesSmoke;



	                //////////// Functions //////////////
	/** Hit Functions */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
