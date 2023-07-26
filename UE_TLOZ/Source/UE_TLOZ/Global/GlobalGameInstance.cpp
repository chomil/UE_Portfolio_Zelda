// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include <Global/Data/MonsterData.h>
#include "GlobalStatic.h"

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

	UGlobalStatic::MainRandom.GenerateNewSeed();
}

UGlobalGameInstance::~UGlobalGameInstance()
{
}

UStaticMesh* UGlobalGameInstance::GetMesh(FName _Name)
{
	return nullptr;
}

FMonsterData* UGlobalGameInstance::GetMonsterData(FName _Name)
{
	if (nullptr == MonsterDatas)
	{
		return nullptr;
	}

	FMonsterData* FindTable = MonsterDatas->FindRow<FMonsterData>(_Name, _Name.ToString());

	if (nullptr == FindTable)
	{
		return nullptr;
	}

	return FindTable;
}
