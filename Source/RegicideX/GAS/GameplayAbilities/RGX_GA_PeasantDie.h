// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_GA_PeasantDie.generated.h"

UCLASS()
class REGICIDEX_API URGX_GA_PeasantDie : public UMCV_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* MontageToPlay = nullptr;

	FGameplayAbilitySpecHandle MyHandle;
	const FGameplayAbilityActorInfo* MyAinfo;
	FGameplayAbilityActivationInfo MyActivationInfo;
	
private:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData);

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled);

	UFUNCTION()
	void OnEndMontage();
};
