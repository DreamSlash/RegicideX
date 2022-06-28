#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_GroundExplosionAbility.generated.h"

class ARGX_GroundExplosion;

UCLASS()
class REGICIDEX_API URGX_GroundExplosionAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARGX_GroundExplosion> ExplosionActorClass;

};