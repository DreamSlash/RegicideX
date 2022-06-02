#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "../BaseAbilities/RGX_GA_CastHitboxAttackAbility.h"
#include "../../RGX_PayloadObjects.h"
#include "RGX_PlayerFallAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_PlayerFallAttackAbility : public URGX_CastHitboxAttackAbility
{
	GENERATED_BODY()

public:

	URGX_PlayerFallAttackAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void OnFinalMontageFinished() override;
};
