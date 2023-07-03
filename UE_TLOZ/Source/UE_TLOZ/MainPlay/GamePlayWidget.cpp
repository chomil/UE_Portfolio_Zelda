// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayWidget.h"
#include "../GameMode/GamePlayMode.h"
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
