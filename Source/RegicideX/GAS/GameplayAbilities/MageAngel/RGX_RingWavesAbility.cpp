#include "RGX_RingWavesAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "RegicideX/Actors/Enemies/RGX_MageAngel.h"
#include "RegicideX/Actors/Weapons/RGX_RingWave.h"

void URGX_RingWavesAbility::OnGround()
{
	Super::OnGround();

	PendingWaves = NumWaves;

	OnSpawnRingWave();
}

void URGX_RingWavesAbility::OnSpawnRingWave()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const ARGX_MageAngel* MageAngel = Cast<const ARGX_MageAngel>(AvatarActor);
	const FTransform RingTransform(MageAngel->RingWaveSource->GetComponentLocation());
	GetWorld()->SpawnActor<ARGX_RingWave>(RingActorClass, RingTransform);

	if (--PendingWaves > 0)
	{
		StartDelay();
	}
	else
	{
		bEndGroundLoop = true;
	}
}

void URGX_RingWavesAbility::StartDelay()
{
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [this]() { OnSpawnRingWave(); }, DelayBetweenWaves, false);
	/*UAbilityTask_WaitDelay* WaitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, DelayBetweenWaves);
	WaitDelayTask->OnFinish.AddDynamic(this, &URGX_RingWavesAbility::OnSpawnRingWave);
	WaitDelayTask->ReadyForActivation();*/
}