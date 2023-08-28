// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_DEATH.h"
#include "Components/CapsuleComponent.h"

EBTNodeResult::Type UBTTask_DEATH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_DEATH::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();	
	if (Time <= GetStateTime(OwnerComp))
	{
		GetGlobalCharacter(OwnerComp)->GetAnimInstance()->StopAllMontages(0);
		if (MONSTER_TYPE::MONSTER == static_cast<MONSTER_TYPE>(GetBlackboardComponent(OwnerComp)->GetValueAsEnum("MonsterType")))
		{
			GetGlobalCharacter(OwnerComp)->GetMesh()->SetSimulatePhysics(true);
			GetGlobalCharacter(OwnerComp)->GetCharacterMovement()->DisableMovement();
		}
	}
	if (Time + 2.0f <= GetStateTime(OwnerComp))
	{
		GetGlobalCharacter(OwnerComp)->Destroy();
		return;
	}
}
