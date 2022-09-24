// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "RGX_TargetQueryContext.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_TargetQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
