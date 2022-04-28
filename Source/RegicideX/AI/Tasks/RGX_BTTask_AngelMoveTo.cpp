// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_AngelMoveTo.h"
#include "RegicideX/Actors/Enemies/RGX_DistanceAngel.h"
#include "AIController.h"



EBTNodeResult::Type URGX_BT_AngelMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	return EBTNodeResult::InProgress;
}



void URGX_BT_AngelMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	TaskResult = EBTNodeResult::InProgress;

	APawn* ControlledPawn = AIController->GetPawn();
	ARGX_DistanceAngel* DistAngelPawn = Cast<ARGX_DistanceAngel>(ControlledPawn);

	FVector TargetLocation;
	if (AActor* TargetActor = DistAngelPawn->TargetActor) {
		TargetLocation = TargetActor->GetActorLocation();
	}
	else
	{ 
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	float Dist = FVector::Distance(DistAngelPawn->GetActorLocation(), TargetLocation);

	if (Dist <= DistAngelPawn->MaxAttackDist)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Task Succeeded, distance to target: %f"), Dist);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	DistAngelPawn->MoveToTarget(DeltaSeconds, TargetLocation);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

}

