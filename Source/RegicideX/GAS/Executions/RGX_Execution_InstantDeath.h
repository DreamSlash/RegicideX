#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RGX_Execution_InstantDeath.generated.h"

UCLASS()
class REGICIDEX_API UExecution_InstantDeath : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UExecution_InstantDeath();
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};