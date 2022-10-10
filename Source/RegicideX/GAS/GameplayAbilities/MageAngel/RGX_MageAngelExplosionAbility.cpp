#include "RGX_MageAngelExplosionAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_PlayMontageAndWaitForEvent.h"

void URGX_MageAngelExplosionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bEndCastLoop = false;
	bEndGroundLoop = false;
}

void URGX_MageAngelExplosionAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Should use constexpr
	const FName StartCastLoop = FName("Enemy.MageAngel.ExplosionAnimation.StartLoop");
	const FName EndCastLoop = FName("Enemy.MageAngel.ExplosionAnimation.EndLoop");
	const FName StartGroundLoop = FName("Enemy.MageAngel.ExplosionAnimation.StartGroundLoop");
	const FName EndGroundLoop = FName("Enemy.MageAngel.ExplosionAnimation.EndGroundLoop");

	const FName TopCast = FName("Enemy.MageAngel.ExplosionAnimation.OnTopCast");
	const FName Ground = FName("Enemy.MageAngel.ExplosionAnimation.OnGround");

	const FName tagName = EventTag.GetTagName();
	if (tagName == StartCastLoop)
	{
		OnStartCastLoop();
	}
	else if (tagName == EndCastLoop)
	{
		OnEndCastLoop();
	}
	else if (tagName == StartGroundLoop)
	{
		OnStartGroundLoop();
	}
	else if (tagName == EndGroundLoop)
	{
		OnEndGroundLoop();
	}
	else if (tagName == TopCast)
	{
		OnTopCast();
	}
	else if (tagName == Ground)
	{
		OnGround();
	}
}

void URGX_MageAngelExplosionAbility::OnStartCastLoop()
{
	if (bEndCastLoop) return; // Gets called after invalidating the timer handle... check this

	if (CastTime > 0.0f)
	{
		if (TimerHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bEndCastLoop = true; }, CastTime, false);
		}
	}
	else
	{
		bEndCastLoop = true;
	}
}

void URGX_MageAngelExplosionAbility::OnEndCastLoop()
{
	if (bEndCastLoop)
	{
		TimerHandle.Invalidate();
		PlayMontageBySectionName(FName("Fall"));
	}
}

void URGX_MageAngelExplosionAbility::OnStartGroundLoop()
{
	if (bEndGroundLoop) return;

	if (GroundTime > 0.0f)
	{
		if (TimerHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bEndGroundLoop = true; }, GroundTime, false);
		}
	}
	else
	{
		bEndGroundLoop = true;
	}
}

void URGX_MageAngelExplosionAbility::OnEndGroundLoop()
{
	if (bEndGroundLoop)
	{
		TimerHandle.Invalidate();
		PlayMontageBySectionName(FName("End"));
	}
}