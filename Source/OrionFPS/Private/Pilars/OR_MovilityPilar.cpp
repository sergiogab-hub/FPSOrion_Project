// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilars/OR_MovilityPilar.h"
#include "Character/OR_MainCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"


AOR_MovilityPilar::AOR_MovilityPilar()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxMovilityPilarRate = 20.0f;
}


void AOR_MovilityPilar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(MainCharacterReference))
	{

		if (!MainCharacterReference->GetHasMovilityUltimateReady() && CurrentMovilityPilarRate > 0 && !MainCharacterReference->GetIsOnPilarMovility())
		{
			if (!MainCharacterReference->GetIsMovilityUltimate())
			{
				//Si no esta en Ulti se drena normalmente con la el valor indicado
				CurrentMovilityPilarRate = FMath::Clamp(CurrentMovilityPilarRate - (PilarDrainRate * DeltaTime), 0.0f, MaxMovilityPilarRate);
			}
			else
			{
				//Si Activo la Ultimate especifica de esta torreta lo dreno super rapido
				CurrentMovilityPilarRate = FMath::Clamp(CurrentMovilityPilarRate - (PilarDrainRate + 10 * DeltaTime), 0.0f, MaxMovilityPilarRate);

			}


		}
		else
		{
			if (MainCharacterReference->GetIsMovilityUltimate())
			{
				//Si Activo la Ultimate especifica de esta torreta lo dreno super rapido
				CurrentMovilityPilarRate = FMath::Clamp(CurrentMovilityPilarRate - (PilarDrainRate + 10 * DeltaTime), 0.0f, MaxMovilityPilarRate);
			}

		}

		MainCharacterReference->SetPilarMovilityPercent(CurrentMovilityPilarRate / MaxMovilityPilarRate);
	}
}


void AOR_MovilityPilar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			Main->SetIsOnPilarMovility(true);
			GetWorld()->GetTimerManager().SetTimer(MovilityRateHandle, this, &AOR_MovilityPilar::StartAddRate, PilarAddRate, true, 0.0);
		}

	}
}

void AOR_MovilityPilar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			Main->SetIsOnPilarMovility(false);
			GetWorld()->GetTimerManager().ClearTimer(MovilityRateHandle);
		}

	}
}

void AOR_MovilityPilar::StartAddRate()
{
	if (IsValid(MainCharacterReference))
	{
		if (!MainCharacterReference->GetHasMovilityUltimateReady())
		{

			if (CurrentMovilityPilarRate + PilarAddRate > MaxMovilityPilarRate)
			{
				CurrentMovilityPilarRate = FMath::Clamp(CurrentMovilityPilarRate + PilarAddRate, 0.0f, MaxMovilityPilarRate);

				MainCharacterReference->SetHasMovilityUltimateReady(true);

				if (MainCharacterReference->GetHasDefenceUltimateReady() || MainCharacterReference->GetHasAttackUltimateReady())
				{
					MainCharacterReference->SetHasDefenceUltimateReady(false);
					MainCharacterReference->SetHasAttackUltimateReady(false);
				}

				GetWorld()->GetTimerManager().ClearTimer(MovilityRateHandle);
			}
			else
			{
				CurrentMovilityPilarRate += PilarAddRate;
			}
		}
		else
		{

			GetWorld()->GetTimerManager().ClearTimer(MovilityRateHandle);
		}

	}
}
