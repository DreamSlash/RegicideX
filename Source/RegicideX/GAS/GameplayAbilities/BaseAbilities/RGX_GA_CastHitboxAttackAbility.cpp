// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_CastHitboxAttackAbility.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

URGX_CastHitboxAttackAbility::URGX_CastHitboxAttackAbility()
{

}

bool URGX_CastHitboxAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	const URGX_HitboxesManagerComponent* HitboxManagerComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();
	if (HitboxManagerComponent == nullptr)
		return false;

	return true;
}

void URGX_CastHitboxAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_CastHitboxAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_CastHitboxAttackAbility::OnSuccessfulAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_CastHitboxAttackAbility::OnFailedAbilityMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void URGX_CastHitboxAttackAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	URGX_HitboxesManagerComponent* HitboxManagerComponent = CurrentActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();

	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	if (Hitbox)
	{
		FRGX_AbilityEffectsInfo AbilityEffectsInfo;
		AbilityEffectsInfo.GameplayEffectsToTarget = EffectsToApplyToTarget;
		AbilityEffectsInfo.GameplayEffectsToOwner = EffectsToApplyToOwner;
		AbilityEffectsInfo.GameplayEventsToTarget = EventsToApplyToTarget;
		AbilityEffectsInfo.GameplayEventsToOwner = EventsToApplyToOwner;
		Hitbox->SetAbilityEffectsInfo(AbilityEffectsInfo);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Hitbox sent to ability does not exist"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, FinalMontageToPlay, FinalMontagePlayRate, FinalMontageStartSectionName, true);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_CastHitboxAttackAbility::OnFinalMontageFinished);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_CastHitboxAttackAbility::OnFinalMontageFinished);
	PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_CastHitboxAttackAbility::OnFinalMontageFinished);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_CastHitboxAttackAbility::OnFinalMontageFinished);
	PlayMontageTask->ReadyForActivation();
}

void URGX_CastHitboxAttackAbility::OnFinalMontageFinished()
{
	URGX_HitboxesManagerComponent* HitboxManagerComponent = CurrentActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();
	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	if (Hitbox)
	{
		Hitbox->RemoveAbilityEffectsInfo();
	}

	UE_LOG(LogTemp, Warning, TEXT("Remove Ability Effects\n"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
