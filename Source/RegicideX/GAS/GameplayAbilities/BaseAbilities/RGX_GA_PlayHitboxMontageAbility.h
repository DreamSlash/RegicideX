// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RGX_GA_PlayHitboxMontageAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_PlayHitboxMontageAbility : public URGX_GameplayAbility
{
	GENERATED_BODY()

public:

	URGX_PlayHitboxMontageAbility();
	virtual ~URGX_PlayHitboxMontageAbility() {}


protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	/* If it is overriden, Base version sould be called at the end of the overrided version */
	UFUNCTION()
	virtual void OnMontageFinished();

	UFUNCTION()
	virtual void PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DamageCurveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttributeScalingCurveName;

	UPROPERTY(EditAnywhere)
	UCurveTable* DamageLevelCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere)
	FName StartSectionName;

	UPROPERTY(EditAnywhere)
	float PlayRatio = 1.0f;

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
