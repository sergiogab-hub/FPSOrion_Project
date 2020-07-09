// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilars/OR_BasePilar.h"
#include "Character/OR_MainCharacter.h"
#include "AI/OR_Enemy.h"


#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AOR_BasePilar::AOR_BasePilar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionVolume = CreateDefaultSubobject <UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(GetRootComponent());

	RootComponent = CollisionVolume;

	MeshSphere = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshSphere"));
	MeshSphere->SetupAttachment(GetRootComponent());

	MeshCilinder = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshCilinder"));
	MeshCilinder->SetupAttachment(MeshSphere);

	MeshCone = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshCone"));
	MeshCone->SetupAttachment(MeshSphere);

	MinPilarInterpMovementSpeed = 1.0f;
	MaxInterpMovementSpeed = 40.0f;
	PilarTickRate = 0.04f;
}

// Called when the game starts or when spawned
void AOR_BasePilar::BeginPlay()
{
	Super::BeginPlay();

	IdleRotation= MeshSphere->GetComponentRotation();

	AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (IsValid(Main))
	{
		MainCharacterReference = Main;
	}

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AOR_BasePilar::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AOR_BasePilar::OnOverlapEnd);
}



void AOR_BasePilar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOR_Enemy* Enemy = Cast<AOR_Enemy>(OtherActor);

	if (IsValid(Enemy))
	{
		EnemyArray.Add(Enemy);

		if (bIsOnUltimate && CurrentEnemyTarget == nullptr)
		{
			StartMovement();
		}
	}
}

void AOR_BasePilar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AOR_Enemy* Enemy = Cast<AOR_Enemy>(OtherActor);

	if (IsValid(Enemy))
	{
		EnemyArray.Remove(Enemy);

		if (bIsOnUltimate && CurrentEnemyTarget == Enemy)
		{
			StartMovement();
		}
	}
}


void AOR_BasePilar::StartUltimate()
{
	bIsOnUltimate = true;

	StartMovement();
}

void AOR_BasePilar::StopUltimate()
{
	bIsOnUltimate = false;
	StartMovement();
}

////////////////////////////////////////////////////////////////////
//
//  Movement
//
////////////////////////////////////////////////////////////////////

void AOR_BasePilar::StartMovement()
{
	if (GetWorldTimerManager().IsTimerActive(MovemenetTorretHandle))
	{
		StopMovement();
	}

	CurrentPilarInterpMovementSpeed = MinPilarInterpMovementSpeed;

	if (bIsOnUltimate)
	{
		if (GetNewTargetEnemy())
		{
			GetWorld()->GetTimerManager().SetTimer(MovemenetTorretHandle, this, &AOR_BasePilar::MoveToEnemy, PilarTickRate, true, 0.0);

			GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &AOR_BasePilar::StartShoot, 4.0f, true , 0.0f);
		}
		else
		{
			if (MeshSphere->GetComponentRotation() != IdleRotation)
			{
				GetWorld()->GetTimerManager().SetTimer(MovemenetTorretHandle, this, &AOR_BasePilar::MoveToIdle, PilarTickRate, true, 0.0);
				StopShoot();
			}
		}
	}
	else
	{
		if (MeshSphere->GetComponentRotation() != IdleRotation)
		{
			GetWorld()->GetTimerManager().SetTimer(MovemenetTorretHandle, this, &AOR_BasePilar::MoveToIdle, PilarTickRate, true, 0.0);
			StopShoot();
		}
	}

	
}


void AOR_BasePilar::StopMovement()
{

	GetWorld()->GetTimerManager().ClearTimer(MovemenetTorretHandle);

}

bool AOR_BasePilar::GetNewTargetEnemy()
{
	if (EnemyArray.Num() > 0)
	{

		CurrentIndexEnemyArray = FMath::RandRange(0, EnemyArray.Num() - 1);

		CurrentEnemyTarget = EnemyArray[CurrentIndexEnemyArray];

		return true;
	}
	else
	{

		CurrentEnemyTarget = nullptr;
		return false;
	}

}


void AOR_BasePilar::MoveToEnemy()
{
	if (IsValid(CurrentEnemyTarget))
	{
		if (CurrentPilarInterpMovementSpeed < MaxInterpMovementSpeed)
		{
			CurrentPilarInterpMovementSpeed =FMath::Clamp(CurrentPilarInterpMovementSpeed + 0.5f, MinPilarInterpMovementSpeed, MaxInterpMovementSpeed);
		}
		FRotator RotationTorret = UKismetMathLibrary::FindLookAtRotation(MeshSphere->GetComponentLocation(), CurrentEnemyTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(MeshSphere->GetComponentRotation(), RotationTorret, GetWorld()->GetDeltaSeconds(), CurrentPilarInterpMovementSpeed);
		MeshSphere->SetWorldRotation(InterpRotation);
	}
}

void AOR_BasePilar::MoveToIdle()
{
	/** Stop Movement if is Already in Idle*/
	if (MeshSphere->GetComponentRotation() == IdleRotation)
	{
		StopMovement();
		return;
	}

	if (CurrentPilarInterpMovementSpeed < MaxInterpMovementSpeed)
	{
		CurrentPilarInterpMovementSpeed = FMath::Clamp(CurrentPilarInterpMovementSpeed + 0.5f, MinPilarInterpMovementSpeed, MaxInterpMovementSpeed);
	}
	FRotator InterpRotation = FMath::RInterpTo(MeshSphere->GetComponentRotation(), IdleRotation, GetWorld()->GetDeltaSeconds(), CurrentPilarInterpMovementSpeed);
	MeshSphere->SetWorldRotation(InterpRotation);
}

////////////////////////////////////////////////////////////////////
//
//   Shoot
//
////////////////////////////////////////////////////////////////////

void AOR_BasePilar::StartShoot()
{
	if (IsValid(ShootTorretParticle))
	{
		ShootParticleSystemComponent=UGameplayStatics::SpawnEmitterAttached(ShootTorretParticle, MeshCone, FName("None"), FVector(ForceInitToZero), MeshCone->GetRelativeRotation(), FVector(2.0f, 2.0f, 2.0f));
		GetWorld()->GetTimerManager().SetTimer(ColliderSpawnHandle, this, &AOR_BasePilar::SpawnCollider, 3.8, false, 3.8);
		
	}

}

void AOR_BasePilar::StopShoot()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootHandle);

	if (IsValid(ShootParticleSystemComponent))
	{
		GetWorld()->GetTimerManager().ClearTimer(ColliderSpawnHandle);
		ShootParticleSystemComponent->DeactivateSystem();
	}

}

void AOR_BasePilar::SpawnCollider()
{
	if (IsValid(Collider))
	{
		GetWorld()->SpawnActor<AActor>(Collider, MeshCone->GetComponentLocation(), MeshSphere->GetComponentRotation());
		BP_CameraShake();

	}
	
}

