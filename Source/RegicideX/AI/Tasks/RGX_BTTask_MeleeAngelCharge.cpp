// Fill out your copyright notice in the Description page of Project Settings.

#include "RGX_BTTask_MeleeAngelCharge.h"

#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"

#include "AIController.h"

EBTNodeResult::Type URGX_BTTask_MeleeAngelCharge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	MeleeAngelPawn = Cast<ARGX_MeleeAngel>(ControlledPawn);

	MeleeAngelPawn->bFlying = true;

	MoveSpeed = MeleeAngelPawn->MoveSpeed;

	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}

void URGX_BTTask_MeleeAngelCharge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	TaskTime += DeltaSeconds;
	if (TaskTime >= MaxTime || MeleeAngelPawn->bCharged)
	{
		TaskTime = 0.0f;
		MeleeAngelPawn->bFlying = false;
		MeleeAngelPawn->bCharged = false;
		MeleeAngelPawn->MoveSpeed = MoveSpeed;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	MeleeAngelPawn->MoveSpeed += DeltaSeconds * AccelerationMultiplier;
	MeleeAngelPawn->RotateToTarget(DeltaSeconds);
	MeleeAngelPawn->MoveToTarget(DeltaSeconds, FVector());

}










