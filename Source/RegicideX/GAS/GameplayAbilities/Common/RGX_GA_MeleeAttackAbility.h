#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_PlayHitboxMontageAbility.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RGX_GA_MeleeAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_MeleeAttackAbility : public URGX_PlayHitboxMontageAbility
{
	GENERATED_BODY()
	
public:
	URGX_MeleeAttackAbility();

protected:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void PopulateGameplayEffectContext(FRGX_GameplayEffectContext& GameplayEffectContext) override;

protected:
	UPROPERTY(EditAnywhere)
	float MoveVectorLength = 0.0f;
};