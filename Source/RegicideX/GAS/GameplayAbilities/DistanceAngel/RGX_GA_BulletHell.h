// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RGX_GA_BulletHell.generated.h"

class ARGX_Projectile;

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_GA_BulletHell : public URGX_GameplayAbility
{
	GENERATED_BODY()
	
protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARGX_Projectile> BulletBP;
	
	UPROPERTY(EditDefaultsOnly)
	float FireRate = 0.2f;

	UPROPERTY(EditDefaultsOnly)
	float FireTime = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float LowerBoundPitchOffset = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float UpperBoundPitchOffset = 20.0f;

	AActor* TargetActor = nullptr;
	TArray<UActorComponent*> BulletOrigins;

	UFUNCTION()
	void OnSpawnBullet();

	UFUNCTION()
	void OnFinished();
};
