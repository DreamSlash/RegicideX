// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_Peasant.h"
#include "RGX_DistancePeasant.generated.h"

class UBehaviorTree;

UCLASS()
class REGICIDEX_API ARGX_DistancePeasant : public ARGX_Peasant
{
	GENERATED_BODY()

public:
	ARGX_DistancePeasant();

	//UPROPERTY(EditDefaultsOnly)
	//	int NumberProjectilesToFire = 10;

	//UPROPERTY(EditDefaultsOnly)
	//	TArray<FVector> PointsToSpawn;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* ClusterSpawnPoint;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

};
