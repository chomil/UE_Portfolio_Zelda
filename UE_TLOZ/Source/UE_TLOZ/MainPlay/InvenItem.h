// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Global/Data/ItemData.h>
#include "InvenItem.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UInvenItem : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void SetItemData(FName ItemName);

	UFUNCTION(BlueprintCallable)
	FItemData GetItemData()
	{
		return *ItemData;
	}
public:
	FItemData* ItemData = nullptr;
	int ItemCnt = 0;
	class UInvenSlotWidget* Widget = nullptr;
};
