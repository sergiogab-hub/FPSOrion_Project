// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_SpectetingCamera.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EOR_SpectatingCameraType :uint8
{
    CameraType_None UMETA(DisplayName = "None"),
	CameraType_Victory UMETA(DisplayName = "Victory"),
	CameraType_GameOver UMETA(DisplayName = "GameOver"),


	CameraType_MAX UMETA(DisplayName = "DefaultMAX")

};


UCLASS()
class ORIONFPS_API AOR_SpectetingCamera : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SpectatingCameraComponent;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
		EOR_SpectatingCameraType CameraType;

public:	
	// Sets default values for this actor's properties
	AOR_SpectetingCamera();

	EOR_SpectatingCameraType GetCameraType() { return CameraType; }
};
