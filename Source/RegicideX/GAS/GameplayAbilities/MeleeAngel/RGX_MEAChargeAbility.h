// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_MEAChargeAbility.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_MEAChargeAbility : public URGX_GameplayAbility
{
	GENERATED_BODY()
	
protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DamageCurveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttributeScalingCurveName;

	UPROPERTY(EditAnywhere)
	UCurveTable* DamageLevelCurve = nullptr;

	UPROPERTY(EditAnywhere)
	FGameplayTag HitboxTag;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToOwner;

	UPROPERTY(EditDefaultsOnly)
	TArray<URGX_RGXEventDataAsset*> EventsToApplyToTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<URGX_RGXEventDataAsset*> EventsToApplyToOwner;
};
