// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_MeleeAttackAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "../../Components/RGX_HitboxesManagerComponent.h"
#include "../../Components/RGX_CombatAssistComponent.h"
#include "../../Character/RGX_PlayerCharacter.h"
#include "../../Components/RGX_HitboxComponent.h"

URGX_MeleeAttackAbility::URGX_MeleeAttackAbility()
{

}

void URGX_MeleeAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	URGX_CombatAssistComponent* CombatAssistComponent = ActorInfo->AvatarActor->FindComponentByClass<URGX_CombatAssistComponent>();
	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->DisableMovementInput();

		if (CombatAssistComponent)
		{
			CombatAssistComponent->PerformAttackAutoAssist();
		}
	}
	
	if (CombatAssistComponent)
	{
		CombatAssistComponent->AddMovementVector(ActorInfo->AvatarActor->GetActorForwardVector(), MoveVectorLength);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_MeleeAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(CurrentActorInfo->AvatarActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->EnableMovementInput();
	}

	URGX_CombatAssistComponent* CombatAssistComponent = CurrentActorInfo->AvatarActor->FindComponentByClass<URGX_CombatAssistComponent>();
	if (CombatAssistComponent)
	{
		CombatAssistComponent->RemoveMovementVector();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
