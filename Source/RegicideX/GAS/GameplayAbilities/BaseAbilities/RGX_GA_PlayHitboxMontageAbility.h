// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_PlayMontageAndWaitForEvent.h"
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

	UPROPERTY()
	class URGX_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEventTask = nullptr;

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	/* If it is overriden, Base version sould be called at the end of the overrided version */
	UFUNCTION()
	virtual void OnMontageFinished(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext);

protected:

	// TODO Check how damage should be handled.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DamageCurveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttributeScalingCurveName;

	UPROPERTY(EditAnywhere)
	UCurveTable* DamageLevelCurve = nullptr;

	// Montage ability will play.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* MontageToPlay;

	// Section of the montage in case we do not want to play from the beginning.
	UPROPERTY(EditAnywhere)
	FName StartSectionName;

	UPROPERTY(EditAnywhere)
	float PlayRatio = 1.0f;

	// The tag pointing to the hitbox component.
	UPROPERTY(EditAnywhere)
	FGameplayTag HitboxTag;

	// The Tag for the montage to wait for.
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer EventTagContainer;

	// A map of a tag that should trigger a gameplay effect.
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> EffectToApplyToTarget;

	// A map of a tag that should trigger a gameplay effect to ability owner.
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> EffectToApplyToOwner;
};
