// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GlobalAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UGlobalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "GameModeValue", EditAnywhere, BlueprintReadWrite)
		int AniState = 0;

	TMap<int, class UAnimMontage*> AllAnimations;

	UFUNCTION()
		void MontageEnd(UAnimMontage* Anim, bool _Inter);

	// 애님인스턴용 Tick과 BeginPlay

protected:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float _DeltaTime) override;

private:
	class UAnimMontage* CurMontage = nullptr;
};
