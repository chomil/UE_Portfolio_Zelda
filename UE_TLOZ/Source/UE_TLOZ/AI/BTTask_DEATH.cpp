// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_DEATH.h"
#include "Components/CapsuleComponent.h"

EBTNodeResult::Type UBTTask_DEATH::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_DEATH::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);
	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();	
	if (Time <= GetStateTime(OwnerComp))
	{
		GetGlobalCharacter(OwnerComp)->GetMesh()->SetSimulatePhysics(true);
		GetGlobalCharacter(OwnerComp)->GetCharacterMovement()->DisableMovement();
	}
	if (Time *4 <= GetStateTime(OwnerComp))
	{
		GetGlobalCharacter(OwnerComp)->Destroy();
		return;
	}
}
