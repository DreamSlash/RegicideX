#include "RGX_PlayerFallAttackAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Character/RGX_PlayerCharacter.h"
#include "../../../Components/RGX_CombatAssistComponent.h"
#include "../../../Components/RGX_HitboxesManagerComponent.h"
#include "../../../Components/RGX_HitboxComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URGX_PlayerFallAttackAbility::URGX_PlayerFallAttackAbility()
{

}

void URGX_PlayerFallAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	URGX_CombatAssistComponent* CombatAssistComponent = Character->FindComponentByClass<URGX_CombatAssistComponent>();
	URGX_HitboxesManagerComponent* HitboxManagerComponent = Character->FindComponentByClass<URGX_HitboxesManagerComponent>();
	UCharacterMovementComponent* CharacterMovementComponent = Character->FindComponentByClass<UCharacterMovementComponent>();

	CombatAssistComponent->PerformAttackAutoAssist();

	if (Character)
	{
		Character->DisableMovementInput();

		CombatAssistComponent->AddMovementVector(Character->GetActorForwardVector(), MoveVectorLength);

		URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);

		FRGX_AbilityEffectsInfo AbilityEffectsInfo;
		AbilityEffectsInfo.GameplayEffectsToTarget = EffectsToApplyToTarget;
		AbilityEffectsInfo.GameplayEffectsToOwner = EffectsToApplyToOwner;
		AbilityEffectsInfo.EventToTargetTags = EventToTargetTags;
		AbilityEffectsInfo.GameplayEventsToTarget = EventsToApplyToTarget;
		AbilityEffectsInfo.EventToOwnerTags = EventToOwnerTags;
		AbilityEffectsInfo.GameplayEventsToOwner = EventsToApplyToOwner;
		Hitbox->SetAbilityEffectsInfo(AbilityEffectsInfo);
		//UE_LOG(LogTemp, Warning, TEXT("Add Ability Effects\n"));

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, NAME_None, StartMontageToPlay, StartMontagePlayRatio, StartMontageStartSectionName, true);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnFinishStartMontage);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnInterruptMontage);
		PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnFinishStartMontage);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnFinishStartMontage);
		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
}

void URGX_PlayerFallAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	URGX_HitboxesManagerComponent* HitboxManagerComponent = Character->FindComponentByClass<URGX_HitboxesManagerComponent>();

	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	Hitbox->RemoveAbilityEffectsInfo();
	UE_LOG(LogTemp, Warning, TEXT("Remove Ability Effects\n"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_PlayerFallAttackAbility::OnFinishStartMontage()
{
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, EndMontageToPlay, EndMontagePlayRatio, EndMontageStartSectionName, true);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnFinishEndMontage);
	PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnInterruptMontage);
	PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnFinishEndMontage);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_PlayerFallAttackAbility::OnInterruptMontage);
	PlayMontageTask->ReadyForActivation();
}

void URGX_PlayerFallAttackAbility::OnFinishEndMontage()
{
	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	URGX_CombatAssistComponent* CombatAssistComponent = Character->FindComponentByClass<URGX_CombatAssistComponent>();

	CombatAssistComponent->RemoveMovementVector();
	Character->EnableMovementInput();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void URGX_PlayerFallAttackAbility::OnInterruptMontage()
{
	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	URGX_CombatAssistComponent* CombatAssistComponent = Character->FindComponentByClass<URGX_CombatAssistComponent>();

	CombatAssistComponent->RemoveMovementVector();
	Character->EnableMovementInput();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
