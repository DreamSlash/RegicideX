// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_Bullet.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;
class URGX_HitboxComponent;

UCLASS()
class REGICIDEX_API ARGX_Bullet : public AActor
{
	GENERATED_BODY()

	float MyTimer = 0.0f;

public:	
	// Sets default values for this actor's properties
	ARGX_Bullet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BulletMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		URGX_HitboxComponent* BulletCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* BulletParticleSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* MyOwner = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Move(float DeltaTime);

	UFUNCTION()
	virtual void Hit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
