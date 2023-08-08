// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PATROL.h"

EBTNodeResult::Type UBTTask_PATROL::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_PATROL::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);
	if (Dead(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::DEATH);
		return;
	}
	if (Damaged(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::HIT);
		return;
	}


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
		return;
	}
}
