// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_PlayHitboxMontageAbility.h"
#include "RGX_MEASweepAbility.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_MEASweepAbility : public URGX_PlayHitboxMontageAbility
{
	GENERATED_BODY()
	
protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext) override;

	UFUNCTION()
	void OnEndSweepDuration();
};
