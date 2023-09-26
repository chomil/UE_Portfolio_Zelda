// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GamePlayMode.generated.h"


UCLASS()
class UE_TLOZ_API AGamePlayMode : public AGameMode
{
	GENERATED_BODY()

protected:
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	void SetWidget(class UGamePlayWidget* _PlayWidget)
	{
		PlayWidget = _PlayWidget;
	}

	void SetWidgetText(FString _Text);
	void AddWidgetText(FString _Text);

private:
	class UGamePlayWidget* PlayWidget = nullptr;
};
