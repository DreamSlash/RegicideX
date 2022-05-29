// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GA_PlayMontageAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URGX_PlayMontageAbility::URGX_PlayMontageAbility()
{

}

void URGX_PlayMontageAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, MontageToPlay, PlayRate, StartSection, bStopWhenAbilityEnds);
	PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &URGX_PlayMontageAbility::OnMontageBlendOut);
	PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &URGX_PlayMontageAbility::OnMontageInterrupted);
	PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &URGX_PlayMontageAbility::OnMontageCancelled);
	PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &URGX_PlayMontageAbility::OnMontageCompleted);
	PlayMontageAndWaitTask->ReadyForActivation();
}

void URGX_PlayMontageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_PlayMontageAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_PlayMontageAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_PlayMontageAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_PlayMontageAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
