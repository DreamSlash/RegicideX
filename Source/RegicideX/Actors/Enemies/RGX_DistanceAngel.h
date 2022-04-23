// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_DistanceAngel.generated.h"

/**
 * 
 */


class UStaticMeshComponent;
UCLASS()
class REGICIDEX_API ARGX_DistanceAngel : public ARGX_EnemyBase
{
	GENERATED_BODY()

public:

	ARGX_DistanceAngel();

		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* Ring_1_Mesh;

		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* Ring_2_Mesh;

		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* Ring_3_Mesh;

		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* LaserEndPointMesh;

		UPROPERTY(EditAnywhere)
			TArray<FVector> BombingPoints;



};
