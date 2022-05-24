// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/RGX_GameplayAbility.h"
#include "RGX_GA_CastSkillAbility.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_CastSkillAbility : public URGX_GameplayAbility
{
	GENERATED_BODY()

public:
	URGX_CastSkillAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	virtual void OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData); // Must be overriden by child classes

protected:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* MontageToPlay = nullptr;

	UPROPERTY(EditAnywhere)
	FName MontageStartSectionName;

	UPROPERTY(EditAnywhere)
	float MontagePlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer WaitForEventTags;
};
