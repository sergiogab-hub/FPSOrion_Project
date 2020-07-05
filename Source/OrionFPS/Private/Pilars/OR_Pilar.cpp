// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilars/OR_Pilar.h"
#include "Character/OR_MainCharacter.h"
#include "AI/OR_Enemy.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOR_Pilar::AOR_Pilar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject <UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(GetRootComponent());

	MeshSphere = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshSphere"));
	MeshSphere->SetupAttachment(GetRootComponent());

	MeshCilinder = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshCilinder"));
	MeshCilinder->SetupAttachment(MeshSphere);

	MeshCone = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshCone"));
	MeshCone->SetupAttachment(MeshSphere);


	bHasMovementEnemy = false;
	bHasBackIdle = false;
	PilarInterpolationSpeed = 3.0;
	MaxPilarRateState = 20;
	PilarDrainRate = 0.5;
	
}


// Called when the game starts or when spawned
void AOR_Pilar::BeginPlay()
{
	Super::BeginPlay();

	IdlePosition = MeshSphere->GetComponentRotation();

	
	AMainCharacter* Main = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (IsValid(Main))
	{
		MainCharacter = Main;
	}
	if (IsValid(MainCharacter))
	{
		
	}


	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AOR_Pilar::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AOR_Pilar::OnOverlapEnd);
	
}

// Called every frame
void AOR_Pilar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHasMovementEnemy)
	{
		if (IsValid(CurrentEnemyTarget))
		{
			RotationTorret = UKismetMathLibrary::FindLookAtRotation(MeshSphere->GetComponentLocation(), CurrentEnemyTarget->GetActorLocation());
			FRotator InterpRotation = FMath::RInterpTo(MeshSphere->GetComponentRotation(), RotationTorret, DeltaTime, PilarInterpolationSpeed);
			MeshSphere->SetWorldRotation(InterpRotation);
		}
	}

	if (bHasBackIdle)
	{
		FRotator InterpRotation = FMath::RInterpTo(MeshSphere->GetComponentRotation(), IdlePosition, DeltaTime, PilarInterpolationSpeed);
		MeshSphere->SetWorldRotation(InterpRotation);

		/** Stop Movement if is Already in Idle*/
		if (MeshSphere->GetComponentRotation() == IdlePosition)
		{
			bHasBackIdle = false;
		}
	}

	if (!MainCharacter->GetHasAttackUltimateReady() && AttackPilarRateState > 0 && !MainCharacter->GetIsOnPilarAttack())
	{
		AttackPilarRateState = FMath::Clamp(AttackPilarRateState - (PilarDrainRate * DeltaTime), 0.0f, MaxPilarRateState);
	}


	if (!MainCharacter->GetHasDefenceUltimateReady() && DefencePilarRateState > 0 && !MainCharacter->GetIsOnPilarDefence())
	{
		DefencePilarRateState = FMath::Clamp(DefencePilarRateState - (PilarDrainRate * DeltaTime), 0.0f, MaxPilarRateState);
	}


	if (!MainCharacter->GetHasMovilityUltimateReady() && MovilityPilarRateState > 0 && !MainCharacter->GetIsOnPilarMovility())
	{
		MovilityPilarRateState = FMath::Clamp(MovilityPilarRateState - (PilarDrainRate * DeltaTime), 0.0f, MaxPilarRateState);
	}

}

void AOR_Pilar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			if (IsValid(MainCharacter))
			{
				SetCharacterOverlapState(true);
				ActivateTimer();
			}	
		}

		AOR_Enemy* Enemy = Cast<AOR_Enemy>(OtherActor);

		if (IsValid(Enemy))
		{
			EnemyArray.Add(Enemy);

			if (bIsInAttack && CurrentEnemyTarget == nullptr)
			{
				ActivateUltimate();
			}
		}
	}
}


void AOR_Pilar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (IsValid(Main))
		{
			SetCharacterOverlapState(false);
			GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
		}

		AOR_Enemy* Enemy = Cast<AOR_Enemy>(OtherActor);

		if (IsValid(Enemy))
		{

			EnemyArray.Remove(Enemy);

			if (Enemy == CurrentEnemyTarget && bIsInAttack)
			{
				StartMovementFunction();
			}
		}
	}
}


void AOR_Pilar::ActivateUltimate()
{
	StartMovementFunction();
	if (bHasMovementEnemy)
	{
		BP_StartFire();
	}
	bIsInAttack = true;
}

void AOR_Pilar::DeactivateUltimate()
{
	//StopMovement();
	//bIsInAttack = false;
}



void AOR_Pilar::StartMovementFunction()
{
	if (GetNewTargertEnemy())
	{
		if (bHasBackIdle)
		{
			bHasBackIdle = false;
		}

		bHasMovementEnemy = true;
	}
	else
	{
		if (bHasMovementEnemy)
		{
			bHasMovementEnemy = false;
		}

		bHasBackIdle = true;
	}

}

void AOR_Pilar::StartFire()
{
	BP_StartFire();
}

void AOR_Pilar::StopFire()
{
	BP_StoptFire();
}

void AOR_Pilar::StopMovement()
{
	if (bHasMovementEnemy)
	{
		bHasMovementEnemy = false;
	}

	if (CurrentEnemyTarget != nullptr)
	{
		CurrentEnemyTarget = nullptr;
	}

	bHasBackIdle = true;
}



void AOR_Pilar::SetCharacterOverlapState(bool state)
{
	if (GetPylarType() == AOR_PilarType::PilarType_Attack)
	{
		MainCharacter->SetIsOnPilarAttack(state);
		return;
	}
	if (GetPylarType() == AOR_PilarType::PilarType_Defence)
	{
		MainCharacter->SetIsOnPilarDefence(state);
		return;
	}
	if (GetPylarType() == AOR_PilarType::PilarType_Movility)
	{
		MainCharacter->SetIsOnPilarMovility(state);
		return;
	}
}

bool AOR_Pilar::GetNewTargertEnemy()
{
	if (EnemyArray.Num() > 0)
	{

		CurretRandArrayEnemy = FMath::RandRange(0, EnemyArray.Num() - 1);

		CurrentEnemyTarget = EnemyArray[CurretRandArrayEnemy];

		return true;
	}
	else
	{
		if (CurrentEnemyTarget != nullptr)
		{
			CurrentEnemyTarget = nullptr;
		}

		return false;
	}
	
}



void AOR_Pilar::ActivateTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AddTorretRateHandle, this, &AOR_Pilar::SendTorretType, 1.0, true, 0.0);
}

void AOR_Pilar::SendTorretType()
{
	switch (PylarType)
	{
	case AOR_PilarType::PilarType_Attack:

		if (!MainCharacter->GetHasAttackUltimateReady())
		{
			if (AttackPilarRateState + 1 > MaxPilarRateState)
			{
				AttackPilarRateState = FMath::Clamp(AttackPilarRateState + 1, 0.0f, MaxPilarRateState);

				MainCharacter->SetHasAttackUltimateReady(true);

				if (MainCharacter->GetHasDefenceUltimateReady() || MainCharacter->GetHasMovilityUltimateReady())
				{
					MainCharacter->SetHasDefenceUltimateReady(false);
					MainCharacter->SetHasMovilityUltimateReady(false);
				}
					
				GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
			}
			else
			{
				AttackPilarRateState++;
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
		}

		break;

	case AOR_PilarType::PilarType_Defence:

		if (!MainCharacter->GetHasDefenceUltimateReady())
		{
			if (DefencePilarRateState + 1 > MaxPilarRateState)
			{
				DefencePilarRateState = FMath::Clamp(DefencePilarRateState + 1, 0.0f, MaxPilarRateState);

				MainCharacter->SetHasDefenceUltimateReady(true);

				if (MainCharacter->GetHasAttackUltimateReady() || MainCharacter->GetHasMovilityUltimateReady())
				{
					MainCharacter->SetHasAttackUltimateReady(false);
					MainCharacter->SetHasMovilityUltimateReady(false);
				}

				GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
			}
			else
			{
				DefencePilarRateState++;
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
		}

		break;

	case AOR_PilarType::PilarType_Movility:

		if (!MainCharacter->GetHasMovilityUltimateReady())
		{
			if (MovilityPilarRateState + 1 > MaxPilarRateState)
			{
				MovilityPilarRateState = FMath::Clamp(MovilityPilarRateState + 1, 0.0f, MaxPilarRateState);

				MainCharacter->SetHasMovilityUltimateReady(true);

				if (MainCharacter->GetHasAttackUltimateReady() || MainCharacter->GetHasDefenceUltimateReady())
				{
					MainCharacter->SetHasAttackUltimateReady(false);
					MainCharacter->SetHasDefenceUltimateReady(false);
				}

				GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
			}
			else
			{
				MovilityPilarRateState++;
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
		}

		break;
	default:
		break;
	}
}


