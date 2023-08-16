// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ATTACK_JUMP_END.h"

EBTNodeResult::Type UBTTask_ATTACK_JUMP_END::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_ATTACK_JUMP_END::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
		return;
	}
}
