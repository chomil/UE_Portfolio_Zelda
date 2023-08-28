// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AIBase.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Components/CapsuleComponent.h"


UBTTask_AIBase::UBTTask_AIBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

void UBTTask_AIBase::OnGameplayTaskActivated(class UGameplayTask&)
{

}

float UBTTask_AIBase::GetStateTime(UBehaviorTreeComponent& OwnerComp)
{
	float StateTime = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("StateTime"));

	return StateTime;
}

MONSTER_AISTATE UBTTask_AIBase::GetAiState(UBehaviorTreeComponent& OwnerComp)
{
	uint8 Enum = GetBlackboardComponent(OwnerComp)->GetValueAsEnum(TEXT("AIState"));

	return static_cast<MONSTER_AISTATE>(Enum);

}

void UBTTask_AIBase::ResetStateTime(UBehaviorTreeComponent& OwnerComp)
{
	GetBlackboardComponent(OwnerComp)->SetValueAsFloat(TEXT("StateTime"), 0.0f);
}

AGlobalCharacter* UBTTask_AIBase::GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp)
{
	AAIMonCon* AiCon = OwnerComp.GetOwner<AAIMonCon>();
	if (nullptr == AiCon || false == AiCon->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Chracter || false == Chracter->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}


	AGlobalCharacter* Character = AiCon->GetPawn<AGlobalCharacter>();

	if (nullptr == Character || false == Character->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Chracter || false == Chracter->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return Character;
}

UBlackboardComponent* UBTTask_AIBase::GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();

	if (nullptr == BlackBoard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == BlackBoard)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return BlackBoard;
}

EBTNodeResult::Type UBTTask_AIBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetGlobalCharacter(OwnerComp)->SetAniState(GetAiState(OwnerComp));


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	//태스크에서 플레이할 몽타주가 없으면 Fail
	if (Montage == nullptr)
	{
		SetStateChange(OwnerComp, static_cast<uint8>(MONSTER_AISTATE::IDLE));
		return EBTNodeResult::Type::Failed;
	}
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_AIBase::SetStateChange(UBehaviorTreeComponent& OwnerComp, uint8 _State)
{
	GetBlackboardComponent(OwnerComp)->SetValueAsEnum(TEXT("AIState"), _State);

	ResetStateTime(OwnerComp);

	FinishLatentTask(OwnerComp, EBTNodeResult::Type::Failed);
}


void UBTTask_AIBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackBoard = GetBlackboardComponent(OwnerComp);

	float StateTime = BlackBoard->GetValueAsFloat(TEXT("StateTime"));
	StateTime += DeltaSeconds;
	BlackBoard->SetValueAsFloat(TEXT("StateTime"), StateTime);

	float LastDamageTime = BlackBoard->GetValueAsFloat(TEXT("LastDamageTime"));
	LastDamageTime += DeltaSeconds;
	BlackBoard->SetValueAsFloat(TEXT("LastDamageTime"), LastDamageTime);

	MONSTER_AISTATE State =  static_cast<MONSTER_AISTATE>(BlackBoard->GetValueAsEnum(TEXT("AIState")));
	
	if (State == MONSTER_AISTATE::DEATH)
	{
		return;
	}

	if (Dead(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::DEATH);
		return;
	}


	if (GetBlackboardComponent(OwnerComp)->GetValueAsBool(TEXT("Stun")) == true)
	{
		if (State != MONSTER_AISTATE::STUN_START && State != MONSTER_AISTATE::STUN_LOOP && State != MONSTER_AISTATE::STUN_END)
		{
			SetStateChange(OwnerComp, MONSTER_AISTATE::STUN_START);
			return;
		}
	}
	
	if (Damaged(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::HIT);
		return;
	}
	
	AActor* ResultActor = GetTargetSearch(OwnerComp);
	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), ResultActor);
}


void UBTTask_AIBase::LookTarget(UBehaviorTreeComponent& OwnerComp, float DeltaSeconds, bool bForce)
{
	AActor* TargetActor = Cast<AActor>(GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor")));

	//플레이어 놓침
	if (nullptr == TargetActor)
	{
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	LookTarget(OwnerComp, TargetPos, DeltaSeconds, bForce);
}

void UBTTask_AIBase::LookTarget(UBehaviorTreeComponent& OwnerComp, FVector TargetPos, float DeltaSeconds, bool bForce)
{
	float Degree = 0.f;

	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	TargetPos.Z = 0.0f;
	ThisPos.Z = 0.0f;

	FVector Dir = TargetPos - ThisPos;
	Dir.Normalize();

	FVector ThisForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
	ThisForward.Normalize();

	FVector Cross = FVector::CrossProduct(ThisForward, Dir);
	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = ThisForward.Rotation().Yaw;
	Degree = Angle0 - Angle1;

	if (bForce)
	{
		FRotator Rot = Dir.Rotation();
		GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
	}
	else
	{
		if (FMath::Abs(Degree) >= 5.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DeltaSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}
}

float UBTTask_AIBase::GetTargetAngle(UBehaviorTreeComponent& OwnerComp)
{
	float Degree = 0.f;

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	//플레이어 놓침
	if (nullptr == TargetActor)
	{
		return 0.f;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	TargetPos.Z = 0.0f;
	ThisPos.Z = 0.0f;

	FVector Dir = TargetPos - ThisPos;
	Dir.Normalize();

	FVector ThisForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
	ThisForward.Normalize();

	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = ThisForward.Rotation().Yaw;
	Degree = Angle0 - Angle1;

	return Degree;
}

bool UBTTask_AIBase::Damaged(UBehaviorTreeComponent& OwnerComp)
{
	float LastDamageTime = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("LastDamageTime"));
	float Damage = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("Damage"));

	if (Damage > 0.f)
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsFloat(TEXT("Damage"), 0.f);


		if (GetBlackboardComponent(OwnerComp)->GetValueAsBool(TEXT("Stun")) == true)
		{
			return false;
		}

		MONSTER_AISTATE State = static_cast<MONSTER_AISTATE>(GetBlackboardComponent(OwnerComp)->GetValueAsEnum(TEXT("AIState")));
		if (State == MONSTER_AISTATE::HIT || State == MONSTER_AISTATE::DEATH || 
			State == MONSTER_AISTATE::STUN_START || State == MONSTER_AISTATE::STUN_LOOP || State == MONSTER_AISTATE::STUN_END)
		{
			return false;
		}

		if (LastDamageTime > 2.f)
		{
			return true;
		}
	}

	return false;
}

bool UBTTask_AIBase::Dead(UBehaviorTreeComponent& OwnerComp)
{
	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);
	if (Character->GetHP() <= 0)
	{
		MONSTER_AISTATE State = static_cast<MONSTER_AISTATE>(GetBlackboardComponent(OwnerComp)->GetValueAsEnum(TEXT("AIState")));
		if (State == MONSTER_AISTATE::DEATH)
		{
			return false;
		}
		return true;
	}
	return false;
}





class AActor* UBTTask_AIBase::GetTargetSearch(UBehaviorTreeComponent& OwnerComp)
{
	FString TargetTag = GetBlackboardComponent(OwnerComp)->GetValueAsString(TEXT("TargetTag"));
	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), *TargetTag, TargetActors);

	AGlobalCharacter* Pawn = GetGlobalCharacter(OwnerComp);

	float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));


	AActor* ResultActor = nullptr;

	if (0 != TargetActors.Num())
	{
		float Range = TNumericLimits<float>::Max();

		for (size_t i = 0; i < TargetActors.Num(); i++)
		{
			float Dis = (Pawn->GetActorLocation() - TargetActors[i]->GetActorLocation()).Size();

			if (SearchRange <= Dis)
			{
				continue;
			}

			if (Range > Dis)
			{
				Range = Dis;
				ResultActor = TargetActors[i];
			}
		}
	}

	return ResultActor;
}


TArray<FVector> UBTTask_AIBase::PathFind(UBehaviorTreeComponent& _OwnerComp, AActor* _TargetActor)
{
	if (_TargetActor == nullptr)
	{
		return TArray<FVector>();
	}
	UCapsuleComponent* Capsule = Cast<AGlobalCharacter>(_TargetActor)->GetCapsuleComponent();
	float Height = Capsule->GetScaledCapsuleHalfHeight();
	FVector TargetPos = _TargetActor->GetActorLocation();
	TargetPos.Z -= Height;

	return PathFind(_OwnerComp, TargetPos);
}

TArray<FVector> UBTTask_AIBase::PathFind(UBehaviorTreeComponent& _OwnerComp, FVector _TargetPos)
{
	UNavigationPath* Path = nullptr;
	FVector StartPos = GetGlobalCharacter(_OwnerComp)->GetActorLocation();

	UCapsuleComponent* Capsule = GetGlobalCharacter(_OwnerComp)->GetCapsuleComponent();
	float Height = Capsule->GetScaledCapsuleHalfHeight();
	StartPos.Z -= Height;

	Path = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), StartPos, _TargetPos);

	if (nullptr == Path)
	{
		return TArray<FVector>();
	}
	if (false == Path->IsValid())
	{
		return TArray<FVector>();
	}

	return Path->PathPoints;
}
