// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAnimInstance.h"
#include "AI/OR_Enemy.h"
#include "Character/OR_MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


UEnemyAnimInstance::UEnemyAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;



}



void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)   //Check if Pawn exist
	{
		Pawn = TryGetPawnOwner();
		if (IsValid(Pawn))
		{
			Owner = Cast<AOR_Enemy>(Pawn);
		}

	}
}

void UEnemyAnimInstance::UpdateAnimationPropertys()
{

	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (IsValid(Pawn))
	{
		if (Owner == nullptr)
		{
			Owner= Cast<AOR_Enemy>(Pawn);

		}

	}

	if (IsValid(Owner))
	{
		Owner->SetMyPitch(OwnPitch);
		Owner->SetMyYaw(OwnYaw);
	}

}
