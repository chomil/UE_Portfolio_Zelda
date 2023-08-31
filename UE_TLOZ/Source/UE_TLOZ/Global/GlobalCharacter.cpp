// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/GlobalCharacter.h"
#include "GlobalAnimInstance.h"

// Sets default values
AGlobalCharacter::AGlobalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGlobalCharacter::Damaged(float _Damage, AGlobalCharacter* _AttackCharacter = nullptr)
{
	HP -= _Damage;
	HP = HP < 0 ? 0 : HP;
}

void AGlobalCharacter::Attacked(float _Damage, AGlobalCharacter* _HitCharacter)
{
	_HitCharacter->Damaged(_Damage, this);
}

void AGlobalCharacter::Stunned(bool _bStun)
{
	bIsStunning = _bStun;
}

// Called when the game starts or when spawned
void AGlobalCharacter::BeginPlay()
{
	Super::BeginPlay();

	GlobalAnimInstance = Cast<UGlobalAnimInstance>(GetMesh()->GetAnimInstance());

	if (GlobalAnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("GlobalAnimInstance == nullptr"));
		return;
	}

	GlobalAnimInstance->AllAnimations = AllAnimations;
}

// Called every frame
void AGlobalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AGlobalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

