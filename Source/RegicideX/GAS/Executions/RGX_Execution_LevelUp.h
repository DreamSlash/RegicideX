// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RGX_Execution_LevelUp.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_Execution_LevelUp : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	URGX_Execution_LevelUp();
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
