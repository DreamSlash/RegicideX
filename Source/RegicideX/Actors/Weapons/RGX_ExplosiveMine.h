// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "RegicideX/Actors/RGX_EffectApplierActor.h"
#include "RGX_ExplosiveMine.generated.h"

UCLASS()
class REGICIDEX_API ARGX_ExplosiveMine : public ARGX_EffectApplierActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_ExplosiveMine();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Sphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ActivationCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeDelayTell = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeAutoactivation = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToExplode = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActivationScaleSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
	float ExplosionRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCurveFloat* RiseCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* TellVFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* ActivationTellVFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* ExplosionVFX = nullptr;

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

	void BeginRise();
	void BeginActivation();

	void TickRise(float DeltaTime);
	void TickActivation(float DeltaTime);

	void Activate();
	void Explode();

private:
	FTimerHandle DelayTellTimerHandle;
	FTimerHandle AutoActivationTimerHandle;
	FTimerHandle ExplosionTimerHandle;
	FTimeline RiseTimeLine;

	bool bIsRising = false;
	bool bIsActivated = false;
	float InitialZ = 0.0f;

};
