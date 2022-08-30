#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/MageAngel/RGX_MageAngelAttackAbility.h"
#include "RGX_GroundExplosionAbility.generated.h"

class ARGX_GroundExplosion;

UCLASS()
class REGICIDEX_API URGX_GroundExplosionAbility : public URGX_MageAngelAttackAbility
{
	GENERATED_BODY()

protected:
	void OnAttackWindow() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_GroundExplosion> ExplosionActorClass;

};