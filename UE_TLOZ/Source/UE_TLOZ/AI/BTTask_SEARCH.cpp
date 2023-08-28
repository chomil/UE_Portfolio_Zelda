// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SEARCH.h"

EBTNodeResult::Type UBTTask_SEARCH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_SEARCH::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);







	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();


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

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::IDLE);
		return;
	}
}
