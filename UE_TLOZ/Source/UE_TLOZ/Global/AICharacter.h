// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/GlobalCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API AAICharacter : public AGlobalCharacter
{
	GENERATED_BODY()

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree()
	{
		return BehaviorTree;
	}

	class UBlackboardComponent* GetBlackboardComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		class UBlackboardComponent* BlackboardComponent;
};
