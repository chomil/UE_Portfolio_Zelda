// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenItem.h"
#include <Global/GlobalGameInstance.h>

void UInvenItem::SetItemData(FName ItemName)
{
	if (GetWorld() == nullptr)
	{
		return;
	}
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		ItemData = Inst->GetItemData(ItemName);
	}
}