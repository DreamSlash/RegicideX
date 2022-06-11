// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/RGX_WeakenedAbility.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

URGX_WeakenedAbility::URGX_WeakenedAbility()
{

}

bool URGX_WeakenedAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ActorInfo->AvatarActor);
	if (Enemy == nullptr)
		return false;

	return true;
}

void URGX_WeakenedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ActorInfo->AvatarActor);
	Enemy->EnableInteraction();
	GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Enemy.Weakened")));
}

void URGX_WeakenedAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Enemy.Weakened")));

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ActorInfo->AvatarActor);
	Enemy->DisableInteraction();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
