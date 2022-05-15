// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PeasantReactionHit.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/Character.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

void URGX_GA_PeasantReactionHit::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor);
	AAIController* Controller = Cast<AAIController>(Character->GetController());

	// If reacting to hit, should not move
	if (Controller)
		Controller->GetBrainComponent()->StopLogic(FString("Animation playing"));

	// Launch Peasant a small bit backwards
	ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(Character);
	FVector TargetActorForward = Peasant->TargetActor->GetActorForwardVector();
	Character->LaunchCharacter(TargetActorForward * 1000.0f, true, false);

	if (MontageToPlay)
	{
		bool bPlayedMontageSuccessfully = false;
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_GA_PeasantReactionHit::OnEndMontage);
			PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_GA_PeasantReactionHit::OnEndMontage);
			PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_GA_PeasantReactionHit::OnEndMontage);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_GA_PeasantReactionHit::OnEndMontage);
			PlayMontageTask->ReadyForActivation();
		}
	}
}

void URGX_GA_PeasantReactionHit::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor);
	if (Character)
	{
		AAIController* Controller = Cast<AAIController>(Character->GetController());
		if (Controller)
			Controller->GetBrainComponent()->StartLogic();
	}
}

void URGX_GA_PeasantReactionHit::OnEndMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
