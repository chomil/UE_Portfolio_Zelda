// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvenItem.h"
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
	UFUNCTION(BlueprintCallable)
	void AddInvenItem(UObject* _Item);


	void Refresh();

	

public:

	UPROPERTY(Category = "ItemSlot", EditAnywhere, blueprintReadWrite)
	int ListIndex = 0;
	UPROPERTY(Category = "ItemSlot", EditAnywhere, blueprintReadWrite)
	UInvenItem* InvenItem = nullptr;


	UPROPERTY(Category = "ItemSlot", EditAnywhere, blueprintReadWrite)
	bool bShowItem = true;

	UPROPERTY(Category = "ItemSlot", EditAnywhere, blueprintReadWrite)
	bool bShowSlot = true;

};
