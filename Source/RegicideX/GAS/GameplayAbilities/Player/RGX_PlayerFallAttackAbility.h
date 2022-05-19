#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "../../RGX_PayloadObjects.h"
#include "RGX_PlayerFallAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_PlayerFallAttackAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

public:

	URGX_PlayerFallAttackAbility();

protected:

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UFUNCTION()
	void OnFinishStartMontage();

	UFUNCTION()
	void OnFinishEndMontage();

	UFUNCTION()
	void OnInterruptMontage();

protected:

	UPROPERTY(EditAnywhere)
	class UAnimMontage* StartMontageToPlay;

	UPROPERTY(EditAnywhere)
	FName StartMontageStartSectionName;

	UPROPERTY(EditAnywhere)
	float StartMontagePlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EndMontageToPlay;

	UPROPERTY(EditAnywhere)
	FName EndMontageStartSectionName;

	UPROPERTY(EditAnywhere)
	float EndMontagePlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
	FGameplayTag HitboxTag;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApplyToOwner;

	UPROPERTY(EditDefaultsOnly)
	TArray<URGX_RGXEventDataAsset*> EventsToApplyToTarget;

	UPROPERTY(EditDefaultsOnly)
	TArray<URGX_RGXEventDataAsset*> EventsToApplyToOwner;

	UPROPERTY(EditAnywhere)
	float MoveVectorLength = 10.0f;
};
