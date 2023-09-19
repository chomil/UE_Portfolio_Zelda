// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/MovableActor.h"
#include <Global/GlobalCharacter.h>


// Sets default values
AMovableActor::AMovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAsyncPhysicsTickEnabled = true;
	MaxRecord = 600;

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

}

// Called when the game starts or when spawned
void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TEXT("MovableActor"));

	Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	Mesh->OnComponentHit.AddDynamic(this,&AMovableActor::HitActor);
	Mesh->SetNotifyRigidBodyCollision(true);
}

// Called every frame
void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Mesh == nullptr)
	{
		return;
	}

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
			if (Transform.Equals(TransformRecord.Last()) == false)
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

void AMovableActor::SetOverlay(bool _IsRewind, bool _IsSelect)
{
	if (_IsSelect == true)
	{
		Mesh->SetRenderCustomDepth(true);
		Mesh->SetCustomDepthStencilValue(1);

		Mesh->SetOverlayMaterial(OverlayMaterial_Rewind);
	}
	else if (_IsRewind == true && _IsSelect == false)
	{
		Mesh->SetRenderCustomDepth(true);
		Mesh->SetCustomDepthStencilValue(1);

		Mesh->SetOverlayMaterial(OverlayMaterial_RewindWait);
	}
	else
	{
		Mesh->SetRenderCustomDepth(false);
		Mesh->SetCustomDepthStencilValue(0);

		Mesh->SetOverlayMaterial(nullptr);
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
