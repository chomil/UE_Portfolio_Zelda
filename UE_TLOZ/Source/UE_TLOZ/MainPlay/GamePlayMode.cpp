// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayMode.h"
#include "GamePlayWidget.h"

void AGamePlayMode::SetWidgetText(FString _Text)
{
	PlayWidget->UserText = _Text;
}