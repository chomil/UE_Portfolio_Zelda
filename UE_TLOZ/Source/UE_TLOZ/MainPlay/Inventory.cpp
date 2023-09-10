// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/Inventory.h"
#include <Global/GlobalGameInstance.h>

UInventory::UInventory()
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	MaxSlotNum = 24;

	for (int i = 0; i < MaxSlotNum; i++)
	{
		UInvenItem* InvenItem = NewObject<UInvenItem>(GetWorld());
		InvenItem->SetItemData(nullptr);
		InvenItems.Add(InvenItem);
	}

	AddItem(TEXT("Apple"), 2);
	AddItem(TEXT("Herb"), 3);
	AddItem(TEXT("Apple"), 3);
}

UInventory::~UInventory()
{
}

void UInventory::AddItem(FName ItemName, int Num)
{
	FItemData* ItemData = GetWorld()->GetGameInstance<UGlobalGameInstance>()->GetItemData(ItemName);
	int GetIndex = -1;
	for (int i = 0; i < InvenItems.Num(); i++)
	{
		if (ItemData == InvenItems[i]->ItemData)
		{
			GetIndex = i;
			break;
		}
		if (GetIndex == -1 && InvenItems[i]->ItemData == nullptr)
		{
			GetIndex = i;
		}
	}

	if (InvenItems.IsValidIndex(GetIndex))
	{
		InvenItems[GetIndex]->ItemData = ItemData;
		InvenItems[GetIndex]->ItemCnt += Num;
	}
}


