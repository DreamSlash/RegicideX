#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_GA_BarrierAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_BarrierAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BarrierClass;

protected:

	UFUNCTION(BlueprintCallable)
	void CastBarrierSkill(AActor* CasterActor);
};