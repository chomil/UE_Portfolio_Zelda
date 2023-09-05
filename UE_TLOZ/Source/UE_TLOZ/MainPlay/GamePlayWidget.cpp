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
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		InventoryVisible = ESlateVisibility::Hidden;
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	}
}

bool UGamePlayWidget::IsInvenShow()
{
	return (InventoryVisible == ESlateVisibility::Visible) ? true : false;
}
