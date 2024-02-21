// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Global/GlobalCharacter.h>
#include <Global/Data/StateEnums.h>
#include "ArrowActor.h"
#include "MainCharacter.generated.h"




UCLASS()
class UE_TLOZ_API AMainCharacter : public AGlobalCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Damaged(float _Damage, AGlobalCharacter* _AttackCharacter) override;
private:
	//Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputActionMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputActionMouseMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputActionJump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputActionDash;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputActionSword;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputActionBow;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputActionAbility;

public:	

	void Move(const struct FInputActionInstance& Instance);
	void Look(const struct FInputActionInstance& Instance);
	void PlayerJump(const struct FInputActionInstance& Instance);
	void Dash(const struct FInputActionInstance& Instance);
	void SwordAttack(const struct FInputActionInstance& Instance);
	void BowAttack(const struct FInputActionInstance& Instance);

	void Ability(const struct FInputActionInstance& Instance);

	void Revereco(bool bStart);

	UFUNCTION(BlueprintCallable)
		float GetRightHandBlending();

	UFUNCTION(BlueprintCallable)
	float GetBowHandBlending();

	UFUNCTION(BlueprintCallable)
	bool IsAttacking();

	UFUNCTION()
	void MontageEnd(UAnimMontage* Anim, bool _Inter);

	void ChangeWeaponSocket(UMeshComponent* _WeaponMesh, FName _SocketName);


public:
	UPROPERTY(Category = "AnimationValue", EditAnywhere, BlueprintReadWrite)
	PLAYER_ANISTATE PlayerAniState = PLAYER_ANISTATE::IDLE;

	UPROPERTY(Category = "AnimationValue", EditAnywhere, BlueprintReadWrite)
	TMap<PLAYER_ANISTATE, class UAnimMontage*> PlayerAllAnimations;

	PLAYER_ABILITY PlayerAbility = PLAYER_ABILITY::REVERECO;
	bool bIsAbility = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursorWidget To Load")
	TSubclassOf<UUserWidget> CursorDefaultReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursorWidget To Load")
	TSubclassOf<UUserWidget> CursorAimReference;
	UPROPERTY()
	class UUserWidget* CursorDefault = nullptr;
	UPROPERTY()
	class UUserWidget* CursorAim = nullptr;


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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AArrowActor> ArrowBP;


	UPROPERTY(Category = "Items", EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> FrontItems;

private:
	class AGamePlayMode* PlayMode = nullptr;
	AArrowActor* ArrowActor = nullptr;



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
