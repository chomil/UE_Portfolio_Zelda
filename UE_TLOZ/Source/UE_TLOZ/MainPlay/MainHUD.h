// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GamePlayWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API AMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMainHUD();
	~AMainHUD();

	UGamePlayWidget* GetMainWidget()
	{
		return MainWidget;
	}

protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

private:

	UGamePlayWidget* MainWidget = nullptr;
};
