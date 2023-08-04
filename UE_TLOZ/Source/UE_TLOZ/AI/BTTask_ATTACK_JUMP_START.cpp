// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ATTACK_JUMP_START.h"

EBTNodeResult::Type UBTTask_ATTACK_JUMP_START::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_ATTACK_JUMP_START::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);

	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		FVector JumpVec = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		JumpVec.Z = 0.f;
		GetGlobalCharacter(OwnerComp)->Jump();
		GetGlobalCharacter(OwnerComp)->GetCharacterMovement()->AddImpulse(JumpVec * 800, true);
		SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK_JUMP_LOOP);
		return;
	}
}
