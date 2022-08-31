// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/AI/Tasks/RGX_BTTask_OrbitAroundTarget.h"

EBTNodeResult::Type URGX_BTTask_OrbitAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}


void URGX_BTTask_OrbitAroundTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
