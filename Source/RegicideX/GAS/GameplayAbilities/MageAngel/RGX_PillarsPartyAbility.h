#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "RegicideX/GAS/GameplayAbilities/MageAngel/RGX_MageAngelExplosionAbility.h"
#include "RGX_PillarsPartyAbility.generated.h"

class ARGX_ExplosivePillar;

UCLASS()
class REGICIDEX_API URGX_PillarsPartyAbility : public URGX_MageAngelExplosionAbility
{
	GENERATED_BODY()

protected:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnExplode() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NumPillars = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEnvQuery* PositionsQuery = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GridSize = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinSpacingBetweenPillars = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinDistanceFromPlayer = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDistanceFromPlayer = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DelayBetweenPillars = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_ExplosivePillar> PillarActorClass;

private:
	void PositionsQueryFinished(TSharedPtr<FEnvQueryResult> Result);
	void SpawnPillarAtLocation(const FVector& Location) const;

	//UFUNCTION()
	//void OnSpawnPillar();

	//void StartDelay();

private:
	FEnvQueryRequest PositionsQueryRequest;

	TSharedPtr<FEnvQueryResult> PositionsQueryResult = nullptr;

	FTimerHandle DelayTimerHandle;

	int PendingPillars = 0;

};