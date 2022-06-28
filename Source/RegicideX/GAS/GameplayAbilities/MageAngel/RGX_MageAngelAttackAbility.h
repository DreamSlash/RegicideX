#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_CastSkillAbility.h"
#include "RGX_MageAngelAttackAbility.generated.h"

UCLASS()
class REGICIDEX_API URGX_MageAngelAttackAbility : public URGX_CastSkillAbility
{
	GENERATED_BODY()

public:
	URGX_MageAngelAttackAbility() = default;

protected:
	void OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void OnStartChannelLoop();
	virtual void OnEndChannelLoop();
	virtual void OnStartAttackLoop();
	virtual void OnEndAttackLoop();
	virtual void OnAttackWindow() {}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ChannelingTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackTime = 0.0f;

protected:
	FTimerHandle TimerHandle;
	
	bool bEndChannelingLoop = false;
	bool bEndAttackingLoop = false;

};