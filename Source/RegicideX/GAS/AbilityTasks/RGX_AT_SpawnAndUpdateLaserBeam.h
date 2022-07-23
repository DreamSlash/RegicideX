// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RGX_AT_SpawnAndUpdateLaserBeam.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpawnAndUpdateLaserBeamDelegate);
class ARGX_LaserBeamWeapon;
class ARGX_DistanceAngel;


UCLASS()
class REGICIDEX_API URGX_AT_SpawnAndUpdateLaserBeam : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FSpawnAndUpdateLaserBeamDelegate	OnFinish;

	URGX_AT_SpawnAndUpdateLaserBeam();

	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	void TickTask(float DeltaTime) override;
	void OnDestroy(bool AbilityIsEnding) override;

	/** Spawn and move laser beam */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static URGX_AT_SpawnAndUpdateLaserBeam* SpawnAndUpdateLaserBeam(UGameplayAbility* OwningAbility, float Time, TSubclassOf<ARGX_LaserBeamWeapon> LaserBeamClass);

private:
	/* Time delay variables */
	float MaxTime = 0.0f;
	float TaskTime = 0.0f;

	TSubclassOf<ARGX_LaserBeamWeapon> LaserBeamWeaponSubclass;
	
	ARGX_LaserBeamWeapon* LaserBeamWeapon = nullptr;

	ARGX_DistanceAngel* Attacker = nullptr;

	void SpawnLaserBeamWeapon();

};

