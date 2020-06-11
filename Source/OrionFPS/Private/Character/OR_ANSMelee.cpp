// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OR_ANSMelee.h"
#include "Character/OR_MainCharacter.h"
#include "Components/SkeletalMeshComponent.h"


void UOR_ANSMelee::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* MYMesh = MeshComp->GetOwner();
	if (IsValid(MYMesh))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(MYMesh);
		if (IsValid(Main))
		{
			Main->SetEnumMeleeCollision(ECollisionEnabled::QueryOnly);
		}
	}


}
void UOR_ANSMelee::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* MYMesh = MeshComp->GetOwner();
	if (IsValid(MYMesh))
	{
		AMainCharacter* Main = Cast<AMainCharacter>(MYMesh);
		if (IsValid(Main))
		{
			Main->SetEnumMeleeCollision(ECollisionEnabled::NoCollision);
		}
	}
}
