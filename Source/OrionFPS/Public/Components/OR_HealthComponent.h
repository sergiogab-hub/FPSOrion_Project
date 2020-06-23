// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OR_HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignature, UOR_HealthComponent*, HealthComponent, AActor*, DamagedActor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(ORION), meta=(BlueprintSpawnableComponent) )
class ORIONFPS_API UOR_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOR_HealthComponent();



protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Component")
		bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Component")
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Component" , meta=(ClampMin=0.0, UMin=0.0))
		float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOwner")
		AActor* MyOwner;

	 ////////////////////////  Burning Damage Propertys /////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOwner")
		bool bIsBurnineable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOwner")
		int32 BurningDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyOwner")
		class UDamageType* MyBurningDamage;

	FTimerHandle BurningDamageHandle;

public:

	
	UPROPERTY(BlueprintAssignable)
		FOnHealthChangeSignature OnHealthChangeDelegate;


	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE void SetBurneable(bool state) { bIsBurnineable = state; }


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void BurningEffectDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

		
};
