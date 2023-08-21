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

void UBTTask_RETURN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);


	FVector PawnPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector("OriginPos");
	PawnPos.Z = 0.0f;
	OriginPos.Z = 0.0f;

	FVector Dir = OriginPos - PawnPos;

	GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);


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

		if (Distance < GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange")) * 0.1f)
		{
			if (Angle < 30.f)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
			}
		}
	}

}
