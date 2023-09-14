// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "GlobalStatic.h"
#include <Global/Data/MonsterData.h>
#include <Global/Data/ItemData.h>

UGlobalGameInstance::UGlobalGameInstance()
{

	{
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrints/Global/Data/DT_MonsterData.DT_MonsterData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			MonsterDatas = DataTable.Object;
		}
	}

	{
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrints/Global/Data/DT_ItemData.DT_ItemData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			ItemDatas = DataTable.Object;
		}
	}


	UGlobalStatic::MainRandom.GenerateNewSeed();
}

UGlobalGameInstance::~UGlobalGameInstance()
{
}

void UGlobalGameInstance::Init()
{
	Super::Init();
	if (GetWorld() == nullptr)
	{
		return;
	}
	Inventory = NewObject<UInventory>(GetWorld());
}

const FMonsterData* UGlobalGameInstance::GetMonsterData(FName _Name)
{
	if (nullptr == MonsterDatas)
	{
		return nullptr;
	}

	const FMonsterData* FindTable = MonsterDatas->FindRow<FMonsterData>(_Name, _Name.ToString());

	if (nullptr == FindTable)
	{
		return nullptr;
	}

	return FindTable;
}

const FItemData* UGlobalGameInstance::GetItemData(FName _Name)
{
	if (nullptr == ItemDatas)
	{
		return nullptr;
	}

	const FItemData* FindTable = ItemDatas->FindRow<FItemData>(_Name, _Name.ToString());

	if (nullptr == FindTable)
	{
		return nullptr;
	}
	return FindTable;
}

const FItemData& UGlobalGameInstance::GetItemDataRef(FName _Name)
{
	const FItemData* ItemData = GetItemData(_Name);

	if (ItemData != nullptr)
	{
		return *ItemData;
	}
	else
	{
		return *GetItemData(TEXT("NULL"));
	}

}
