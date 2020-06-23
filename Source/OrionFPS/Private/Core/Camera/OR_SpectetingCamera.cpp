// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Camera/OR_SpectetingCamera.h"

#include "Components/StaticMeshComponent.h"
// Sets default values
AOR_SpectetingCamera::AOR_SpectetingCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpectatingCameraComponent = CreateDefaultSubobject<UStaticMeshComponent>("SpectatingCameraComponent");
	RootComponent = SpectatingCameraComponent;

}
