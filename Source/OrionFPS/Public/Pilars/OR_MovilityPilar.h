// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pilars/OR_BasePilar.h"
#include "OR_MovilityPilar.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API AOR_MovilityPilar : public AOR_BasePilar
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AOR_MovilityPilar();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovilityPilar | Rate")
		float CurrentMovilityPilarRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MovilityPilar | Rate")
		float MaxMovilityPilarRate;

	FTimerHandle MovilityRateHandle;

protected:

	void StartAddRate();


public:

	virtual void Tick(float DeltaTime) override;

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	
};
