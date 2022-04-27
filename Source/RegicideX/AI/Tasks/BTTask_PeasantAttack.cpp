// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PeasantAttack.h"
#include "AIController.h"
#include "RegicideX/Actors/Enemies/RGX_Peasant.h"

EBTNodeResult::Type UBTTask_PeasantAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const PeasantController = OwnerComp.GetAIOwner();
	ARGX_Peasant* const self = Cast<ARGX_Peasant>(PeasantController->GetPawn());

	if (self)
	{
		self->Attack();
	}
	return EBTNodeResult::Succeeded;
}
