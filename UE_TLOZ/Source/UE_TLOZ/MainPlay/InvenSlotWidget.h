// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "InvenSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UInvenSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	int ListIndex = 0;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	UItem* Item = nullptr;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	bool bShowItem = true;


	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	int ItemCnt = 0;
	
};
