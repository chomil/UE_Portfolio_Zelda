// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/ZeldaAnimInstance.h"
#include "MainCharacter.h"

void UZeldaAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();


	OnMontageBlendingOut.AddDynamic(this, &UZeldaAnimInstance::MontageEnd);

	AMainCharacter* Chracter = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == Chracter || false == Chracter->IsValidLowLevel())
	{
		return;
	}

	AllAnimations = Chracter->AllAnimations;
}


void UZeldaAnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
	Super::NativeUpdateAnimation(_DeltaTime);


	if (0 == AllAnimations.Num())
	{
		return;
	}

	AMainCharacter* Chracter = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == Chracter && false == Chracter->IsValidLowLevel())
	{
		return;
	}

	aniState = Chracter->aniState;

	class UAnimMontage* Montage = AllAnimations[aniState];

	if (nullptr == Montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		Montage_Play(Montage, 1.0f);
	}
}


void UZeldaAnimInstance::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	
}