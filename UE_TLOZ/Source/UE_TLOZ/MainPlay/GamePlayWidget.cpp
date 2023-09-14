// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include <Global/Data/ItemData.h>

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

void UGamePlayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (ItemPopVisible == ESlateVisibility::Visible)
	{
		ItemPopTick += InDeltaTime;
		if (ItemPopTick > 3.f)
		{
			ItemPopVisible = ESlateVisibility::Hidden;
			ItemPopTick = 0.f;
		}
	}
}

void UGamePlayWidget::ToggleInvenShow()
{
	if (IsInvenShow() == true)
	{
		InventoryVisible = ESlateVisibility::Hidden;
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		InventoryVisible = ESlateVisibility::Visible;
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	}
}

bool UGamePlayWidget::IsInvenShow()
{
	return (InventoryVisible == ESlateVisibility::Visible) ? true : false;
}

void UGamePlayWidget::SetAbilityVisible(bool _Visible)
{
	if (_Visible == true)
	{
		AbilityVisible = ESlateVisibility::Visible;

		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		int32 ScreenX = 0, ScreenY = 0;
		GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);
		GetWorld()->GetFirstPlayerController()->SetMouseLocation(ScreenX / 2, ScreenY / 2);
	}
	else
	{
		AbilityVisible = ESlateVisibility::Hidden;

		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	}
}

void UGamePlayWidget::SetBossHPVisible(bool bShow, AGlobalCharacter* _TargetBoss)
{
	if (bShow == true)
	{
		BossHPVisible = ESlateVisibility::Visible;
		TargetBoss = _TargetBoss;
	}
	else
	{
		BossHPVisible = ESlateVisibility::Hidden;
	}
}

void UGamePlayWidget::ItemGetPop(const FItemData& _ItemData)
{
	ItemPopVisible = ESlateVisibility::Visible;
	ItemPopTick = 0.0f;
	ItemTex = _ItemData.SlotIcon;
	ItemName = _ItemData.ItemName;
}
