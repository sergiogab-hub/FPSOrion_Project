// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OR_Pilar.generated.h"


class USphereComponent;
class UBoxComponent;
class UStaticMeshComponent;
class AMainCharacter;

UENUM(BlueprintType)
enum class AOR_PilarType:uint8
{
	PilarType_Attack UMETA(DisplayName = "Attack"),
	PilarType_Defense UMETA(DisplayName = "Defense"),
	PilarType_Movility UMETA(DisplayName = "Movility"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class ORIONFPS_API AOR_Pilar : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOR_Pilar();


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Collisions")
		UBoxComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshCilinder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pilar / Mesh")
		UStaticMeshComponent* MeshCone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pilar / Enum")
		AOR_PilarType PylarType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pilar / Main")
		AMainCharacter* MainCharacter;

	FTimerHandle AddTorretRateHandle;

protected:

	UFUNCTION()
		void SendTorretType();

	void ActivateTimer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	   void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	

};
