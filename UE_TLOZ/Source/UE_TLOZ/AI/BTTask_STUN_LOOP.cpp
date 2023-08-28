// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_STUN_LOOP.h"

EBTNodeResult::Type UBTTask_STUN_LOOP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_STUN_LOOP::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	//float Time = Montage->CalculateSequenceLength();

	if (5.f <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::STUN_END);
		return;
	}
}
