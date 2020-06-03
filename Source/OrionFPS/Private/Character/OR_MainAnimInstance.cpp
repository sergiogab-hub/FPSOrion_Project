// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OR_MainAnimInstance.h"
#include "Character/OR_MainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


//Base Constructor
UMainAnimInstance::UMainAnimInstance()
{
	MovementSpeed = 0.0f;
	bIsInAir = false;
	VelocityZ = 0.0f;
}


////////////////////////////////////////////////////////////////////
//
//   Native Initialize
//
////////////////////////////////////////////////////////////////////
void UMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)   //Check if Pawn exist
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Main = Cast<AMainCharacter>(Pawn);
		}

	}
}


////////////////////////////////////////////////////////////////////
//
//   Update Animation Propertys-> Tick Funcion Version
//
////////////////////////////////////////////////////////////////////
void UMainAnimInstance::UpdateAnimationPropertys()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	
	if (Pawn)
	{
		if (Main == nullptr)
		{
			Main = Cast<AMainCharacter>(Pawn);
		}

		//Get Speed / SpeedZ
		FVector Speed = Pawn->GetVelocity();
		////////FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f); /////Debug
		MovementSpeed = Speed.Size();


		//// Jump Logics//////
		VelocityZ = Speed.Z;

		if (VelocityZ != 0.0)
		{

			// When is Jumpin  and Left Shift Still -> Keep Sprinting
			if (Main->bIsRuning)
			{
				Main->GetCharacterMovement()->MaxWalkSpeed = 500;
			}

			// When is Jumpin  and Right Mouse Still Pressed Pointed Idle
			if (Main->bIsPointed)
			{
				MovementSpeed = 0;
			}
		    	
		}

	}
}
