// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BT_AngelMoveTo.h"
#include "RegicideX/Actors/Enemies/RGX_DistanceAngel.h"
#include "AIController.h"


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
		TaskResult = EBTNodeResult::Failed;
	}

	float Dist = FVector::Distance(DistAngelPawn->GetActorLocation(), TargetLocation);

	if (Dist >= DistAngelPawn->MaxAttackDist)
	{
		TaskResult = EBTNodeResult::Succeeded;
	}

	DistAngelPawn->MoveToTarget(DeltaSeconds, TargetLocation);
	
	//FinishLatentTask(OwnerComp, TaskResult);

}

