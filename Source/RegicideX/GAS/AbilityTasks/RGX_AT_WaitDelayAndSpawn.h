// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RGX_AT_WaitDelayAndSpawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitDelayDelegate);
/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_AT_WaitDelayAndSpawn : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FWaitDelayDelegate	OnFinish;

	virtual void Activate() override;

	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	/** Wait specified time. This is functionally the same as a standard Delay node. Spawn specified actors at a specified rate */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URGX_AT_WaitDelayAndSpawn* WaitDelayAndSpawn(UGameplayAbility* OwningAbility, float Time);

	void TickTask(float DeltaTime) override;
	void OnDestroy(bool AbilityIsEnding) override;

private:
	void OnTimeFinish();

	float Time;
	float TimeStarted;
};
