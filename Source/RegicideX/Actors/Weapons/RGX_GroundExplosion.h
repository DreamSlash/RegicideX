// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "RegicideX/Actors/RGX_EffectApplierActor.h"
#include "RGX_GroundExplosion.generated.h"

class USphereComponent;
class UDecalComponent;
class UCurveFloat;
class UGameplayEffect;
class UMaterialInstanceDynamic;
class UNiagaraSystem;

UCLASS()
class REGICIDEX_API ARGX_GroundExplosion : public ARGX_EffectApplierActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_GroundExplosion();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* ExplosionCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TellVFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ExplosionVFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DurationParameter;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

	UFUNCTION()
	void Explode();

private:
	FTimerHandle ExplosionTimerHandle;

};
