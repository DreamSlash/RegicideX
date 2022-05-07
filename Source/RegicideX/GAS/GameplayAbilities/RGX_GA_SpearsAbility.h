#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_GA_SpearsAbility.generated.h"

class ARGX_SpearProjectile;

UCLASS()
class REGICIDEX_API URGX_SpearsAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	float SpearsSpawnAngle = 180.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxNumSpears = 12.0f; // wtf

	UPROPERTY(EditDefaultsOnly)
	float DistanceFromCaster = 100.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARGX_SpearProjectile> SpearProjectileClass;

	UPROPERTY()
	TArray<ARGX_SpearProjectile*> SpearsArray;

protected:

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UFUNCTION(BlueprintCallable)
	void CastSpearsAttack(AActor* CasterActor);

	UFUNCTION(BlueprintCallable)
	void LaunchSpearsAttack();		 
};