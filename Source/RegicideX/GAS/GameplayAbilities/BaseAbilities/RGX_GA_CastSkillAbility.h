#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RegicideX/GAS/RGX_GameplayEffectContext.h"
#include "RegicideX/GAS/RGX_PayloadObjects.h"
#include "RGX_GA_CastSkillAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_CastSkillAbility : public URGX_GameplayAbility
{
	GENERATED_BODY()

public:
	URGX_CastSkillAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	virtual void OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	virtual void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData); // Must be overriden by child classes

	void PlayMontageBySectionName(const FName& SectionName);

protected:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* MontageToPlay = nullptr;

	UPROPERTY(EditAnywhere)
	FName MontageStartSectionName;

	UPROPERTY(EditAnywhere)
	float MontagePlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer WaitForEventTags;

	// A map of a tag that should trigger a gameplay effect with assigned payload to ability owner.
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FRGX_EffectContextContainer> EffectToApplyToOwnerWithPayload;

	UPROPERTY()
	class URGX_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEventTask = nullptr;
};
