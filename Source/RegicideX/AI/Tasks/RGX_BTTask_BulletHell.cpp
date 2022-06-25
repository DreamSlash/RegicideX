// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_BTTask_BulletHell.h"

#include "RegicideX/Actors/Enemies/RGX_DistanceAngel.h"

#include "AIController.h"


EBTNodeResult::Type URGX_BTTask_BulletHell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();

	DistAngelPawn = Cast<ARGX_DistanceAngel>(ControlledPawn);

	OriginalBulletHellSphereScale = 10.0;
	DistAngelPawn->BulletHellSphere->SetHiddenInGame(false);
	bNotifyTick = true;
	return EBTNodeResult::InProgress;
}
void URGX_BTTask_BulletHell::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	TaskTime += DeltaSeconds;
	if(TaskTime >= MaxTime)
	{
		TaskTime = 0.0f;
		DistAngelPawn->BulletHellSphere->SetRelativeScale3D(FVector(OriginalBulletHellSphereScale));
		DistAngelPawn->BulletHellSphere->SetHiddenInGame(true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	DistAngelPawn->RotateMe(DeltaSeconds, 1.0 + TaskTime * RotationSpeedMultiplier);

	FVector CurrentScale = DistAngelPawn->BulletHellSphere->GetRelativeScale3D();
	if (CurrentScale.Z < MaxSize) 
	{
		FVector NewScale = DistAngelPawn->BulletHellSphere->GetRelativeScale3D() + FVector(TaskTime) * 0.4;
		DistAngelPawn->BulletHellSphere->SetRelativeScale3D(NewScale);
	}
	

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}
