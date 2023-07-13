// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums.h"
#include "ZeldaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UZeldaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float _DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	PLAYER_ANISTATE aniState = PLAYER_ANISTATE::IDLE;

	TMap<PLAYER_ANISTATE, class UAnimMontage*> AllAnimations;

	UFUNCTION()
	void MontageEnd(UAnimMontage* Anim, bool _Inter);



};
