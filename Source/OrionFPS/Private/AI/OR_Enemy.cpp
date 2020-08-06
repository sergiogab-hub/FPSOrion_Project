// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OR_Enemy.h"
#include "AI/EnemyAnimInstance.h"
#include "Character/OR_MainCharacter.h"
#include "Projectiles/OR_BulletProjectile.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AOR_Enemy::AOR_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Player Camera Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), "WeaponSocket");


	LocoPoint0 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint0"));
	LocoPoint0->SetupAttachment(GetRootComponent());
	LocoPoint0->InitSphereRadius(15.f);

	LocoPoint1 = CreateDefaultSubobject<USphereComponent>(TEXT("LocoPoint1"));
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
	bIsIronFiringMoving = false;
	bIsIronFiringQuiet = false;
	bIsFiring = false;

	bHasNeedReload = false;
	bIsRealod = false;
	MaxMunition = 7;
	CurrentIndex = 0;

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

	CurrentMunition = MaxMunition;

	MainReference = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	GetWorld()->GetTimerManager().SetTimer(UpdateNavegationSystemHandle, this, &AOR_Enemy::UpdateNavegationSystem, 0.05, true, 0.0);

	InitialMinDistanceToRun = MinDistanceToRun;
}

// Called every frame
void AOR_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOR_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}





void AOR_Enemy::StartHipsFire()
{
	if (bIsHipsFiring) { return; }

	GetWorld()->GetTimerManager().SetTimer(ShootHipsHandle, this, &AOR_Enemy::Firing, 0.5 , true, 0.0);
	bIsHipsFiring = true;

	
}

void AOR_Enemy::StartIroSightFire(bool Moving)
{
	if (bIsIronFiringMoving || bIsIronFiringQuiet) { return;}

	if (!Moving)
	{
		GetWorld()->GetTimerManager().SetTimer(ShootIronHandle, this, &AOR_Enemy::Firing, 0.233, true, 0.0);
		bIsIronFiringQuiet = true;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ShootIronHandle, this, &AOR_Enemy::Shoot, 1.0, true, 1.0);
		bIsIronFiringMoving = true;
	}
	


	
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

				if (CurrentMunition <= 0)
				{
					bHasNeedReload = true;
					return;
				}
				else
				{
					OwnAnimInstance->Montage_Play(ShootIronSightMontage, 1.0f);
					OwnAnimInstance->Montage_JumpToSection("Default", ShootIronSightMontage);
					CurrentMunition--;
				}
				
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
	GetWorld()->GetTimerManager().ClearTimer(ShootIronHandle);

	if (bIsIronFiringMoving)
	{
		bIsIronFiringMoving = false;
	}
	if (bIsIronFiringQuiet)
	{
		bIsIronFiringQuiet = false;
	}
	
}

void AOR_Enemy::StopFiring()
{
	bIsFiring = false;
}

void AOR_Enemy::Shoot()
{
	if (CurrentMunition <= 0)
	{
		bHasNeedReload = true;
		return;
	}
	if (IsValid(BulletClass))
	{
		/*Get Socket Transforms*/
		FVector MuzzleLocation = Weapon->GetSocketLocation("muzzlee");
		FRotator MuzzleRotation = Weapon->GetSocketRotation("muzzlee");
		MuzzleRotation.Pitch = MuzzleRotation.Pitch + FMath::RandRange(-1, 1);
		MuzzleRotation.Yaw = MuzzleRotation.Yaw + FMath::RandRange(-5 , 0);
	

		/*Spawn Projectile*/
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(BulletClass, MuzzleLocation, MuzzleRotation);

		BP_Shoot();
	}

	/*FHitResult Hit;
	FVector MuzzleSocket = Weapon->GetSocketLocation("MuzzleSocket");

	FVector shakevector;     //	Dispersion de la balas con el rango que le mando del Shootimer2

	if (bIsIronFiringMoving)
	{
		shakevector.X = FMath::RandRange(FMath::RandRange(-105, -95), FMath::RandRange(105, 95));
		shakevector.Y = 100;
		shakevector.Z = FMath::RandRange(50, 70);
	}
	else
	{
		shakevector.X = FMath::RandRange(FMath::RandRange(-105, -95), FMath::RandRange(105, 95));
		shakevector.Y = 100;
		shakevector.Z = FMath::RandRange(50, 70);
	}


	AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld() ,0));

	if (IsValid(Main))
	{
		GetWorld()->LineTraceSingleByChannel(Hit, MuzzleSocket, Main->GetActorLocation() + shakevector, ECC_GameTraceChannel8);
		/*DrawDebugLine(GetWorld(),
			MuzzleSocket,
			Main->GetActorLocation() + shakevector,
			FColor::Red,
			false,
			3);
	}

	BP_Shoot(Hit.TraceEnd);

	if (IsValid(Hit.GetActor()))
	{
		if (Main == Hit.GetActor())
		{
			UGameplayStatics::ApplyPointDamage(Hit.GetActor(), 4, Hit.Location, Hit, this->GetInstigatorController(), this, nullptr);
		}
	}
	*/

	CurrentMunition--;
}

void AOR_Enemy::StartReload()
{
	if (IsValid(IronSightReload) && IsValid(OwnAnimInstance))
	{
		OwnAnimInstance->Montage_Play(IronSightReload, 1.0f);
		OwnAnimInstance->Montage_JumpToSection("Default", IronSightReload);
		bIsRealod = true;
		
	}
}

void AOR_Enemy::EndReload()
{
	bIsRealod = false;
	bHasNeedReload = false;
	CurrentMunition = MaxMunition;
}

FVector AOR_Enemy::CheckNavigationPoint(FVector TargetPosition)
{
	FVector PathStart = GetActorLocation();
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), PathStart, TargetPosition, NULL);

	if (NavPath->IsValid())
	{
		return TargetPosition;
	}
	else
	{
		float InterSpeed = 90.0;
		FVector InterpVector = FMath::VInterpTo(TargetPosition, GetActorLocation(), GetWorld()->GetDeltaSeconds(), InterSpeed);
		return InterpVector;
	}

}

FVector AOR_Enemy::GetMovementDirection(float PlayerDistance)
{
	FVector CurrentTargetLocation;

	if (PlayerDistance > MinDistanceToRun)
	{
		if (IsValid(MainReference))
		{
			CurrentTargetLocation = MainReference->GetActorLocation();
			return CurrentTargetLocation;
		}
		
	}

	if (PlayerDistance >= MinDistanceToStay && PlayerDistance <= MaxDistanceToStay)
	{
		CurrentTargetLocation=GetActorLocation();
		return CurrentTargetLocation;
	}


	if (PlayerDistance > MaxDistanceToStay)
	{
		// Left Side Locomotion

		if (MyYaw< 0 && MyYaw >-20)
		{

			CurrentTargetLocation=LocomotionPoints[2];
		
		}
		if (MyYaw < -20 && MyYaw > -40)
		{
			CurrentTargetLocation=LocomotionPoints[1];
		}
		if (MyYaw < -40)
		{
			CurrentTargetLocation=LocomotionPoints[0];
		}


		// Righ Side Locomotion

		if (MyYaw > 0 && MyYaw < 30)
		{
			CurrentTargetLocation=LocomotionPoints[5];
		}
		if (MyYaw > 30)
		{
			CurrentTargetLocation=LocomotionPoints[0];
		}

	}
	else
	{
		// Left Side Locomotion (Inverse)

		if (MyYaw > 0 && MyYaw < 20)
		{
			CurrentTargetLocation=LocomotionPoints[3];
		}
		if (MyYaw > 20 && MyYaw < 40)
		{
			CurrentTargetLocation=LocomotionPoints[4];
		}
		if (MyYaw > 40)
		{
			CurrentTargetLocation=LocomotionPoints[5];
		}


		// Righ Side Locomotion

		if (MyYaw < 0 && MyYaw >-30)
		{
			CurrentTargetLocation=LocomotionPoints[3];
		}
		if (MyYaw < -30)
		{
			CurrentTargetLocation=LocomotionPoints[2];
		}

	}

	
	return CheckNavigationPoint(CurrentTargetLocation);
}




void AOR_Enemy::UpdateNavegationSystem()
{
	LocomotionPoints[0] = LocoPoint0->GetComponentLocation();
	LocomotionPoints[1] = LocoPoint1->GetComponentLocation();
	LocomotionPoints[2] = LocoPoint2->GetComponentLocation();
	LocomotionPoints[3] = LocoPoint3->GetComponentLocation();
	LocomotionPoints[4] = LocoPoint4->GetComponentLocation();
	LocomotionPoints[5] = LocoPoint5->GetComponentLocation();
	LocomotionPoints[6] = LocoPoint6->GetComponentLocation();
	LocomotionPoints[7] = LocoPoint7->GetComponentLocation();
}

void AOR_Enemy::HitReact()
{
	if (IsValid(HitReactMontage) && IsValid(OwnAnimInstance))
	{
		OwnAnimInstance->Montage_Play(HitReactMontage, 1.0f);
		OwnAnimInstance->Montage_JumpToSection("Default", HitReactMontage);
	}
}

void AOR_Enemy::GetProportionalSpeed(float distance)
{
	if (distance > MinDistanceToRun)
	{
		GetCharacterMovement()->MaxWalkSpeed = 400;
		return;
	}
	if (distance > MaxDistanceToStay)
	{
		float TempMax = MinDistanceToRun - MaxDistanceToStay;

		GetCharacterMovement()->MaxWalkSpeed = 200 + ((distance - MaxDistanceToStay) * 100) / TempMax;
	}

	if (distance < MinDistanceToStay)
	{
		GetCharacterMovement()->MaxWalkSpeed = 250;
	}
}

