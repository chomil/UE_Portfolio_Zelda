// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenWidget.h"
#include "Components/TileView.h"
#include "InvenItem.h"
#include <Global/GlobalGameInstance.h>
#include "Inventory.h"
#include "InvenSlotWidget.h"


void UInvenWidget::Refresh()
{
	InvenList->ClearListItems();
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	Inst->GetInventory()->RefreshInven();

	const TArray<UInvenItem*>& InvenItems = Inst->GetInventory()->GetItems();

	for (UInvenItem* Item : InvenItems)
	{
		InvenList->AddItem(Item);
		if (Item->GetWidget() != nullptr)
		{
			Item->GetWidget()->Refresh();
		}
	}
}

void UInvenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InvenList = Cast<UTileView>(GetWidgetFromName(TEXT("InvenTileView")));

	Refresh();
}

void UInvenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	//Refresh();
}
