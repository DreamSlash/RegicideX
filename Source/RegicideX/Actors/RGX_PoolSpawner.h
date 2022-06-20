// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Components/RGX_PoolComponent.h"
#include "GameFramework/Actor.h"
#include "RGX_PoolSpawner.generated.h"

UCLASS()
class REGICIDEX_API ARGX_PoolSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_PoolSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	float GetLifeSpan();
	class UBoxComponent* SpawnVolume = nullptr;

	UPROPERTY(EditAnywhere, Category = "Spawner")
		URGX_PoolComponent* ActorPool = nullptr;

	UPROPERTY(EditAnywhere, Category = "Trigger")
		bool Trigger;

	UPROPERTY(EditAnywhere, Category = "LifeSpan")
		float LifeSpan = 0.0f;

	UPROPERTY(EditAnywhere, Category = "SpawnCooldown")
		float SpawnCooldown = 0.0f;

	FTimerHandle SpawnCooldownTimerHandle;

	void Spawn();

};
