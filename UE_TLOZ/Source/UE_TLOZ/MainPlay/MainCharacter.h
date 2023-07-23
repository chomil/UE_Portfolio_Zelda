// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums.h"
#include "MainCharacter.generated.h"




UCLASS()
class UE_TLOZ_API AMainCharacter : public ACharacter
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

	void Attack();

	void TurnCamera(float Val);
	void LookUpCamera(float Val);



	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
		float BaseTurnRate;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
		float BaseLookUpRate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	PLAYER_ANISTATE aniState = PLAYER_ANISTATE::IDLE;


	UPROPERTY(Category = "AnimationValue", EditAnywhere, BlueprintReadWrite)
	TMap<PLAYER_ANISTATE, class UAnimMontage*> AllAnimations;


public:
	FVector vInputDir = FVector::ZeroVector;
	bool bIsDash = false;


	UPROPERTY(Category = "Child", EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponPtr;


	UPROPERTY(Category = "Child", EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* BowPtr;

private:
	class AGamePlayMode* PlayMode = nullptr;
	float fComboTime = 0.f;


};
