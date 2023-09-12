// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenSlotWidget.h"


void UInvenSlotWidget::AddInvenItem(UObject* _InvenItem)
{
	//새로운 슬롯 생성할 때
	UInvenItem* NewInvenItem = Cast<UInvenItem>(_InvenItem);
	if (NewInvenItem == nullptr)
	{
		return;
	}

	if (NewInvenItem->GetItemData() == nullptr) //빈칸을 만들때
	{
		InvenItem = NewInvenItem;
		InvenItem->SetWidget(this);
	}
	else //아이템 정보가 있을 때
	{
		InvenItem = NewInvenItem;
		InvenItem->SetWidget(this);
	}
}

void UInvenSlotWidget::Refresh() //인벤토리창 켜져있을때 리프레시 필요할때
{
	if (InvenItem == nullptr) //오류
	{
		return;
	}
	if (InvenItem->GetItemData() == nullptr) //빈칸일때 업데이트
	{
		InvenItem->SetWidget(this);
	}
	else
	{
		InvenItem->SetWidget(this);
	}
}
