// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_LauncherProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USphereComponent;
class UDamageType;

class AMainCharacter;


UCLASS()
class ORIONFPS_API AOR_LauncherProjectile : public AActor
{
	GENERATED_BODY()

public:

	// Constructor
	AOR_LauncherProjectile();

	//MainCharacterClass
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		AMainCharacter* MainCharacter;

	//Setter Main
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
		UParticleSystem* Explosion;

	/** SmokeGrenade */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		UParticleSystem* SmokeGrenade;


	           //////////// Propertys //////////////

	/** Damage Ammount */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		float GrenadeBaseDamage;

	/** RadialDamage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
		float ExplotionRadiusDamage;

	/** Damage Type */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		TSubclassOf<UDamageType> DamageType;

	/** Pivot GrenadeCount */
	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly , Category="Projectile")
	int32 PivotCount;

	/** EmptyArray RadialDamageExeptions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TArray<AActor*> ExplotionExceptions;

	/** ExplotionTimer */
	FTimerHandle ExploteTime;


	/////////// Functions //////////////

	/** Hit Function */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** DetonateGrenade */
	UFUNCTION()
		void DetonateMyGrenade();


	


};
