// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RGX_SlowEffect.generated.h"

/**
 * Marks a character as dead
 */
UCLASS()
class REGICIDEX_API URGX_SlowEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URGX_SlowEffect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowCoefficient = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlowDuration = 3.0f;

};
