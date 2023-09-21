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

	void SetOverlay(bool _bIsAbility, bool _bIsSelect);

	void SetInPlayerRange(bool _bInPlayerRange)
	{
		bInPlayerRange = _bInPlayerRange;
	}

	void UpdateRewindArrow();

private:
	UPROPERTY()
	UMaterialInstance* OverlayMaterial_Rewind = nullptr;

	UPROPERTY()
	UMaterialInstance* OverlayMaterial_RewindWait = nullptr;

	UPROPERTY()
	UMaterial* Material_RewindArrow = nullptr;


	UPROPERTY()
	UStaticMesh* Mesh_RewindArrow = nullptr;


	TDeque<FTransform> TransformRecord;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int CurRecord = 0;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int MaxRecord = 0;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bTimeRewind = false;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSelect = false;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY()
	class USplineComponent* Spline = nullptr;

	UPROPERTY()
	TArray<class USplineMeshComponent*> SplineMeshs;	

	UPROPERTY()
	TArray<class UStaticMeshComponent*> RewindStaticMeshs;

	FVector DeltaPos = FVector::ZeroVector;
	float VelPerSec = 0.f;

	float MovableDeltaVel = 0.f;

	bool bInPlayerRange = false;

	UFUNCTION()
	void HitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnCursorBegin(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnCursorEnd(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void Clicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);


};
