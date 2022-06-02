// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PlayHitboxMontageAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_CombatAssistComponent.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

URGX_PlayHitboxMontageAbility::URGX_PlayHitboxMontageAbility()
{

}

bool URGX_PlayHitboxMontageAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	const URGX_HitboxesManagerComponent* HitboxManagerComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();
	if (HitboxManagerComponent == nullptr)
		return false;

	return true;
}

void URGX_PlayHitboxMontageAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData); // Parent already calls CommitAbility function

	URGX_HitboxesManagerComponent* HitboxManagerComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();

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
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, PlayRatio, StartSectionName, true);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_PlayHitboxMontageAbility::OnMontageFinished);
	PlayMontageTask->ReadyForActivation();
}

void URGX_PlayHitboxMontageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	URGX_HitboxesManagerComponent* HitboxManagerComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_HitboxesManagerComponent>();
	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	if (Hitbox)
	{
		Hitbox->RemoveAbilityEffectsInfo();
	}

	UE_LOG(LogTemp, Warning, TEXT("Remove Ability Effects\n"));

	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_PlayHitboxMontageAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
