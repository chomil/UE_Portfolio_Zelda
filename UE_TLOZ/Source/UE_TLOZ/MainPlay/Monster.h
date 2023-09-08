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

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

	void Destroyed() override;


public:
	UFUNCTION(BlueprintCallable)
		void Damaged(float _Damage, AGlobalCharacter* _AttackCharacter) override;


	UFUNCTION(BlueprintCallable)
		void Stunned(bool _bStun) override;

	UFUNCTION(BlueprintCallable)
	void Attacked(float _Damage, AGlobalCharacter* _HitCharacter) override;

	UFUNCTION(BlueprintCallable)
	void GetStone();


private:


	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName DataName = "NONE";


	UPROPERTY(Category = "Child", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* WeaponComponent;

	const struct FMonsterData* CurMonsterData;
	
	MONSTER_TYPE MonsterType;

	UPROPERTY(Category = "Monster", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DeathTime = 0;

	UPROPERTY(Category = "Monster", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bShowHP = false;

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
