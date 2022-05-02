// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_DT_PrepareForAttack.h"

#include "RegicideX/Actors/Enemies/RGX_DistanceAngel.h"

#include "AIController.h"

EBTNodeResult::Type URGX_BTTask_DT_PrepareForAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	APawn* ControlledPawn = AIController->GetPawn();

	DistAngelPawn = Cast<ARGX_DistanceAngel>(ControlledPawn);

	bNotifyTick = true;

	return EBTNodeResult::InProgress;
}

void URGX_BTTask_DT_PrepareForAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;

	if (TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		DistAngelPawn->RingRotatingSpeed = DistAngelPawn->RingOriginalRotatingSpeed;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	DistAngelPawn->RingRotatingSpeed += TaskTime * RingsRotationSpeedMultiplier;
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
