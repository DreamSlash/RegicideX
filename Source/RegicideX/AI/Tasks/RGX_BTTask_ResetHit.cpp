// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_ResetHit.h"

EBTNodeResult::Type URGX_BTTask_ResetHit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("Getting hit ..."));
	return EBTNodeResult::Succeeded;
}
