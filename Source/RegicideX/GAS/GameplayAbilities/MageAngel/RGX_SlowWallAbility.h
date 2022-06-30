#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/MageAngel/RGX_MageAngelAttackAbility.h"
#include "RGX_SlowWallAbility.generated.h"

class ARGX_SlowWall;

UCLASS()
class REGICIDEX_API URGX_SlowWallAbility : public URGX_MageAngelAttackAbility
{
	GENERATED_BODY()

public:
	URGX_SlowWallAbility() = default;

protected:
	void OnAttackWindow() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistanceToTarget = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_SlowWall> WallActorClass;

};