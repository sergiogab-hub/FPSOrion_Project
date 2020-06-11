// Fill out your copyright notice in the Description page of Project Settings.


#include "Pilars/OR_LauncherPad.h"
#include "Character/OR_MainCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALauncherPad::ALauncherPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // Not Used

	// Collision Volument Shepere Component
	CollisionVolume = CreateDefaultSubobject <USphereComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(GetRootComponent());

	// Mesh Component
	Mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(CollisionVolume);

}


// Called when the game starts or when spawned
void ALauncherPad::BeginPlay()
{
	Super::BeginPlay();

	//Set Dynamics
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALauncherPad::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &ALauncherPad::OnOverlapEnd);
}


// Not Use Yet
void ALauncherPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}   


////////////////////////////////////////////////////////////////////
//
//   Overlaps Events
//
////////////////////////////////////////////////////////////////////
void ALauncherPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (IsValid(Main))
		{
			if (Main->GetKeyBoolValue())
			{
				float ZVelocity = Main->GetCharacterMovement()->JumpZVelocity;
				ZVelocity = ZVelocity * FMath::FRandRange(3, 5);
				Main->LaunchCharacter(FVector(0.f, 0.f, ZVelocity), false, true);
			}		
		}
	}
}

void ALauncherPad::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	return;
}

