// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/SpawnedItem.h"
#include <Global/GlobalGameInstance.h>
#include <Global/Data/ItemData.h>
#include "Inventory.h"
#include "MainHUD.h"
#include "GamePlayWidget.h"

// Sets default values
ASpawnedItem::ASpawnedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMeshComponent;
	ItemMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpawnedItem::OverlapItem);
}

// Called when the game starts or when spawned
void ASpawnedItem::BeginPlay()
{
	Super::BeginPlay();

	InitByName(SpawnedName);
}

// Called every frame
void ASpawnedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnedItem::InitByName(FName _SpawnedName)
{
	SpawnedName = _SpawnedName;
	if ( SpawnedName.IsNone() == false)
	{
		UGlobalGameInstance* inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();
		ItemData = inst->GetItemData(SpawnedName);
		if (ItemData == nullptr)
		{
			return;
		}
		ItemMeshComponent->SetStaticMesh(ItemData->DropMesh);
		ItemMeshComponent->SetSimulatePhysics(true);

		Tags.Add(TEXT("Item"));
	}
}

void ASpawnedItem::OverlapItem(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("ItemGet")))
	{
		if (SpawnedName.IsNone() == false)
		{
			UGlobalGameInstance* inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();
			inst->GetInventory()->AddItem(SpawnedName);

			AMainHUD* hud = GetWorld()->GetFirstPlayerController()->GetHUD< AMainHUD>();
			if (ItemData != nullptr)
			{
				hud->GetMainWidget()->ItemGetPop(*ItemData);
			}

			Destroy();
		}
	}
}

