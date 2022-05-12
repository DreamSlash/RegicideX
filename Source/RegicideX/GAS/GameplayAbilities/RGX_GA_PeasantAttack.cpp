// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PeasantAttack.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"
#include "RegicideX/Actors/Enemies/RGX_GroupManager.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void URGX_GA_PeasantAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor);
	AAIController* Controller = Cast<AAIController>(Character->GetController());

	if (MontageToPlay && Character)
	{
		bool bPlayedMontageSuccessfully = false;
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_GA_PeasantAttack::OnEndMontage);
			PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_GA_PeasantAttack::OnEndMontage);
			PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_GA_PeasantAttack::OnEndMontage);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_GA_PeasantAttack::OnEndMontage);
			PlayMontageTask->ReadyForActivation();
		}
	}
}

void URGX_GA_PeasantAttack::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(ActorInfo->OwnerActor);
	if (Peasant)
		Peasant->manager->SwitchFighter();
}

void URGX_GA_PeasantAttack::OnEndMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
