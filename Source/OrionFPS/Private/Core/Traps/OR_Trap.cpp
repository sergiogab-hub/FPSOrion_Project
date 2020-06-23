// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Traps/OR_Trap.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"



// Sets default values
AOR_Trap::AOR_Trap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HitVolume = CreateDefaultSubobject <USphereComponent>(TEXT("HitVolume"));
	HitVolume->SetupAttachment(GetRootComponent());

	CollisionVolume = CreateDefaultSubobject <USphereComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(HitVolume);

	Mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(HitVolume);


	BaseDamage = 1.0f;
	RadiusDamage = 1.0f;
	TrapExceptions.Empty();


}

// Called when the game starts or when spawned
void AOR_Trap::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AOR_Trap::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AOR_Trap::OnOverlapEnd);
	HitVolume->OnComponentHit.AddDynamic(this, &AOR_Trap::OnHit);
}



void AOR_Trap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
}

void AOR_Trap::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AOR_Trap::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, GetActorLocation(), RadiusDamage, DamageType, TrapExceptions, this, GetInstigatorController());
		SpawnEmitter();
		Destroy();
	}
				
}

