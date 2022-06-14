#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_GA_DodgeAbility.generated.h"

class UGameplayEffect;
class ACharacter;

UCLASS()
class REGICIDEX_API URGX_DodgeAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* MontageToPlay = nullptr;

	UPROPERTY(EditAnywhere)
	FName BackDodgeSection;

	UPROPERTY(EditAnywhere)
	FName SideDodgeSection;

	UPROPERTY(EditDefaultsOnly)
	float DodgeSpeed = 900.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> InvulnerabilityEffect;

protected:

	UFUNCTION()
	void FinishDodge();
};