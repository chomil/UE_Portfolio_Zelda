// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
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

	const struct FMonsterData* GetMonsterData(FName _Name);

	const struct FItemData* GetItemData(FName _Name);

	UFUNCTION(BlueprintCallable)
	const struct FItemData& GetItemDataRef(FName _Name);

	UFUNCTION(BlueprintCallable)
	class UInventory* GetInventory()
	{
		return Inventory;
	};
	void SetInventory(class UInventory* _Inventory)
	{
		Inventory = _Inventory;
	};


	UFUNCTION(BlueprintCallable)
	void SetGrayScreenBlend(float _Blend)
	{
		GrayScreenBlend = _Blend;
	}
	UFUNCTION(BlueprintCallable)
	float GetGrayScreenBlend()
	{
		return GrayScreenBlend;
	}

private:

	UPROPERTY()
		UDataTable* MonsterDatas;


	UPROPERTY()
	UDataTable* ItemDatas;

	UPROPERTY()
	class UInventory* Inventory;


	UPROPERTY()
	float GrayScreenBlend = 0.f;

};
