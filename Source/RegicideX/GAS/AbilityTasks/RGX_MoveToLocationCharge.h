// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "RGX_MoveToLocationCharge.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_MoveToLocationCharge : public UAbilityTask_MoveToLocation
{
	GENERATED_BODY()

private:
	void TickTask(float DeltaTime) override;
};
