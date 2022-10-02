#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/MageAngel/RGX_MageAngelExplosionAbility.h"
#include "RGX_PillarsFieldAbility.generated.h"

class ARGX_ExplosivePillar;

struct FEnvQueryResult;

UCLASS()
class REGICIDEX_API URGX_PillarsFieldAbility : public URGX_MageAngelExplosionAbility
{
	GENERATED_BODY()

public:
	URGX_PillarsFieldAbility() = default;

protected:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnStartLoop() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistanceToTarget = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEnvQuery* PillarsPositionsQuery;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_ExplosivePillar> PillarActorClass;

private:
	void PillarsPositionsQueryFinished(TSharedPtr<FEnvQueryResult> Result);

private:
	TSharedPtr<FEnvQueryResult> PillarsPositionsResult = nullptr;

};