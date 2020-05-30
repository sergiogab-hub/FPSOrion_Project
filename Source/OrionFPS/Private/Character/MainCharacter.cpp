// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Base Constructor
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Player Camera Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh());

	// Player Spring Arm Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Camera);

	// Player Arms Mesh
	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(SpringArm);

	// Player Weapon Mesh
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Arms, "R_GunSocket");


	// Set Initial Default Movement Status
	MovementStatus = EMovementStatus::EMS_Idle;


	// State Movement Variables
	RotationSpeed = 45.0f;
	bIsRuning = false;
	bIsMoving = false;
	bIsPointed = false;
	bIsJumping = false;


	//Set Default Current Velocity
	CurrentVelocity = 0.f;


	// Open/Close Variables
	bIsSprintCalled = false;
	bIsPointedCalled = false;
	bIsJumpCalled = false;
	bIsDelay = false;


	//Combar Varaibles
	bIsShooting = false;
	bIsUnderShootCalled = false;
	bIsPointedShootCalled = false;
	
}


////////////////////////////////////////////////////////////////////
//
//   Begin Play
//
////////////////////////////////////////////////////////////////////
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


////////////////////////////////////////////////////////////////////
//
//   Character Player Input Movement
//
////////////////////////////////////////////////////////////////////

// Player Basic Movement Inputs Handles 
void AMainCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}
void AMainCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}
void AMainCharacter::RotatePitch(float value)
{
	AddControllerPitchInput(value * RotationSpeed * GetWorld()->GetDeltaSeconds());
}
void AMainCharacter::RotateYaw(float value)
{
	AddControllerYawInput(value * RotationSpeed * GetWorld()->GetDeltaSeconds());
}


// Star / End - Jump Space Bar Method
void AMainCharacter::StarJump()
{
	Jump();
}
void AMainCharacter::EndJump()
{
	StopJumping();
}


// Star / End - Sprint Left Shift Method
void AMainCharacter::StarSprint()
{
	bIsRuning = true;
}
void AMainCharacter::StopSprint()
{
	bIsRuning = false;
}


// Star / End - Gun Pointed Right Mouse Method
void AMainCharacter::StarGunPoint()
{
	bIsPointed = true;

}
void AMainCharacter::EndGunPoint()
{
	bIsPointed = false;
}


// Star/End Shoot
void AMainCharacter::StarShoot()
{
	bIsShooting = true;

	GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &AMainCharacter::Shoot, 0.095, true, 0.0);
}

void AMainCharacter::EndShoot()
{
	bIsShooting = false;
	
	GetWorldTimerManager().ClearTimer(ShootHandle);
}


////////////////////////////////////////////////////////////////////
//
//   Basic Event Tick
//
////////////////////////////////////////////////////////////////////
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get CurretVelocity every tick
	CurrentVelocity = GetVelocity().Size();

	//Check Main Basic Variables//
	CheckCurrentVariables();

	//Check/Set Current Movement Status //
	SetCurrentStatus();

	//Set Current Movement Status//
	SetCameraMovement();

}


////////////////////////////////////////////////////////////////////
//
//   Movement Internal Function
//
////////////////////////////////////////////////////////////////////

// Check Movement/Jump State
void AMainCharacter::CheckCurrentVariables()
{
	////Set Moving Control Varible/////
	if (CurrentVelocity != 0)
	{
		bIsMoving = true;

	}
	else
	{
		bIsMoving = false;
	}

	///// Set Jumping Control ////
	if (GetVelocity().Z != 0)
	{
		if (!bIsJumpCalled)
		{
			bIsJumping = true;
			bIsJumpCalled = true;
			bIsDelay = true;   /// Delay Control Sprint/Jump Variable - Set by C++ and BP

			BP_StarCameraJump();
		}
	}
	else
	{
		if (bIsJumpCalled)
		{
			bIsJumping = false;
			bIsJumpCalled = false;

			BP_EndCameraJump();
		}
	}
}

// Set Current Movement Status -> Idle/Walk/Sprint/Pointed
void AMainCharacter::SetCurrentStatus()
{
	///////////// Combat Status ///////////////////
	if (!bIsShooting)
	{
		SetCombatStatus(ECombatStatus::EMS_NoCombat);
	}
	if (bIsShooting && bIsPointed)
	{
		SetCombatStatus(ECombatStatus::EMS_PointedFire);
	}
	if (bIsShooting && !bIsPointed)
	{
		SetCombatStatus(ECombatStatus::EMS_FireUnder);
	}


	///////////// Movement Status ////////////////
	if ((bIsMoving && !bIsRuning && !bIsPointed ) || (bIsMoving && !bIsPointed && GetCombatStatus()==ECombatStatus::EMS_FireUnder ))
	{
		SetMovementStatus(EMovementStatus::EMS_Walking);
	}
	if (bIsMoving && bIsRuning && !bIsPointed && !bIsDelay && GetCombatStatus() != ECombatStatus::EMS_FireUnder )
	{
		SetMovementStatus(EMovementStatus::EMS_Sprinting);
	}
	if (bIsPointed)
	{
		SetMovementStatus(EMovementStatus::EMS_Pointing);
	}
	if (!bIsMoving && !bIsPointed || bIsDelay && !bIsPointed )
	{
		SetMovementStatus(EMovementStatus::EMS_Idle);
	}

	
}

// Set Camera Movement -> Sprint/Pointed/
void AMainCharacter::SetCameraMovement()
{

	//Activa/Deactive -> Camera Shoot Under
	if (GetCombatStatus() == ECombatStatus::EMS_FireUnder)
	{
		if (!bIsUnderShootCalled)
		{
			BP_StarUnderShoot();
			bIsUnderShootCalled = true;
		}
	}
	else
	{
		if (bIsUnderShootCalled)
		{
			BP_EndUnderShoot();
			bIsUnderShootCalled = false;

		}
	}

	//Activa/Deactive -> Camera Shoot Under
	if (GetCombatStatus() == ECombatStatus::EMS_PointedFire)
	{
		if (!bIsPointedShootCalled)
		{
			BP_StarPointedShoot();
			bIsPointedShootCalled = true;
		}
	}
	else
	{
		if (bIsPointedShootCalled)
		{
			BP_EndPointedShoot();
			bIsPointedShootCalled = false;

		}
	}



	//Activa/Deactive -> Camera Sprinting Movements
	if (GetMovementStatus() == EMovementStatus::EMS_Sprinting)
	{
		if (!bIsSprintCalled)
		{
			bIsSprintCalled = true;

			BP_StarCameraSprint();
			GetCharacterMovement()->MaxWalkSpeed = 700;
		}

	}
	else
	{
		if (bIsSprintCalled)
		{
			bIsSprintCalled = false;

			BP_EndCameraSprint();
			GetCharacterMovement()->MaxWalkSpeed = 350;
		}
	}


	//Activa/Deactive -> Camera Pointed Movements
	if (GetMovementStatus() == EMovementStatus::EMS_Pointing)
	{
		if (!bIsPointedCalled)
		{
			BP_StarCameraPointed();
			bIsPointedCalled = true;
			SpringArm->bEnableCameraLag = false;
		}
	}
	else
	{
		if (bIsPointedCalled)
		{
			BP_EndCameraPointed();
			bIsPointedCalled = false;
			SpringArm->bEnableCameraLag = true;
		}
	}

}

void AMainCharacter::Shoot()
{
	UAnimInstance* AnimInstance = Arms->GetAnimInstance();
	if (AnimInstance && ShootMontage)
	{
		if (GetMovementStatus() == EMovementStatus::EMS_Pointing)
		{
			int32 Section = FMath::RandRange(1, 2);
			switch (Section)
			{
			case 1:
				AnimInstance->Montage_Play(PointedShoot_Montage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("PShoot01"), PointedShoot_Montage);
				break;
			case 2:
				AnimInstance->Montage_Play(PointedShoot_Montage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("PShoot02"), PointedShoot_Montage);
				break;
			default:
				break;
			}
		}
		else
		{
			int32 Section = FMath::RandRange(1, 2);
			switch (Section)
			{
			case 1:
				AnimInstance->Montage_Play(ShootMontage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("Shoot01"), ShootMontage);
				break;
			case 2:
				AnimInstance->Montage_Play(ShootMontage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("Shoot02"), ShootMontage);
				break;
			default:
				break;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////
//
//   Player input handling
//
////////////////////////////////////////////////////////////////////
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);  //Check Macro


	// Axis mappings
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	// Mouse Delta
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::RotatePitch);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::RotateYaw);

	//Action Mappings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StarJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::EndJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::StarSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::StopSprint);

	PlayerInputComponent->BindAction("GunPoint", IE_Pressed, this, &AMainCharacter::StarGunPoint);
	PlayerInputComponent->BindAction("GunPoint", IE_Released, this, &AMainCharacter::EndGunPoint);

	PlayerInputComponent->BindAction("GunPoint", IE_Pressed, this, &AMainCharacter::StarGunPoint);
	PlayerInputComponent->BindAction("GunPoint", IE_Released, this, &AMainCharacter::EndGunPoint);


	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacter::StarShoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMainCharacter::EndShoot);

}

