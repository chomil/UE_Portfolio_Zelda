// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	Tags.Add(TEXT("Player"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{

	SetAllAnimation(PlayerAllAnimations);
	SetAniState(PLAYER_ANISTATE::IDLE);

	Super::BeginPlay();


	AGameModeBase* GameModePtr = UGameplayStatics::GetGameMode(GetWorld());
	if (nullptr == GameModePtr)
	{
		return;
	}

	AGamePlayMode* GamePlayMode = Cast<AGamePlayMode>(GameModePtr);
	PlayMode = GamePlayMode;


	GetMesh()->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &AMainCharacter::MontageEnd);

	WeaponComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::BeginWeaponOverLap);

	SpringArmCom = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float LeftButtonTime = GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(EKeys::LeftMouseButton);
	if (LeftButtonTime > 0)
	{
		AttackAction();
		//UE_LOG(LogTemp, Log, TEXT("%f Second"), LeftButtonTime);
	}

	float RightButtonTime = GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(EKeys::RightMouseButton);
	if (RightButtonTime > 0)
	{
		BowAttackStart();
		//UE_LOG(LogTemp, Log, TEXT("%f Second"), RightButtonTime);
	}
	

	if (PlayMode == nullptr)
	{
		return;
	}

	if (fBowChargeTime > 0)
	{ //활시위 당길때 등 카메라의 스프링 암 오프셋 이동
		SpringArmCom->TargetArmLength = FMath::Lerp(SpringArmCom->TargetArmLength, 200.f, fBowChargeTime * 30.f * DeltaTime);
		SpringArmCom->SocketOffset = FMath::Lerp(SpringArmCom->SocketOffset, FVector(0, 40, 50), fBowChargeTime * 30.f * DeltaTime);

		bUseControllerRotationRoll = false;
		bUseControllerRotationPitch = true;
		bUseControllerRotationYaw = true;

		FRotator Rot = GetControlRotation();
		if (Rot.Pitch > 180)
		{
			Rot.Pitch -= 360;
		}
		if (Rot.Pitch < -10)
		{
			Rot.Pitch = -10;
		}
		else if (40 < Rot.Pitch)
		{
			Rot.Pitch = 40;
		}
		UE_LOG(LogTemp, Log, TEXT("%f Pitch"), Rot.Pitch);
		GetController()->ClientSetRotation(Rot);
		//FVector NewVec = FVector(0, 0, 0);
		//GetController()->SetControlRotation(NewVec.Rotation());
	}
	else
	{
		SpringArmCom->TargetArmLength = FMath::Lerp(SpringArmCom->TargetArmLength ,360.f, 30.f * DeltaTime);
		SpringArmCom->SocketOffset = FMath::Lerp(SpringArmCom->SocketOffset, FVector(0, 0, 0), 30.f * DeltaTime);

		FRotator Rot = GetActorRotation();
		if (Rot.Pitch != 0)
		{
			Rot.Pitch = 0;
			SetActorRotation(Rot);
		}
		bUseControllerRotationRoll = false;
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
	}

	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::JUMP:		
		if (JumpCurrentCount == 0)
		{
			if (vInputDir.IsZero())
			{
				SetAniState(PLAYER_ANISTATE::LAND);
			}
			else
			{
				SetAniState(PLAYER_ANISTATE::RUN);
			}
		}
		break;
	case PLAYER_ANISTATE::ATTACK1:
	case PLAYER_ANISTATE::ATTACK2:
	case PLAYER_ANISTATE::ATTACK3:
	case PLAYER_ANISTATE::ATTACK4:
	case PLAYER_ANISTATE::ATTACK_DASH:
		fComboTime += DeltaTime;
		break;

	case PLAYER_ANISTATE::BOW_CHARGE:
		fBowChargeTime += DeltaTime;
		break;
	default:
		GetCharacterMovement()->GroundFriction = 8.f;
		fComboTime = 0.f;
		fBowChargeTime = 0.f;
		break;
	}

	//UE_LOG(LogTemp, Log, TEXT("%d"), (int)aniState);
	PlayMode->SetWidgetText(GetActorLocation().ToString());

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveForward", EKeys::S, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveRight", EKeys::D, 1.f));


		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_Dash", EKeys::LeftShift, 1.f));


		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_Turn", EKeys::MouseX, 0.5f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_LookUp", EKeys::MouseY, -0.5f));


		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Player_Jump", EKeys::SpaceBar));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Player_LClick", EKeys::LeftMouseButton));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Player_RClick", EKeys::RightMouseButton));


	}

	PlayerInputComponent->BindAxis("Player_MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Player_MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Player_Turn", this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Player_LookUp", this, &AMainCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Player_Dash", this, &AMainCharacter::Dash);

	PlayerInputComponent->BindAction("Player_Jump", EInputEvent::IE_Pressed, this, &AMainCharacter::PlayerJump);
	PlayerInputComponent->BindAction("Player_LClick", EInputEvent::IE_Pressed, this, &AMainCharacter::AttackAction);
	PlayerInputComponent->BindAction("Player_RClick", EInputEvent::IE_Repeat, this, &AMainCharacter::BowAttackStart);
	PlayerInputComponent->BindAction("Player_RClick", EInputEvent::IE_Released, this, &AMainCharacter::BowAttackEnd);

}


void AMainCharacter::MoveRight(float Val)
{
	vInputDir.Y = Val;
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
		break;
	default:
		return;
	}


	if (Val != 0.f)
	{
		if (bIsDash == true)
		{
			SetAniState(PLAYER_ANISTATE::DASH);
		}
		else
		{
			SetAniState(PLAYER_ANISTATE::RUN);
		}
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(RightDirection, Val);
	}
	else
	{
		if (vInputDir.IsZero())
		{
			SetAniState(PLAYER_ANISTATE::IDLE);
		}
	}
}

void AMainCharacter::MoveForward(float Val)
{
	vInputDir.X = Val;
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
		break;
	default:
		return;
	}

	if (Val != 0.f)
	{

		if (bIsDash == true)
		{
			SetAniState(PLAYER_ANISTATE::DASH);
		}
		else
		{
			SetAniState(PLAYER_ANISTATE::RUN);
		}
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// add movement 
		AddMovementInput(ForwardDirection, Val);
	}
	else
	{
		if (vInputDir.IsZero())
		{
			SetAniState(PLAYER_ANISTATE::IDLE);
		}
	}
}

void AMainCharacter::Dash(float Val)
{
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
		break;
	default:
		return;
	}
	if (Val == 1.f)
	{
		if (bIsDash == false)
		{
			bIsDash = true;
			GetCharacterMovement()->MaxWalkSpeed = 450.f;
		}
	}
	else
	{
		if (bIsDash == true)
		{
			bIsDash = false;
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
		}
	}
}


void AMainCharacter::PlayerJump()
{
	if (GetAniState() == (int)PLAYER_ANISTATE::JUMP || GetAniState() == (int)PLAYER_ANISTATE::LAND)
	{
		return;
	}

	SetAniState(PLAYER_ANISTATE::JUMP);
	Jump();
}

void AMainCharacter::AttackAction()
{
	if (bEquipBow)
	{
		SetAniState(PLAYER_ANISTATE::BOW_OFF);
		ChangeWeaponSocket(BowComponent, "Bow_Wait");
		return;
	}

	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::DASH:
		GetCharacterMovement()->GroundFriction = 0;
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 500, true);
		SetAniState(PLAYER_ANISTATE::ATTACK_DASH);
		ChangeWeaponSocket(WeaponComponent, "Weapon_R");
		break;
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::RUN:
	{
		if (bEquipSword == false)
		{
			SetAniState(PLAYER_ANISTATE::SWORD_ON);
			ChangeWeaponSocket(WeaponComponent, "Weapon_R");
			break;
		}
		else
		{
			SetAniState(PLAYER_ANISTATE::ATTACK1);
			break;
		}
	}
	case PLAYER_ANISTATE::ATTACK1:
		if (fComboTime > 0.3f)
		{
			fComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK2);
			break;
		}
	case PLAYER_ANISTATE::ATTACK2:
		if (fComboTime > 0.3f)
		{
			fComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK3);
			break;
		}
	case PLAYER_ANISTATE::ATTACK3:
		if (fComboTime > 0.3f)
		{
			fComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK4);
			break;
		}
	case PLAYER_ANISTATE::ATTACK4:
		if (fComboTime > 0.9f)
		{
			fComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK1);
			break;
		}
	case PLAYER_ANISTATE::ATTACK_DASH:
		if (fComboTime > 0.6f)
		{
			fComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK1);
			break;
		}
	default:
		return;
	}


}

void AMainCharacter::BowAttackStart()
{
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
		break;
	default:
		return;
	}
	if (bEquipSword)
	{
		SetAniState(PLAYER_ANISTATE::SWORD_ON);
		ChangeWeaponSocket(WeaponComponent, "Sword_Wait");
	}
	else
	{
		SetAniState(PLAYER_ANISTATE::BOW_CHARGE);
		ChangeWeaponSocket(BowComponent, "Weapon_L");
	}
	//ChangeWeaponSocket(BowPtr, "Weapon_L");
}

void AMainCharacter::BowAttackEnd()
{
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::BOW_CHARGE:
		break;
	default:
		return;
	}
	if (bEquipBow)
	{
		SetAniState(PLAYER_ANISTATE::BOW_SHOOT);
		//ChangeWeaponSocket(BowComponent, "Bow_Wait");
	}

}

float AMainCharacter::GetRightHandBlending()
{

	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
	case PLAYER_ANISTATE::LAND:
	case PLAYER_ANISTATE::JUMP:
	if (bEquipSword)
	{
		return 1.0f;
	}
	return 0.0f;
	default:
		return 0.0f;
	}
}

float AMainCharacter::GetBowHandBlending()
{
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
	case PLAYER_ANISTATE::LAND:
	case PLAYER_ANISTATE::JUMP:
		if (bEquipBow)
		{
			return 1.0f;
		}
		return 0.0f;
	default:
		return 0.0f;
	}
}

void AMainCharacter::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	if (_Inter == false)
	{
		if (GetAnimMontage(PLAYER_ANISTATE::LAND) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::ATTACK1) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::ATTACK2) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::ATTACK3) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::ATTACK4) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::ATTACK_DASH) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::SWORD_ON) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::SWORD_OFF) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::HIT_S) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::HIT_M) == Anim || 
			GetAnimMontage(PLAYER_ANISTATE::BOW_ON) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::BOW_OFF) == Anim||
			GetAnimMontage(PLAYER_ANISTATE::BOW_SHOOT) == Anim)
		{
			SetAniState(PLAYER_ANISTATE::IDLE);

			GetMesh()->GetAnimInstance()->Montage_Play(GetAnimMontage(PLAYER_ANISTATE::IDLE), 1.0f);
		}
	}
}

void AMainCharacter::ChangeWeaponSocket(UMeshComponent* _WeaponMesh, FName _SocketName)
{
	FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
	Rule.ScaleRule = EAttachmentRule::KeepWorld;
	_WeaponMesh->AttachToComponent(_WeaponMesh->GetAttachParent(), Rule, _SocketName);
	if (_SocketName == FName("Sword_Wait"))
	{
		bEquipSword = false;
	}
	else if (_SocketName == FName("Weapon_R"))
	{
		bEquipSword = true;
	}
	else if (_SocketName == FName("Bow_Wait"))
	{
		bEquipBow = false;
	}
	else if (_SocketName == FName("Weapon_L"))
	{
		bEquipBow = true;
	}
}

void AMainCharacter::Damaged(float _Damage, AGlobalCharacter* _AttackCharacter)
{
	Super::Damaged(_Damage, _AttackCharacter);

	UE_LOG(LogTemp, Log, TEXT("Player Damaged"));

	SetAniState(PLAYER_ANISTATE::HIT_M);
}

void AMainCharacter::BeginWeaponOverLap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Monster")))
	{
		PLAYER_ANISTATE Ani = static_cast<PLAYER_ANISTATE>(GetAniState());
		if (PLAYER_ANISTATE::ATTACK1 == Ani ||
			PLAYER_ANISTATE::ATTACK2 == Ani ||
			PLAYER_ANISTATE::ATTACK3 == Ani ||
			PLAYER_ANISTATE::ATTACK4 == Ani ||
			PLAYER_ANISTATE::ATTACK_DASH == Ani)
		{
			AGlobalCharacter* Mon = Cast<AGlobalCharacter>(OtherActor);
			Attacked(1.0f, Mon);
		}
	}
}
