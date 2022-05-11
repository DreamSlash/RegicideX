// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_PeasantWander.h"
#include "NavigationSystem.h"

EBTNodeResult::Type URGX_BTTask_PeasantWander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	bNotifyTick = true;

	return EBTNodeResult::InProgress;
}
