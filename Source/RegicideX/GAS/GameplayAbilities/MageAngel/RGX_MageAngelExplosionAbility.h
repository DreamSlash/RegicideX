#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "RGX_MageAngelExplosionAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_MageAngelExplosionAbility : public URGX_CastSkillAbility
{
	GENERATED_BODY()

public:
	URGX_MageAngelExplosionAbility() = default;

protected:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void OnStartLoop();
	virtual void OnEndLoop();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LoopTime = 0.0f;

protected:
	FTimerHandle TimerHandle;
	
	bool bEndLoop = false;

};