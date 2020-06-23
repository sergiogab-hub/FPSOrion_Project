// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/OR_MyGameMOde.h"
#include "Character/OR_MainCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


void AOR_MyGameMOde::Victory(AMainCharacter* Main)
{
	Main->DisableInput(nullptr);
	BP_Victory(Main);
}
void AOR_MyGameMOde::GameOver(AMainCharacter* Main)
{
	Main->GetMovementComponent()->StopMovementImmediately();
	Main->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Main->DetachFromControllerPendingDestroy();
	Main->SetLifeSpan(5);

	BP_GameOver(Main);
}
