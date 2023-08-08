// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Global/GlobalCharacter.h>
#include <Global/Data/StateEnums.h>
#include "MainCharacter.generated.h"




UCLASS()
class UE_TLOZ_API AMainCharacter : public AGlobalCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	void MoveRight(float Val);

	void MoveForward(float Val);

	void Dash(float Val);

	void PlayerJump();

	void AttackAction();
	void BowAttackStart();
	void BowAttackEnd();

	UFUNCTION(BlueprintCallable)
	float GetRightHandBlending();



	UFUNCTION()
	void MontageEnd(UAnimMontage* Anim, bool _Inter);


	UPROPERTY(Category = "AnimationValue", EditAnywhere, BlueprintReadWrite)
	PLAYER_ANISTATE PlayerAniState = PLAYER_ANISTATE::IDLE;


	UPROPERTY(Category = "AnimationValue", EditAnywhere, BlueprintReadWrite)
	TMap<PLAYER_ANISTATE, class UAnimMontage*> PlayerAllAnimations;


public:
	FVector vInputDir = FVector::ZeroVector;
	bool bIsDash = false;
	bool bEquipSword = false;
	bool bEquipBow = false;


	void ChangeWeaponSocket(UMeshComponent* _WeaponMesh, FName _SocketName);

	UPROPERTY(Category = "Child", EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponComponent;


	UPROPERTY(Category = "Child", EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BowComponent;

protected:
	void Damaged(float _Damage, AGlobalCharacter* _AttackCharacter) override;

private:
	class AGamePlayMode* PlayMode = nullptr;
	float fComboTime = 0.f;


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
