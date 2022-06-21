// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/MeleeAngel/RGX_MEAChargeAbility.h"
#include "GameFramework/Character.h"
#include "RegicideX/GAS/AbilityTasks/RGX_MoveToLocationCharge.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

bool URGX_MEAChargeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
	if (Enemy == nullptr)
		return false;

	return true;
}

void URGX_MEAChargeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Pass info to hitbox

	// Execute move task
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(GetAvatarActorFromActorInfo());
	FVector TargetLocation = Enemy->TargetActor->GetActorLocation();
	URGX_MoveToLocationCharge* MoveToLocationTask = URGX_MoveToLocationCharge::MoveToLocationCharge(this, NAME_None, TargetLocation, 5.0f, nullptr, nullptr);
	MoveToLocationTask->OnTargetLocationReached.AddDynamic(this, &URGX_MEAChargeAbility::OnDestinationReached);
	//UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, NAME_None, TargetLocation, 5.0f, nullptr, nullptr);
	MoveToLocationTask->ReadyForActivation();
}

void URGX_MEAChargeAbility::OnDestinationReached()
{
	UE_LOG(LogTemp, Warning, TEXT("Destination Reached\n"));
}

void URGX_MEAChargeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
