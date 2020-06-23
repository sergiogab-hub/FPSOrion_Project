// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Traps/OR_Trap_ExplosiveMine.h"
#include "Character/OR_MainCharacter.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AOR_Trap_ExplosiveMine::AOR_Trap_ExplosiveMine()
{
	bIsNear = false;
}

void AOR_Trap_ExplosiveMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (IsValid(Main))
		{
			FTimerDelegate AlertDelegate = FTimerDelegate::CreateUObject(this, &AOR_Trap_ExplosiveMine::CalculateDistance, OtherActor);
			GetWorld()->GetTimerManager().SetTimer(DistanceHandle, AlertDelegate, 0.2, true, 0.0);
			bIsNear = true;
		}
	}
}

void AOR_Trap_ExplosiveMine::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (IsValid(Main))
		{
			if (bIsNear)
			{
				bIsNear = false;
				GetWorldTimerManager().ClearTimer(DistanceHandle);
			}
		}
	}
}



void AOR_Trap_ExplosiveMine::CalculateDistance(AActor* TheOtherActor)
{
	float Distance = FVector::Dist(GetActorLocation(), TheOtherActor->GetActorLocation());
	AlertMessage(Distance);
}

