// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "RGX_TargetQueryContext.generated.h"

/**
 * 
 */

UCLASS()
class REGICIDEX_API URGX_EnvQueryItemType_Actor : public UEnvQueryItemType_Actor
{
	GENERATED_BODY()

public:
	static void SetContextHelper(FEnvQueryContextData& ContextData, const AActor* SingleActor);

	FVector GetItemLocation(const uint8* RawData) const override;
};

UCLASS()
class REGICIDEX_API URGX_TargetQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
