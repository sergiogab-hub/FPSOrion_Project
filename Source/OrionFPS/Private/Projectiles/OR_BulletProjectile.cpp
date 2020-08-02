// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/OR_BulletProjectile.h"
#include "Character/OR_MainCharacter.h"
#include "AI/OR_Enemy.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Collision 
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(2.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);	// set up a notification for when this component hits something blocking


	RootComponent = CollisionComp;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionComp);

	// Projectile
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	Projectile->UpdatedComponent = CollisionComp;
	Projectile->InitialSpeed = 33.f;
	Projectile->MaxSpeed = 33.f;
	Projectile->bRotationFollowsVelocity = true;
	Projectile->bInitialVelocityInLocalSpace = true;
	Projectile->bShouldBounce = true;

	//Projectie Life
	InitialLifeSpan = 2.0f;

	Damage = 10.f;
	
	

}
////////////////////////////////////////////////////////////////////
//
//  Projectile Hit
//
////////////////////////////////////////////////////////////////////
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((IsValid(OtherActor)) && (OtherActor != this) && (IsValid(OtherComp)))
	{
		if (ImpactParticlesStone && ImpactParticlesSmokeStone && ImpactParticlesEnemy && ImpactParticlesSmokeEnemy)
		{

			if (IsValid(MainCharacter))
			{			
				if (IsValid(Hit.GetActor()))
				{
					FRotator RotationImpact = UKismetMathLibrary::FindLookAtRotation(Hit.Location, MainCharacter->GetActorLocation());

					AOR_Enemy* Enemy = Cast<AOR_Enemy>(Hit.GetActor());
					
				

                    if(IsValid(Enemy))
					{
						UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, MainCharacter->GetActorRotation().Vector(), Hit, MainCharacter->GetInstigatorController(), this, DamageType);

						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticlesEnemy, Hit.Location, RotationImpact, FVector(0.4f), true);//0.4
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticlesSmokeEnemy, Hit.Location, RotationImpact, FVector(0.1f), true);//0.2
					}
					else
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticlesStone, Hit.Location, RotationImpact, FVector(1.0f), true);//1.0
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticlesSmokeStone, Hit.Location, RotationImpact, FVector(0.5f), true);//0.5
					}

					/*AActor* Actoristo = Hit.GetActor();
					FString Name = Actoristo->GetName();

					if (Name == FString("Enemy_5"))
					{
					
					}
					else 
					{
					
					}	*/
				}
          		Destroy();
			}	
		}
	}	
}
