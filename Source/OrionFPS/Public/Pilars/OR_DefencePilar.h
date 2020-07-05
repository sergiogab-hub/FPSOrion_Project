// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pilars/OR_BasePilar.h"
#include "OR_DefencePilar.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API AOR_DefencePilar : public AOR_BasePilar
{
	GENERATED_BODY()

public:

	AOR_DefencePilar();

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar/Rate")
		float CurrentDefencePilarRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pilar/Rate")
		float MaxDefencePilarRate;

	FTimerHandle DefenceRateHandle;

protected:

	void StartAddRate();

public:

	virtual void Tick(float DeltaTime) override;

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
