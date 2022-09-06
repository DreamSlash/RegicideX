// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RGX_AT_RotateAndUpdateForceField.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRotateAndUpdateForceFieldDelegate);
class ARGX_DistanceAngel;


UCLASS()
class REGICIDEX_API URGX_AT_RotateAndUpdateForceField : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FRotateAndUpdateForceFieldDelegate	OnFinish;

	URGX_AT_RotateAndUpdateForceField();


	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	void TickTask(float DeltaTime) override;
	void OnDestroy(bool AbilityIsEnding) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URGX_AT_RotateAndUpdateForceField* RotateAndUpdateForceField(UGameplayAbility* OwningAbility, float Time, float MaxSize);

private:
	/* Time delay variables */
	float MaxTime = 0.0f;
	float TaskTime = 0.0f;

	float MaxSize = 20.0;

	float OriginalBulletHellSphereScale = 10.0;

	float RotationSpeedMultiplier = 2.0;

	ARGX_DistanceAngel* Attacker = nullptr;
	
};
