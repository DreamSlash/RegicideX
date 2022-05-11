// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PeasantDie.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
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
	if (Controller)
	{
		Controller->StopMovement();
	}

	if (MontageToPlay && Character)
	{
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance) 
		{
			float duration = AnimInstance->Montage_Play(MontageToPlay);
			FTimerDelegate TimerCallback;

			MyHandle = Handle; MyAinfo = ActorInfo; MyActivationInfo = ActivationInfo;
			TimerCallback.BindLambda([this]
			{
				EndAbility(MyHandle, MyAinfo, MyActivationInfo, false, false);
			});

			FTimerHandle MontageTimerHandle;
			//en vez de esto puedes usar el delegate de onmontage ended pero queria hacer que funcionase rapido
			//IMPORTANTE: en cuanto hagas play del montage, debes parar el Behaviour tree ya que puede que no te hagan el montage pq 
			//haran lo que les diga el peasant manager y se seguiran moviendo hasta que se destruyan
			GetWorld()->GetTimerManager().SetTimer(MontageTimerHandle, TimerCallback, duration - 0.5, false);
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
	Peasant->manager->RemovePeasant(Peasant);
}
