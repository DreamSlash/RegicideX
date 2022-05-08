// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTaskRotateToTarget.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

#include "AIController.h"

EBTNodeResult::Type URGX_BTTaskRotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void URGX_BTTaskRotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;
	if (TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	Enemy->RotateToTarget(DeltaSeconds);

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
