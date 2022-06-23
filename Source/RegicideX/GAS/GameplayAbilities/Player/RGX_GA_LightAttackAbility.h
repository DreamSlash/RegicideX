#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/RGX_GameplayAbility.h"
#include "RGX_GA_LightAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_LightAttackAbility : public URGX_GameplayAbility
{
	GENERATED_BODY()

	URGX_LightAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere)
	FName StartSectionName;
};