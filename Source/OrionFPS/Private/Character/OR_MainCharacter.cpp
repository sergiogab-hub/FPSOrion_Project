// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OR_MainCharacter.h"
#include "Character/OR_Projectile.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Particles/ParticleSystemComponent.h"


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
	RotationSpeed = 50.0f;
	bIsRuning = false;
	bIsMoving = false;
	bIsPointed = false;
	bIsJumping = false;
	bIsReload = false;


	//Set Default Current Velocity
	CurrentVelocity = 0.f;


	// Open/Close Variables
	bIsSprintCalled = false;
	bIsPointedCalled = false;
	bIsJumpCalled = false;
	bIsDelay = false;


	//Combat Varaibles
	bIsShooting = false;
	bIsUnderShootCalled = false;
	bIsPointedShootCalled = false;
	Ammo = 100;
	
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
	if (value == 0.0) { return;}
	bIsMoving = true;
	UpdatePlayerProperties();
	AddMovementInput(GetActorForwardVector(), value);
	
}
void AMainCharacter::MoveRight(float value)
{
	if (value == 0.0) { return;}
	bIsMoving = true;
	UpdatePlayerProperties();
	AddMovementInput(GetActorRightVector(), value);
	
}
void AMainCharacter::RotatePitch(float value)
{
	AddControllerPitchInput(value * RotationSpeed * GetWorld()->GetDeltaSeconds());
}
void AMainCharacter::RotateYaw(float value)
{
	if (FMath::Abs(value) < 0.001) { return; }
	AddControllerYawInput(value * RotationSpeed * GetWorld()->GetDeltaSeconds());
}


// Star / End - Jump Space Bar Method
void AMainCharacter::StarJump()
{
	Jump();
	UpdatePlayerProperties();
}
void AMainCharacter::EndJump()
{
	StopJumping();
}


// Star / End - Sprint Left Shift Method
void AMainCharacter::StarSprint()
{
	bIsRuning = true;
	UpdatePlayerProperties();
	
}
void AMainCharacter::StopSprint()
{
	bIsRuning = false;
	UpdatePlayerProperties();
}


// Star / End - Gun Pointed Right Mouse Method
void AMainCharacter::StarGunPoint()
{
	if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		bIsReload = false;
		UAnimInstance* AnimInstance = Arms->GetAnimInstance();
		AnimInstance->StopAllMontages(0.2);
	}
	bIsPointed = true;
	UpdatePlayerProperties();
}
void AMainCharacter::EndGunPoint()
{
	bIsPointed = false;
	UpdatePlayerProperties();
}


// Star/End Shoot
void AMainCharacter::StarShoot()
{
    if (Ammo <= 0) 
	{ 
		StarReload(); 
	    return; 
	}
    if (GetCombatStatus() == ECombatStatus::EMS_Reload)	
	{
		bIsReload = false; 
		UAnimInstance* AnimInstance = Arms->GetAnimInstance();
		AnimInstance->StopAllMontages(0.2); 
	}

	bIsShooting = true;
	UpdatePlayerProperties();
	GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &AMainCharacter::Shoot, 0.09, true, 0.0);
}

void AMainCharacter::EndShoot()
{
	bIsShooting = false;
	UpdatePlayerProperties();
	GetWorldTimerManager().ClearTimer(ShootHandle);		
}



//Star/ End Reload
void AMainCharacter::StarReload()
{
    if (Ammo >= 100) { return;}

	if (GetCombatStatus() == ECombatStatus::EMS_Reload) { return; }

    if (GetCombatStatus()==ECombatStatus::EMS_FireUnder || GetCombatStatus() == ECombatStatus::EMS_PointedFire) { EndShoot();}

	bIsReload = true;
	UpdatePlayerProperties();

   if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	 {
		if (IsValid(ReloadMontage))
		{
			UAnimInstance* AnimInstance = Arms->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(ReloadMontage, 1.2f);
				AnimInstance->Montage_JumpToSection(FName("Reload"), ReloadMontage);
			}
			if (IsValid(SmokeReload))
			{
				UGameplayStatics::SpawnEmitterAttached(SmokeReload, Weapon, FName("RifleMag"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.13f, 0.01, 0.01f));
			}
      	}
	}
}

void AMainCharacter::EndReload()
{
	Ammo = 100;
	bIsReload = false;
	UAnimInstance* AnimInstance = Arms->GetAnimInstance();
	AnimInstance->StopAllMontages(0.3);
	UpdatePlayerProperties();	
}


////////////////////////////////////////////////////////////////////
//
//   Basic Event Tick
//
////////////////////////////////////////////////////////////////////
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get CurretVelocity 
	CurrentVelocity = GetVelocity().Size();

	// Check Moving False
	if (CurrentVelocity == 0)
	{
		bIsMoving = false;
		UpdatePlayerProperties();
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


////////////////////////////////////////////////////////////////////
//
//   Chracter Movement / Combat Function
//
////////////////////////////////////////////////////////////////////


// UpdatePlayerPropertys // Status-Camera
void AMainCharacter::UpdatePlayerProperties()
{

	/////////// Player Status////////////

//Combat Status
	if (!bIsShooting && !bIsReload)
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
	if (bIsReload && !bIsShooting)
	{
		SetCombatStatus(ECombatStatus::EMS_Reload);
	}


	//Movement Status
	if ((bIsMoving && !bIsRuning && !bIsPointed ) || (bIsMoving && !bIsPointed && GetCombatStatus() == ECombatStatus::EMS_FireUnder) || (bIsMoving && GetCombatStatus() == ECombatStatus::EMS_Reload))
	{
		SetMovementStatus(EMovementStatus::EMS_Walking);
	}
	if (bIsMoving && bIsRuning && !bIsPointed && !bIsDelay && GetCombatStatus() != ECombatStatus::EMS_FireUnder && !bIsReload)
	{
		SetMovementStatus(EMovementStatus::EMS_Sprinting);
	}
	if (bIsPointed && GetCombatStatus() != ECombatStatus::EMS_Reload)
	{
		SetMovementStatus(EMovementStatus::EMS_Pointing);
	}
	if (!bIsMoving && !bIsPointed || bIsDelay && !bIsPointed || !bIsMoving && GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		SetMovementStatus(EMovementStatus::EMS_Idle);
	}

	


	              /////////////Camera Movement//////////////

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

	//Activa/Deactive -> Camera Shoot Pointed
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
	if (Ammo <= 0) 
	{
		EndShoot();
		StarReload();
		return;
	}
	//Set Montage Play Shoot Animation
	UAnimInstance* AnimInstance = Arms->GetAnimInstance();

	if (IsValid(AnimInstance) && IsValid(ShootMontage) && IsValid(PointedShoot_Montage) && IsValid(ProjectileClass))
	{
		if (IsValid(MuzzleShoot1) && IsValid(MuzzleShoot2) && IsValid(ShellEject) && IsValid(SmokeMuzzle) && IsValid(SmokeShell))
		{
			if (GetMovementStatus() == EMovementStatus::EMS_Pointing)
			{
				/*Play Random Section*/
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

				/*Get Socket Transforms*/
				FVector MuzzleLocation = Weapon->GetSocketLocation("Muzzle");
				FRotator MuzzleRotation = Weapon->GetSocketRotation("Muzzle");
				MuzzleRotation.Pitch = MuzzleRotation.Pitch - 4.f; //Adjust Socket Rotation for pointed

				/*Spawn Projectile*/
				GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation);
		
				/** Spawn Emitters */
				UGameplayStatics::SpawnEmitterAttached(MuzzleShoot1, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.3f));
				UGameplayStatics::SpawnEmitterAttached(MuzzleShoot2, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.15f));
				UGameplayStatics::SpawnEmitterAttached(SmokeMuzzle, Weapon, FName("Muzzle"), FVector(ForceInitToZero), GetActorLocation().ToOrientationRotator(), FVector(0.02f, 0.02f, 0.15f));
				UGameplayStatics::SpawnEmitterAttached(SmokeShell, Weapon, FName("Smoke"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.2f, 0.6f, 0.6f));
				UGameplayStatics::SpawnEmitterAttached(ShellEject, Weapon, FName("Shell"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.6f));
			}
			else 
			{
				/*Play Random Section*/
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

				/*Get Socket Transforms*/
				FVector MuzzleLocation = Weapon->GetSocketLocation("Muzzle");
				FRotator MuzzleRotation = Weapon->GetSocketRotation("Muzzle");
			

				/*Spawn Projectile*/
				GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation);

				/** Spawn Emitters */
				UGameplayStatics::SpawnEmitterAttached(MuzzleShoot1, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.6f));
				UGameplayStatics::SpawnEmitterAttached(MuzzleShoot2, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.3f));
				UGameplayStatics::SpawnEmitterAttached(SmokeMuzzle, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.02f, 1.5f, 0.15f));
				UGameplayStatics::SpawnEmitterAttached(SmokeShell, Weapon, FName("Smoke"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.15f, 0.2f, 0.7f));
				UGameplayStatics::SpawnEmitterAttached(ShellEject, Weapon, FName("Shell"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.6f));
			}
		}
		Ammo--;
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

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacter::StarShoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMainCharacter::EndShoot);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacter::StarShoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMainCharacter::EndShoot);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::StarReload);


}

