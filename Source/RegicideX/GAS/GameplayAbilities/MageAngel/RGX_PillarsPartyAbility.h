#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/MageAngel/RGX_MageAngelAttackAbility.h"
#include "RGX_PillarsPartyAbility.generated.h"

class ARGX_ExplosivePillar;

UCLASS()
class REGICIDEX_API URGX_PillarsPartyAbility : public URGX_MageAngelAttackAbility
{
	GENERATED_BODY()

protected:
	void OnAttackWindow() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NumPillars = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PartyRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DelayBetweenPillars = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_ExplosivePillar> PillarActorClass;

private:
	UFUNCTION()
	void OnSpawnPillar();

	void StartDelay();

private:
	FTimerHandle DelayTimerHandle;

	int PendingPillars = 0;

};