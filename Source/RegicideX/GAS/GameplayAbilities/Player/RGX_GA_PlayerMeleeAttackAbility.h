#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "../../RGX_PayloadObjects.h"
#include "RGX_GA_PlayerMeleeAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_PlayerMeleeAttackAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	URGX_PlayerMeleeAttackAbility();

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere)
	FName StartSectionName;

	UPROPERTY(EditAnywhere)
	float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
	FGameplayTag HitboxTag;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToOwner;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EventToTargetTags;

	UPROPERTY(EditDefaultsOnly)
	TArray<UDataAsset*> EventsToApplyToTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EventToOwnerTags;

	UPROPERTY(EditDefaultsOnly)
	TArray<UDataAsset*> EventsToApplyToOwner;

	UPROPERTY(EditAnywhere)
	float MoveVectorLength = 10.0f;

	UPROPERTY(EditAnywhere)
	float ZAirForce = 800.0f;

protected:

	UFUNCTION()
	virtual FRGX_AbilityEffectsInfo GetAbilityEffectsInfo();

	UFUNCTION()
	void FinishAttack();
};