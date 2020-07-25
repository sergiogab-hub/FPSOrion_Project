// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OR_Enemy.h"


#include "Engine/World.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AOR_Enemy::AOR_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Player Camera Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),  "WeaponSocket" );


	LocoPoint0 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint0"));
	LocoPoint0->SetupAttachment(GetRootComponent());
	LocoPoint0->InitSphereRadius(15.f);
		
	LocoPoint1 =CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint1"));
	LocoPoint1->SetupAttachment(GetRootComponent());
	LocoPoint1->InitSphereRadius(15.f);

	LocoPoint2 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint2"));
	LocoPoint2->SetupAttachment(GetRootComponent());
	LocoPoint2->InitSphereRadius(15.f);


	LocoPoint3 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint3"));
	LocoPoint3->SetupAttachment(GetRootComponent());
	LocoPoint3->InitSphereRadius(15.f);

	LocoPoint4 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint4"));
	LocoPoint4->SetupAttachment(GetRootComponent());
	LocoPoint4->InitSphereRadius(15.f);


	LocoPoint5 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint5"));
	LocoPoint5->SetupAttachment(GetRootComponent());
	LocoPoint5->InitSphereRadius(15.f);

	LocoPoint6 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint6"));
	LocoPoint6->SetupAttachment(GetRootComponent());
	LocoPoint6->InitSphereRadius(15.f);

	LocoPoint7 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint7"));
	LocoPoint7->SetupAttachment(GetRootComponent());
	LocoPoint7->InitSphereRadius(15.f);

	bIsHipsFiring = false;
	bIsIronSightFiring = false;

}

// Called when the game starts or when spawned
void AOR_Enemy::BeginPlay()
{
	Super::BeginPlay();

	// Set AnimInstance
    OwnAnimInstance = GetMesh()->GetAnimInstance();


	//LocomotionMap
	LocomotionPoints.Add(LocoPoint0->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint1->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint2->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint3->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint4->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint5->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint6->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint7->GetComponentLocation());


	EnemyAnimInstance = GetMesh()->GetAnimInstance();

}

// Called every frame
void AOR_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LocomotionPoints[0] = LocoPoint0->GetComponentLocation();
	LocomotionPoints[1] = LocoPoint1->GetComponentLocation();
	LocomotionPoints[2] = LocoPoint2->GetComponentLocation();
	LocomotionPoints[3] = LocoPoint3->GetComponentLocation();
	LocomotionPoints[4] = LocoPoint4->GetComponentLocation();
	LocomotionPoints[5] = LocoPoint5->GetComponentLocation();
	LocomotionPoints[6] = LocoPoint6->GetComponentLocation();
	LocomotionPoints[7] = LocoPoint7->GetComponentLocation();



}

// Called to bind functionality to input
void AOR_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOR_Enemy::StartHipsFire()
{
	if (bIsHipsFiring) { return; }

	GetWorld()->GetTimerManager().SetTimer(ShootHipsHandle, this, &AOR_Enemy::Firing, 0.25, true, 0.0);
	bIsHipsFiring = true;
}

void AOR_Enemy::StartIroSightFire()
{
	if (bIsIronSightFiring) { return; }

	GetWorld()->GetTimerManager().SetTimer(ShootIronHandle, this, &AOR_Enemy::Firing, 0.25, true, 0.0);
	bIsIronSightFiring = true;

}

void AOR_Enemy::Firing()
{
	if (IsValid(OwnAnimInstance))
	{
		if (ShootHipsMontage && ShootIronSightMontage)
		{
			if (bIsHipsFiring)
			{
				OwnAnimInstance->Montage_Play(ShootHipsMontage, 1.0f);
				OwnAnimInstance->Montage_JumpToSection("Default", ShootHipsMontage);
			}
			else
			{
				OwnAnimInstance->Montage_Play(ShootIronSightMontage, 1.0f);
				OwnAnimInstance->Montage_JumpToSection("Default", ShootIronSightMontage);
			}
		}
	  
	}

}

void AOR_Enemy::StopHipsFire()
{
	if (bIsHipsFiring)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootHipsHandle);
		bIsHipsFiring = false;
	}

	
}

void AOR_Enemy::StopIronFire()
{
	if(bIsIronSightFiring)
	{
	GetWorld()->GetTimerManager().ClearTimer(ShootIronHandle);
	bIsIronSightFiring = false;
	}
}

FVector AOR_Enemy::GetMovementDirection(float PlayerDistance)
{

	if (PlayerDistance > 1800)
	{
		return LocomotionPoints[0];
	}

	if (PlayerDistance >= 800 && PlayerDistance <= 1000)
	{
		return GetActorLocation();
	}


	if (PlayerDistance > 1000)
	{
		// Left Side Locomotion

		if (MyYaw< 0 && MyYaw >-20)
		{
			return   LocomotionPoints[2];
		}
		if (MyYaw < -20 && MyYaw > -40)
		{
			return   LocomotionPoints[1];
		}
		if (MyYaw < -40)
		{
			return   LocomotionPoints[0];
		}


		// Righ Side Locomotion

		if (MyYaw > 0 && MyYaw < 30)
		{
			return   LocomotionPoints[5];
		}
		if (MyYaw > 30)
		{
			return   LocomotionPoints[0];
		}

	}
	else
	{
		// Left Side Locomotion (Inverse)

		if (MyYaw > 0 && MyYaw < 20)
		{
			return   LocomotionPoints[3];
		}
		if (MyYaw > 20 && MyYaw < 40)
		{
			return   LocomotionPoints[4];
		}
		if (MyYaw > 40)
		{
			return   LocomotionPoints[5];
		}


		// Righ Side Locomotion

		if (MyYaw < 0 && MyYaw >-30)
		{
			return   LocomotionPoints[3];
		}
		if (MyYaw < -30)
		{
			return   LocomotionPoints[2];
		}

	}



	return GetActorLocation();
}

