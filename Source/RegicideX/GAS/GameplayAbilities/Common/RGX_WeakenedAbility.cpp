// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/Common/RGX_WeakenedAbility.h"
#include "Components/MCV_AbilitySystemComponent.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"

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

	AAIController* Controller = Cast<AAIController>(Enemy->GetController());

	// If reacting to hit, should not move
	if (Controller) {
		Controller->GetBrainComponent()->StopLogic(FString("Animation playing"));
		Controller->SetFocus(nullptr);
	}
}

void URGX_WeakenedAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ActorInfo->OwnerActor);
	Enemy->bWeak = false;
	// Restart brain logic
	AAIController* Controller = Cast<AAIController>(Enemy->GetController());
	if (Controller) {
		Controller->GetBrainComponent()->StartLogic();
		if (Enemy->TargetActor)
			Controller->SetFocus(Enemy->TargetActor);
	}

	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Enemy.Weakened")));

	Enemy->DisableInteraction();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
