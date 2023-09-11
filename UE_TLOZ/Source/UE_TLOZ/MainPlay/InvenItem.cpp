// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenItem.h"
#include <Global/GlobalGameInstance.h>
#include "InvenSlotWidget.h"

UInvenItem::UInvenItem()
{
}

UInvenItem::~UInvenItem()
{
}

void UInvenItem::SetItemData(FName _ItemName)
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		ItemData = Inst->GetItemData(_ItemName);
		if (ItemData == nullptr)
		{
			ItemCnt = 0;
		}
	}
}

void UInvenItem::SetItemData(const FItemData* _ItemData)
{
	ItemData = _ItemData;
	if (_ItemData == nullptr)
	{
		ItemCnt = 0;
	}
}

FName UInvenItem::GetItemName()
{
	if (ItemData == nullptr)
	{
		return FName();
	}
	return ItemData->ItemName;
}

void UInvenItem::UseItem()
{
	if (ItemData == nullptr || ItemCnt <= 0 || Widget == nullptr)
	{
		return;
	}
	ItemCnt--;
	if (ItemCnt <= 0)
	{
		ItemData = nullptr;
	}
	Widget->Refresh();
}

