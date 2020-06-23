// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/OR_HealthComponent.h"
#include "Core/Traps/BurningDamageType.h"


#include "GameFramework/DamageType.h"
#include "TimerManager.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UOR_HealthComponent::UOR_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	MaxHealth = 100;
	
	bIsBurnineable = false;
	// ...
}


// Called when the game starts
void UOR_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();
	if (IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UOR_HealthComponent::TakingDamage);

	}

	CurrentHealth = MaxHealth;

}

void UOR_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || bIsDead)
	{
		return;
	}

	if (bIsBurnineable)
	{
		BurningDuration = 6;
		Damage = Damage / BurningDuration;
		FTimerDelegate AlertDelegate = FTimerDelegate::CreateUObject(this, &UOR_HealthComponent::BurningEffectDamage, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
		GetWorld()->GetTimerManager().SetTimer(BurningDamageHandle, AlertDelegate, 1.0, true, 0.0);
		return;
	}

	if (IsValid(DamageType))
	{
		const UBurningDamageType* BurningDamage = Cast<UBurningDamageType>(DamageType);

		if (IsValid(BurningDamage))
		{
			if (BurningDamage->GetBurningProperty())
			{
				if (BurningDamage->GetBurningDuration() + 1 > 0)
				{
					BurningDuration = BurningDamage->GetBurningDuration() + 1;
					Damage = Damage / BurningDuration;
					FTimerDelegate AlertDelegate = FTimerDelegate::CreateUObject(this, &UOR_HealthComponent::BurningEffectDamage, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
					GetWorld()->GetTimerManager().SetTimer(BurningDamageHandle, AlertDelegate, 1.0, true, 0.0);
					return;
				}
			}

		}
	}
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0F, MaxHealth);

	if (CurrentHealth == 0)
	{
		bIsDead = true;
	}

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);		
		
}

void UOR_HealthComponent::BurningEffectDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (--BurningDuration <= 0) 
	{
		GetWorld()->GetTimerManager().ClearTimer(BurningDamageHandle);
		return;
	}
    
	CurrentHealth -= Damage;

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0.0;
		bIsDead = true;
	}

	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
}



