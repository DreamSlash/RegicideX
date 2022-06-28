// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RGX_AT_WaitDelayAndSpawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitDelayAndSpawnDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnDelegate);
/**
  Wait a delay time while calling spawn delegate at a specified rate
 */
UCLASS()
class REGICIDEX_API URGX_AT_WaitDelayAndSpawn : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FWaitDelayAndSpawnDelegate	OnFinish;

	UPROPERTY(BlueprintAssignable)
	FSpawnDelegate OnSpawn;

	virtual void Activate() override;

	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	/** Wait specified time. This is functionally the same as a standard Delay node. Spawn specified actors at a specified rate */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URGX_AT_WaitDelayAndSpawn* WaitDelayAndSpawn(UGameplayAbility* OwningAbility, float Time, float SpawnRate);

	void TickTask(float DeltaTime) override;
	void OnDestroy(bool AbilityIsEnding) override;

private:
	void OnTimeFinish();

	/* Time delay variables */
	float Time;
	float TimeStarted;

	/* Time spawn variables */
	float NextSpawnTime;
	float SpawnRate;
};
