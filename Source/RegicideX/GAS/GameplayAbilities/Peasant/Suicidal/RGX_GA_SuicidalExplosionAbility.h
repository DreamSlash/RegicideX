// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_GA_SuicidalExplosionAbility.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_GA_SuicidalExplosionAbility : public URGX_CastSkillAbility
{
	GENERATED_BODY()

public:
	URGX_GA_SuicidalExplosionAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData);

	void OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData);

	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ExplosionRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	URGX_LaunchEventDataAsset* LaunchEventData;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFX;
};
