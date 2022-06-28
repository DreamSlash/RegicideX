#include "RGX_MageAngelAttackAbility.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_PlayMontageAndWaitForEvent.h"

void URGX_MageAngelAttackAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	// Should use constexpr
	const FName StartChannelLoop = FName("Enemy.MageAngel.AttackAnimation.StartChannelLoop");
	const FName EndChannelLoop = FName("Enemy.MageAngel.AttackAnimation.EndChannelLoop");
	const FName StartAttackLoop = FName("Enemy.MageAngel.AttackAnimation.StartAttackLoop");
	const FName EndAttackLoop = FName("Enemy.MageAngel.AttackAnimation.EndAttackLoop");
	const FName AttackWindow = FName("Enemy.MageAngel.AttackAnimation.AttackWindow");

	const FName tagName = EventTag.GetTagName();
	if (tagName == StartChannelLoop)
	{
		OnStartChannelLoop();
	}
	else if (tagName == EndChannelLoop)
	{
		OnEndChannelLoop();
	}
	else if (tagName == StartAttackLoop)
	{
		OnStartAttackLoop();
	}
	else if (tagName == EndAttackLoop)
	{
		OnEndAttackLoop();
	}
	else if (tagName == AttackWindow)
	{
		OnAttackWindow();
	}
}

void URGX_MageAngelAttackAbility::OnStartChannelLoop()
{
	if (bEndChannelingLoop) return; // Gets called after invalidating the timer handle... check this

	if (ChannelingTime > 0.0f)
	{
		if (TimerHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bEndChannelingLoop = true; }, ChannelingTime, false);
		}
	}
	else
	{
		bEndChannelingLoop = true;
	}
}

void URGX_MageAngelAttackAbility::OnEndChannelLoop()
{
	if (bEndChannelingLoop)
	{
		TimerHandle.Invalidate();
		PlayMontageBySectionName(FName("attack_start"));
	}
}

void URGX_MageAngelAttackAbility::OnStartAttackLoop()
{
	if (bEndAttackingLoop) return;

	if (AttackTime > 0.0f)
	{
		if (TimerHandle.IsValid() == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bEndAttackingLoop = true; }, AttackTime, false);
		}
	}
	else
	{
		bEndAttackingLoop = true;
	}
}

void URGX_MageAngelAttackAbility::OnEndAttackLoop()
{
	if (bEndAttackingLoop)
	{
		TimerHandle.Invalidate();
		PlayMontageBySectionName(FName("attack_end"));
	}
}
