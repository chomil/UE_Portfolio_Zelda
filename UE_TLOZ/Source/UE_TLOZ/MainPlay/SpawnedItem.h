// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnedItem.generated.h"

UCLASS()
class UE_TLOZ_API ASpawnedItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnedItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void InitByName(FName _SpawnedName);

	UFUNCTION()
	void OverlapItem(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(Category = "Spawned Item", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName SpawnedName;

	const struct FItemData* ItemData = nullptr;

	UPROPERTY(Category = "Spawned Item", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMeshComponent = nullptr;

};
