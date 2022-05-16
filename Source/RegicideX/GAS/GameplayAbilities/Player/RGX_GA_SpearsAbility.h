#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "RGX_GA_SpearsAbility.generated.h"

class ARGX_SpearProjectile;

UCLASS()
class REGICIDEX_API URGX_SpearsAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	float SpearsSpawnAngle = 180.0f;

	UPROPERTY(EditDefaultsOnly)
	int MaxNumSpears = 12;

	UPROPERTY(EditDefaultsOnly)
	float DistanceFromCaster = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHoldSpearsTime = 3.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARGX_SpearProjectile> SpearProjectileClass;

	UPROPERTY()
	TArray<ARGX_SpearProjectile*> SpearsArray;

private:

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = nullptr;

protected:

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UFUNCTION(BlueprintCallable)
	void CastSpearsAttack(AActor* CasterActor);

	UFUNCTION(BlueprintCallable)
	void LaunchSpearsAttack();

	UFUNCTION()
	void OnHoldSpearsTimeOut();
};