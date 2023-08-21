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

	if (2.0f <= GetStateTime(OwnerComp))
	{
		int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 3);
		if (RandomInt == 1)
		{
			SetStateChange(OwnerComp, MONSTER_AISTATE::SEARCH);
			return;
		}
		else if (RandomInt == 2)
		{
			//patroll
		}
		// if (RandomInt == 3) IDLE 유지
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

		float Range = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		
		if (FMath::Abs(TargetDegree) < 90.f)
		{
			//서칭거리 50%내 +-90도 이내에 존재하면
			if (Dist.Size() < Range * 0.5f)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::FIND);
				return;
			}
		}
		else
		{
			//각도 안에 안들어와도 서칭거리 10%로 가까워지면
			if (Dist.Size() < Range * 0.1f)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::FIND);
				return;
			}
		}
	}

	return;
}
