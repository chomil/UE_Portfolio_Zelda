// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/MovableActor.h"

// Sets default values
AMovableActor::AMovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAsyncPhysicsTickEnabled = true;
}

// Called when the game starts or when spawned
void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
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
}

void AMovableActor::SetTimeRewind(bool _bTimeRewind)
{
	bTimeRewind = _bTimeRewind;
	if (Mesh != nullptr)
	{
		if (_bTimeRewind == true)
		{
			Mesh->SetEnableGravity(false);
			Mesh->SetSimulatePhysics(false);

		}
		else
		{
			Mesh->SetSimulatePhysics(true);
			Mesh->SetEnableGravity(true);
			Mesh->ComponentVelocity = FVector::Zero();
		}
	}
}
