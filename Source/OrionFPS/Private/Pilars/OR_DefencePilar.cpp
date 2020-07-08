// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilars/OR_DefencePilar.h"
#include "Character/OR_MainCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"


AOR_DefencePilar::AOR_DefencePilar()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxDefencePilarRate = 20.0f;
}


void AOR_DefencePilar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(MainCharacterReference))
	{
		if (!MainCharacterReference->GetHasDefenceUltimateReady() && CurrentDefencePilarRate > 0 && !MainCharacterReference->GetIsOnPilarDefence())
		{
			if (!MainCharacterReference->GetIsDefenceUltimate())
			{
				//Si no esta en Ulti se drena normalmente con la el valor indicado
				CurrentDefencePilarRate = FMath::Clamp(CurrentDefencePilarRate - (PilarDrainRate * DeltaTime), 0.0f, MaxDefencePilarRate);
			}
			else
			{
				//Se Drena Rapido cuando activa la Ultia y no esta en el Pilar
				CurrentDefencePilarRate = FMath::Clamp(CurrentDefencePilarRate - (PilarDrainRate + 10 * DeltaTime), 0.0f, MaxDefencePilarRate);
			}
	
		}
		else
		{
			if (MainCharacterReference->GetIsDefenceUltimate())
			{
				//Se Drena Rapido cuando activa la Ulti cuando esta en el Pilar
				CurrentDefencePilarRate = FMath::Clamp(CurrentDefencePilarRate - (PilarDrainRate + 10 * DeltaTime), 0.0f, MaxDefencePilarRate);
            }
		}

		//Enviar el Percent al Main para imprimirlo
		MainCharacterReference->SetPilarDefencePercent(CurrentDefencePilarRate / MaxDefencePilarRate);
    }
	
}

void AOR_DefencePilar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			Main->SetIsOnPilarDefence(true);
			GetWorld()->GetTimerManager().SetTimer(DefenceRateHandle, this, &AOR_DefencePilar::StartAddRate, PilarAddRate, true, 0.0);
		}

	}
}

void AOR_DefencePilar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			Main->SetIsOnPilarDefence(false);
			GetWorld()->GetTimerManager().ClearTimer(DefenceRateHandle);
		}

	}
}

void AOR_DefencePilar::StartAddRate()
{
	if (IsValid(MainCharacterReference))
	{
		if (!MainCharacterReference->GetHasDefenceUltimateReady())
		{

			if (CurrentDefencePilarRate + PilarAddRate > MaxDefencePilarRate)
			{
				CurrentDefencePilarRate = FMath::Clamp(CurrentDefencePilarRate + PilarAddRate, 0.0f, MaxDefencePilarRate);

				MainCharacterReference->SetHasDefenceUltimateReady(true);

				if (MainCharacterReference->GetHasDefenceUltimateReady() || MainCharacterReference->GetHasMovilityUltimateReady())
				{
					MainCharacterReference->SetHasAttackUltimateReady(false);
					MainCharacterReference->SetHasMovilityUltimateReady(false);
				}

				GetWorld()->GetTimerManager().ClearTimer(DefenceRateHandle);
			}
			else
			{
				CurrentDefencePilarRate += PilarAddRate;
			}
		
		}
		else
		{
			
			GetWorld()->GetTimerManager().ClearTimer(DefenceRateHandle);
		}

	}
}