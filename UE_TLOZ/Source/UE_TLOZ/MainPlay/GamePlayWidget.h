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
	UFUNCTION(BlueprintCallable)
	void ToggleInvenShow();
	UFUNCTION(BlueprintCallable)
	bool IsInvenShow();

	UFUNCTION(BlueprintCallable)
	void SetBossHPVisible(bool bShow, AGlobalCharacter* _TargetBoss = nullptr);


private:
	UPROPERTY(Category = "Visible", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility InventoryVisible = ESlateVisibility::Hidden;


	UPROPERTY(Category = "BossHPWidget", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility BossHPVisible = ESlateVisibility::Hidden;

	UPROPERTY(Category = "BossHPWidget", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AGlobalCharacter* TargetBoss = nullptr;
	
};
