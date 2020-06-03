// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OR_Projectile.h"
#include "Character/OR_MainCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
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
		if (ImpactParticles && ImpactParticlesSmoke)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, Hit.Location,FRotator(0.f), FVector(1.0f), true);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticlesSmoke, Hit.Location, FRotator(0.f), FVector(0.5f), true);
		    Destroy();
		}
	}
	
}
