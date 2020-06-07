// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USphereComponent;
class AMainCharacter;




UCLASS()
class ORIONFPS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		 AMainCharacter* MainCharacter;

	void SetMain (AMainCharacter* Main) { MainCharacter = Main; }


	

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

	/** Impact Particle System 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		UParticleSystem* TraceImpact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		 FName ParamName; */



	                //////////// Functions //////////////
	/** Hit Functions */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
