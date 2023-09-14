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

protected:
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite)
	FString UserText;


public:
	UFUNCTION(BlueprintCallable)
	void ToggleInvenShow();
	UFUNCTION(BlueprintCallable)
	bool IsInvenShow();


	UFUNCTION(BlueprintCallable)
	void SetAbilityVisible(bool _Visible);

	UFUNCTION(BlueprintCallable)
	void SetBossHPVisible(bool bShow, AGlobalCharacter* _TargetBoss = nullptr);


	UFUNCTION(BlueprintCallable)
	void ItemGetPop(const struct FItemData& _ItemData);


private:
	UPROPERTY(Category = "Visible", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility InventoryVisible = ESlateVisibility::Hidden;


	UPROPERTY(Category = "Visible", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility AbilityVisible = ESlateVisibility::Hidden;


	UPROPERTY(Category = "BossHPWidget", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility BossHPVisible = ESlateVisibility::Hidden;

	UPROPERTY(Category = "BossHPWidget", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AGlobalCharacter* TargetBoss = nullptr;


	UPROPERTY(Category = "ItemPop", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESlateVisibility ItemPopVisible = ESlateVisibility::Hidden;

	UPROPERTY(Category = "ItemPop", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture* ItemTex = nullptr;

	UPROPERTY(Category = "ItemPop", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName ItemName = "";

	float ItemPopTick = 0.f;



};
