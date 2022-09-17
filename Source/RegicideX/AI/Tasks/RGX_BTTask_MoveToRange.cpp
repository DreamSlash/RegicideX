// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_MoveToRange.h"

EBTNodeResult::Type URGX_BT_MoveToRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AcceptableRadius = FMath::RandRange(MinAcceptableRadius, MaxAcceptableRadius);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
