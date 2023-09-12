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

void UInvenItem::SetItemData(FName _ItemName, int _Num, bool _Equip)
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
			bIsItem = false;
		}
		else
		{
			ItemCnt = _Num;
			bIsItem = true;

			if (ItemData->bEqipable == true)
			{
				bIsEquip = _Equip;
			}
		}
	}
}

void UInvenItem::SetItemData(const FItemData* _ItemData, int _Num, bool _Equip)
{
	ItemData = _ItemData;
	if (ItemData == nullptr)
	{
		ItemCnt = 0;
		bIsItem = false;
	}
	else
	{
		ItemCnt = _Num;
		bIsItem = true;
		if (ItemData->bEqipable == true)
		{
			bIsEquip = _Equip;
		}
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

UTexture* UInvenItem::GetItemTexture()
{
	if (ItemData == nullptr)
	{
		return nullptr;
	}
	return ItemData->SlotIcon;
}

void UInvenItem::UseItem()
{
	if (ItemData == nullptr || ItemCnt <= 0 || Widget == nullptr)
	{
		return;
	}
	if (ItemData->bUsable == true)
	{
		ItemCnt--;
		if (ItemCnt <= 0)
		{
			ItemData = nullptr;
		}
	}
	Widget->Refresh();
}

void UInvenItem::SelectItem(bool _Select)
{
	bIsSelect = _Select;
}


