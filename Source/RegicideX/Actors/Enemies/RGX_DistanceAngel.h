// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_DistanceAngel.generated.h"

/**
 * 
 */


class UStaticMeshComponent;
class USphereComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class REGICIDEX_API ARGX_DistanceAngel : public ARGX_EnemyBase
{
	GENERATED_BODY()

	bool TickMe = true;

public:

	ARGX_DistanceAngel();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Ring_1_Mesh = nullptr;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Ring_2_Mesh = nullptr;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Ring_3_Mesh = nullptr;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BulletHellSphere = nullptr;


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LaserEndPointMesh = nullptr;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollider = nullptr;

	UPROPERTY(EditAnywhere)
		USceneComponent* FloorReturnPlace = nullptr;

	UPROPERTY(EditAnywhere)
		TArray<FVector> BombingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RingOriginalRotatingSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RingRotatingSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FullBodyRotatingSpeed = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeightPos = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SimpleBombsCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SimpleBombsOffset = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ActorMidHeight = 50.0f;

	UPROPERTY(BlueprintReadWrite)
		bool Invincible = false;

	AActor* LaserBeamRef = nullptr;

	UMaterialInterface* MaterialInterface = nullptr;
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;

	void BeginPlay() override;

	// Called every frame
	void Tick(float DeltaTime) override;


	/** Movement methods */
	void MoveToTarget(float DeltaTime, FVector TargetPos) override;

	void RotateToTarget(float DeltaTime) override;

	void RotateRings(float DeltaTime);

	void RotateMe(float DeltaTime, float Speed);

	UFUNCTION(BlueprintCallable)
	void TPToFloor();

	UFUNCTION(BlueprintCallable)
	void TPToOriginalHeight();

	/** Attack methods */

	UFUNCTION(BlueprintCallable)
		float GetDistanceToTarget() const;

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnSimpleBullet(FTransform Transform, AActor* MyOwner);

	UFUNCTION()
		void SetLocationHeight(float Height);

	UFUNCTION()
		void DestroyMyself(float Time);

	UFUNCTION(BlueprintCallable)
		void ChangeEyeColor(FLinearColor Color);

	UFUNCTION(BlueprintCallable)
		FVector GetEyeWorldLocation();

};
