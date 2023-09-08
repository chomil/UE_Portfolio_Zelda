// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "MainHUD.h"




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
	
	WeaponMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::BeginWeaponOverLap);

	SpringArmCom = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));

	


	HP = 10;
	BowChargeTime = 0.f;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayMode == nullptr)
	{
		return;
	}

	//무기 ON/OFF 애님 노티파이를 만들고 그 때 무기를 붙일 소켓 변경
	for (FAnimNotifyEventReference NotifyRef : GetMesh()->GetAnimInstance()->NotifyQueue.AnimNotifies)
	{
		if (NotifyRef.GetNotify()->NotifyName == TEXT("SwordOff"))
		{
			ChangeWeaponSocket(WeaponMeshComponent, "Sword_Wait");
		}
		else if (NotifyRef.GetNotify()->NotifyName == TEXT("SwordOn"))
		{
			ChangeWeaponSocket(WeaponMeshComponent, "Weapon_R");
		}
		else if (NotifyRef.GetNotify()->NotifyName == TEXT("BowOff"))
		{
			ChangeWeaponSocket(BowMeshComponent, "Bow_Wait");
		}
		else if (NotifyRef.GetNotify()->NotifyName == TEXT("BowOn"))
		{
			ChangeWeaponSocket(BowMeshComponent, "Weapon_L");
		}
	}

	//마우스 눌려있을시 연속해서 공격
	float LeftButtonTime = GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(EKeys::LeftMouseButton);
	if (LeftButtonTime > 0)
	{
		AttackAction();
	}

	float RightButtonTime = GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(EKeys::RightMouseButton);
	if (RightButtonTime > 0)
	{
		BowAttackStart();
	}
	
	if (bBowZoom)
	{ //활시위 당길때 등 카메라의 스프링 암 오프셋 이동
		float LerpFloat = BowChargeTime * 30.f * DeltaTime;
		LerpFloat = LerpFloat > 1.f ? 1.f : LerpFloat;
		SpringArmCom->TargetArmLength = FMath::Lerp(SpringArmCom->TargetArmLength, 200.f, LerpFloat);
		SpringArmCom->SocketOffset = FMath::Lerp(SpringArmCom->SocketOffset, FVector(0, 40, 50), LerpFloat);

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
		//UE_LOG(LogTemp, Log, TEXT("%f Pitch"), Rot.Pitch);
		GetController()->ClientSetRotation(Rot);
		//FVector NewVec = FVector(0, 0, 0);
		//GetController()->SetControlRotation(NewVec.Rotation());


		// 활 시위 본 당기기
		
	}
	else
	{
		SpringArmCom->TargetArmLength = FMath::Lerp(SpringArmCom->TargetArmLength ,360.f, 2 * DeltaTime);
		SpringArmCom->SocketOffset = FMath::Lerp(SpringArmCom->SocketOffset, FVector(0, 0, 0), 2 * DeltaTime);

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

	PLAYER_ANISTATE CurAniState = static_cast<PLAYER_ANISTATE>(GetAniState());


	//State-Stamina
	if (CurAniState == PLAYER_ANISTATE::DASH)
	{
		//대시 스태미나 10초에 걸쳐 소모
		SP -= DeltaTime * 0.1f;
		if (SP <= 0)
		{
			SP = 0;
			bTired = true;
		}
	}
	else
	{
		//스태미나 2초에 걸쳐 회복
		SP += DeltaTime * 0.5f;
		if (SP >= MaxSP)
		{
			SP = MaxSP;
			bTired = false;
		}
	}

	//State-Attack/Jump
	switch (CurAniState)
	{
	case PLAYER_ANISTATE::JUMP:
		bBowZoom = false;
		if (JumpCurrentCount == 0)
		{
			if (InputDir.IsZero())
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
		ComboTime += DeltaTime;
		break;

	case PLAYER_ANISTATE::BOW_CHARGE:
		BowChargeTime += DeltaTime;
		bBowZoom = true;
		break;

	case PLAYER_ANISTATE::DASH:
	case PLAYER_ANISTATE::RUN:
		bBowZoom = false;
	default:
		GetCharacterMovement()->GroundFriction = 8.f;
		ComboTime = 0.f;
		BowChargeTime = 0.f;
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
	InputDir.Y = Val;

	bool AttackMove = false;
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
		break;


	case PLAYER_ANISTATE::ATTACK1:
	case PLAYER_ANISTATE::ATTACK2:
	case PLAYER_ANISTATE::ATTACK3:
	case PLAYER_ANISTATE::BOW_CHARGE:
		AttackMove = true;
		break;
	default:
		return;
	}


	if (Val != 0.f)
	{
		if (AttackMove == false)
		{
			if (bIsDash == true)
			{
				SetAniState(PLAYER_ANISTATE::DASH);
			}
			else
			{
				SetAniState(PLAYER_ANISTATE::RUN);
			}
		}
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		RightDirection.Normalize();
		// add movement 
		if (AttackMove == false)
		{
			AddMovementInput(RightDirection, Val);
		}
		else
		{
			AddMovementInput(RightDirection*0.1f, Val);
		}
	}
	else
	{
		if (AttackMove == false)
		{
			if (InputDir.IsZero())
			{
				SetAniState(PLAYER_ANISTATE::IDLE);
			}
		}
	}
}

void AMainCharacter::MoveForward(float Val)
{
	InputDir.X = Val;
	bool AttackMove = false;
	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::WALK:
	case PLAYER_ANISTATE::RUN:
	case PLAYER_ANISTATE::DASH:
		break;

	case PLAYER_ANISTATE::ATTACK1:
	case PLAYER_ANISTATE::ATTACK2:
	case PLAYER_ANISTATE::ATTACK3:
	case PLAYER_ANISTATE::BOW_CHARGE:
		AttackMove = true;
		break;
	default:
		return;
	}

	if (Val != 0.f)
	{
		if (AttackMove == false)
		{
			if (bIsDash == true)
			{
				SetAniState(PLAYER_ANISTATE::DASH);
			}
			else
			{
				SetAniState(PLAYER_ANISTATE::RUN);
			}
		}
		
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		ForwardDirection.Normalize();
		// add movement 
		if (AttackMove == false)
		{
			AddMovementInput(ForwardDirection, Val);
		}
		else
		{
			AddMovementInput(ForwardDirection*0.1f, Val); 
		}
	}
	else
	{
		if (AttackMove == false)
		{
			if (InputDir.IsZero())
			{
				SetAniState(PLAYER_ANISTATE::IDLE);
			}
		}
	}
}

void AMainCharacter::Dash(float Val)
{
	if (bTired)
	{
		bIsDash = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		return;
	}
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
			GetCharacterMovement()->MaxWalkSpeed = 550.f;
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
	if (bEquipBow && GetAniState() != (int)PLAYER_ANISTATE::JUMP)
	{
		SetAniState(PLAYER_ANISTATE::BOW_OFF);
		ChangeWeaponSocket(BowMeshComponent, "Bow_Wait");
		if (ArrowActor != nullptr)
		{
			ArrowActor->Destroy();
			ArrowActor = nullptr;
		}
		return;
	}

	switch (static_cast<PLAYER_ANISTATE>(GetAniState()))
	{
	case PLAYER_ANISTATE::DASH:
		GetCharacterMovement()->GroundFriction = 0;
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 500, true);
		SetAniState(PLAYER_ANISTATE::ATTACK_DASH);
		ChangeWeaponSocket(WeaponMeshComponent, "Weapon_R");

		break;
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::RUN:
	{

		if (bEquipSword == false)
		{
			SetAniState(PLAYER_ANISTATE::SWORD_ON);
			break;
		}
		else
		{
			SetAniState(PLAYER_ANISTATE::ATTACK1);
			break;
		}
	}

	case PLAYER_ANISTATE::JUMP:
	{
		if(bEquipSword==true)
		{
			SetAniState(PLAYER_ANISTATE::ATTACK1);
			break;
		}
	}
	case PLAYER_ANISTATE::ATTACK1:
		if (ComboTime > 0.3f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK2);
			break;
		}
	case PLAYER_ANISTATE::ATTACK2:
		if (ComboTime > 0.3f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK3);
			break;
		}
	case PLAYER_ANISTATE::ATTACK3:
		if (ComboTime > 0.3f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK4);
			break;
		}
	case PLAYER_ANISTATE::ATTACK4:
		if (ComboTime > 0.9f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK1);
			break;
		}
	case PLAYER_ANISTATE::ATTACK_DASH:
		if (ComboTime > 0.6f)
		{
			ComboTime = 0.f;
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
		SetAniState(PLAYER_ANISTATE::SWORD_OFF);
	}
	else
	{
		if (bEquipBow)
		{
			SetAniState(PLAYER_ANISTATE::BOW_CHARGE);

			if (ArrowActor == nullptr)
			{
				FSoftObjectPath Path = TEXT("/Script/Engine.Blueprint'/Game/BluePrints/GamePlay/Player/BP_Arrow.BP_Arrow'");

				UBlueprint* BP = Cast<UBlueprint>(Path.TryLoad());

				ArrowActor = GetWorld()->SpawnActor(BP->GeneratedClass);

				FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
				Rule.ScaleRule = EAttachmentRule::KeepWorld;
				ArrowActor->AttachToComponent(GetMesh(), Rule, FName("Weapon_R"));
			}
		}
		else
		{
			SetAniState(PLAYER_ANISTATE::BOW_ON);
			if (ArrowActor == nullptr)
			{
				FSoftObjectPath Path = TEXT("/Script/Engine.Blueprint'/Game/BluePrints/GamePlay/Player/BP_Arrow.BP_Arrow'");

				UBlueprint* BP = Cast<UBlueprint>(Path.TryLoad());

				ArrowActor = GetWorld()->SpawnActor(BP->GeneratedClass);

				FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
				Rule.ScaleRule = EAttachmentRule::KeepWorld;
				ArrowActor->AttachToComponent(GetMesh(), Rule, FName("Weapon_R"));
			}
		}
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
		if (BowChargeTime > 0.4f)
		{
			SetAniState(PLAYER_ANISTATE::BOW_SHOOT);

			if (ArrowActor != nullptr)
			{
				ArrowActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				UStaticMeshComponent* ArrowMesh = Cast<UStaticMeshComponent>(ArrowActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				//ArrowMesh->SetSimulatePhysics(true);
				FVector Forward = ArrowMesh->GetForwardVector();

				UProjectileMovementComponent* ArrowMove = Cast<UProjectileMovementComponent>(ArrowActor->GetComponentByClass(UProjectileMovementComponent::StaticClass()));

				ArrowMove->bSimulationEnabled = true;
				ArrowMove->Velocity = (Forward * 3000.f);

				UNiagaraComponent* ArrowTrail = Cast<UNiagaraComponent>(ArrowActor->GetComponentByClass(UNiagaraComponent::StaticClass()));

				ArrowTrail->Activate();
				ArrowActor->Tags.Add(TEXT("PlayerAttack"));
				ArrowActor = nullptr;
			}

		}
		else
		{
			SetAniState(PLAYER_ANISTATE::IDLE);
		}
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
		if (GetAnimMontage(PLAYER_ANISTATE::BOW_OFF) == Anim)
		{
			bBowZoom = false;
		}

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
			UE_LOG(LogTemp, Log, TEXT("Player State : %s"), *UEnum::GetValueAsString(static_cast<PLAYER_ANISTATE>(GetAniState())));
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

	//UE_LOG(LogTemp, Log, TEXT("Player Damaged"));

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

	PLAYER_ANISTATE Ani = static_cast<PLAYER_ANISTATE>(GetAniState());
	if (PLAYER_ANISTATE::ATTACK1 == Ani ||
		PLAYER_ANISTATE::ATTACK2 == Ani ||
		PLAYER_ANISTATE::ATTACK3 == Ani ||
		PLAYER_ANISTATE::ATTACK4 == Ani ||
		PLAYER_ANISTATE::ATTACK_DASH == Ani)
	{
		if (OtherActor->ActorHasTag(TEXT("Monster")))
		{
			AGlobalCharacter* Mon = Cast<AGlobalCharacter>(OtherActor);
			if (Mon->GetHP() > 0)
			{
				Attacked(1.0f, Mon);

				FVector HitPoint = WeaponMeshComponent->GetSocketLocation(TEXT("HitSocket"));
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitDefaultParticle, HitPoint, FRotator::ZeroRotator);
			}
		}
	}
}
