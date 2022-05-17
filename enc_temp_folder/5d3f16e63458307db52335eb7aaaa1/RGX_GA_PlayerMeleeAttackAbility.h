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
	TArray<FGameplayTag> EventsToApplyToTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToOwner;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EventsToApplyToOwner;

	UPROPERTY(EditAnywhere)
	float MoveVectorLength = 10.0f;

	UPROPERTY(EditAnywhere)
	float ZAirForce = 800.0f;

	// TODO [REFACTOR]: Interface for all possible ability events?
	UPROPERTY(EditAnywhere)
	float LaunchHorizontalForce = 0.0f; //TODO: Do not hardcode this

	UPROPERTY(EditAnywhere)
	float LaunchVerticalForce = 0.0f;

	UPROPERTY(EditAnywhere)
	bool bOverrideHorizontal = true;

	UPROPERTY(EditAnywhere)
	bool bOverrideVertical = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataAsset* EventsTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataAsset* EventsOwner;

protected:

	UFUNCTION()
	virtual FRGX_AbilityEffectsInfo GetAbilityEffectsInfo();

	UFUNCTION()
	void FinishAttack();
};