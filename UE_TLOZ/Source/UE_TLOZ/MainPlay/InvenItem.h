// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Global/Data/ItemData.h>
#include "InvenItem.generated.h"



UCLASS(Blueprintable)
class UE_TLOZ_API UInvenItem : public UObject
{
	GENERATED_BODY()

public: //nπ¯ƒ≠ æ∆¿Ã≈€
	UInvenItem();
	~UInvenItem();

	void SetItemData(FName _ItemName);
	void SetItemData(const FItemData* _ItemData);

	const FItemData* GetItemData()
	{
		return ItemData;
	}


	int GetItemCnt()
	{
		return ItemCnt;
	}

	void AddItemCnt(int _Cnt)
	{
		ItemCnt += _Cnt;
	}
	void SetItemCnt(int _Cnt)
	{
		ItemCnt = _Cnt;
	}

	class UInvenSlotWidget* GetWidget()
	{
		return Widget;
	}
	void SetWidget(class UInvenSlotWidget* _Widget)
	{
		Widget = _Widget;
	}

	FName GetItemName();

	UFUNCTION(BlueprintCallable)
	void UseItem();
private: 
	const FItemData* ItemData = nullptr;
	int ItemCnt = 0;
	class UInvenSlotWidget* Widget = nullptr;
};
