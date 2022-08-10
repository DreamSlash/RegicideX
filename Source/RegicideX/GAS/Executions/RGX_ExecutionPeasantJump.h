// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RGX_ExecutionPeasantJump.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_ExecutionPeasantJump : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	URGX_ExecutionPeasantJump();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
