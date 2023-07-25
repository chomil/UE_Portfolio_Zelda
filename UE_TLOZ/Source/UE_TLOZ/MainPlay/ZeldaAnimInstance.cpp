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

	if (nullptr == Chracter || false == Chracter->IsValidLowLevel())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("PrevState %d"), (int)aniState);
	aniState = Chracter->aniState;

	UE_LOG(LogTemp, Log, TEXT("NextState %d"), (int)aniState);
	class UAnimMontage* Montage = AllAnimations[aniState];

	if (nullptr == Montage)
	{
		return;
	}
	
	
	if (false == Montage_IsPlaying(Montage))
	{
		if (Montage == CurMontage && Montage->bLoop == true)
		{
			return;
		}

		CurMontage = Montage;
		Montage_Play(Montage, 1.0f);
	}
}


void UZeldaAnimInstance::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	TSubclassOf<UAnimInstance> Inst = UZeldaAnimInstance::StaticClass();

	AMainCharacter* Chracter = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == Chracter || false == Chracter->IsValidLowLevel())
	{
		return;
	}

	if (_Inter == false)
	{
		if (AllAnimations[PLAYER_ANISTATE::LAND] == Anim ||
			AllAnimations[PLAYER_ANISTATE::ATTACK1] == Anim ||
			AllAnimations[PLAYER_ANISTATE::ATTACK2] == Anim ||
			AllAnimations[PLAYER_ANISTATE::ATTACK3] == Anim ||
			AllAnimations[PLAYER_ANISTATE::ATTACK4] == Anim ||
			AllAnimations[PLAYER_ANISTATE::ATTACK_DASH] == Anim||
			AllAnimations[PLAYER_ANISTATE::SWORD_ON] == Anim||
			AllAnimations[PLAYER_ANISTATE::SWORD_OFF] == Anim)
		{

			aniState = PLAYER_ANISTATE::IDLE;
			Chracter->aniState = aniState;
			Montage_Play(AllAnimations[aniState], 1.0f);
		}
	}

}
