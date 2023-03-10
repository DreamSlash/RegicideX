// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_MeleeAngelTornadoAbility.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/GAS/AbilityTasks/RGX_AT_RotateAndMoveCharacter.h"

URGX_GA_MeleeAngelTornadoAbility::URGX_GA_MeleeAngelTornadoAbility()
{

}

bool URGX_GA_MeleeAngelTornadoAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
	if (Enemy == nullptr)
		return false;

	return true;
}

void URGX_GA_MeleeAngelTornadoAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	URGX_AT_RotateAndMoveCharacter* RotateAndMoveCharacterMngr = URGX_AT_RotateAndMoveCharacter::RotateAndMoveCharacter(this, MaxTime);
	RotateAndMoveCharacterMngr->OnFinish.AddDynamic(this, &URGX_GA_MeleeAngelTornadoAbility::OnFinished);
	RotateAndMoveCharacterMngr->ReadyForActivation();
}

void URGX_GA_MeleeAngelTornadoAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_GA_MeleeAngelTornadoAbility::OnFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}