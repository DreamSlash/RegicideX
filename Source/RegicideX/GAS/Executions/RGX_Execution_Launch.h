#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RGX_Execution_Launch.generated.h"

UCLASS()
class REGICIDEX_API UExecution_Launch : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UExecution_Launch();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};