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


	UFUNCTION(BlueprintCallable)
		float GetBowHandBlending();



	UFUNCTION()
	void MontageEnd(UAnimMontage* Anim, bool _Inter);

	void ChangeWeaponSocket(UMeshComponent* _WeaponMesh, FName _SocketName);


public:
	UPROPERTY(Category = "AnimationValue", EditAnywhere, BlueprintReadWrite)
		PLAYER_ANISTATE PlayerAniState = PLAYER_ANISTATE::IDLE;

	UPROPERTY(Category = "AnimationValue", EditAnywhere, BlueprintReadWrite)
		TMap<PLAYER_ANISTATE, class UAnimMontage*> PlayerAllAnimations;

	FVector InputDir = FVector::ZeroVector;
	bool bIsDash = false;
	bool bEquipSword = false;
	bool bEquipBow = false;

	float ComboTime = 0.f;

	UPROPERTY(Category = "Weapon", BlueprintReadWrite)
	float BowChargeTime = 0.f;

	bool bBowZoom = false;


	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMeshComponent;


	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BowMeshComponent;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ArrowMeshComponent;


	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* HitDefaultParticle;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* HitStrongParticle;

protected:
	void Damaged(float _Damage, AGlobalCharacter* _AttackCharacter) override;

private:
	class AGamePlayMode* PlayMode = nullptr;
	bool bPressingL = false;
	bool bPressingR = false;

	AActor* ArrowActor = nullptr;


	UPROPERTY(BlueprintReadWrite, Category = "CharacterInfo", meta = (AllowPrivateAccess = "true"))
	float MaxSP = 1.f;
	UPROPERTY(BlueprintReadWrite, Category = "CharacterInfo", meta = (AllowPrivateAccess = "true"))
	float SP = 1.f;
	UPROPERTY(BlueprintReadWrite, Category = "CharacterInfo", meta = (AllowPrivateAccess = "true"))
	bool bTired = false;


	UFUNCTION()
		void BeginWeaponOverLap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	class USpringArmComponent* SpringArmCom;
};
