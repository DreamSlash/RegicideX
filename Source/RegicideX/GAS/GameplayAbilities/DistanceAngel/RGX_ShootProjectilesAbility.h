// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_ShootProjectilesAbility.generated.h"

class ARGX_Bullet;
class ARGX_DistanceAngel;

UCLASS()
class REGICIDEX_API URGX_ShootProjectilesAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	URGX_ShootProjectilesAbility();

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ARGX_Bullet> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName DamageCurveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName AttributeScalingCurveName;

	UPROPERTY(EditAnywhere)
		UCurveTable* DamageLevelCurve = nullptr;

	UFUNCTION()
		void Shoot(ARGX_DistanceAngel* DistAngel);
};
