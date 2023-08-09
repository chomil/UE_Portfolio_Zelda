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
	FVector TargetPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector("OriginPos");
	PawnPos.Z = 0.0f;
	TargetPos.Z = 0.0f;

	FVector Dir = TargetPos - PawnPos;

	GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);


	if (Dir.Size() <= 100.f)
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::IDLE);
		return;
	}
}
