// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenWidget.h"
#include "Components/TileView.h"
#include "Item.h"

void UInvenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InvenList = Cast<UTileView>(GetWidgetFromName(TEXT("InvenView")));

	UItem* NewItem = NewObject<UItem>();

	InvenList->AddItem(NewItem);

}

void UInvenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}
