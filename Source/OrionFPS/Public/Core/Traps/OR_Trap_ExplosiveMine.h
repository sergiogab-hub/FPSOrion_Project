// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Traps/OR_Trap.h"
#include "OR_Trap_ExplosiveMine.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API AOR_Trap_ExplosiveMine : public AOR_Trap
{
	GENERATED_BODY()

public:

	AOR_Trap_ExplosiveMine();

protected:

	UPROPERTY(VisibleAnywhere , Category="Trap")
	bool bIsNear;


protected:

	UFUNCTION()
	void CalculateDistance(AActor* TheOther);

	FTimerHandle DistanceHandle;
	
public:


	 void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

     void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	UFUNCTION(BlueprintImplementableEvent)
	void AlertMessage(float Dist);


};
