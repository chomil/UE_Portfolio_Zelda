// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Global/Data/ItemData.h>
#include "Item.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UItem : public UObject
{
	GENERATED_BODY()

public:
	FItemData* ItemData;
};
