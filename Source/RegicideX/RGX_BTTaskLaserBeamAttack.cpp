// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTaskLaserBeamAttack.h"

#include "RegicideX\Actors\Weapons\RGX_LaserBeamWeapon.h"

#include "AIController.h"

EBTNodeResult::Type URGX_BTTaskLaserBeamAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	OwnerActor = AIController->GetPawn();

	LaserWeapon = GetWorld()->SpawnActor<ARGX_LaserBeamWeapon>(LaserBeamClass);

	LaserWeapon->SetSourcePoint(OwnerActor->GetActorLocation());

	return EBTNodeResult::InProgress;
}

void URGX_BTTaskLaserBeamAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TaskTime += DeltaSeconds;
	if (TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	LaserWeapon->MoveRay(DeltaSeconds);

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
