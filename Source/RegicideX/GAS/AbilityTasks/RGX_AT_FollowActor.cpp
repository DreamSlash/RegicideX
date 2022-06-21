// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/AbilityTasks/RGX_AT_FollowActor.h"

void URGX_AT_FollowActor::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);
}

URGX_AT_FollowActor* URGX_AT_FollowActor::FollowActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Speed)
{
	URGX_AT_FollowActor* MyObj = NewAbilityTask<URGX_AT_FollowActor>(OwningAbility, TaskInstanceName);

	return MyObj;
}

void URGX_AT_FollowActor::Activate()
{
}

void URGX_AT_FollowActor::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
}

void URGX_AT_FollowActor::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}
