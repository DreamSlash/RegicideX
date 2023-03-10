// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "RegicideX/Actors/RGX_EffectApplierActor.h"
#include "RGX_ExplosivePillar.generated.h"

class UBoxComponent;
class UCapsuleComponent;
class UCurveFloat;
class UGameplayEffect;
class UNiagaraSystem;

UCLASS()
class REGICIDEX_API ARGX_ExplosivePillar : public ARGX_EffectApplierActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_ExplosivePillar();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* PillarCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* ActivationCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ExplosionSource = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeDelayTell = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToActivate = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToExplode = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
	float ExplosionRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* RiseCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* TellVFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ExplosionVFX = nullptr;

public:
	UFUNCTION(BlueprintCallable)
		void Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void Detonate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	void Rise();
	void Activate();
	void Explode();

private:
	FTimerHandle DelayTellTimerHandle;
	FTimerHandle ActivationTimerHandle;
	FTimerHandle ExplosionTimerHandle;
	FTimeline RiseTimeLine;

	bool bIsRising = false;
	float InitialZ = 0.0f;

};
