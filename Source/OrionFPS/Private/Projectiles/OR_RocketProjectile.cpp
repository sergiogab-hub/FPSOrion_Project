// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/OR_RocketProjectile.h"
#include "Character/OR_MainCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


// Sets default values
AOR_RocketProjectile::AOR_RocketProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision 
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(6.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AOR_RocketProjectile::OnHit);	// set up a notification for when this component hits something blocking


	RootComponent = CollisionComp;


	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionComp);

	// Projectile
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	Projectile->UpdatedComponent = CollisionComp;
	Projectile->InitialSpeed = 3000.f;
	Projectile->MaxSpeed = 3000.f;
	Projectile->bRotationFollowsVelocity = true;
	Projectile->bInitialVelocityInLocalSpace = true;
	Projectile->bShouldBounce = true;

	InitialLifeSpan = 6.0f;
	ExplotionExceptions.Empty();


}

// Called when the game starts or when spawned
void AOR_RocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MainCharacter))
	{
		ExplotionExceptions.Add(MainCharacter);

	}

	if (IsValid(Trail))

	{
		UGameplayStatics::SpawnEmitterAttached(Trail, Mesh, FName("RocketLauncher"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.2f));
	}
	
}

void AOR_RocketProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(ImpactParticles))
	{
		if (IsValid(MainCharacter))
		{
			FRotator RotationImpact = UKismetMathLibrary::FindLookAtRotation(Hit.Location, MainCharacter->GetActorLocation());
			RotationImpact.Pitch = RotationImpact.Pitch + 50.F;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, Hit.Location, RotationImpact, FVector(1.0f), true);//0.4*/
			UGameplayStatics::ApplyRadialDamage(GetWorld(), 50, GetActorLocation(), 800, nullptr, ExplotionExceptions, this, MainCharacter->GetInstigatorController());
		}
	}

	Destroy();
}



