// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/Monster.h"
#include <Global/GlobalGameInstance.h>
#include <Global/Data/MonsterData.h>
#include "BehaviorTree/BlackboardComponent.h"

AMonster::AMonster()
{
	Tags.Add(TEXT("Monster"));
}

void AMonster::BeginPlay()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurMonsterData = Inst->GetMonsterData(DataName);
		
		SetAllAnimation<MONSTER_AISTATE>(CurMonsterData->MapAnimation);
		SetAniState<MONSTER_AISTATE>(MONSTER_AISTATE::IDLE);

		HP = CurMonsterData->HP;
		ATT = CurMonsterData->ATT;
		MonsterType = CurMonsterData->MonsterType;
	}

	Super::BeginPlay();


	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(MONSTER_AISTATE::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 2400.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 120.0f);
	FVector Pos = GetActorLocation();
	GetBlackboardComponent()->SetValueAsVector(TEXT("OriginPos"), Pos);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("Damage"), 0);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("LastDamageTime"), 0);
	GetBlackboardComponent()->SetValueAsEnum(TEXT("MonsterType"), static_cast<uint8>(MonsterType));

	WeaponComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::BeginWeaponOverLap);
}

void AMonster::Damaged(float _Damage, AGlobalCharacter* _AttackCharacter = nullptr)
{
	Super::Damaged(_Damage, _AttackCharacter);


	GetBlackboardComponent()->SetValueAsFloat(TEXT("Damage"), _Damage);
}

void AMonster::Stunned(bool _bStun)
{
	Super::Stunned(_bStun);

	GetBlackboardComponent()->SetValueAsBool(TEXT("Stun"), bIsStunning);
}

void AMonster::Attacked(float _Damage, AGlobalCharacter* _HitCharacter)
{
	Super::Attacked(_Damage, _HitCharacter);
}

void AMonster::BeginWeaponOverLap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		MONSTER_AISTATE State = static_cast<MONSTER_AISTATE>(GetAniState());
		if (MONSTER_AISTATE::ATTACK == State ||
			MONSTER_AISTATE::ATTACK_JUMP_END == State ||
			MONSTER_AISTATE::ATTACK_STRONG == State)
		{
			AGlobalCharacter* Player = Cast<AGlobalCharacter>(OtherActor);
			Attacked(1.0f, Player);
		}
	}
}
