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

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	//플레이어 놓침
	if (nullptr == TargetActor)
	{
		//SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		return;
	}

	//회전
	{
		float Degree = 0.f;
		FVector TargetPos = TargetActor->GetActorLocation();
		FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		TargetPos.Z = 0.0f;
		ThisPos.Z = 0.0f;

		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector ThisForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		ThisForward.Normalize();

		FVector Cross = FVector::CrossProduct(ThisForward, Dir);
		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = ThisForward.Rotation().Yaw;
		Degree = Angle0 - Angle1;

		if (FMath::Abs(Degree) >= 5.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 1000.0f * DelataSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
		return;
	}
}
