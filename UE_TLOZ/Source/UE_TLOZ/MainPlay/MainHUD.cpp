// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/MainHUD.h"

AMainHUD::AMainHUD()
{
}

AMainHUD::~AMainHUD()
{
}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	FSoftClassPath ClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/GamePlay/UI/WBP_GamePlayWidget.WBP_GamePlayWidget_C'"));
	TSubclassOf<UUserWidget> MainWidgetClass = ClassPath.TryLoadClass<UUserWidget>();

	MainWidget = Cast<UGamePlayWidget>(CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass));
	MainWidget->AddToViewport();
}

void AMainHUD::Tick(float _Delta)
{
	Super::Tick(_Delta);

}
