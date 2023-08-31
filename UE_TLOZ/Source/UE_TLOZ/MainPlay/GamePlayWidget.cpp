// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "GamePlayMode.h"
#include "Kismet/GameplayStatics.h"

void UGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();


	AGameModeBase* GameModePtr = UGameplayStatics::GetGameMode(GetWorld());
	if (nullptr == GameModePtr && GameModePtr->IsValidLowLevel())
	{
		return;
	}

	AGamePlayMode* GamePlayMode = Cast<AGamePlayMode>(GameModePtr);
	GamePlayMode->SetWidget(this);
}

void UGamePlayWidget::ToggleInvenShow()
{
	if (InventoryVisible == ESlateVisibility::Hidden)
	{
		InventoryVisible = ESlateVisibility::Visible;
		GetWorld()->GetPlayerControllerIterator()->Get()->SetPause(true);
		GetWorld()->GetPlayerControllerIterator()->Get()->bShowMouseCursor = true;
		GetWorld()->GetPlayerControllerIterator()->Get()->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		InventoryVisible = ESlateVisibility::Hidden;
		GetWorld()->GetPlayerControllerIterator()->Get()->SetPause(false);
		GetWorld()->GetPlayerControllerIterator()->Get()->bShowMouseCursor = false;
		GetWorld()->GetPlayerControllerIterator()->Get()->SetInputMode(FInputModeGameOnly());
	}
}

bool UGamePlayWidget::IsInvenShow()
{
	return (InventoryVisible == ESlateVisibility::Visible) ? true : false;
}
