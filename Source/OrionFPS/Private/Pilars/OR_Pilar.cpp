// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilars/OR_Pilar.h"
#include "Character/OR_MainCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
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



}


// Called when the game starts or when spawned
void AOR_Pilar::BeginPlay()
{
	Super::BeginPlay();


	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AOR_Pilar::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AOR_Pilar::OnOverlapEnd);
	
}

// Called every frame
void AOR_Pilar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOR_Pilar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			if (MainCharacter == nullptr)
			{
				MainCharacter = Main;
			}

			ActivateTimer();
		}
	}
}

void AOR_Pilar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
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
			MainCharacter->AddAttackPilarRate();
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
		}

		break;
	case AOR_PilarType::PilarType_Defense:

		if (!MainCharacter->GetHasDefenceUltimateReady())
		{
		   MainCharacter->AddDefencePilarRate();
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(AddTorretRateHandle);
		}
		break;

	case AOR_PilarType::PilarType_Movility:

		if (!MainCharacter->GetHasMovilityUltimateReady())
		{
		   MainCharacter->AddMovilityPilarRate();
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


