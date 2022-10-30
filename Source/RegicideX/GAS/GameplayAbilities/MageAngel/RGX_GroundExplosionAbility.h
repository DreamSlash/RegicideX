#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/MageAngel/RGX_MageAngelExplosionAbility.h"
#include "RGX_GroundExplosionAbility.generated.h"

class ARGX_GroundExplosion;

UCLASS()
class REGICIDEX_API URGX_GroundExplosionAbility : public URGX_MageAngelExplosionAbility
{
	GENERATED_BODY()

protected:
	void OnGround() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_GroundExplosion> ExplosionActorClass;

};