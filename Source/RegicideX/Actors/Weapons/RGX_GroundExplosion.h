// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "RGX_GroundExplosion.generated.h"

class USphereComponent;
class UDecalComponent;
class UCurveFloat;
class UGameplayEffect;
class UMaterialInstanceDynamic;
class UParticleSystem;

UCLASS()
class REGICIDEX_API ARGX_GroundExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_GroundExplosion();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* ExplosionCollider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDecalComponent* CircumferenceDecal = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDecalComponent* AreaDecal = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* AreaCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleSystem = nullptr;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

	UFUNCTION()
	void Explode();

private:
	FTimerHandle ExplosionTimerHandle;
	FTimeline AreaTimeLine;

	UMaterialInstanceDynamic* CircumRef = nullptr;
	UMaterialInstanceDynamic* AreaRef = nullptr;

};
