// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_RingWave.generated.h"

class UStaticMeshComponent;

UCLASS()
class REGICIDEX_API ARGX_RingWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_RingWave();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RingTimeLife = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleSpeed = 10.0f;

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void DamageActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
