// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_IDLE.h"

EBTNodeResult::Type UBTTask_IDLE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_IDLE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
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

	if (2.0f <= GetStateTime(OwnerComp))
	{
		ResetStateTime(OwnerComp);
		return;
	}

	AActor* ResultTarget = Cast<AActor>(GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor")));

	//서칭 거리내에 존재
	if (nullptr != ResultTarget)
	{
		FVector Dist = ResultTarget->GetActorLocation() - GetGlobalCharacter(OwnerComp)->GetActorLocation();
		Dist.Z = 0;
		FVector Forward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		Forward.Z = 0;
		float TargetDegree = Dist.Rotation().Yaw - Forward.Rotation().Yaw;

		//서칭거리 내 +-80도 이내에 존재하면
		if (FMath::Abs(TargetDegree) < 80.f)
		{
			SetStateChange(OwnerComp, MONSTER_AISTATE::FIND);
			return;
		}
		else
		{
			//각도 안에 안들어와도 서칭거리 절반 이하로 가까워지면
			float Range = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
			if (Dist.Size() < Range * 0.5f) 
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::FIND);
				return;
			}
		}
	}

	return;
}
