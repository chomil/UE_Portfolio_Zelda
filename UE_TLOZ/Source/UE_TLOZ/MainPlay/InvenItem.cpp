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
	}
}

void UInvenItem::SetItemData(FItemData* _ItemData)
{
	ItemData = _ItemData;
}

