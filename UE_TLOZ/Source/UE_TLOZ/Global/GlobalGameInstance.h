// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include <MainPlay/Inventory.h>
#include "GlobalGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UGlobalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGlobalGameInstance();
	~UGlobalGameInstance();

	void Init()override;

	struct FMonsterData* GetMonsterData(FName _Name);


	struct FItemData* GetItemData(FName _Name);

	UInventory* GetInventory()
	{
		return Inventory;
	};

private:

	UPROPERTY()
		UDataTable* MonsterDatas;


	UPROPERTY()
	UDataTable* ItemDatas;

	UPROPERTY()
	UInventory* Inventory;

};
