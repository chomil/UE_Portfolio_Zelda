// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenSlotWidget.h"

void UInvenSlotWidget::AddItem(UInvenItem* _Item)
{
	if (_Item == nullptr || _Item->ItemData == nullptr)
	{
		return;
	}
	if (Item == nullptr)
	{
		Item = _Item;
		bShowItem = true;
	}
}
