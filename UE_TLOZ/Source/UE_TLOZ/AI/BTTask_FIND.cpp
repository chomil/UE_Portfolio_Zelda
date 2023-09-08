// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FIND.h"

EBTNodeResult::Type UBTTask_FIND::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_FIND::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor")) == nullptr)
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		return;
	}


	LookTarget(OwnerComp, DeltaSeconds*2);


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
		return;
	}
}
