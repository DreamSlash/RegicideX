// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_PeasantAttack.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

URGX_BTTask_PeasantAttack::URGX_BTTask_PeasantAttack()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = true;
	NodeName = "Peasant Attack Task";
}

EBTNodeResult::Type URGX_BTTask_PeasantAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const PeasantController = OwnerComp.GetAIOwner();
	ARGX_Peasant* const self = Cast<ARGX_Peasant>(PeasantController->GetPawn());
	if (!self)
		return EBTNodeResult::Failed;

	self->Attack();
	return EBTNodeResult::InProgress;
}

void URGX_BTTask_PeasantAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
