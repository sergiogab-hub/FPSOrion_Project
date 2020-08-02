// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/OR_LauncherProjectile.h"
#include "Character/OR_MainCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


#include "DrawDebugHelpers.h"
// Sets default values
AOR_LauncherProjectile::AOR_LauncherProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Collision 
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(6.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AOR_LauncherProjectile::OnHit);	// set up a notification for when this component hits something blocking


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


	// Set Base Propertys
	PivotCount = 0;
	GrenadeBaseDamage = 50;
	ExplotionRadiusDamage = 700;
	ExplotionExceptions.Empty();
}

// Called when the game starts or when spawned
void AOR_LauncherProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ExploteTime, this, &AOR_LauncherProjectile::DetonateMyGrenade, 4.0f, false, 4.0f);

	if (IsValid(MainCharacter)) 
	{
		ExplotionExceptions.Add(MainCharacter);

	}	

	if (IsValid(SmokeGrenade))
	{
		UGameplayStatics::SpawnEmitterAttached(SmokeGrenade, Mesh, FName("Grenade"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(8.0f));
	}
}



void AOR_LauncherProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsValid(OtherActor) && IsValid(OtherComp))
	{
		PivotCount++;
		if (PivotCount == 3)
		{
			DetonateMyGrenade();
		}
	}
}


void AOR_LauncherProjectile::DetonateMyGrenade()
{
	if (IsValid(Explosion))
	{
		if (IsValid(MainCharacter))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation(), FRotator::ZeroRotator, FVector(1.2f), true);//0.4*/
			UGameplayStatics::ApplyRadialDamage(GetWorld(), GrenadeBaseDamage, GetActorLocation(), ExplotionRadiusDamage, nullptr, ExplotionExceptions, this, MainCharacter->GetInstigatorController());
			BP_SpawnRadialForce();
		}
	}

	Destroy();
}










