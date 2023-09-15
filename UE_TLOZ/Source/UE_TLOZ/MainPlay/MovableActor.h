// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Deque.h"

#include "MovableActor.generated.h"


UCLASS()
class UE_TLOZ_API AMovableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTimeRewind(bool _bTimeRewind);

	void SetOverlay(bool _IsRewind, bool _IsSelect);

private:
	UPROPERTY()
	UMaterialInstance* OverlayMaterial_Rewind = nullptr;

	UPROPERTY()
	UMaterialInstance* OverlayMaterial_RewindWait = nullptr;


	TDeque<FTransform> TransformRecord;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int CurRecord = 0;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int MaxRecord = 0;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bTimeRewind = false;
	
	UPROPERTY()
	UStaticMeshComponent* Mesh = nullptr;

	FVector DeltaPos;
	float VelPerSec;

	float MovableDeltaVel = 0.f;

	UFUNCTION()
	void HitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
