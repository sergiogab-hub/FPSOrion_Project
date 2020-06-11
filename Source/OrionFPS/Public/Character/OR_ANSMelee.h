// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "OR_ANSMelee.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API UOR_ANSMelee : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )override;
};
