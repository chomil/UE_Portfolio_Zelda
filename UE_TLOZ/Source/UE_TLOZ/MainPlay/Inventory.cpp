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

	MaxSlotNum = 10;

	//초기 인벤토리 칸만큼 빈칸 생성
	for (int i = 0; i < MaxSlotNum; i++)
	{
		UInvenItem* InvenItem = NewObject<UInvenItem>(GetWorld());
		InvenItem->SetItemData(nullptr);
		InvenItems.Add(InvenItem);
	}

	//초기 인벤토리 아이템 추가
	AddItem(TEXT("Sword"), 1, true);
	AddItem(TEXT("Bow"), 1, true);
	AddItem(TEXT("Apple"), 2);
	AddItem(TEXT("Herb"), 3);
	AddItem(TEXT("Apple"), 3);
	AddItem(TEXT("BokoHorn"), 2);
}

UInventory::~UInventory()
{
}

void UInventory::AddItem(FName _ItemName, int _Num, bool _Equip)
{
	const FItemData* ItemData = GetWorld()->GetGameInstance<UGlobalGameInstance>()->GetItemData(_ItemName);
	int GetIndex = -1;
	for (int i = 0; i < InvenItems.Num(); i++)
	{
		if (ItemData == InvenItems[i]->GetItemData()) //이미 있으면 추가
		{
			InvenItems[i]->AddItemCnt(_Num);
			return;
		}
		if (GetIndex == -1 && InvenItems[i]->GetItemData() == nullptr) 
		{
			GetIndex = i;
		}
	}

	if (InvenItems.IsValidIndex(GetIndex)) //없으면 가장 앞의 빈칸에 추가
	{
		InvenItems[GetIndex]->SetItemData(ItemData, _Num, _Equip);
	}
}


void UInventory::RefreshInven() //인벤토리 칸 자동 정렬
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
			InvenItems[i]->SetItemData(NewInvenItems[i]->GetItemData(), NewInvenItems[i]->GetItemCnt(), NewInvenItems[i]->GetIsEquip());
		}
		else
		{
			InvenItems[i]->SetItemData(nullptr);
		}
	}
}


