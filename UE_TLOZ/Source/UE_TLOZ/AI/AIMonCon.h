// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIMonCon.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API AAIMonCon : public AAIController
{
	GENERATED_BODY()

public:
	AAIMonCon();

protected:
	void OnPossess(APawn* _InPawn) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;
	
};
