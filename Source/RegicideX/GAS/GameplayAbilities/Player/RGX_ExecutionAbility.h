// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "RGX_ExecutionAbility.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_ExecutionAbility : public URGX_CastSkillAbility
{
	GENERATED_BODY()
	
public:
	URGX_ExecutionAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData) override;
	void OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData) override;
	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

protected:
	const AActor* TargetActor;
};
