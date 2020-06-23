// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OR_MyGameMOde.generated.h"

/**
 * 
 */

class AMainCharacter;
class AOR_SpectetingCamera;


UCLASS()
class ORIONFPS_API AOR_MyGameMOde : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spectating Camera")
		float SpectatingBlendTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
		AOR_SpectetingCamera* VictoryCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
		AOR_SpectetingCamera* GameOverCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameratoSpectatingPoint(AOR_SpectetingCamera* SpectatingCamera , AMainCharacter* Main);

public:

	UFUNCTION()
		void Victory(AMainCharacter* Main);

	UFUNCTION()
		void GameOver(AMainCharacter* Main);


	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
		void BP_Victory(AMainCharacter* Main);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_GameOver(AMainCharacter* Main);
	
};
