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

public:
	/** Move to the specified location, using the vector curve (range 0 - 1) if specified, otherwise the float curve (range 0 - 1) or fallback to linear interpolation */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URGX_MoveToLocationCharge* MoveToLocationCharge(UGameplayAbility* OwningAbility, FName TaskInstanceName, FVector Location, float Duration, UCurveFloat* OptionalInterpolationCurve, UCurveVector* OptionalVectorInterpolationCurve);
	
	void Activate() override;
	void TickTask(float DeltaTime) override;
	void OnDestroy(bool AbilityIsEnding) override;
};
