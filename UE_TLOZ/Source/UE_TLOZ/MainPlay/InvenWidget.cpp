// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenWidget.h"
#include "Components/TileView.h"
#include "InvenItem.h"
#include <Global/GlobalGameInstance.h>
#include "Inventory.h"
#include "InvenSlotWidget.h"

void UInvenWidget::AddInvenItem(FName ItemName)
{
	if (InvenList == nullptr)
	{
		return;
	}
	int InvenNum = InvenList->GetNumItems();

	for (int i = 0; i < InvenNum; i++)
	{
		UInvenItem* InvenItem = Cast<UInvenItem>(InvenList->GetItemAt(i));

		if (InvenItem->ItemData == nullptr)
		{
			InvenItem->SetItemData(ItemName);
			return;
		}
	}

}

void UInvenWidget::Refresh()
{
	InvenList->ClearListItems();

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	const TArray<UInvenItem*>& InvenItems = Inst->GetInventory()->GetItems();

	for (UInvenItem* Item : InvenItems)
	{
		InvenList->AddItem(Item);
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
