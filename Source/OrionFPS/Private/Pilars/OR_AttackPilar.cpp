// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilars/OR_AttackPilar.h"
#include "Character/OR_MainCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"

AOR_AttackPilar::AOR_AttackPilar()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxAttackPilarRate = 20.0f;
}

void AOR_AttackPilar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(MainCharacterReference))
	{

		if (!MainCharacterReference->GetHasAttackUltimateReady() && CurrentAttackPilarRate > 0 && !MainCharacterReference->GetIsOnPilarAttack())
		{
			if (!MainCharacterReference->GetIsAttackUltimate())
			{
				//Si no esta en Ulti se drena normalmente con la el valor indicado
				CurrentAttackPilarRate = FMath::Clamp(CurrentAttackPilarRate - (PilarDrainRate * DeltaTime), 0.0f, MaxAttackPilarRate);
			}
			else
			{
				//Si Activo la Ultimate especifica de esta torreta lo dreno super rapido
				CurrentAttackPilarRate = FMath::Clamp(CurrentAttackPilarRate - (PilarDrainRate + 10 * DeltaTime), 0.0f, MaxAttackPilarRate);

			}
		

		}
		else
		{
			if (MainCharacterReference->GetIsAttackUltimate())
			{
				//Si Activo la Ultimate especifica de esta torreta lo dreno super rapido
				CurrentAttackPilarRate = FMath::Clamp(CurrentAttackPilarRate - (PilarDrainRate + 10 * DeltaTime), 0.0f, MaxAttackPilarRate);
			}
			
		}

		MainCharacterReference->SetPilarAttackPercent(CurrentAttackPilarRate / MaxAttackPilarRate);
	}

}

void AOR_AttackPilar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent ,OtherActor,OtherComp,OtherBodyIndex , bFromSweep , SweepResult);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			Main->SetIsOnPilarAttack(true);
			GetWorld()->GetTimerManager().SetTimer(AttackRateHandle, this, &AOR_AttackPilar::StartAddRate, PilarAddRate, true, 0.0);
		}

	}

}

void AOR_AttackPilar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);

		if (IsValid(Main))
		{
			Main->SetIsOnPilarAttack(false);
			GetWorld()->GetTimerManager().ClearTimer(AttackRateHandle);
		}

	}

}


void AOR_AttackPilar::StartAddRate()
{
	if (IsValid(MainCharacterReference))
	{
		if (!MainCharacterReference->GetHasAttackUltimateReady())
		{
			
			if (CurrentAttackPilarRate + PilarAddRate > MaxAttackPilarRate)
			{
				CurrentAttackPilarRate = FMath::Clamp(CurrentAttackPilarRate + PilarAddRate, 0.0f, MaxAttackPilarRate);

				MainCharacterReference->SetHasAttackUltimateReady(true);

				if (MainCharacterReference->GetHasDefenceUltimateReady() || MainCharacterReference->GetHasMovilityUltimateReady())
				{
					MainCharacterReference->SetHasDefenceUltimateReady(false);
					MainCharacterReference->SetHasMovilityUltimateReady(false);
				}

				GetWorld()->GetTimerManager().ClearTimer(AttackRateHandle);
			}
			else
			{
				CurrentAttackPilarRate+= PilarAddRate;
			}
		}
		else
		{
	
			GetWorld()->GetTimerManager().ClearTimer(AttackRateHandle);
		}

	}
}

