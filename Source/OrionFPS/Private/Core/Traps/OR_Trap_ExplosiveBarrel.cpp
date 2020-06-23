// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Traps/OR_Trap_ExplosiveBarrel.h"
#include "Character/OR_MainCharacter.h"
#include "Components/OR_HealthComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AOR_Trap_ExplosiveBarrel::AOR_Trap_ExplosiveBarrel()
{
	bUseOverlapBurningLogic = false;
}

void AOR_Trap_ExplosiveBarrel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (bUseOverlapBurningLogic)
	{
		if (IsValid(OtherActor))
		{
			AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
			if (IsValid(Main))
			{
				UOR_HealthComponent* Health = Main->GetHealthComponent();
				Health->SetBurneable(true);
			}

		}
    }
}

void AOR_Trap_ExplosiveBarrel::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (bUseOverlapBurningLogic)
  {
	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (IsValid(Main))
		{
			UOR_HealthComponent* Health = Main->GetHealthComponent();
			Health->SetBurneable(false);
		}

	}
  }
}
