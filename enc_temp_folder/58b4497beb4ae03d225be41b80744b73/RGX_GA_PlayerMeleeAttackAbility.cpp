#include "RGX_GA_PlayerMeleeAttackAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Character/RGX_PlayerCharacter.h"
#include "../../../Components/RGX_CombatAssistComponent.h"
#include "../../../Components/RGX_HitboxesManagerComponent.h"
#include "../../../Components/RGX_HitboxComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

URGX_PlayerMeleeAttackAbility::URGX_PlayerMeleeAttackAbility()
{
}

void URGX_PlayerMeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	URGX_CombatAssistComponent* CombatAssistComponent = Character->FindComponentByClass<URGX_CombatAssistComponent>();
	URGX_HitboxesManagerComponent* HitboxManagerComponent = Character->FindComponentByClass<URGX_HitboxesManagerComponent>();

	CombatAssistComponent->PerformAttackAutoAssist();

	if (Character)
	{
		Character->DisableMovementInput();
		CombatAssistComponent->AddMovementVector(Character->GetActorForwardVector(), MoveVectorLength);

		URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);

		FRGX_AbilityEffectsInfo AbilityEffectsInfo = GetAbilityEffectsInfo();
		Hitbox->SetAbilityEffectsInfo(AbilityEffectsInfo);
		UE_LOG(LogTemp, Warning, TEXT("Add Ability Effects\n"));

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, PlayRatio, StartSectionName, true);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_PlayerMeleeAttackAbility::FinishAttack);
		PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_PlayerMeleeAttackAbility::FinishAttack);
		PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_PlayerMeleeAttackAbility::FinishAttack);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_PlayerMeleeAttackAbility::FinishAttack);
		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}


	UE_LOG(LogTemp, Warning, TEXT("Melee Attack Ability"));
}

void URGX_PlayerMeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	URGX_HitboxesManagerComponent* HitboxManagerComponent = Character->FindComponentByClass<URGX_HitboxesManagerComponent>();

	URGX_HitboxComponent* Hitbox = HitboxManagerComponent->GetHitboxByTag(HitboxTag);
	Hitbox->RemoveAbilityEffectsInfo();
	UE_LOG(LogTemp, Warning, TEXT("Remove Ability Effects\n"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

FRGX_AbilityEffectsInfo URGX_PlayerMeleeAttackAbility::GetAbilityEffectsInfo()
{
	FRGX_AbilityEffectsInfo AbilityEffectsInfo;
	AbilityEffectsInfo.GameplayEffects = EffectsToApply;

	for (FGameplayTag EventTag : EventsToApply)
	{
		FRGX_AbilityGameplayEvent Event = {};
		Event.GameplayEvent = EventTag;

		// TODO [REFACTOR]: Hardcoded
		FGameplayEventData EventData = {};
		if (EventTag == FGameplayTag::RequestGameplayTag("GameplayEvent.Launched"))
		{
			URGX_LaunchEventPayload* LaunchPayloadObject = NewObject<URGX_LaunchEventPayload>();
			LaunchPayloadObject->LaunchHorizontalForce = LaunchHorizontalForce;
			LaunchPayloadObject->LaunchVerticalForce = LaunchVerticalForce;
			LaunchPayloadObject->bOverrideHorizontal = bOverrideHorizontal;
			LaunchPayloadObject->bOverrideVertical = bOverrideVertical;
			LaunchPayloadObject->ForceOrigin = CurrentActorInfo->AvatarActor.Get()->GetActorLocation();

			EventData.OptionalObject = LaunchPayloadObject;
		}
		else
		{
			Event.EventData = EventData;
		}

		AbilityEffectsInfo.GameplayEvents.Add(Event);
	}

	return AbilityEffectsInfo;
}

void URGX_PlayerMeleeAttackAbility::FinishAttack()
{
	ARGX_PlayerCharacter* Character = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	URGX_CombatAssistComponent* CombatAssistComponent = Character->FindComponentByClass<URGX_CombatAssistComponent>();

	CombatAssistComponent->RemoveMovementVector();
	Character->EnableMovementInput();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
