#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_PlayHitboxMontageAbility.h"
#include "RGX_PlayerFallAttackAbility.generated.h"

/*
* We make a different class from MeleeAttackAbility because we do not want any Combat Assist for this specific attack.
*/
UCLASS()
class REGICIDEX_API URGX_PlayerFallAttackAbility : public URGX_PlayHitboxMontageAbility
{
	GENERATED_BODY()

public:

	URGX_PlayerFallAttackAbility();

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
