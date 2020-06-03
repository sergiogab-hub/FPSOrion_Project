// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_LauncherPad.generated.h"

class USphereComponent;
class UStaticMeshComponent;


UCLASS()
class ORIONFPS_API ALauncherPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALauncherPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	// Actor Basic Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item / Collisions")
	  USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item / Mesh")
	  UStaticMeshComponent* Mesh;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Begin Overlap Function 
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// End Overlap Function
	UFUNCTION()
		 void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
