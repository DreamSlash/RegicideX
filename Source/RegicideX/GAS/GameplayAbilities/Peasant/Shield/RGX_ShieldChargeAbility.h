// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_PlayHitboxMontageAbility.h"
#include "RGX_ShieldChargeAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_ShieldChargeAbility : public URGX_PlayHitboxMontageAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartCharge();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void HandleReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName JumpToSectionIfOverlapped;

private:
	UFUNCTION()
	void OnChargeFinished();

	UFUNCTION()
	void OnOverlapped();

private:
	FTimerHandle ChargeTimerHandle;

	bool bCharge = false;

};
