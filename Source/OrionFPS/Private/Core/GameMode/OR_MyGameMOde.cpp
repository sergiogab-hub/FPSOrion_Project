// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/OR_MyGameMOde.h"
#include "Character/OR_MainCharacter.h"
#include "Core/Camera/OR_SpectetingCamera.h"

#include "Engine//World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


void AOR_MyGameMOde::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();
	
}

void AOR_MyGameMOde::SetupSpectatingCameras()
{
	TArray <AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOR_SpectetingCamera::StaticClass(), SpectatingCameraActors);

	if (SpectatingCameraActors.Num() > 0)
	{
		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			AOR_SpectetingCamera* SpectatingCamera = Cast<AOR_SpectetingCamera>(SpectatingActor);

			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case  EOR_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;

				case  EOR_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;

				default:
					break;
				}
			}
		}
	}
}

void AOR_MyGameMOde::MoveCameratoSpectatingPoint(AOR_SpectetingCamera* SpectatingCamera , AMainCharacter* Main)
{
	if (!IsValid(SpectatingCamera) || !IsValid(Main))
	{
		return;
	}

	AController* CharacterController = Main->GetController();
	if (IsValid(CharacterController))
	{
		APlayerController* PlayerController = Cast <APlayerController>(CharacterController);
		if (IsValid(PlayerController))
		{
				PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}
}

void AOR_MyGameMOde::Victory(AMainCharacter* Main)
{
	Main->DisableInput(nullptr);
	MoveCameratoSpectatingPoint(VictoryCamera, Main);
	BP_Victory(Main);
}
void AOR_MyGameMOde::GameOver(AMainCharacter* Main)
{
	Main->GetMovementComponent()->StopMovementImmediately();
	Main->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Main->HasToDestroy())
	{
		Main->DetachFromControllerPendingDestroy();
		Main->SetLifeSpan(5);
	}
	else
	{
		Main->DisableInput(nullptr);
		MoveCameratoSpectatingPoint(GameOverCamera, Main);
	}

	BP_GameOver(Main);
}
