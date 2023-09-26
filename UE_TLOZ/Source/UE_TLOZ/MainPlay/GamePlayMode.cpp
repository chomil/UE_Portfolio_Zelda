// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayMode.h"
#include "GamePlayWidget.h"
#include <Global/GlobalGameInstance.h>
#include "Inventory.h"

void AGamePlayMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (GetWorld() == nullptr)
	{
		return;
	}

	if (GetGameInstance<UGlobalGameInstance>()->GetInventory() == nullptr)
	{
		GetGameInstance<UGlobalGameInstance>()->SetInventory(NewObject<UInventory>(GetWorld()));
	}
}

void AGamePlayMode::SetWidgetText(FString _Text)
{
	PlayWidget->UserText = _Text;
}

void AGamePlayMode::AddWidgetText(FString _Text)
{
	PlayWidget->UserText += "\n" + _Text;
}
