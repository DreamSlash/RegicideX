// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_PeasantMove.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

EBTNodeResult::Type URGX_BTTask_PeasantMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* PeasantController = OwnerComp.GetAIOwner();
	if (PeasantController)
	{
		PeasantController->MoveToLocation(FVector());
	}
	
	/*ARGX_Peasant* const self = Cast<ARGX_Peasant>(PeasantController->GetPawn());
	if (self)
	{
		self->Move();
	}*/

	return EBTNodeResult::Succeeded;
}
