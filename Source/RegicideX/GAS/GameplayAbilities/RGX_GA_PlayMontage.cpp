// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GA_PlayMontage.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"

void URGX_GA_PlayMontage::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//AnimInstance es nullptr asi que uso avatar actor casteo a character y hago playmontage
	//cojo el tiempo y hago un timer que llame a endability
	if (MontageToPlay && ActorInfo->AvatarActor.IsValid())
	{
		ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor);
		USkeletalMeshComponent* SkelMesh = nullptr;
		SkelMesh = Character->GetMesh();
		UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance();
		float duration = AnimInstance->Montage_Play(MontageToPlay);
		FTimerDelegate TimerCallback;
		//aqui esta la guarrada padre, tengo que guardarme estos valores, ya que si los paso por la lambda, a la hora de llamar endability 
		//ya el garbage collector se los habra cargado
		MyHandle = Handle; MyAinfo = ActorInfo; MyActivationInfo = ActivationInfo;
		TimerCallback.BindLambda([this]
			{
				EndAbility(MyHandle, MyAinfo, MyActivationInfo, false, false);
			});
		FTimerHandle MontageTimerHandle;
		//en vez de esto puedes usar el delegate de onmontage ended pero queria hacer que funcionase rapido
		//IMPORTANTE: en cuanto hagas play del montage, debes parar el Behaviour tree ya que puede que no te hagan el montage pq 
		//haran lo que les diga el peasant manager y se seguiran moviendo hasta que se destruyan
		GetWorld()->GetTimerManager().SetTimer(MontageTimerHandle, TimerCallback, duration-0.5, false);
	}
}

void URGX_GA_PlayMontage::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AActor* Actor = Cast<AActor>(ActorInfo->AvatarActor); //porsiacaso
	Actor->Destroy();

}
