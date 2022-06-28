// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/AbilityTasks/RGX_AT_WaitDelayAndSpawn.h"
#include "AbilitySystemGlobals.h"
#include "TimerManager.h"

URGX_AT_WaitDelayAndSpawn::URGX_AT_WaitDelayAndSpawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	Time = 0.f;
	TimeStarted = 0.f;
	NextSpawnTime = 0.0f;
}

URGX_AT_WaitDelayAndSpawn* URGX_AT_WaitDelayAndSpawn::WaitDelayAndSpawn(UGameplayAbility* OwningAbility, float Time, float SpawnRate)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Time);

	URGX_AT_WaitDelayAndSpawn* MyObj = NewAbilityTask<URGX_AT_WaitDelayAndSpawn>(OwningAbility);
	MyObj->Time = Time;
	MyObj->SpawnRate = SpawnRate;
	return MyObj;
}

void URGX_AT_WaitDelayAndSpawn::Activate()
{
	UWorld* World = GetWorld();
	TimeStarted = World->GetTimeSeconds();
	NextSpawnTime = TimeStarted;

	// Use a dummy timer handle as we don't need to store it for later but we don't need to look for something to clear
	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, this, &URGX_AT_WaitDelayAndSpawn::OnTimeFinish, Time, false);
}

FString URGX_AT_WaitDelayAndSpawn::GetDebugString() const
{
	float TimeLeft = Time - GetWorld()->TimeSince(TimeStarted);
	return FString::Printf(TEXT("WaitDelay. Time: %.2f. TimeLeft: %.2f"), Time, TimeLeft);
}

void URGX_AT_WaitDelayAndSpawn::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	UWorld* World = GetWorld();
	float CurrentTime = World->GetTimeSeconds();
	if (CurrentTime > NextSpawnTime)
	{
		NextSpawnTime = CurrentTime + SpawnRate;

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnSpawn.Broadcast();
		}
	}
}

void URGX_AT_WaitDelayAndSpawn::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

void URGX_AT_WaitDelayAndSpawn::OnTimeFinish()
{
	UE_LOG(LogTemp, Warning, TEXT("Timer Finished\n"));

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnFinish.Broadcast();
	}
	EndTask();
}
