#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "GA_LightAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_LightAttackAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

		URGX_LightAttackAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* MontageToPlay;
};