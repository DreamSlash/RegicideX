// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_CastSkillAbility.h"
#include "GameFramework/Character.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_PlayMontageAndWaitForEvent.h"

URGX_CastSkillAbility::URGX_CastSkillAbility()
{

}

bool URGX_CastSkillAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	// Add custom logic

	return true;
}

void URGX_CastSkillAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PlayMontageBySectionName(MontageStartSectionName);
}

void URGX_CastSkillAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_CastSkillAbility::OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_CastSkillAbility::OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void URGX_CastSkillAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{

}

void URGX_CastSkillAbility::PlayMontageBySectionName(const FName& SectionName)
{
	URGX_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEventTask = URGX_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this, NAME_None, MontageToPlay, WaitForEventTags, MontagePlayRate, SectionName, true);
	PlayMontageAndWaitForEventTask->OnInterrupted.AddDynamic(this, &URGX_CastSkillAbility::OnFailedAbilityMontage);
	PlayMontageAndWaitForEventTask->OnBlendOut.AddDynamic(this, &URGX_CastSkillAbility::OnSuccessfulAbilityMontage);
	PlayMontageAndWaitForEventTask->OnCancelled.AddDynamic(this, &URGX_CastSkillAbility::OnFailedAbilityMontage);
	PlayMontageAndWaitForEventTask->OnCompleted.AddDynamic(this, &URGX_CastSkillAbility::OnSuccessfulAbilityMontage);
	PlayMontageAndWaitForEventTask->EventReceived.AddDynamic(this, &URGX_CastSkillAbility::OnReceivedEvent);
	PlayMontageAndWaitForEventTask->ReadyForActivation();
}
