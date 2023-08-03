// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_CHASE.h"

EBTNodeResult::Type UBTTask_CHASE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 200.0f;
	}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_CHASE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	//플레이어 놓침
	if (nullptr == TargetActor)
	{
		//SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		return;
	}

	float Dot;
	//회전
	{
		FVector TargetPos = TargetActor->GetActorLocation();
		FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		TargetPos.Z = 0.0f;
		ThisPos.Z = 0.0f;

		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector ThisForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		ThisForward.Normalize();

		FVector Cross = FVector::CrossProduct(ThisForward, Dir);
		Dot = FVector::DotProduct(ThisForward, Dir);
		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = ThisForward.Rotation().Yaw;

		if (FMath::Abs(Angle0 - Angle1) >= 5.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DelataSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}


	//이동
	{
		FVector PawnPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector TargetPos = TargetActor->GetActorLocation();
		PawnPos.Z = 0.0f;
		TargetPos.Z = 0.0f;

		FVector Dir = TargetPos - PawnPos;

		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);

		FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
		float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

		OriginPos.Z = 0.0f;
		FVector OriginDir = OriginPos - PawnPos;

		//if (OriginDir.Size() >= SearchRange * 1.5f)
		//{
		//	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
		//	SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		//	int a = 0;
		//	return;
		//}

		if (SearchRange < Dir.Size())
		{
			SetStateChange(OwnerComp, MONSTER_AISTATE::IDLE);
			return;
		}

		// 충분히 근접했다.
		if (AttackRange >= Dir.Size())
		{
			//+-30도 이내면 공격 
			if (Dot > cosf(30.f/180.f*3.14f))
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK);
				return;
			}
		}
	}
}
