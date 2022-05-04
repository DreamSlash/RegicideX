#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RGX_Execution_Damage.generated.h"

UCLASS()
class REGICIDEX_API UExecution_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UExecution_Damage();
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};