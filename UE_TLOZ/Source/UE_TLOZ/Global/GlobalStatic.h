// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GlobalStatic.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UGlobalStatic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FRandomStream MainRandom;
};
