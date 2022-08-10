// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_ExplosivePillar.generated.h"

class UBoxComponent;
class UCurveFloat;
class UGameplayEffect;
class UNiagaraSystem;

UCLASS()
class REGICIDEX_API ARGX_ExplosivePillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_ExplosivePillar();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* PillarCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ExplosionVFX = nullptr;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	UFUNCTION()
	void Explode(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
