// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/MovableActor.h"
#include <Global/GlobalCharacter.h>
#include "MainCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"




// Sets default values
AMovableActor::AMovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAsyncPhysicsTickEnabled = true;
	MaxRecord = 600;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->bRenderCustomDepth = true;

	RootComponent = Mesh;
	
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	Spline->SetupAttachment(Mesh);


	{
		FString Path = TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Resources/OverlayMaterial/MI_Overlay_Rewind.MI_Overlay_Rewind'");
		ConstructorHelpers::FObjectFinder<UMaterialInstance> Asset(*Path);

		if (Asset.Succeeded())
		{
			OverlayMaterial_Rewind = Asset.Object;
		}
	}
	{
		FString Path = TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Resources/OverlayMaterial/MI_Overlay_RewindWait.MI_Overlay_RewindWait'");
		ConstructorHelpers::FObjectFinder<UMaterialInstance> Asset(*Path);

		if (Asset.Succeeded())
		{
			OverlayMaterial_RewindWait = Asset.Object;
		}
		
	}
	{
		FString Path = TEXT("/Script/Engine.Material'/Game/Resources/OverlayMaterial/M_RewindArrow.M_RewindArrow'");
		ConstructorHelpers::FObjectFinder<UMaterial> Asset(*Path);

		if (Asset.Succeeded())
		{
			Material_RewindArrow = Asset.Object;
		}

	}
	{
		FString Path = TEXT("/Script/Engine.StaticMesh'/Game/Resources/OverlayMaterial/SM_RewindArrow.SM_RewindArrow'");
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(*Path);

		if (Asset.Succeeded())
		{
			Mesh_RewindArrow = Asset.Object;
		}

	}
}

// Called when the game starts or when spawned
void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TEXT("MovableActor"));

	Mesh->OnComponentHit.AddDynamic(this,&AMovableActor::HitActor);

	//커서 상호작용 이벤트 바인딩
	Mesh->OnBeginCursorOver.AddDynamic(this, &AMovableActor::OnCursorBegin);
	Mesh->OnEndCursorOver.AddDynamic(this, &AMovableActor::OnCursorEnd);
	Mesh->OnClicked.AddDynamic(this, &AMovableActor::Clicked);

	Spline->SetAbsolute(true, true, true);
}

// Called every frame
void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FTransform Transform = Mesh->GetRelativeTransform();

	if (TransformRecord.IsEmpty() == false)
	{
		DeltaPos = (Transform.GetLocation() - TransformRecord.Last().GetLocation());
		VelPerSec = DeltaPos.Size()/DeltaTime;
	}

	if (bTimeRewind == false) //Record Transform
	{
		if (TransformRecord.IsEmpty()==false)
		{
			if (Transform.Equals(TransformRecord.Last(),1.0) == false) //마지막 기록 위치와 같지 않을때
			{
				TransformRecord.EmplaceLast(Transform);
				if (TransformRecord.Num() > 600)
				{
					TransformRecord.PopFirst();
				}
			}
		}
		else
		{
			TransformRecord.EmplaceLast(Transform);
		}


		MaxRecord = TransformRecord.Num();
	}
	else //Rewind
	{
		if (TransformRecord.IsEmpty()==true)
		{
			SetTimeRewind(false);
		}
		else
		{
			FTransform NewTransform = TransformRecord.Last();
			Mesh->SetRelativeTransform(NewTransform);
			TransformRecord.PopLast();
		}
	}

	CurRecord = TransformRecord.Num();

	if (bIsSelect == true)
	{
		UpdateRewindArrow();
	}
}

void AMovableActor::SetTimeRewind(bool _bTimeRewind)
{
	bTimeRewind = _bTimeRewind;
	if (Mesh != nullptr)
	{
		Mesh->ComponentVelocity = FVector::Zero();
		if (_bTimeRewind == true)
		{
			Mesh->SetEnableGravity(false);
			Mesh->SetSimulatePhysics(false);
			Mesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector, false);
			Mesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);
			
			SetOverlay(true, true);
		}
		else
		{
			Mesh->SetSimulatePhysics(true);
			Mesh->SetEnableGravity(true);
			Mesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector, false);
			Mesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);

			SetOverlay(false, false);
		}
	}
}

void AMovableActor::SetOverlay(bool _bIsAbility, bool _bIsSelect)
{
	bIsSelect = _bIsSelect;
	if (_bIsSelect == true) //마우스 선택 시, 클릭됐을 시
	{
		Mesh->SetRenderCustomDepth(true);
		Mesh->SetCustomDepthStencilValue(1);

		Mesh->SetOverlayMaterial(OverlayMaterial_Rewind);

		UpdateRewindArrow();
	}
	else if (_bIsAbility == true && _bIsSelect == false) //능력 대기 시
	{
		Mesh->SetRenderCustomDepth(true);
		Mesh->SetCustomDepthStencilValue(1);

		Mesh->SetOverlayMaterial(OverlayMaterial_RewindWait);


		//스플라인 메시 삭제
		for (USplineMeshComponent* SplineMeshCom : SplineMeshs)
		{
			SplineMeshCom->DestroyComponent();
		}
		SplineMeshs.Empty();

		//잔상 삭제
		for (UStaticMeshComponent* RewindMesh : RewindStaticMeshs)
		{
			RewindMesh->DestroyComponent();
		}
		RewindStaticMeshs.Empty();
	}
	else
	{
		Mesh->SetRenderCustomDepth(false);
		Mesh->SetCustomDepthStencilValue(0);

		Mesh->SetOverlayMaterial(nullptr);


		//스플라인 메시 삭제
		for (USplineMeshComponent* SplineMeshCom : SplineMeshs)
		{
			SplineMeshCom->DestroyComponent();
		}
		SplineMeshs.Empty();

		//잔상 삭제
		for (UStaticMeshComponent* RewindMesh : RewindStaticMeshs)
		{
			RewindMesh->DestroyComponent();
		}
		RewindStaticMeshs.Empty();
	}
}

void AMovableActor::UpdateRewindArrow()
{
	//스플라인 업데이트
	Spline->ClearSplinePoints(true);

	const FTransform CenterTrans = Mesh->GetSocketTransform(TEXT("Center"), ERelativeTransformSpace::RTS_Component);
	for (int i = 0; i < CurRecord; i++)
	{
		if (i % 10 == 0 || i == CurRecord - 1)
		{
			Spline->AddSplineWorldPoint((CenterTrans * TransformRecord[i]).GetLocation());
		}
	}

	int PointNum = Spline->GetNumberOfSplinePoints();
	int SplineMeshNum = SplineMeshs.Num();

	if (PointNum > SplineMeshNum)
	{
		//빈 포인트 만큼 스플라인 메시 생성
		for (int i = SplineMeshNum; i < PointNum - 1; i++)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			if (SplineMesh)
			{
				SplineMesh->SetMobility(EComponentMobility::Movable);
				SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				SplineMesh->RegisterComponentWithWorld(GetWorld());

				SplineMesh->SetStaticMesh(Mesh_RewindArrow);
				SplineMesh->SetMaterial(0, Material_RewindArrow);
				SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				FVector StartLocation, StartTangent, EndLocation, EndTangent;
				Spline->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::Local);
				Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);
				SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);

				SplineMeshs.Add(SplineMesh);
			}
		}
	}
	else
	{
		if (SplineMeshs.IsValidIndex(SplineMeshNum - 1))
		{
			SplineMeshs[SplineMeshNum - 1]->DestroyComponent();
			SplineMeshs.RemoveAt(SplineMeshNum - 1);
		}
	}


	//잔상 업데이트
	for (UStaticMeshComponent* RewindMesh : RewindStaticMeshs)
	{
		RewindMesh->DestroyComponent();
	}
	RewindStaticMeshs.Empty();
	for (int i = 0; i < TransformRecord.Num(); i++)
	{
		if (i % 60 == 0)
		{
			UStaticMeshComponent* RewindMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
			if (RewindMesh)
			{
				RewindMesh->SetMobility(EComponentMobility::Movable);
				RewindMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
				RewindMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				RewindMesh->RegisterComponentWithWorld(GetWorld());

				RewindMesh->SetStaticMesh(Mesh->GetStaticMesh());
				int MatNum = RewindMesh->GetNumMaterials();
				for (int j = 0; j < MatNum; j++)
				{
					RewindMesh->SetMaterial(j, Material_RewindArrow);
				}
				RewindMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				RewindMesh->SetWorldTransform(TransformRecord[i]);
				RewindStaticMeshs.Add(RewindMesh);
			}
		}
	}

}

void AMovableActor::HitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag(TEXT("Monster")))
	{
		if (VelPerSec > 500.f)
		{
			Cast<AGlobalCharacter>(OtherActor)->Damaged((int)VelPerSec / 100, nullptr);
		}
	}
	else if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		if (VelPerSec > 1000.f)
		{
			Cast<AGlobalCharacter>(OtherActor)->Damaged((int)VelPerSec / 1000, nullptr);
		}
	}
}

void AMovableActor::OnCursorBegin(UPrimitiveComponent* TouchedComponent)
{
	if (bInPlayerRange == true)
	{
		SetOverlay(true, true);
	}
}

void AMovableActor::OnCursorEnd(UPrimitiveComponent* TouchedComponent)
{
	if (bInPlayerRange == true)
	{
		SetOverlay(true, false);
	}
}

void AMovableActor::Clicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (bInPlayerRange == true)
	{
		if (bTimeRewind == false)
		{
			AMainCharacter* Player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			Player->Revereco(false);


			SetTimeRewind(true);
		}
	}
}
