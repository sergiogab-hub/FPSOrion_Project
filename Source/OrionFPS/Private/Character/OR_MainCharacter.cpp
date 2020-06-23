// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OR_MainCharacter.h"
#include "Core/GameMode/OR_MyGameMOde.h"
#include "Components/OR_HealthComponent.h"
#include "Projectiles/OR_RocketProjectile.h"
#include "Projectiles/OR_BulletProjectile.h"
#include "Projectiles/OR_LauncherProjectile.h"




#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"



// Base Constructor
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponSocketName = "R_GunSocket";
	MeleeCapsuleSocketName = "Melee";


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
	Weapon->SetupAttachment(Arms, WeaponSocketName);

	Rocket = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(Arms, WeaponSocketName);


	// Player Weapon Mesh
	MeleeDetector = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetector"));
	MeleeDetector->SetupAttachment(Weapon, MeleeCapsuleSocketName);
	MeleeDetector->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetector->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeleeDetector->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//HealthComponent

	Health = CreateDefaultSubobject<UOR_HealthComponent>(TEXT("Health"));

	// Set Initial Default Movement Status
	MovementStatus = EMovementStatus::EMS_Idle;
	CurrentWeaponStatus = ECurrentWeapon::EMS_Rifle;


	// State Movement Variables
	RotationSpeed = 50.0f;
	bIsRuning = false;
	bIsMoving = false;
	bIsPointed = false;
	bIsJumping = false;
	bIsReload = false;
	bIsSwitching = false;
	

	//Set Default Current Velocity
	CurrentVelocity = 0.f;


	// Open/Close Variables
	bIsDelay = false;
	bIsJumpCalled = false;
	bIsSprintCalled = false;
	bIsReloadCalled = false;
	bIsPointedCalled = false;
	bIsUnderShootCalled = false;
	bIsMeleeAttackCalled = false;
	bIsPointedShootCalled = false;
	bIsGrenadeLauncherCalled = false;
	


	//Combat Varaibles
	WeaponAmmo = 50;
	GrenadeAmmo = 3;
	bIsShooting = false;
	bIsKeyShootPressed = false;
	bIsMeleeAttack = false;
	bIsGrenadeLauncher = false;
	bIsRuningControlDelay = false;


	//Ultimate Varaibles
	bIsUltimate = false;
	RocketAmmo = 5;
	bIsAttackUltimate = false;
	bIsDefenseUltimate = false;
	bIsMovilityUltimate = false;
	
	
	

	
	
}


////////////////////////////////////////////////////////////////////
//
//   Begin Play
//
////////////////////////////////////////////////////////////////////
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set AnimInstance
	MainAnimInstance = Arms->GetAnimInstance();

	//Set Game Mode
	GameModeReference = Cast<AOR_MyGameMOde>(GetWorld()->GetAuthGameMode());

	// Mele Combat
	MeleeDetector->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::MakeMeleeDamage);

	// HealthDelegate
	Health->OnHealthChangeDelegate.AddDynamic(this, &AMainCharacter::OnHealthChange);

	//SetRocketWeaponInitialVisibility
	Rocket->SetVisibility(false);
	
}


////////////////////////////////////////////////////////////////////
//
//   Character Player Input Movement
//
////////////////////////////////////////////////////////////////////


//////////Player Basic Movement Inputs Handles///////
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



////////////////////////////////////////////////////////////////////
//
//   Character Player Movement Functions
//
////////////////////////////////////////////////////////////////////

////////Star / End - Jump Space Bar Method /////////
void AMainCharacter::StarJump()
{
	if (!bIsUltimate)
	{
		Jump();
		UpdatePlayerProperties();
	}
}
void AMainCharacter::EndJump()
{
	StopJumping();
}



/////////// Star / End - Sprint Left Shift Method ///////
void AMainCharacter::StarSprint()
{
	/** Check Reload State */
	if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		bIsReload = false;
		StopMyMontage(0.5);	
	}

	/** Set Variable */
	bIsRuning = true;
	UpdatePlayerProperties();
}

void AMainCharacter::StopSprint()
{
	/** Set Variable */
	bIsRuning = false;
	UpdatePlayerProperties();
}


///////// Star / End - Gun Pointed Right Mouse Method /////////
void AMainCharacter::StarGunPoint()
{
	/** Check Reload State */
	if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		bIsReload = false;
		StopMyMontage(0.2);
	}

	/* Set Variable */
	bIsPointed = true;
	UpdatePlayerProperties();
}

void AMainCharacter::EndGunPoint()
{
	/** Set Variable */
	bIsPointed = false;
	UpdatePlayerProperties();
}

////////////////////////////////////////////////////////////////////
//
//   Character Player Combat Functions
//
////////////////////////////////////////////////////////////////////


///////////////////// Star/End Shoot ///////////////////
void AMainCharacter::StarShoot()
{
	bIsKeyShootPressed = true;

	/** Rocket Shoot */
	if (GetCurrentWeaponStatus() == ECurrentWeapon::EMS_Rocket)
	{
		if (RocketAmmo > 0 && bIsAttackUltimate)
		{
			RocketShoot();
			bIsShooting = true;
			UpdatePlayerProperties();
		}
		
		return;
	}


	/** Check Movement Conditions */
	if (WeaponAmmo <= 0)
	{
		StarReload();
		return;
	}

	if (GetCombatStatus() == ECombatStatus::EMS_Grenade || GetCombatStatus() == ECombatStatus::EMS_Melee || bIsSwitching)
	{
		return;
	}

	if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		bIsReload = false;
		StopMyMontage(0.2);
	}


	/** Set Variables */
	bIsShooting = true;
	UpdatePlayerProperties();
	GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &AMainCharacter::Shoot, 0.09, true, 0.0);

}

void AMainCharacter::EndShoot()
{
	/** Set Variables */
	bIsShooting = false;
	bIsKeyShootPressed = false;
	UpdatePlayerProperties();
	GetWorldTimerManager().ClearTimer(ShootHandle);		
}

void AMainCharacter::EndShootByOther()
{
	/** Set Variables */
	bIsShooting = false;
	UpdatePlayerProperties();
	GetWorldTimerManager().ClearTimer(ShootHandle);
}



/////////////////////////Star/ End Reload //////////////////
void AMainCharacter::StarReload()
{
	/** Check Movement Conditions */
    if (WeaponAmmo >= 50 || GetCombatStatus() == ECombatStatus::EMS_Reload || GetCombatStatus() == ECombatStatus::EMS_Grenade || bIsSwitching)
	{ 
		return;
	}
	
	if (GetCombatStatus() == ECombatStatus::EMS_FireUnder || GetCombatStatus() == ECombatStatus::EMS_PointedFire)
	{ 
		EndShootByOther(); 
	}

	if (GetCombatStatus() == ECombatStatus::EMS_Melee)
	{
		bIsMeleeAttack = false;
		StopMyMontage(0.8);
	}


	/** Set Variables / Play montage */
	bIsReload = true;
	UpdatePlayerProperties();
	PlayMyMontage(ReloadMontage, 1.2f, FName("Reload"));

	/** Spawn Emitter */
	if (IsValid(SmokeReload))
	{
		UGameplayStatics::SpawnEmitterAttached(SmokeReload, Weapon, FName("RifleMag"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.13f, 0.01, 0.01f));
	}
}
void AMainCharacter::EndReload()
{
	/** Set Variables / Stop Montage */
	WeaponAmmo = 50;
	bIsReload = false;
	StopMyMontage(0.3f);
	UpdatePlayerProperties();	

	/** If Left Mouse Button Still Pressed Continue Shooting */
	if (bIsKeyShootPressed) { StarShoot(); }	
}



//////////////////////////////Star/ End MeleeAttack ///////////////////
void AMainCharacter::StarMeleeAtaack()
{
	/** Check Movement Conditions*/
	if (GetCombatStatus() == ECombatStatus::EMS_Melee || GetCombatStatus() == ECombatStatus::EMS_Grenade || bIsSwitching)
	{ 
		return; 
	}

	if (GetCombatStatus() == ECombatStatus::EMS_FireUnder || GetCombatStatus() == ECombatStatus::EMS_PointedFire) 
	{ 
		EndShootByOther();
	}

	if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		bIsReload = false;
		StopMyMontage(0.2);
	}

	/** Set Variables / Play Montage*/
	bIsMeleeAttack = true;
	UpdatePlayerProperties();
	PlayMyMontage(MeleMontage, 1.0f, "Melee");

}
void AMainCharacter::EndMeleeAttack()
{

	/** Set Variables / End Montage*/
	bIsMeleeAttack = false;
	UpdatePlayerProperties();
	StopMyMontage(1.0);

	/** If Left Mouse Button Still Pressed Continue Shooting */
	if (bIsKeyShootPressed) { StarShoot(); }
}
void AMainCharacter::SetEnumMeleeCollision(ECollisionEnabled::Type CollisionState) //Call By Anim Notify
{
	MeleeDetector->SetCollisionEnabled(CollisionState);
}
void AMainCharacter::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, 20.f, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
	}
}



/////////////////////////Star/ End GrenadeLauncher ///////////////////
void AMainCharacter::StarGrenadeLauncher()
{
	/** Check Movement Conditions*/
	if (GrenadeAmmo <= 0 || GetCombatStatus() == ECombatStatus::EMS_Grenade || bIsSwitching)
	{ 
	     return;
	}

	if (GetCombatStatus() == ECombatStatus::EMS_FireUnder || GetCombatStatus() == ECombatStatus::EMS_PointedFire) 
	{ 
	    EndShootByOther(); 
	}

	if (GetCombatStatus() == ECombatStatus::EMS_Melee)
	{
		bIsMeleeAttack = false;
		StopMyMontage(0.8);
	}

	if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
	    bIsReload = false;
		StopMyMontage(0.2);
	}

	if (GetMovementStatus() == EMovementStatus::EMS_Sprinting)
	{
		bIsRuningControlDelay = true;
	}

	/** Set Variables / Play Montage*/
	bIsGrenadeLauncher = true;
	PlayMyMontage(GrenadeMontage, 0.8f, "Grenade");
	UpdatePlayerProperties();	



}
void AMainCharacter::SpawnGrenadeLauncher() //Call By Anim Notify
{
	if (IsValid(LauncherClass))
	{
		FVector GrenadeLocation = Arms->GetSocketLocation("GrenadeSocket");
		FRotator GrenadeRotation = Camera->GetComponentRotation();
		GrenadeRotation.Pitch = GrenadeRotation.Pitch + 10;

		/*Spawn Projectile*/
		AOR_LauncherProjectile* Launcher = GetWorld()->SpawnActor<AOR_LauncherProjectile>(LauncherClass, GrenadeLocation, GrenadeRotation);
		Launcher->SetMain(this);
	}
}
void AMainCharacter::EndGrenadeLauncher() //Call By Anim Notify
{
	if (bIsRuningControlDelay)
	{
		bIsRuningControlDelay = false;
	}

	/** Set Variables / End Montage*/
	bIsGrenadeLauncher = false;
	UpdatePlayerProperties();
	StopMyMontage(0.2);


	/** If Left Mouse Button Still Pressed Continue Shooting */
	if (bIsKeyShootPressed) { StarShoot(); }
}


/////////////////////////Star/ End SwitchWeapon ///////////////////
void AMainCharacter::StarSwtichWeapon()
{
	if (GetCombatStatus() == ECombatStatus::EMS_Grenade  || GetCombatStatus() == ECombatStatus::EMS_Melee || GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		bIsReload = false;
		bIsGrenadeLauncher = false;
	    bIsMeleeAttack = false;
		StopMyMontage(0.1);
	}
	if (GetCombatStatus() == ECombatStatus::EMS_FireUnder || GetCombatStatus() == ECombatStatus::EMS_PointedFire)
	{
		EndShootByOther();
	}
	bIsSwitching = true;
	UpdatePlayerProperties();
}
void AMainCharacter::EndSwitchWeapom()
{
	bIsSwitching = false;
	if (GetCurrentWeaponStatus() == ECurrentWeapon::EMS_Rifle)
	{
		Rocket->SetVisibility(true);
		Weapon->SetVisibility(false);
		SetCurrentWeaponStatus(ECurrentWeapon::EMS_Rocket);
	}
	else
	{
		Rocket->SetVisibility(false);
		Weapon->SetVisibility(true);
		SetCurrentWeaponStatus(ECurrentWeapon::EMS_Rifle);
	}
	UpdatePlayerProperties();
}


void AMainCharacter::ActivateCurrentUltimate()
{
	if (!bIsJumping && !bIsUltimate)
	{
		BP_StarAttackUltimate();
		bIsUltimate = true;
		GetWorld()->GetTimerManager().SetTimer(AttackUltimateHandle, this, &AMainCharacter::StarAttackUltimate, 0.5f, false, 0.5f);
	}
	
}
void AMainCharacter::StarAttackUltimate()
{
	GetWorldTimerManager().ClearTimer(AttackUltimateHandle);
	GetCharacterMovement()->JumpZVelocity = 2500;
	GetCharacterMovement()->AirControl = 3.0;
	Jump();
	StarSwtichWeapon();
	bIsAttackUltimate = true;
	GetWorld()->GetTimerManager().SetTimer(AttackUltimateHandle, this, &AMainCharacter::EndAttackUltimate, 15.0f, false, 15.0f);
}
void AMainCharacter::EndAttackUltimate()
{
	GetCharacterMovement()->GravityScale = 1.0;
	bIsAttackUltimate = false;
	RocketAmmo = 5;
	GetWorldTimerManager().ClearTimer(AttackUltimateHandle);
}



void AMainCharacter::OnHealthChange(UOR_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealthComponent->GetIsDead())
	{
		if (IsValid(GameModeReference))
		{
			UE_LOG(LogTemp, Warning, TEXT("Ta morto xD"));
			GameModeReference->GameOver(this);
		}
		
	}
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


	///// Set Jumping Control ////  /// No Optimizado
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

			if (GetCurrentWeaponStatus() == ECurrentWeapon::EMS_Rocket)
			{
				BP_EndAttackUltimate();
				if (bIsUltimate)
				{
					bIsUltimate = false;
				}
				GetCharacterMovement()->JumpZVelocity = 600;
				GetCharacterMovement()->AirControl = 0.02;
				StarSwtichWeapon();
			}
			
		}
	}

	///// Set Camera Rotation Movement with Pawn //// 
	if(!Camera->bUsePawnControlRotation)
	{
		CameraRotationWithPawn.Pitch = GetControlRotation().Pitch;
	}

}



////////////////////////////////////////////////////////////////////
//
//   Character Update Propertys and BP Functions
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
	if (bIsGrenadeLauncher && !bIsMeleeAttack && !bIsReload && !bIsShooting)
	{
		SetCombatStatus(ECombatStatus::EMS_Grenade);
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
	if (bIsMeleeAttack && !bIsReload && !bIsShooting)
	{
		SetCombatStatus(ECombatStatus::EMS_Melee);
	}
	
	


	//Movement Status

	if ((bIsMoving && !bIsRuning && !bIsPointed && !bIsSwitching) || (bIsMoving && !bIsPointed && GetCombatStatus() == ECombatStatus::EMS_FireUnder && !bIsSwitching) || (bIsMoving && (GetCombatStatus() == ECombatStatus::EMS_Reload || GetCombatStatus() == ECombatStatus::EMS_Melee || GetCombatStatus() == ECombatStatus::EMS_Grenade) && !bIsSwitching))
	{
		SetMovementStatus(EMovementStatus::EMS_Walking);
	}
	if (bIsMoving && bIsRuning && !bIsPointed && !bIsDelay && GetCombatStatus() != ECombatStatus::EMS_FireUnder && !bIsReload && !bIsGrenadeLauncher && !bIsSwitching)
	{
		SetMovementStatus(EMovementStatus::EMS_Sprinting);
	}
	if (bIsPointed && GetCombatStatus() != ECombatStatus::EMS_Reload && GetCombatStatus() != ECombatStatus::EMS_Melee && GetCombatStatus() != ECombatStatus::EMS_Grenade && !bIsSwitching)
	{
		SetMovementStatus(EMovementStatus::EMS_Pointing);
	}
	if (!bIsMoving && !bIsPointed || bIsDelay && !bIsPointed || !bIsMoving && ( GetCombatStatus() == ECombatStatus::EMS_Reload || GetCombatStatus() == ECombatStatus::EMS_Melee || GetCombatStatus() == ECombatStatus::EMS_Grenade) && !bIsSwitching)
	{
		SetMovementStatus(EMovementStatus::EMS_Idle);
	}
	if (bIsSwitching)
	{
		SetMovementStatus(EMovementStatus::EMS_Switch);
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
	if (GetMovementStatus() == EMovementStatus::EMS_Sprinting )
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

			/**Rocket Scoop Logic*/
			if (GetCurrentWeaponStatus() == ECurrentWeapon::EMS_Rocket)
			{
				SetScoopVisibility(true);
				Rocket->SetVisibility(false);
			}
		}
	}
	else
	{
		if (bIsPointedCalled)
		{
			BP_EndCameraPointed();
			bIsPointedCalled = false;
			SpringArm->bEnableCameraLag = true;

			/**Rocket Scoop Logic*/
			if (GetCurrentWeaponStatus() == ECurrentWeapon::EMS_Rocket)
			{
				SetScoopVisibility(false);
				Rocket->SetVisibility(true);
			}
		}
	}

	//Activa/Deactive -> Camera Grenade MOvement
	if (GetCombatStatus() == ECombatStatus::EMS_Grenade)
	{
		if (!bIsGrenadeLauncherCalled)
		{
			
			bIsGrenadeLauncherCalled = true;
			CameraRotationWithPawn = Camera->GetRelativeRotation();
			Camera->bUsePawnControlRotation = false;
			BP_StarGrenadeLauncher();
		}
	}
	else
	{
		if (bIsGrenadeLauncherCalled)
		{
			bIsGrenadeLauncherCalled = false;
			BP_EndGrenadeLauncher();
		}
	}


	//Activa/Deactive -> Camera Melee Attack
	if (GetCombatStatus() == ECombatStatus::EMS_Melee)
	{
		if (!bIsMeleeAttackCalled)
		{

			bIsMeleeAttackCalled = true;
			CameraRotationWithPawn = Camera->GetRelativeRotation();
			Camera->bUsePawnControlRotation = false;
			BP_StarMeleeAttack();
		}
	}
	else
	{
		if (bIsMeleeAttackCalled)
		{
			bIsMeleeAttackCalled = false;
			BP_EndMeleeAttack();
		}
	}


	//Activa/Deactive -> Camera Reload Movement
	if (GetCombatStatus() == ECombatStatus::EMS_Reload)
	{
		if (!bIsReloadCalled)
		{

			bIsReloadCalled = true;
			CameraRotationWithPawn = Camera->GetRelativeRotation();
			Camera->bUsePawnControlRotation = false;
			BP_StarReload();
		}
	}
	else
	{
		if (bIsReloadCalled)
		{
			if (!Camera->bUsePawnControlRotation)
			{
				Camera->bUsePawnControlRotation = true;
				BP_EndReload();
			}
			bIsReloadCalled = false;
		}
	}
}


////////////////////////
//Play - Stop Montage //
////////////////////////
void AMainCharacter::PlayMyMontage(UAnimMontage* MontageToPlay, float Ratio, FName Section)
{
	if (IsValid(MainAnimInstance))
	{
		if (IsValid(MontageToPlay))
		{
			MainAnimInstance->Montage_Play(MontageToPlay, Ratio);
			MainAnimInstance->Montage_JumpToSection(Section, MontageToPlay);
		}	
	}	
}

void AMainCharacter::StopMyMontage(float RatioStop)
{
	if (IsValid(MainAnimInstance))
	{
		MainAnimInstance->StopAllMontages(RatioStop);
	}
}



////////////////////////////////////////////////////////////////////
//
//  Shot Function
//
////////////////////////////////////////////////////////////////////
void AMainCharacter::Shoot()
{
	if (WeaponAmmo <= 0) 
	{
		EndShootByOther();
		StarReload();
		return;
	}

	if (IsValid(BulletClass))
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
					PlayMyMontage(PointedShoot_Montage, 2.0F, FName("PShoot01"));
					break;
				case 2:
					PlayMyMontage(PointedShoot_Montage, 2.0F, FName("PShoot02"));
					break;
				default:
					break;
				}

				/*Get Socket Transforms*/
				FVector MuzzleLocation = Weapon->GetSocketLocation("Muzzle");
				FRotator MuzzleRotation = Weapon->GetSocketRotation("Muzzle");
				MuzzleRotation.Pitch = MuzzleRotation.Pitch - 4.f; //Adjust Socket Rotation for pointed

				/*Spawn Projectile*/
				AProjectile* Projectile= GetWorld()->SpawnActor<AProjectile>(BulletClass, MuzzleLocation, MuzzleRotation);
				Projectile->SetMain(this); // Set This to Projectile
		
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
					PlayMyMontage(ShootMontage, 2.0f, FName("Shoot01"));
					break;
				case 2:
					PlayMyMontage(ShootMontage, 2.0f, FName("Shoot02"));
					break;
				default:
					break;
				}

				/*Get Socket Transforms*/
				FVector MuzzleLocation = Weapon->GetSocketLocation("Muzzle");
				FRotator MuzzleRotation = Weapon->GetSocketRotation("Muzzle");
				MuzzleRotation.Pitch = MuzzleRotation.Pitch - 2.5f; //Adjust Socket Rotation for Fire Under
				MuzzleRotation.Yaw = MuzzleRotation.Yaw + 2;//Adjust Socket Rotation Fire Under
				
			
				/*Spawn Projectile*/
				AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(BulletClass, MuzzleLocation, MuzzleRotation);
				Projectile->SetMain(this); // Set This to Projectile


	

				/** Spawn Emitters */
				UGameplayStatics::SpawnEmitterAttached(MuzzleShoot1, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.6f));
				UGameplayStatics::SpawnEmitterAttached(MuzzleShoot2, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.3f));
				UGameplayStatics::SpawnEmitterAttached(SmokeMuzzle, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.02f, 1.5f, 0.15f)); 
				UGameplayStatics::SpawnEmitterAttached(SmokeShell, Weapon, FName("Smoke"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.15f, 0.2f, 0.7f));
				UGameplayStatics::SpawnEmitterAttached(ShellEject, Weapon, FName("Shell"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(0.6f));
			}
		}
		WeaponAmmo--;
	}
}

void AMainCharacter::RocketShoot()
{
	if (IsValid(RocketClass) && IsValid(Weapon) && IsValid(FinalGun))
	{
		/*Get Socket Transforms*/
		FVector MuzzleLocation = Weapon->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = Weapon->GetSocketRotation("Muzzle");
		MuzzleRotation.Pitch = MuzzleRotation.Pitch - 4.f; //	Adjust Projectiel Direction

		/*Spawn Projectile*/
		AOR_RocketProjectile* MyRocket = GetWorld()->SpawnActor<AOR_RocketProjectile>(RocketClass, MuzzleLocation, MuzzleRotation);
		MyRocket->SetMain(this);
		RocketAmmo--;
		/*Check State*/
		if (GetMovementStatus()!= EMovementStatus::EMS_Pointing)
		{
			PlayMyMontage(ShootMontage, 1.0f, FName("Shoot01"));
			UGameplayStatics::SpawnEmitterAttached(FinalGun, Weapon, FName("Muzzle"), FVector(ForceInitToZero), FRotator::ZeroRotator, FVector(2.0f));
		}
		/*Check Ammo and State*/
		if (RocketAmmo == 0 && bIsAttackUltimate)
		{
			EndAttackUltimate();
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

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacter::StarShoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMainCharacter::EndShoot);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::StarReload);

	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &AMainCharacter::StarMeleeAtaack);

	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &AMainCharacter::StarGrenadeLauncher);

	PlayerInputComponent->BindAction("CurrentUltimate", IE_Pressed, this, &AMainCharacter::ActivateCurrentUltimate);

	
}

