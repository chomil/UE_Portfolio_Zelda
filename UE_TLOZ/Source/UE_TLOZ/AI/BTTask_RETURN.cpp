// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RETURN.h"

EBTNodeResult::Type UBTTask_RETURN::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 150.0f;
	}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_RETURN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	FVector PawnPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector("OriginPos");

	TArray<FVector> Path = PathFind(OwnerComp, OriginPos);

	PawnPos.Z = 0.0f;
	OriginPos.Z = 0.0f;

	FVector Dir = OriginPos - PawnPos;

	if (Path.Num() <= 1)
	{
		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);
	}
	else
	{
		FVector PathDir = Path[1] - PawnPos;
		PathDir.Z = 0;
		GetGlobalCharacter(OwnerComp)->AddMovementInput(PathDir);
	}

	if (Dir.Size() <= 100.f)
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::IDLE);
		return;
	}

	AActor* ResultTarget = Cast<AActor>(GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor")));
	if (ResultTarget != nullptr)
	{
		FVector TargetPos =  ResultTarget->GetActorLocation();
		TargetPos.Z = 0;
		float Distance = (TargetPos - PawnPos).Length();
		float Angle = FMath::Abs(GetTargetAngle(OwnerComp));
		float Range = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));

		if (Angle < 90.f)
		{
			//서칭거리 30%내 +-90도 이내에 존재하면
			if (Distance  < Range * 0.3f)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
				return;
			}
		}
		else
		{
			//각도 안에 안들어와도 서칭거리 10%로 가까워지면
			if (Distance < Range * 0.1f)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
				return;
			}
		}


	}

}
