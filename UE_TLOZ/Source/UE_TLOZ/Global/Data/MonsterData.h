// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include <Global/Data/StateEnums.h>
#include "MonsterData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UE_TLOZ_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

	FMonsterData() {}
	~FMonsterData() {}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		MONSTER_TYPE MonsterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float ATT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float SearchRange = 2400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AttackRange = 120.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		class UBehaviorTree* AI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<MONSTER_AISTATE, class UAnimMontage*> MapAnimation;
};