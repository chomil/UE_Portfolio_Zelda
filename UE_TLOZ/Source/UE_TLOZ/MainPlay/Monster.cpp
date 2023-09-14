// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/Monster.h"
#include <Global/GlobalGameInstance.h>
#include <Global/Data/MonsterData.h>
#include "Kismet/GameplayStatics.h"
#include "NavMesh/RecastNavMesh.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainHUD.h"

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

		MaxHP = CurMonsterData->HP;
		HP = CurMonsterData->HP;
		ATT = CurMonsterData->ATT;
		MonsterType = CurMonsterData->MonsterType;

		GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), CurMonsterData->SearchRange);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), CurMonsterData->AttackRange);
		GetBlackboardComponent()->SetValueAsEnum(TEXT("MonsterType"), static_cast<uint8>(MonsterType));
	}

	Super::BeginPlay();


	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(MONSTER_AISTATE::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	FVector Pos = GetActorLocation();
	GetBlackboardComponent()->SetValueAsVector(TEXT("OriginPos"), Pos);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("Damage"), 0);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("LastDamageTime"), 0);

	WeaponComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::BeginWeaponOverLap);



	TArray<AActor*> RecastNavMeshs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARecastNavMesh::StaticClass(), RecastNavMeshs);
	AActor* RecastActor = nullptr;
	for (AActor* Recast : RecastNavMeshs)
	{
		if (MonsterType == MONSTER_TYPE::BOSS_HINOX)
		{
			if (Recast->GetName() == FString("RecastNavMesh-Boss"))
			{
				UE_LOG(LogTemp, Log, TEXT("Boss PathFinding Attach"));
				RecastActor = Recast;
				break;
			}
		}
		else
		{
			if (Recast->GetName() == FString("RecastNavMesh-Default"))
			{
				UE_LOG(LogTemp, Log, TEXT("Default PathFinding Attach"));
				RecastActor = Recast;
				break;
			}
		}
	}
	GetBlackboardComponent()->SetValueAsObject(TEXT("NaviMesh"), RecastActor);
}

void AMonster::Damaged(float _Damage, AGlobalCharacter* _AttackCharacter = nullptr)
{
	if (IsAniState(MONSTER_AISTATE::STUN_START) || IsAniState(MONSTER_AISTATE::STUN_LOOP))
	{
		Super::Damaged(_Damage * 5, _AttackCharacter);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("Damage"), _Damage * 5);
	}
	else
	{
		Super::Damaged(_Damage, _AttackCharacter);
		GetBlackboardComponent()->SetValueAsFloat(TEXT("Damage"), _Damage);
	}
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

void AMonster::GetStone()
{
}

void AMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (HP <= 0)
	{
		DeathTime += _DeltaTime;
	}


	float ShowHPRange = GetBlackboardComponent()->GetValueAsFloat(TEXT("SearchRange"));

	AGlobalCharacter* Player = Cast<AGlobalCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if ((Player->GetActorLocation() - GetActorLocation()).Size() < ShowHPRange)
	{
		bShowHP = true;
	}
	else
	{
		switch (static_cast<MONSTER_AISTATE>(GetAniState()))
		{
		case MONSTER_AISTATE::HIT:
		case MONSTER_AISTATE::DEATH:
			bShowHP = true;
			break;
		default:
			bShowHP = false;
			break;
		}
	}

	if (MonsterType == MONSTER_TYPE::BOSS_HINOX)
	{
		APlayerController* HUDController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		AMainHUD* HUD = HUDController->GetHUD<AMainHUD>();
		if (HUD == nullptr || HUD->IsValidLowLevel() == false)
		{
			return;
		}
		if (bShowHP == true)
		{
			HUD->GetMainWidget()->SetBossHPVisible(true, this);
		}
		else
		{
			HUD->GetMainWidget()->SetBossHPVisible(false);
		}
	}

}

void AMonster::Destroyed()
{
	Super::Destroyed();
	if (MonsterType == MONSTER_TYPE::BOSS_HINOX)
	{
		APlayerController* HUDController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		AMainHUD* HUD = HUDController->GetHUD<AMainHUD>();
		if (HUD == nullptr || HUD->IsValidLowLevel() == false)
		{
			return;
		}
		HUD->GetMainWidget()->SetBossHPVisible(false);
	}
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
