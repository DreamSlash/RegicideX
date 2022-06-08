// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PeasantDie.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"
#include "RegicideX/GameplayFramework/RGX_RoundGameMode.h"
#include "GameFramework/Character.h"

void URGX_GA_PeasantDie::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->OwnerActor);
	AAIController* Controller = Cast<AAIController>(Character->GetController());

	// If dead, should not move
	if (Controller)
	{
		Controller->GetBrainComponent()->StopLogic("Actor dead");
		Controller->StopMovement();
		Controller->SetFocus(nullptr);	// Stop focusing the player
	}

	// Disable Collision
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (ARGX_RoundGameMode* MyGameMode = Cast<ARGX_RoundGameMode>(GetWorld()->GetAuthGameMode()))
	{
		MyGameMode->OnEnemyDeath(0);
	}

	if (MontageToPlay && Character)
	{
		bool bPlayedMontageSuccessfully = false;
		if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
		{
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &URGX_GA_PeasantDie::OnEndMontage);
			PlayMontageTask->OnCancelled.AddDynamic(this, &URGX_GA_PeasantDie::OnEndMontage);
			PlayMontageTask->OnCompleted.AddDynamic(this, &URGX_GA_PeasantDie::OnEndMontage);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &URGX_GA_PeasantDie::OnEndMontage);
			PlayMontageTask->ReadyForActivation();
		}
	}

}

void URGX_GA_PeasantDie::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(ActorInfo->OwnerActor);
	Peasant->GetMesh()->bPauseAnims = true;
	Peasant->HandleDeath();
}

void URGX_GA_PeasantDie::OnEndMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
