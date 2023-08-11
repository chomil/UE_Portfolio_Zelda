// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/AICharacter.h"
#include <Global/Data/StateEnums.h>
#include <Global/Data/MonsterData.h>
#include "Monster.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API AMonster : public AAICharacter
{
	GENERATED_BODY()

	AMonster();

	const struct FMonsterData* CurMonsterData;

	void BeginPlay() override;

	void Damaged(float _Damage, AGlobalCharacter* _AttackCharacter) override;
	void Attacked(float _Damage, AGlobalCharacter* _HitCharacter) override;

private:
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName DataName = "NONE";


	UPROPERTY(Category = "Child", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* WeaponComponent;

	MONSTER_TYPE MonsterType;

	UFUNCTION()
		void BeginWeaponOverLap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);
};
