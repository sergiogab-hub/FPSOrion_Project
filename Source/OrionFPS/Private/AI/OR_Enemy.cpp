// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OR_Enemy.h"
#include "AI/EnemyAnimInstance.h"
#include "Character/OR_MainCharacter.h"
#include "Projectiles/OR_BulletProjectile.h"
#include "Components/OR_HealthComponent.h"
#include "AI/OR_AIBaseEnemyController.h"
#include "DamageTypes/MeleeDamageType.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AOR_Enemy::AOR_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Weapon AI Component

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), "WeaponSocket");

	// Locomotion Navigation points 

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


	//HealthComponent
	Health = CreateDefaultSubobject<UOR_HealthComponent>(TEXT("Health"));

	// Control Combat Variables
	bIsHipsFiring = false;
	bIsIronFiringMoving = false;
	bIsIronFiringQuiet = false;
	bIsFiring = false;
	bHasNeedReload = false;
	bIsRealod = false;


	//Munition Variables
	MaxMunition = 7;
	CurrentIndex = 0;

}

////////////////////////////////////////////////////////////////////
//
//   Begin Play
//
////////////////////////////////////////////////////////////////////

void AOR_Enemy::BeginPlay()
{
	Super::BeginPlay();

	SetupReferences();

	//Add Locomotion points to vector
	LocomotionPoints.Add(LocoPoint0->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint1->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint2->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint3->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint4->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint5->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint6->GetComponentLocation());
	LocomotionPoints.Add(LocoPoint7->GetComponentLocation());

	//Current Munition
	CurrentMunition = MaxMunition;

	//Distance to run
	InitialMinDistanceToRun = MinDistanceToRun;

	// HealthDelegate
	Health->OnHealthChangeDelegate.AddDynamic(this, &AOR_Enemy::OnHealthChange);

	//Navegation System Timer
	GetWorld()->GetTimerManager().SetTimer(UpdateNavegationSystemHandle, this, &AOR_Enemy::UpdateNavegationSystem, 0.05, true, 0.0);

}

void AOR_Enemy::SetupReferences()
{
	// AnimInstance Reference
	EnemyAnimInstaceReference = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	// Get player reference
	MainReference = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	//AI Controller Reference
	OwnController = Cast<AOR_AIBaseEnemyController>(GetController());
}
////////////////////////////////////////////////////////////////////
//
//   Event Tick 
//
////////////////////////////////////////////////////////////////////

void AOR_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


////////////////////////////////////////////////////////////////////
//
//  Character AI Movement 
//
////////////////////////////////////////////////////////////////////

// Check if Navigation points is valid  , if  is not , make interpolations to make a valid move.
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

// Get AI point to move
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
		CurrentTargetLocation = GetActorLocation();
		return CurrentTargetLocation;
	}


	if (PlayerDistance > MaxDistanceToStay)
	{
		// Left Side Locomotion

		if (MyYaw< 0 && MyYaw >-20)
		{

			CurrentTargetLocation = LocomotionPoints[2];

		}
		if (MyYaw < -20 && MyYaw > -40)
		{
			CurrentTargetLocation = LocomotionPoints[1];
		}
		if (MyYaw < -40)
		{
			CurrentTargetLocation = LocomotionPoints[0];
		}


		// Righ Side Locomotion

		if (MyYaw > 0 && MyYaw < 30)
		{
			CurrentTargetLocation = LocomotionPoints[5];
		}
		if (MyYaw > 30)
		{
			CurrentTargetLocation = LocomotionPoints[0];
		}

	}
	else
	{
		// Left Side Locomotion (Inverse)

		if (MyYaw > 0 && MyYaw < 20)
		{
			CurrentTargetLocation = LocomotionPoints[3];
		}
		if (MyYaw > 20 && MyYaw < 40)
		{
			CurrentTargetLocation = LocomotionPoints[4];
		}
		if (MyYaw > 40)
		{
			CurrentTargetLocation = LocomotionPoints[5];
		}


		// Righ Side Locomotion

		if (MyYaw < 0 && MyYaw >-30)
		{
			CurrentTargetLocation = LocomotionPoints[3];
		}
		if (MyYaw < -30)
		{
			CurrentTargetLocation = LocomotionPoints[2];
		}

	}


	return CheckNavigationPoint(CurrentTargetLocation);
}


//Delegate

void AOR_Enemy::OnHealthChange(UOR_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (DamageType)
	{
		const UMeleeDamageType* MeleeDamage = Cast<UMeleeDamageType>(DamageType);
		{
			if (MeleeDamage)
			{
				OwnController->UnPossess();
				BP_Death();
				EnemyAnimInstaceReference->SetBlendIronSightWeigth(1.0f);
				EnemyAnimInstaceReference->Montage_Play(DeathMontage, 1.0f);
				EnemyAnimInstaceReference->Montage_JumpToSection("Slot6", DeathMontage);
				AddActorLocalOffset(FVector(-120.0f, 0.0f, 0.0f));
				return;
			}
		}

		
	}


	if (IsValid(DamageCauser))
	{
		AProjectile* Bullet = Cast<AProjectile>(DamageCauser);

		if (IsValid(Bullet))
		{
		
			if (CurrentHealthComponent->GetIsDead())
			{
				if (IsValid(OwnController)&& DeathMontage)
				{
					OwnController->UnPossess();
					PlayMontageDeathAnimation();
					StopIronFire();
					BP_Death();
				}
			}
		}
		else
		{
			OwnController->UnPossess();
			StopIronFire();
			GetMesh()->SetCollisionProfileName("Pawn");
			GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
			GetMesh()->SetSimulatePhysics(true);
			BP_Death();
			
		}
	}
}

//Update Locomotions Navigations potition
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

// AI Speed proportional to Player position
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


////////////////////////////////////////////////////////////////////
//
//  Character Combat 
//
////////////////////////////////////////////////////////////////////

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
	if (IsValid(EnemyAnimInstaceReference))
	{

	  if (ShootHipsMontage && ShootIronSightMontage)
		{
			if (bIsHipsFiring)
			{
				EnemyAnimInstaceReference->Montage_Play(ShootHipsMontage, 1.0f);
				EnemyAnimInstaceReference->Montage_JumpToSection("Default", ShootHipsMontage);
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
					EnemyAnimInstaceReference->Montage_Play(ShootIronSightMontage, 1.0f);
					EnemyAnimInstaceReference->Montage_JumpToSection("Default", ShootIronSightMontage);
					CurrentMunition--;
				}
				
			}
		}
	  
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

void AOR_Enemy::PlayMontageDeathAnimation()
{
	if (IsValid(EnemyAnimInstaceReference))
	{
		EnemyAnimInstaceReference->SetBlendIronSightWeigth(1.0f);

		int32 Section = FMath::RandRange(1, 5);
		switch (Section)
		{
		case 1:
			EnemyAnimInstaceReference->Montage_Play(DeathMontage, 1.0);
			EnemyAnimInstaceReference->Montage_JumpToSection("Slot1", DeathMontage);
			break;
		case 2:
			EnemyAnimInstaceReference->Montage_Play(DeathMontage, 1.0);
			EnemyAnimInstaceReference->Montage_JumpToSection("Slot2", DeathMontage);
		
			break;
		case 3:
			EnemyAnimInstaceReference->Montage_Play(DeathMontage, 1.0);
			EnemyAnimInstaceReference->Montage_JumpToSection("Slot3", DeathMontage);
		
			break;
		case 4:
			EnemyAnimInstaceReference->Montage_Play(DeathMontage, 1.0);
			EnemyAnimInstaceReference->Montage_JumpToSection("Slot4", DeathMontage);

			break;
		case 5:
			EnemyAnimInstaceReference->Montage_Play(DeathMontage, 1.0);
			EnemyAnimInstaceReference->Montage_JumpToSection("Slot5", DeathMontage);
		
			break;
		default:
			break;
		}
	}

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

	CurrentMunition--;
}

void AOR_Enemy::StartReload()
{
	if (IsValid(IronSightReload) && IsValid(EnemyAnimInstaceReference))
	{
		EnemyAnimInstaceReference->Montage_Play(IronSightReload, 1.0f);
		EnemyAnimInstaceReference->Montage_JumpToSection("Default", IronSightReload);
		bIsRealod = true;
		
	}
}

void AOR_Enemy::EndReload()
{
	bIsRealod = false;
	bHasNeedReload = false;
	CurrentMunition = MaxMunition;
}


void AOR_Enemy::HitReact()
{
	if (IsValid(HitReactMontage) && IsValid(EnemyAnimInstaceReference))
	{
		EnemyAnimInstaceReference->Montage_Play(HitReactMontage, 1.0f);
		EnemyAnimInstaceReference->Montage_JumpToSection("Default", HitReactMontage);
	}
}
