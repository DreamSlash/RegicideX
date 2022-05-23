// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_MeleeAngel.generated.h"

/**
 * 
 */


UCLASS()
class REGICIDEX_API ARGX_MeleeAngel : public ARGX_EnemyBase
{
	GENERATED_BODY()
	
public:
	ARGX_MeleeAngel();

	void Tick(float DeltaTime) override;
};
