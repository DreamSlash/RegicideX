#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/MageAngel/RGX_MageAngelAttackAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpec.h"
#include "RGX_RingWavesAbility.generated.h"

class ARGX_RingWave;

struct FGameplayAbilityActorInfo;

UCLASS()
class REGICIDEX_API URGX_RingWavesAbility : public URGX_MageAngelAttackAbility
{
	GENERATED_BODY()

protected:
	void OnStartAttackLoop() override;
	void OnAttackWindow() override;

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
	FTimerHandle DelayTimerHandle;

	int PendingWaves = 0;

};