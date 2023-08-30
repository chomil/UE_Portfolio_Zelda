// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"

#include "AI/BTTask_CHASE.h"

EBTNodeResult::Type UBTTask_CHASE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 300.0f;
	}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_CHASE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* TargetActor = Cast<AActor>(GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor")));

	//플레이어 놓침
	if (nullptr == TargetActor)
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		return;
	}

	float TargetLookAngle = GetTargetAngle(OwnerComp);




	//이동
	{
		FVector PawnPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector TargetPos = TargetActor->GetActorLocation();
		PawnPos.Z = 0.0f;
		TargetPos.Z = 0.0f;

		FVector Dir = TargetPos - PawnPos;


		TArray<FVector> Path = PathFind(OwnerComp, TargetActor);

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

		//AAIMonCon* AICon = Cast<AAIMonCon>(GetGlobalCharacter(OwnerComp)->GetController());
		//AICon->MoveToActor(TargetActor);



		FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
		float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

		OriginPos.Z = 0.0f;
		FVector OriginDir = OriginPos - PawnPos;

		//시작 위치로부터 멀어지면
		if (OriginDir.Size() >= SearchRange * 2.f)
		{
			GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
			SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
			return;
		}

		//타깃과 멀어지면
		if (SearchRange< Dir.Size())
		{
			GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
			SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
			return;
		}

		// 충분히 근접했다.
		MONSTER_TYPE Type = static_cast<MONSTER_TYPE>(GetBlackboardComponent(OwnerComp)->GetValueAsEnum(TEXT("MonsterType")));
		switch (Type)
		{
		case MONSTER_TYPE::NONE:
			break;
		case MONSTER_TYPE::MONSTER:
			if (Dir.Size() <= AttackRange)
			{
				//+-30도 이내면 공격 
				if (FMath::Abs(TargetLookAngle) < 30.f)
				{
					int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 3);
					if (RandomInt == 1)
					{
						SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK_STRONG);
					}
					else
					{
						SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK);
					}
					return;
				}
			}
			else if (AttackRange * 2 <= Dir.Size() && Dir.Size() <= AttackRange * 4)
			{
				if (static_cast<MONSTER_TYPE>(GetBlackboardComponent(OwnerComp)->GetValueAsEnum(TEXT("MonsterType"))) == MONSTER_TYPE::MONSTER)
				{
					//랜덤으로 점프공격
					int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 100);
					if (RandomInt == 1 && Path.Num() <= 2)
					{
						SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK_JUMP_START);
						return;
					}
				}
			}
			break;
		case MONSTER_TYPE::BOSS_HINOX:
			if (Dir.Size() <= AttackRange) //근거리일때
			{
				//+-30도 이내면 공격 
				if (FMath::Abs(TargetLookAngle) < 30.f)
				{
					int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 3);
					if (RandomInt == 1)
					{
						SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK);
					}
					else
					{
						SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK_HAND);
					}
					return;
				}
			}
			else //원거리일때
			{					
				int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 100);
				if (RandomInt == 1)
				{
					//SetStateChange(OwnerComp, MONSTER_AISTATE::STONE_GET);
				}
			}
			break;
		default:
			break;
		}
	}
}
