// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenWidget.h"
#include "Components/TileView.h"
#include "InvenItem.h"

void UInvenWidget::AddInvenItem(FName ItemName)
{
	if (InvenList == nullptr)
	{
		return;
	}

	UInvenItem* NewItem = NewObject<UInvenItem>();
	//NewItem->SetItemData(TEXT("Apple"));
	InvenList->AddItem(NewItem);
}

void UInvenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InvenCnt = 6;

	InvenList = Cast<UTileView>(GetWidgetFromName(TEXT("InvenTileView")));

	for (int i = 0; i < InvenCnt; i++)
	{
		AddInvenItem();
	}

}

void UInvenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}
