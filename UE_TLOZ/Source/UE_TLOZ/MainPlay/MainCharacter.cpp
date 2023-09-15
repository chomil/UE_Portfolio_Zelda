// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "MainHUD.h"
#include "GamePlayWidget.h"
#include "MovableActor.h"




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
	
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	Tags.Add(TEXT("Player"));



}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerCon = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCon->GetLocalPlayer());
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
	Input->BindAction(InputActionMove, ETriggerEvent::Completed, this, &AMainCharacter::Move);
	Input->BindAction(InputActionBow, ETriggerEvent::Triggered, this, &AMainCharacter::BowAttack);
	Input->BindAction(InputActionBow, ETriggerEvent::Completed, this, &AMainCharacter::BowAttack);
	Input->BindAction(InputActionSword, ETriggerEvent::Triggered, this, &AMainCharacter::SwordAttack);
	Input->BindAction(InputActionJump, ETriggerEvent::Triggered, this, &AMainCharacter::PlayerJump);
	Input->BindAction(InputActionDash, ETriggerEvent::Triggered, this, &AMainCharacter::Dash);
	Input->BindAction(InputActionDash, ETriggerEvent::Completed, this, &AMainCharacter::Dash);
	Input->BindAction(InputActionMouseMove, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
	Input->BindAction(InputActionAbility, ETriggerEvent::Canceled, this, &AMainCharacter::Ability);
	Input->BindAction(InputActionAbility, ETriggerEvent::Triggered, this, &AMainCharacter::Ability);

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


	if (CursorAim == nullptr)
	{
		CursorAim = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), CursorAimReference);
	}

	if (CursorDefault == nullptr)
	{
		CursorDefault = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), CursorDefaultReference);
	}
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
		GetController()->ClientSetRotation(Rot);

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
		//대시 스태미나 5초에 걸쳐 소모
		SP -= DeltaTime * 0.2f;
		if (SP <= 0)
		{
			SP = 0;
			bTired = true;
			SetAniState(PLAYER_ANISTATE::TIRED);
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
		GetCharacterMovement()->GroundFriction = 8.f;
		ComboTime = 0.f;
		BowChargeTime = 0.f;
		break;
	default:
		GetCharacterMovement()->GroundFriction = 8.f;
		ComboTime = 0.f;
		BowChargeTime = 0.f;
		break;
	}

	//UE_LOG(LogTemp, Log, TEXT("%d"), (int)aniState);

	
	PlayMode->SetWidgetText(GetActorLocation().ToString());

}



void AMainCharacter::Dash(const FInputActionInstance& Instance)
{
	bIsDash = Instance.GetValue().Get<bool>();
	if (bTired)
	{
		bIsDash = false;
	}
	if (bIsDash == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
}


void AMainCharacter::PlayerJump(const FInputActionInstance& Instance)
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

	SetAniState(PLAYER_ANISTATE::JUMP);
	Jump();
}


void AMainCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D MoveVec = Instance.GetValue().Get<FVector2D>();


	InputDir.X = MoveVec.X;
	InputDir.Y = MoveVec.Y;
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
		AttackMove = true;
		break;
	default:
		return;
	}

	if (MoveVec != FVector2D::ZeroVector)
	{
		if (AttackMove == false)
		{
			if (bIsDash == true)
			{
				if (bEquipBow)
				{
					SetAniState(PLAYER_ANISTATE::BOW_OFF);
					if (ArrowActor != nullptr)
					{
						ArrowActor->Destroy();
						ArrowActor = nullptr;
					}
				}
				else if (bEquipSword)
				{
					SetAniState(PLAYER_ANISTATE::SWORD_OFF);
				}
				else
				{
					SetAniState(PLAYER_ANISTATE::DASH);
				}
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
		ForwardDirection *= MoveVec.X;

		// get right vector 
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		RightDirection.Normalize();
		RightDirection *= MoveVec.Y;

		// add movement 
		if (AttackMove == false)
		{
			AddMovementInput(ForwardDirection, 1);
			AddMovementInput(RightDirection, 1);
		}
		else
		{
			AddMovementInput(ForwardDirection * 0.1f, 1);
			AddMovementInput(RightDirection * 0.1f, 1);
		}
	}
	else
	{
		if (AttackMove == false)
		{
			SetAniState(PLAYER_ANISTATE::IDLE);
		}
	}
}

void AMainCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D DeltaPos = Instance.GetValue().Get<FVector2D>();
	DeltaPos *= 0.5;
	//UE_LOG(LogTemp, Log, TEXT("%s"), *DeltaPos.ToString())

	AddControllerYawInput(DeltaPos.X);
	AddControllerPitchInput(-DeltaPos.Y);
}

void AMainCharacter::SwordAttack(const FInputActionInstance& Instance)
{
	if (bEquipBow && IsAniState(PLAYER_ANISTATE::JUMP)==false)
	{
		SetAniState(PLAYER_ANISTATE::BOW_OFF);
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
		if (bEquipSword == true)
		{
			SetAniState(PLAYER_ANISTATE::ATTACK1);
		}
		break;
	}
	case PLAYER_ANISTATE::ATTACK1:
	{
		if (ComboTime > 0.3f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK2);
		}
		break;
	}
	case PLAYER_ANISTATE::ATTACK2:
	{
		if (ComboTime > 0.3f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK3);
		}
		break;
	}
	case PLAYER_ANISTATE::ATTACK3:
	{
		if (ComboTime > 0.3f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK4);
		}
		break;
	}
	case PLAYER_ANISTATE::ATTACK4:
	{
		if (ComboTime > 0.9f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK1);
		}
		break;
	}
	case PLAYER_ANISTATE::ATTACK_DASH:
	{
		if (ComboTime > 0.6f)
		{
			ComboTime = 0.f;
			SetAniState(PLAYER_ANISTATE::ATTACK1);
		}
		break;
	}
	default:
		break;
	}

}

void AMainCharacter::BowAttack(const FInputActionInstance& Instance)
{
	if (Instance.GetValue().Get<bool>() == true) //오른쪽 마우스 버튼 꾹 눌렀을때
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
	}
	else //오른쪽 마우스 버튼 뗏을 때
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

					float Velocity = BowChargeTime > 0.7f ? 5000.f * 0.7f : 5000.f * BowChargeTime;
					ArrowMove->Velocity = (Forward * Velocity);

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
}

void AMainCharacter::Ability(const FInputActionInstance& Instance)
{
	bool Triggered = Instance.GetValue().Get<bool>();
	const float Time = Instance.GetElapsedTime();
	APlayerController* HUDController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	AMainHUD* HUD = HUDController->GetHUD<AMainHUD>();
	if (HUD == nullptr || HUD->IsValidLowLevel() == false)
	{
		return;
	}
	if (Triggered == true) //능력 변경 UI
	{
		UE_LOG(LogTemp, Log, TEXT("능력변경 시작"));
		HUD->GetMainWidget()->SetAbilityVisible(true);
	}
	else 
	{
		if (Time < 0.5f) //능력 시전,취소
		{
			UE_LOG(LogTemp, Log, TEXT("능력시전"));
			Revereco(!bIsAbility);
		}
		else //능력 변경 캔슬
		{
			UE_LOG(LogTemp, Log, TEXT("능력변경 완료"));
			HUD->GetMainWidget()->SetAbilityVisible(false);
		}
	}
}

void AMainCharacter::Revereco(bool bStart)
{
	bIsAbility = bStart;

	TArray<AActor*> MovableActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("MovableActor"), MovableActors);

	if (bStart == true)
	{
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		
		GetWorld()->GetFirstPlayerController()->SetMouseCursorWidget(EMouseCursor::Default, CursorAim);
		int32 ScreenX, ScreenY;
		GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);
		GetWorld()->GetFirstPlayerController()->SetMouseLocation(ScreenX / 2, ScreenY/2);

		SetAniState(PLAYER_ANISTATE::ABILITY_START);

		for (AActor* Actor : MovableActors)
		{
			AMovableActor* MoveActor = Cast<AMovableActor>(Actor);
			MoveActor->SetTimeRewind(false);
			FVector TargetPos = Actor->GetActorLocation();
			FVector MyPos = GetActorLocation();
			if ((TargetPos - MyPos).Size() < 1500.f)
			{
				MoveActor->SetOverlay(true, false);
			}
		}
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());

		GetWorld()->GetFirstPlayerController()->SetMouseCursorWidget(EMouseCursor::Default, CursorDefault);


		SetAniState(PLAYER_ANISTATE::ABILITY_END);

		for (AActor* Actor : MovableActors)
		{
			AMovableActor* MoveActor = Cast<AMovableActor>(Actor);
			MoveActor->SetOverlay(false, false);
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

bool AMainCharacter::IsAttacking()
{
	return false;
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
			GetAnimMontage(PLAYER_ANISTATE::BOW_SHOOT) == Anim||
			GetAnimMontage(PLAYER_ANISTATE::ABILITY_END) == Anim ||
			GetAnimMontage(PLAYER_ANISTATE::TIRED) == Anim)
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
