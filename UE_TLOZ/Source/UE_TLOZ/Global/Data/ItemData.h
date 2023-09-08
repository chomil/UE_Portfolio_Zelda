// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include <Global/Data/StateEnums.h>
#include "ItemData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct UE_TLOZ_API FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData() {}
	~FItemData() {}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ITEM_TYPE ItemType = ITEM_TYPE::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture* SlotIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStaticMesh* DropMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int MaxStack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bUsable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bEqipable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ATT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Value;
};