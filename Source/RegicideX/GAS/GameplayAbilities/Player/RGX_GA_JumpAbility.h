#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RegicideX/Character/RGX_PlayerAnimInstance.h"
#include "RGX_GA_JumpAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_JumpAbility : public URGX_GameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditAnywhere)
	class UParticleSystem* JumpFeedbackVFX;
};