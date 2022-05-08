#include "RGX_RingWavesAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "../../Actors/Weapons/RGX_RingWave.h"

void URGX_RingWavesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	EndHandle = Handle;
	EndActorInfo = ActorInfo;
	EndActivationInfo = ActivationInfo;

	PendingWaves = NumWaves;

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	OnSpawnRingWave();
}

void URGX_RingWavesAbility::OnSpawnRingWave()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const FTransform RingTransform(AvatarActor->GetActorLocation());
	GetWorld()->SpawnActor<ARGX_RingWave>(RingActorClass, RingTransform);

	if (--PendingWaves > 0)
	{
		StartDelay();
	}
	else
	{
		EndAbility(EndHandle, EndActorInfo, EndActivationInfo, false, false);
	}
}

void URGX_RingWavesAbility::StartDelay()
{
	UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, DelayBetweenWaves);
	WaitDelayTask->OnFinish.AddDynamic(this, &URGX_RingWavesAbility::OnSpawnRingWave);
	WaitDelayTask->ReadyForActivation();
}