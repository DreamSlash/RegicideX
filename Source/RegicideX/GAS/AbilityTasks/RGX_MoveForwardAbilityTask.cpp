// Fill out your copyright notice in the Description page of Project Settings.

#include "RegicideX/GAS/AbilityTasks/RGX_MoveForwardAbilityTask.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

#pragma optimize("", off)

URGX_MoveForwardAbilityTask::URGX_MoveForwardAbilityTask(const FObjectInitializer& ObjectInitializer)
	: UAbilityTask(ObjectInitializer)
{
	bTickingTask = true;
}

URGX_MoveForwardAbilityTask* URGX_MoveForwardAbilityTask::MoveForwardAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Duration)
{
	URGX_MoveForwardAbilityTask* task = NewAbilityTask<URGX_MoveForwardAbilityTask>(OwningAbility, TaskInstanceName);

	task->Duration = Duration;

	return task;
}

void URGX_MoveForwardAbilityTask::Activate()
{
	Super::Activate();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bEndTask = true; }, Duration, false);
}

void URGX_MoveForwardAbilityTask::TickTask(float DeltaTime)
{		
	if (bEndTask)
	{
		EndTask();
		OnFinished.Broadcast();
		return;
	}

	ARGX_EnemyBase* owner = Cast<ARGX_EnemyBase>(GetAvatarActor());
	if (owner == nullptr)
	{
		return;
	}
		
	APawn* pawn = owner->GetController()->GetPawn();
	if (pawn == nullptr)
	{
		return;
	}
		
	pawn->AddMovementInput(owner->GetActorForwardVector());
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Speed: %f"), owner->GetVelocity().Size2D()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, owner->GetActorForwardVector().ToText().ToString());
}

void URGX_MoveForwardAbilityTask::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}
