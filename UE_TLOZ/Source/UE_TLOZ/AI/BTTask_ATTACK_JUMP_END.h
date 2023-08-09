// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask_AIBase.h"
#include "BTTask_ATTACK_JUMP_END.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UBTTask_ATTACK_JUMP_END : public UBTTask_AIBase
{
	GENERATED_BODY()

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds) override;
};
