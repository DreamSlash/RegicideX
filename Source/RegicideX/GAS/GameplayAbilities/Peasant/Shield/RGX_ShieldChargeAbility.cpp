// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_ShieldChargeAbility.h"

#include "RegicideX/Actors/Enemies/RGX_Peasant_Shield.h"
#include "RegicideX/GAS/AbilityTasks/RGX_MoveForwardAbilityTask.h"

bool URGX_ShieldChargeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool canActivate = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	canActivate &= Cast<ARGX_Peasant_Shield>(GetAvatarActorFromActorInfo()) != nullptr;

	return canActivate;
}

void URGX_ShieldChargeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_ShieldChargeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_ShieldChargeAbility::HandleReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::HandleReceivedEvent(EventTag, EventData);

	const FName StartChargeTag = FName("GameplayEvent.Ability.ShieldPeasant.StartCharge");
	if (EventTag.GetTagName() == StartChargeTag)
	{
		StartCharge();
	}
}

void URGX_ShieldChargeAbility::StartCharge()
{
	URGX_MoveForwardAbilityTask* task = URGX_MoveForwardAbilityTask::MoveForwardAbilityTask(this, NAME_None, ChargeDuration, HitboxTag);
	task->OnFinished.AddDynamic(this, &URGX_ShieldChargeAbility::OnChargeFinished);
	task->OnOverlapped.AddDynamic(this, &URGX_ShieldChargeAbility::OnOverlapped);
	task->ReadyForActivation();

	/*bCharge = true;
	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, [this]() { EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false); }, ChargeDuration, false);*/
}

void URGX_ShieldChargeAbility::OnChargeFinished()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void URGX_ShieldChargeAbility::OnOverlapped()
{
	MontageJumpToSection(JumpToSectionIfOverlapped);
}

//void URGX_ShieldChargeAbility::Tick(float DeltaTime)
//{
//	if (bCharge == false)
//	{
//		return;
//	}
//
//	ARGX_Peasant_Shield* owner = Cast<ARGX_Peasant_Shield>(GetAvatarActorFromActorInfo());
//	if (owner == nullptr)
//	{
//		return;
//	}
//
//	APawn* pawn = owner->GetController<APawn>();
//	if (pawn == nullptr)
//	{
//		return;
//	}
//
//	pawn->AddMovementInput(FVector::ForwardVector);
//}
