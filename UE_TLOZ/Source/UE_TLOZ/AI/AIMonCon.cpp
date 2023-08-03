// Fill out your copyright notice in the Description page of Project Settings.


#include <AI/AIMonCon.h>
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include <Global/AICharacter.h>

AAIMonCon::AAIMonCon()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));;
}

void AAIMonCon::OnPossess(APawn* _InPawn)
{

	Super::OnPossess(_InPawn);

	if (nullptr != BehaviorTreeComponent && true == BehaviorTreeComponent->IsValidLowLevel())
	{
		AAICharacter* AIPawn = Cast<AAICharacter>(_InPawn);

		if (AIPawn == nullptr)
		{
			return;
		}

		UBehaviorTree* BehaviorTree = AIPawn->GetBehaviorTree();

		if (nullptr == BehaviorTree || false == BehaviorTree->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == BehaviorTree || false == BehaviorTree->IsValidLowLevel())"), __FUNCTION__, __LINE__);
			return;
		}

		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

		BlackboardComponent->SetValueAsObject(TEXT("SelfActor"), _InPawn);

		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}

}
