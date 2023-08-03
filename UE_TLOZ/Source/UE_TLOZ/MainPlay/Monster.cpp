// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/Monster.h"
#include <Global/GlobalGameInstance.h>
#include <Global/Data/MonsterData.h>
#include "BehaviorTree/BlackboardComponent.h"

void AMonster::BeginPlay()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurMonsterData = Inst->GetMonsterData(DataName);
		
		SetAllAnimation<MONSTER_AISTATE>(CurMonsterData->MapAnimation);
		SetAniState<MONSTER_AISTATE>(MONSTER_AISTATE::IDLE);
	}

	Super::BeginPlay();


	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<int>(MONSTER_AISTATE::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 1200.0f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 120.0f);
	FVector Pos = GetActorLocation();
	GetBlackboardComponent()->SetValueAsVector(TEXT("OriginPos"), Pos);
}