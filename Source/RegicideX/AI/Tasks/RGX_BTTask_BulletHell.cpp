// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_BulletHell.h"
#include "RegicideX/Actors/Enemies/RGX_DistanceAngel.h"
#include "AIController.h"



EBTNodeResult::Type URGX_BTTask_BulletHell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	APawn* ControlledPawn = AIController->GetPawn();

	DistAngelPawn = Cast<ARGX_DistanceAngel>(ControlledPawn);

	bNotifyTick = true;

	return EBTNodeResult::InProgress;
}



void URGX_BTTask_BulletHell::Shoot()
{
	DistAngelPawn->ShootSimpleBullets();
}

void URGX_BTTask_BulletHell::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;
	if(TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	AAIController* AIController = OwnerComp.GetAIOwner();

	APawn* ControlledPawn = AIController->GetPawn();

	DistAngelPawn = Cast<ARGX_DistanceAngel>(ControlledPawn);

	DistAngelPawn->RotateMe(DeltaSeconds, 1.0 + TaskTime * RotationSpeedMultiplier);

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
