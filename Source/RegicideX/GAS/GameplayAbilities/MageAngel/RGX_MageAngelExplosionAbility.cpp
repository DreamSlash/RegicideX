#include "RGX_MageAngelExplosionAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_PlayMontageAndWaitForEvent.h"

void URGX_MageAngelExplosionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bEndLoop = false;
}

void URGX_MageAngelExplosionAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Should use constexpr
	const FName StartLoop = FName("Enemy.MageAngel.ExplosionAnimation.StartLoop");
	const FName EndLoop = FName("Enemy.MageAngel.ExplosionAnimation.EndLoop");

	const FName tagName = EventTag.GetTagName();
	if (tagName == StartLoop)
	{
		OnStartLoop();
	}
	else if (tagName == EndLoop)
	{
		OnEndLoop();
	}
}

void URGX_MageAngelExplosionAbility::OnStartLoop()
{
	if (bEndLoop) return; // Gets called after invalidating the timer handle... check this

	if (LoopTime > 0.0f)
	{
		if (TimerHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bEndLoop = true; }, LoopTime, false);
		}
	}
	else
	{
		bEndLoop = true;
	}
}

void URGX_MageAngelExplosionAbility::OnEndLoop()
{
	if (bEndLoop)
	{
		TimerHandle.Invalidate();
		PlayMontageBySectionName(FName("attack_start"));
	}
}
