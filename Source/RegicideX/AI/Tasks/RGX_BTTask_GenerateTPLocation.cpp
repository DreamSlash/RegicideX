// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_GenerateTPLocation.h"

#include "..\Controllers\RGX_DistanceAngelController.h"

EBTNodeResult::Type URGX_BTTask_GenerateTPLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARGX_DistanceAngelController* AIController = Cast<ARGX_DistanceAngelController>(OwnerComp.GetAIOwner());
	
	AIController->SetRandomLocationAroundTarget();

	return EBTNodeResult::Succeeded;
}






