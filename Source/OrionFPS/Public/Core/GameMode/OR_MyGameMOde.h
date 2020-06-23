// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OR_MyGameMOde.generated.h"

/**
 * 
 */

class AMainCharacter;

UCLASS()
class ORIONFPS_API AOR_MyGameMOde : public AGameModeBase
{
	GENERATED_BODY()


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
