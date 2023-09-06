// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SLEEP_LOOP.h"

EBTNodeResult::Type UBTTask_SLEEP_LOOP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_SLEEP_LOOP::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	//float Time = Montage->CalculateSequenceLength();

	if (60.f <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::SLEEP_END);
		return;
	}
}
