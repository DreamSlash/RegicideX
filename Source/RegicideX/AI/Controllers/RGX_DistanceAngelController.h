// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RGX_DistanceAngelController.generated.h"

/**
 * 
 */


UENUM()
enum class ERGX_DistAngelAttackMode 
{
	PREPAREFORATTACK, BULLETHELL, LASERBEAM, GUIDEDBOMBS
};

UCLASS()
class REGICIDEX_API ARGX_DistanceAngelController : public AAIController
{
	GENERATED_BODY()
public:
	ARGX_DistanceAngelController();

	/*UFUNCTION()
	void SetRandomLocationAroundTarget();*/
};
