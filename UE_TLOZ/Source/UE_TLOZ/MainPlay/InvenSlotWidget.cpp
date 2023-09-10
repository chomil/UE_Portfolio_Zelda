// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenSlotWidget.h"


void UInvenSlotWidget::AddInvenItem(UObject* _InvenItem)
{
	UInvenItem* NewInvenItem = Cast<UInvenItem>(_InvenItem);
	if (NewInvenItem == nullptr)
	{
		return;
	}


	if (NewInvenItem->ItemData == nullptr) //빈칸으로 만들때
	{
		InvenItem = NewInvenItem;
		InvenItem->Widget = this;
		ItemTexture = nullptr;
		ItemCnt = 0;
		bShowItem = false;
		bShowSlot = true;
		bIsItem = false;
		bIsEquip = false;
		bIsSelect = false;
		return;
	}

	if (InvenItem == nullptr) //빈 슬롯에 추가될 때
	{
		InvenItem = NewInvenItem;
		InvenItem->Widget = this;
		ItemTexture = InvenItem->ItemData->SlotIcon;
		ItemCnt = NewInvenItem->ItemCnt;
		bShowItem = true;
		bShowSlot = true;
		bIsItem = true;
		bIsEquip = false;
		bIsSelect = false;
	}
	else
	{
		//기존 슬롯에 다른 아이템이 있을때 스왑

		//기존 슬롯에 같은 아이템이 있을때 합치기
	}
}

void UInvenSlotWidget::Refresh() //인벤토리창 켜져있을때 리프레시 필요할때
{
	if (InvenItem == nullptr) //오류
	{
		return;
	}
	if (InvenItem->ItemData == nullptr) //빈칸일때 업데이트
	{
		InvenItem->Widget = this;
		ItemTexture = nullptr;
		ItemCnt = 0;
		bShowItem = false;
		bShowSlot = true;
		bIsItem = false;
		bIsEquip = false;
		bIsSelect = false;
		return;
	}
	else
	{
		InvenItem->Widget = this;
		ItemTexture = InvenItem->ItemData->SlotIcon;
		ItemCnt = InvenItem->ItemCnt;
		bShowItem = true;
		bShowSlot = true;
		bIsItem = true;
		bIsEquip = false;
		bIsSelect = false;
	}
}
