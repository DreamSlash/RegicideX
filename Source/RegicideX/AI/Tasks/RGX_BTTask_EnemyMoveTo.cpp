// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_EnemyMoveTo.h"

#include "AIController.h"

#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"

EBTNodeResult::Type URGX_BT_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void URGX_BT_EnemyMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(ControlledPawn);

	if (Enemy->TargetActor == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	Enemy->RotateToTarget(DeltaSeconds);

	const FVector TargetLocation = Enemy->TargetActor->GetActorLocation();
	const float Dist = FVector::Distance(Enemy->GetActorLocation(), TargetLocation);

	//UE_LOG(LogTemp, Warning, TEXT("Before, distance to target: %f"), Dist);

	if (Dist <= Enemy->AttackRadius)
	{
		//UE_LOG(LogTemp, Warning, TEXT("After, distance to target: %f"), Dist);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	Enemy->MoveToTarget(DeltaSeconds, FVector());
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}

