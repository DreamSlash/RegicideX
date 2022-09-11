// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RGX_LaserTellActorSequence.generated.h"


class UParticleSystemComponent;



UCLASS()
class REGICIDEX_API ARGX_LaserTellActorSequence : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_LaserTellActorSequence();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UParticleSystemComponent*> Particles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* Particle1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* Particle2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* Particle3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* Particle4 = nullptr;

	USceneComponent* CenterOfMass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMaxScale = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAtCenter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxScale = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitialCenterLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> InitialParticlesLocations;

	int Iterations = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ScaleParticles(float DeltaTime);

	void MoveToCenter(float DeltaTime);

	void InitParticlePositions();

};
