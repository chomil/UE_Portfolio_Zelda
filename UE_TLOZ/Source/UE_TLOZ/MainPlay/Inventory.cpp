// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/Inventory.h"
#include <Global/GlobalGameInstance.h>
#include "InvenSlotWidget.h"

UInventory::UInventory()
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	MaxSlotNum = 4;

	for (int i = 0; i < MaxSlotNum; i++)
	{
		UInvenItem* InvenItem = NewObject<UInvenItem>(GetWorld());
		InvenItem->SetItemData(nullptr);
		InvenItems.Add(InvenItem);
	}

	AddItem(TEXT("Apple"), 2);
	AddItem(TEXT("Herb"), 3);
	AddItem(TEXT("Apple"), 3);
	AddItem(TEXT("BokoHorn"), 2);
}

UInventory::~UInventory()
{
}

void UInventory::AddItem(FName ItemName, int Num)
{
	const FItemData* ItemData = GetWorld()->GetGameInstance<UGlobalGameInstance>()->GetItemData(ItemName);
	int GetIndex = -1;
	for (int i = 0; i < InvenItems.Num(); i++)
	{
		if (ItemData == InvenItems[i]->GetItemData())
		{
			GetIndex = i;
			break;
		}
		if (GetIndex == -1 && InvenItems[i]->GetItemData() == nullptr)
		{
			GetIndex = i;
		}
	}

	if (InvenItems.IsValidIndex(GetIndex))
	{
		InvenItems[GetIndex]->SetItemData(ItemData);
		InvenItems[GetIndex]->AddItemCnt(Num);
	}
}

void UInventory::RefreshInven()
{
	TArray<UInvenItem*> NewInvenItems;
	for (int i = 0; i < InvenItems.Num(); i++)
	{
		if (InvenItems[i]->GetItemData() != nullptr && InvenItems[i]->GetItemCnt() > 0)
		{
			NewInvenItems.Add(InvenItems[i]);
		}
	}

	for (int i = 0; i < MaxSlotNum; i++)
	{
		if (NewInvenItems.IsValidIndex(i))
		{
			InvenItems[i]->SetItemData(NewInvenItems[i]->GetItemData());
			InvenItems[i]->SetItemCnt(NewInvenItems[i]->GetItemCnt());
		}
		else
		{
			InvenItems[i]->SetItemData(nullptr);
		}
	}
}


