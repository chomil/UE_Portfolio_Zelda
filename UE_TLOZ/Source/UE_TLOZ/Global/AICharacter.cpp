// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <AI/AIMonCon.h>

UBlackboardComponent* AAICharacter::GetBlackboardComponent()
{
	if (nullptr == BlackboardComponent)
	{
		AAIMonCon* AiCon = GetController<AAIMonCon>();

		if (nullptr == AiCon)
		{
			return nullptr;
		}

		BlackboardComponent = AiCon->GetBlackboardComponent();
	}

	return BlackboardComponent;
}