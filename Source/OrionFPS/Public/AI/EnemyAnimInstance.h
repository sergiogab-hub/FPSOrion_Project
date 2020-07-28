// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ORIONFPS_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	/** Setup default properties */
	UEnemyAnimInstance();

	/** Called at start of play */
	virtual void NativeInitializeAnimation() override;

	/** Called every frame */
	UFUNCTION(BluePrintCallable, Category = Animation)
		void UpdateAnimationPropertys();


	FORCEINLINE void SetIsIronSigth(bool value) { bIsIronSight = value; }

protected:

	////////////////////////////////////////////////////////////////////
	//
	//    Movement Animation Variables
	//
	////////////////////////////////////////////////////////////////////

	 /** Current Movement Speed (BS) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Speed;

	/** Current Movement Speed (BS) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Direction;

	/**  Pawn  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

	/**  owner Class  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		class  AOR_Enemy* Owner;

	/**  owner Class  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		class  AMainCharacter* MainCharacter;

	/** Current Movement Speed (BS) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float OwnPitch;

	/** Current Movement Speed (BS) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float OwnYaw;

	/** Current Movement Speed (BS) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bIsIronSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float BlendIronSightWeigth;





};
