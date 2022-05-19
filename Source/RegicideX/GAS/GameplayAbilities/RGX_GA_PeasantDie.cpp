// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PeasantDie.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"
#include "RegicideX/Actors/Enemies/RGX_GroupManager.h"
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
	/*UBrainComponent* Brain = Controller->GetBrainComponent();
	if (Brain)
		Brain->StopLogic("");*/
	if (Controller)
	{
		Controller->StopMovement();
		Controller->UnPossess();
		Controller->Destroy();
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
	//Peasant->GetMesh()->GetAnimInstance()->
	//Peasant->GetMesh()->SetSimulatePhysics(true);
	/*if(Peasant)
		Peasant->manager->RemovePeasant(Peasant);*/
	Peasant->ToBeDestroyed = true;
}

void URGX_GA_PeasantDie::OnEndMontage()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
