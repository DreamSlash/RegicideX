// Fill out your copyright notice in the Description page of Project Settings.

#include "RegicideX/GAS/AbilityTasks/RGX_MoveForwardAbilityTask.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Components/RGX_HitboxesManagerComponent.h"
#include "RegicideX/Components/RGX_HitboxComponent.h"

#pragma optimize("", off)

URGX_MoveForwardAbilityTask::URGX_MoveForwardAbilityTask(const FObjectInitializer& ObjectInitializer)
	: UAbilityTask(ObjectInitializer)
{
	bTickingTask = true;
}

URGX_MoveForwardAbilityTask* URGX_MoveForwardAbilityTask::MoveForwardAbilityTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Duration, FGameplayTag HitboxTag)
{
	URGX_MoveForwardAbilityTask* task = NewAbilityTask<URGX_MoveForwardAbilityTask>(OwningAbility, TaskInstanceName);

	task->Duration = Duration;
	task->HitboxTag = HitboxTag;

	return task;
}

void URGX_MoveForwardAbilityTask::Activate()
{
	Super::Activate();

	if (ARGX_EnemyBase* owner = Cast<ARGX_EnemyBase>(GetAvatarActor()))
	{
		if (URGX_HitboxesManagerComponent* managerHB = Cast<URGX_HitboxesManagerComponent>(owner->GetComponentByClass(URGX_HitboxesManagerComponent::StaticClass())))
		{
			if (URGX_HitboxComponent* hitbox = managerHB->GetHitboxByTag(HitboxTag))
			{
				hitbox->OnHitboxOverlap.__Internal_AddDynamic(this, &URGX_MoveForwardAbilityTask::OnHitboxOverlap, "OnHitboxOverlap");
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() { bEndTask = true; }, Duration, false);
}

void URGX_MoveForwardAbilityTask::TickTask(float DeltaTime)
{		
	if (bEndTask)
	{
		EndTask();

		if (OnFinished.IsBound())
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
}

void URGX_MoveForwardAbilityTask::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}

void URGX_MoveForwardAbilityTask::OnHitboxOverlap(AActor* OverlappedActor)
{
	EndTask();

	if (OnOverlapped.IsBound())
		OnOverlapped.Broadcast();
}
