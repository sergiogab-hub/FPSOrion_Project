// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OR_Enemy.h"

// Sets default values
AOR_Enemy::AOR_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOR_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOR_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOR_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

