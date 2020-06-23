// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Traps/OR_Trap.h"
#include "OR_Trap_ExplosiveBarrel.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API AOR_Trap_ExplosiveBarrel : public AOR_Trap
{
	GENERATED_BODY()

public:

	AOR_Trap_ExplosiveBarrel();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TrapBarrel")
    bool bUseOverlapBurningLogic;



public:

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
