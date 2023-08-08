// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FIND.h"

EBTNodeResult::Type UBTTask_FIND::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_FIND::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
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

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	//플레이어 놓침
	if (nullptr == TargetActor)
	{
		//SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		return;
	}

	LookTarget(OwnerComp, DelataSeconds);


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
		return;
	}
}
