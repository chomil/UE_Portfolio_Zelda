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

	void SetItemData(FName _ItemName, int _Num = 1, bool _Equip = false);
	void SetItemData(const FItemData* _ItemData, int _Num = 1, bool _Equip = false);

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

	UFUNCTION(BlueprintCallable)
	FName GetItemName();

	bool GetIsEquip()
	{
		return bIsEquip;
	}


	UFUNCTION(BlueprintCallable)
	UTexture* GetItemTexture();

	UFUNCTION(BlueprintCallable)
	void UseItem();

	
	UFUNCTION(BlueprintCallable)
	void SelectItem(bool _Select = true);


private: 
	const FItemData* ItemData = nullptr;
	class UInvenSlotWidget* Widget = nullptr; 	

	UPROPERTY(Category = "Item", blueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int ItemCnt = 0;

	UPROPERTY(Category = "Item", blueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsItem = false;
	UPROPERTY(Category = "Item", blueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSelect = false;
	UPROPERTY(Category = "Item", blueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsEquip = false;
};
