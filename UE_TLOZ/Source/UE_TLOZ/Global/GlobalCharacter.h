// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GlobalCharacter.generated.h"

UCLASS()
class UE_TLOZ_API AGlobalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGlobalCharacter();



	template<typename EnumType>
	bool IsAniState(EnumType _AniState)
	{
		return AniState == static_cast<int>(_AniState);
	}

	UFUNCTION(BlueprintCallable)
	int GetAniState()
	{
		return AniState;
	}

	template<typename EnumType>
	void SetAniState(EnumType _AniState)
	{
		AniState = static_cast<int>(_AniState);
	}

	void SetAniState(int _AniState)
	{
		AniState = _AniState;
	}


	template<typename EnumType>
	class UAnimMontage* GetAnimMontage(EnumType _Index)
	{
		return GetAnimMontage(static_cast<int>(_Index));
	}


	class UAnimMontage* GetAnimMontage(int _Index)
	{
		if (false == AllAnimations.Contains(_Index))
		{
			return nullptr;
		}

		return AllAnimations[_Index];
	}

	class UGlobalAnimInstance* GetAnimInstance()
	{
		return GlobalAnimInstance;
	}

	template<typename EnumType>
	void SetAllAnimation(const TMap<EnumType, class UAnimMontage*>& _MapAnimation)
	{
		for (TPair<EnumType, UAnimMontage*> Pair : _MapAnimation)
		{
			AllAnimations.Add(static_cast<int>(Pair.Key), Pair.Value);
		}
	}

	virtual void Damaged(float _Damage, AGlobalCharacter* _AttackCharacter);

	virtual void Attacked(float _Damage, AGlobalCharacter* _HitCharacter);

	virtual void Stunned(bool _bStun);

	float GetHP()
	{
		return HP;
	}
	void AddHP(float _HP);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	template<typename EnumType>
	void PushAnimation(EnumType _Index, class UAnimMontage* _Montage)
	{
		PushAnimation(static_cast<int>(_Index), _Montage);
	}

	void PushAnimation(int _Index, class UAnimMontage* _Montage)
	{
		if (true == AllAnimations.Contains(_Index))
		{
			return;
		}

		AllAnimations.Add(_Index, _Montage);
	}


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;




	UPROPERTY(BlueprintReadWrite, Category = "CharacterInfo", meta = (AllowPrivateAccess = "true"))
	float MaxHP = 1.f;
	UPROPERTY(BlueprintReadWrite, Category = "CharacterInfo", meta = (AllowPrivateAccess = "true"))
		float HP = 1.f;




	UPROPERTY(BlueprintReadWrite, Category = "CharacterInfo", meta = (AllowPrivateAccess = "true"))
		float ATT = 1.f;


	bool bIsStunning = false;

private:

	int AniState = 0;

	TMap<int, class UAnimMontage*> AllAnimations;

	class UGlobalAnimInstance* GlobalAnimInstance = nullptr;

};
