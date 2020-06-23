// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "BurningDamageType.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API UBurningDamageType : public UDamageType
{
	GENERATED_BODY()

public:

	UBurningDamageType();


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DamageProperty")
		bool bIsHaveBurningEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageProperty", meta = (ClampMax = 10.0, UMax = 10.0))
		int32 BurningEffectDuration;



public:

	bool GetBurningProperty() const { return bIsHaveBurningEffect; }

   int32 GetBurningDuration() const { return BurningEffectDuration; }

	
	
};
