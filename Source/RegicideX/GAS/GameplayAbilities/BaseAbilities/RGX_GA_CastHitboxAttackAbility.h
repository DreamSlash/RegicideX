
#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_GA_CastSkillAbility.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RGX_GA_CastHitboxAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_CastHitboxAttackAbility : public URGX_CastSkillAbility
{
	GENERATED_BODY()

public:
	URGX_CastHitboxAttackAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UFUNCTION()
	virtual void OnFinalMontageFinished();

	UFUNCTION()
	virtual void PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DamageCurveName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttributeScalingCurveName;

	UPROPERTY(EditAnywhere)
	UCurveTable* DamageLevelCurve = nullptr;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* FinalMontageToPlay = nullptr;

	UPROPERTY(EditAnywhere)
	FName FinalMontageStartSectionName;

	UPROPERTY(EditAnywhere)
	float FinalMontagePlayRate = 1.0f;

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
