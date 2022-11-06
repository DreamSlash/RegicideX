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
	AActor* avatarActor = GetAvatarActorFromActorInfo();
	APawn* enemy = Cast<APawn>(avatarActor);
	const ARGX_MageAngel* MageAngel = Cast<const ARGX_MageAngel>(avatarActor);
	const FTransform RingTransform(MageAngel->RingWaveSource->GetComponentLocation());

	FActorSpawnParameters params;
	params.Instigator = enemy;
	GetWorld()->SpawnActor<ARGX_RingWave>(RingActorClass, RingTransform, params);

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