// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GlobalAnimInstance.h"
#include "GlobalCharacter.h"
#include <Global/Data/StateEnums.h>

void UGlobalAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OnMontageBlendingOut.AddDynamic(this, &UGlobalAnimInstance::MontageEnd);
}

// 시작할때 한번 업데이트
void UGlobalAnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
	Super::NativeUpdateAnimation(_DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AGlobalCharacter* Chracter = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == Chracter || false == Chracter->IsValidLowLevel())
	{
		return;
	}

	AniState = Chracter->GetAniState();

	if (!AllAnimations.Contains(AniState))
	{
		return;
	}
	UAnimMontage* Montage = AllAnimations[AniState];

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

		if (Chracter->ActorHasTag(TEXT("Player")))
		{
			UE_LOG(LogTemp, Log, TEXT("Player State : %s"), *UEnum::GetValueAsString(static_cast<PLAYER_ANISTATE>(AniState)));
		}
	}
}

void UGlobalAnimInstance::MontageEnd(UAnimMontage* Anim, bool _Inter)
{

}
