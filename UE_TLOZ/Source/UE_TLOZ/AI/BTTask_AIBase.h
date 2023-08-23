// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <Global/GlobalCharacter.h>
#include <Global/GlobalAnimInstance.h>
#include <AI/AIMonCon.h>
#include <Global/Data/StateEnums.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <Global/GlobalStatic.h>
#include "BTTask_AIBase.generated.h"
/**
 * 
 */
UCLASS()
class UE_TLOZ_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_AIBase();
	void OnGameplayTaskActivated(class UGameplayTask&) override;

	float GetStateTime(UBehaviorTreeComponent& OwnerComp);

	MONSTER_AISTATE GetAiState(UBehaviorTreeComponent& OwnerComp);

	void ResetStateTime(UBehaviorTreeComponent& OwnerComp);

	template<typename EnumType>
	void SetStateChange(UBehaviorTreeComponent& OwnerComp, EnumType _State)
	{
		SetStateChange(OwnerComp, static_cast<uint8>(_State));
	}

	void SetStateChange(UBehaviorTreeComponent& OwnerComp, uint8 _State);

	class AGlobalCharacter* GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp);

	class AActor* GetTargetSearch(UBehaviorTreeComponent& OwnerComp);

	class UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp);

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void LookTarget(UBehaviorTreeComponent& OwnerComp, float DelataSeconds, bool bForce = false);

	float GetTargetAngle(UBehaviorTreeComponent& OwnerComp);

	bool Damaged(UBehaviorTreeComponent& OwnerComp);
	bool Dead(UBehaviorTreeComponent& OwnerComp);


	//AI Path Find
	//TArray<FVector> PathFind();
};
