// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RGX_ShootSingleProjectileAbility.generated.h"

/**
 * 
 */
class ARGX_Projectile;

UCLASS()
class REGICIDEX_API URGX_ShootSingleProjectileAbility : public URGX_GameplayAbility
{
	GENERATED_BODY()

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:

	URGX_ShootSingleProjectileAbility();

	UPROPERTY(EditAnywhere)
		TSubclassOf<ARGX_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
		FVector Offset;

	UFUNCTION()
		void Shoot(APawn* Actor);
};
