// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()


	void NativeConstruct();

public:
	UPROPERTY(BlueprintReadWrite)
	FString UserText;


public:
	void ToggleInvenShow();

private:
	UPROPERTY(Category = "Visible", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility InventoryVisible = ESlateVisibility::Hidden;
	
};
