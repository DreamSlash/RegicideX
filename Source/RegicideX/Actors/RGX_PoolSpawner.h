// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegicideX/Components/RGX_PoolComponent.h"
#include "RegicideX/Data/RGX_EnemyDataAsset.h"
#include "RGX_PoolSpawner.generated.h"

struct FBox;

UCLASS()
class REGICIDEX_API ARGX_PoolSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARGX_PoolSpawner();

	UPROPERTY(EditAnywhere)
	FBox SpawnBox;

	ARGX_EnemyBase* Spawn(ERGX_EnemyTypes Type);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	class UBoxComponent* SpawnVolume = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pool|MeleePeasantPool")
		URGX_PoolComponent* MeleePeasantPool = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pool|DistancePeasantPool")
		URGX_PoolComponent* DistancePeasantPool = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pool|MolePeasantPool")
		URGX_PoolComponent* MolePeasantPool = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pool|SuicidePeasantPool")
		URGX_PoolComponent* SuicidePeasantPool = nullptr;

	UPROPERTY(EditAnywhere, Category = "Trigger")
		bool Trigger;

	UPROPERTY(EditAnywhere, Category = "LifeSpan")
		float LifeSpan = 0.0f;

	UPROPERTY(EditAnywhere, Category = "SpawnCooldown")
		float SpawnCooldown = 0.0f;

	FTimerHandle SpawnCooldownTimerHandle;
};
