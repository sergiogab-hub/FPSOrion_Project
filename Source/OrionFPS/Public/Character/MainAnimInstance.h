// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	/** Setup default properties */
	UMainAnimInstance();

	/** Called at start of play */
	virtual void NativeInitializeAnimation() override;

	/** Called every frame */
	UFUNCTION(BluePrintCallable, Category = Animation)
		void UpdateAnimationPropertys();

public:
	////////////////////////////////////////////////////////////////////
	//
	//    Movement Animation Variables
	//
	////////////////////////////////////////////////////////////////////

	 /** Current Movement Speed (BS) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeed;

	/** Current Jump State (AB) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bIsInAir;

	/** Current Velocity.Z  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement) ///////Debug
		float VelocityZ;

protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Character Cast Class
	//
	////////////////////////////////////////////////////////////////////

	 /**  Pawn  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

	/**  MainCharacter Class  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class AMainCharacter* Main;
};
