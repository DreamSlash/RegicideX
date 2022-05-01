// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_ShootSingleProjectileAbility.generated.h"

/**
 * 
 */
class ARGX_Bullet;

UCLASS()
class REGICIDEX_API URGX_ShootSingleProjectileAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<ARGX_Bullet> ProjectileClass;

	UFUNCTION()
		void Shoot(AActor* Actor);
};
