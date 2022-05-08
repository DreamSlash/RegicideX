#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpec.h"
#include "RGX_RingWavesAbility.generated.h"

class ARGX_RingWave;

struct FGameplayAbilityActorInfo;

UCLASS()
class REGICIDEX_API URGX_RingWavesAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayBetweenWaves = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", ClampMax = "5", UIMin = "1", UIMax = "5"))
	int NumWaves = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_RingWave> RingActorClass;

private:
	UFUNCTION()
	void OnSpawnRingWave();

	void StartDelay();

private:
	FGameplayAbilitySpecHandle EndHandle;
	const FGameplayAbilityActorInfo* EndActorInfo = nullptr;
	FGameplayAbilityActivationInfo EndActivationInfo;

	int PendingWaves = 0;

};